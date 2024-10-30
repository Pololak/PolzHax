// USE THIS FILE AS EDITORUI, LEVELEDITORLAYER AND EDITORPAUSELAYER HOOKS.

#define NOMINMAX
#include "pch.h"
//#include "Windows.h"
#include "preview-mode.hpp"
#include "state.h"
#include <gd.h>
//#include "gd.hpp"
#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "utils.hpp"
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include <cocos2d.h>
#include <cocos-ext.h>
#include "hsv.hpp"
#include <stdint.h>
#include "matdash/detail/impl.hpp"
#include "Hitboxes.h"
#include "moveForCommand.h"
#include <array>
#include "GameVariables.hpp"

using namespace cocos2d;

gd::EditorPauseLayer* editorPauseLayer;
gd::LevelEditorLayer* levelEditorLayer;
gd::EditorUI* editUI;
std::string savedClipboard;
CCScheduler* scheduler;

bool hideUI = false;

CCSprite* hideui_toggle_texture(CCSprite* unhide, CCSprite* hide) {
	return (hideUI) ? unhide : hide;
}

void preview_mode::Callback::hideUI_btn(CCObject*) {
	hideUI = !hideUI;
	if (hideUI) {
		editUI->setVisible(0);
	}
	else {
		editUI->setVisible(1);
	}
}

void preview_mode::showZoomText(gd::EditorUI* ui) {
	auto zoomLabel = reinterpret_cast<CCLabelBMFont*>(ui->getLevelEditorLayer()->getChildByTag(87600));

	if (zoomLabel) {
		std::string currentZoom = CCString::createWithFormat("%.02f%", ui->getLevelEditorLayer()->gameLayer()->getScale())->getCString();
		zoomLabel->setString(std::string("Zoom: " + currentZoom + "x").c_str());
		zoomLabel->setOpacity(255);
		zoomLabel->stopAllActions();
		zoomLabel->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCFadeOut::create(.5f), nullptr));
	}
}

struct CompareTriggers {
	bool operator()(gd::GameObject* a, gd::GameObject* b) const {
		return a->getPosition().x < b->getPosition().x;
	}

	bool operator()(gd::GameObject* a, float b) const {
		return a->getPosition().x < b;
	}

	bool operator()(float a, gd::GameObject* b) const {
		return a < b->getPosition().x;
	}
};

template <class T = void*>

struct ValueContainer {
	T value;
	virtual ~ValueContainer() {
		value.~T();
	}
	template <class A>
	A& get() {
		return reinterpret_cast<ValueContainer<A>*>(this)->value;
	}
};

template <class T>
struct Field{};

template <class T>
class ExtendBase {
public:
	static auto& get_extra_fields() {
		static std::unordered_map<void*, ValueContainer<>*> fields;
		return fields;
	}
		
	static void destroy() {
		auto& fields = get_extra_fields();
		for (auto& [_, value] : fields) {
			delete value;
		}
		fields.clear();
	}
};

template <class T, class A>
A& operator->*(ExtendBase<T>* self, Field<A>& dummy) {
	auto& fields = ExtendBase<T>::get_extra_fields();
	void* key = reinterpret_cast<void*>(&dummy);
	auto it = fields.find(key);
	if (it != fields.end()) return it->second->template get<A>();
	auto container = reinterpret_cast<ValueContainer<>*>(new ValueContainer<A>());
	fields[key] = container;
	return container->get<A>();
}

bool is_editor_paused = false;

template <class T>
T lerp(float amt, T a, T b) {
	return static_cast<T>(static_cast<float>(a) * (1.f - amt) + static_cast<float>(b) * amt);
}

struct GDColor {
	u8 r, g, b;
	bool blending = false;

	GDColor() {}
	constexpr GDColor(u8 r, u8 g, u8 b, bool blending) : r(r), g(g), b(b), blending(blending) {}
	constexpr GDColor(const ccColor3B color, bool blending = false) : r(color.r), g(color.g), b(color.b), blending(blending) {}
	GDColor(gd::GameObject* object) : GDColor(object->triggerColor(), object->triggerBlending()) {}
	GDColor(gd::ColorAction* color) : GDColor(color->m_color, color->m_blending) {}
	operator ccColor3B() const { return { r, g, b }; }

	bool operator==(const GDColor& other) { return std::tie(r, g, b, blending) == std::tie(other.r, other.g, other.b, other.blending); }
};

GDColor mix_color(float value, GDColor a, GDColor b) {
	return GDColor{
		lerp<u8>(value, a.r, b.r),
		lerp<u8>(value, a.g, b.g),
		lerp<u8>(value, a.b, b.b),
		b.blending
	};
}

enum class ColorTriggers {
	BG = 29,
	Obj = 105,
	// 3DL
	DLine = 744,
	Col1 = 221,
	Col2 = 717,
	Col3 = 718,
	Col4 = 743,
};

static std::unordered_set<int> color_trigger_ids = { 29, 105, 221, 717, 718, 743, 744 };
bool is_color_trigger(gd::GameObject* object) {
	return color_trigger_ids.find(object->getObjectID()) != color_trigger_ids.end();
}

void log_obj_vector(const std::vector<gd::GameObject*>& objects) {
	std::cout << '[';
	for (auto obj : objects)
		std::cout << "id=" << obj << " x=" << obj->getPosition().x << ", ";
	std::cout << ']' << std::endl;
}

// accurate lbg calculation, thanks to sSolsta and zmx
GDColor calculate_lbg(const GDColor& bg_color, const GDColor& p1_color) {
	auto hsv = color_utils::rgb_to_hsv({
		bg_color.r / 255., bg_color.g / 255., bg_color.b / 255. });
	hsv.s = std::max(hsv.s - 0.2, 0.0);
	hsv.v = std::min(hsv.v + 0.2, 1.0);

	const auto rgb = color_utils::hsv_to_rgb(hsv);

	const auto lbg_color = GDColor{
		static_cast<u8>(rgb.r * 255.),
		static_cast<u8>(rgb.g * 255.),
		static_cast<u8>(rgb.b * 255.),
		true
	};

	const auto amt = (static_cast<float>(bg_color.r) + static_cast<float>(bg_color.g) + static_cast<float>(bg_color.b)) / 150.f;

	if (amt < 1.f) {
		return mix_color(amt, p1_color, lbg_color);
	}
	else {
		return lbg_color;
	}
}

bool was_preview_mode_enabled = false;

class MyEditorLayer : public gd::LevelEditorLayer, public ExtendBase<MyEditorLayer> {
public:
	Field<std::unordered_map<ColorTriggers, std::vector<gd::GameObject*>>> m_color_triggers;
	Field<std::unordered_map<gd::CustomColorMode, bool>> m_current_color;
	Field<std::unordered_set<gd::GameObject*>> m_blending_objs;
	Field<float> m_last_pos;
	Field<CCSpriteBatchNode*> m_blending_batch_node;
	static MyEditorLayer* s_instance;

	void dtor() {
		ExtendBase::destroy();
		matdash::orig<&MyEditorLayer::dtor>(this);
		s_instance = nullptr;
	}

	bool init(gd::GJGameLevel* level) {
		if (!matdash::orig<&MyEditorLayer::init>(this, level)) return false;
		s_instance = this;
		levelEditorLayer = this;

		auto& triggers = this->*m_color_triggers;
		triggers[ColorTriggers::BG];
		triggers[ColorTriggers::Obj];
		triggers[ColorTriggers::DLine];
		triggers[ColorTriggers::Col1];
		triggers[ColorTriggers::Col2];
		triggers[ColorTriggers::Col3];
		triggers[ColorTriggers::Col4];

		auto gamesheet_texture = CCTextureCache::sharedTextureCache()->addImage("GJ_GameSheet.png", false);
		auto blending_batch_node = CCSpriteBatchNode::createWithTexture(gamesheet_texture);
		this->gameLayer()->addChild(blending_batch_node, 0);

		blending_batch_node->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

		this->*m_blending_batch_node = blending_batch_node;

		hideUI = false;

		auto director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();
		auto menuh = CCMenu::create();

		auto unhide = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
		if (!unhide->initWithFile("BE_eye-off-btn.png")) {
			unhide->initWithSpriteFrameName("GJ_checkOff_001.png");
		}
		auto hide = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		if (!hide->initWithFile("BE_eye-on-btn.png")) {
			hide->initWithSpriteFrameName("GJ_checkOn_001.png");
		}

		unhide->setOpacity(100);

		auto hideui_btn = gd::CCMenuItemToggler::create(
			hideui_toggle_texture(unhide, hide),
			hideui_toggle_texture(hide, unhide),
			this,
			menu_selector(preview_mode::Callback::hideUI_btn));

		auto ui = this->getEditorUI();

		hideui_btn->setPosition({ 165.f, -19.5f });
		hideui_btn->setScale(0.5f);

		menuh->setZOrder(100);
		menuh->setPosition({ director->getScreenLeft(), winSize.height});

		menuh->addChild(hideui_btn);
		this->addChild(menuh);

		auto playerDrawNode = CCDrawNode::create();
		playerDrawNode->setZOrder(1000);
		playerDrawNode->setTag(124);
		this->gameLayer()->addChild(playerDrawNode);

		auto objDrawNode = CCDrawNode::create();
		objDrawNode->setZOrder(1000);
		objDrawNode->setTag(125);
		this->gameLayer()->addChild(objDrawNode);

		auto zoomLabel = CCLabelBMFont::create("Zoom: ", "bigFont.fnt");
		zoomLabel->setScale(0.5f);
		zoomLabel->setZOrder(9999);
		zoomLabel->setPosition({ winSize.width / 2, director->getScreenTop() - 50.f });
		zoomLabel->setOpacity(0);
		zoomLabel->setTag(87600);

		this->addChild(zoomLabel);

		//auto currentGround = from<int>(this->getLevelSettings(), 0x124);
		//auto groundLayer = gd::GJGroundLayer::create(currentGround);
		//groundLayer->setTag(87601);
		//groundLayer->hideShadows(true);
		//groundLayer->setZOrder(100);

		//this->gameLayer()->addChild(groundLayer);

		return true;
	}

	void insert_trigger(gd::GameObject* object) {
		auto& triggers = (this->*m_color_triggers)[ColorTriggers(object->getObjectID())];
		for (size_t i = 0; i < triggers.size(); ++i) {
			if (CompareTriggers()(object, triggers[i])) {
				triggers.insert(triggers.begin() + i, object);
				return;
			}
		}
		triggers.push_back(object);
	}

	void remove_trigger(gd::GameObject* object) {
		auto& triggers = (this->*m_color_triggers)[ColorTriggers(object->getObjectID())];
		for (size_t i = 0; i < triggers.size(); ++i) {
			if (triggers[i] == object) {
				triggers.erase(triggers.begin() + i);
				break;
			}
		}
	}

	void move_trigger(gd::GameObject* object) {
		if (is_color_trigger(object)) {
			
			
			this->insert_trigger(object);
			this->remove_trigger(object);

			if (setting().onEditorPreview)
				this->update_preview_mode();
		}
	}

	float time_between_pos(float a, float b) {
		auto l = this->getDrawGrid();
		return std::abs(l->timeForXPos(a) - l->timeForXPos(b));
	}

	GDColor calculate_color(std::vector<gd::GameObject*> triggers, const float pos, const GDColor starting_color) {
		if (triggers.empty()) return starting_color;
		auto bound = std::lower_bound(triggers.begin(), triggers.end(), pos, CompareTriggers()) - triggers.begin();
		if (bound == 0) {
			return starting_color;
		}
		else {
			auto trigger = triggers[bound - 1];
			if (trigger->triggerDuration() < 0) trigger->triggerDuration() = 0;
			GDColor color_to = trigger;
			auto dist = time_between_pos(trigger->getPosition().x, pos) / (trigger->triggerDuration());
			auto color_from = starting_color;
			if (bound > 1) {
				auto trigger = triggers[bound - 2];
				if (trigger->triggerDuration() < 0) trigger->triggerDuration() = 0;
				auto dist = time_between_pos(trigger->getPosition().x, pos) / trigger->triggerDuration();
				color_from = trigger;
				if (dist < 1.f)
					color_from = mix_color(dist, bound > 2 ? triggers[bound - 3] : starting_color, color_from);
			}
			return mix_color(std::min(dist, 1.f), color_from, color_to);
		}
	}

	void addSpecial(gd::GameObject* object) {
		matdash::orig<&MyEditorLayer::addSpecial>(this, object);
		if (is_color_trigger(object)) {
			this->insert_trigger(object);
		}
	}

	void removeSpecial(gd::GameObject* object) {
		matdash::orig<&MyEditorLayer::removeSpecial>(this, object);
		if (is_color_trigger(object)) {
			this->remove_trigger(object);
		}
	}

	void update_object_color(gd::GameObject* object, const GDColor& color) {
		object->setCustomColor(color);
		CCSprite* node = object;
		if (object->getHasColor())
			node = object->getChildSprite();
		auto batch = this->*m_blending_batch_node;
		
		if (color.blending) {
			if (node->getParent() != batch) {
				node->removeFromParent();
				batch->addChild(node);
			}
		}
		else {
			if (node->getParent() == batch) {
				node->removeFromParent();
				this->getObjectBatchNode()->addChild(node);
			}
		}
	}

	bool is_color_blending(gd::CustomColorMode mode) {
		return (this->*m_current_color)[mode];
	}

	void updateVisibility(float dt) {
		matdash::orig<&MyEditorLayer::updateVisibility, matdash::Thiscall>(this, dt);
		
		if (is_editor_paused) return;
		if (!setting().onEditorPreview) {
			if (was_preview_mode_enabled)
				reset_colors();
			was_preview_mode_enabled = false;
			return;
		}

		const auto pos = this->get_preview_pos();

		if (was_preview_mode_enabled && pos == this->*m_last_pos) return;
		this->*m_last_pos = pos;
		was_preview_mode_enabled = true;

		this->update_preview_mode();
	}

	float get_preview_pos() {
		if (this->getPlaytestState() != 0)
			return this->getPlayer1()->getPositionX();
		else if (this->getEditorUI()->isPlayback())
			return this->getDrawGrid()->getPlaybackLinePos();
		else
			return -(this->gameLayer()->getPosition().x) / this->gameLayer()->getScale() + 285;
	}

	void update_preview_mode() {
		GDColor bg_color, obj_color, dl_color, color1, color2, color3, color4;

		const auto pos = this->get_preview_pos();

		auto settings = this->getLevelSettings();
		for (auto& [type, triggers] : this->*m_color_triggers) {
			GDColor* color = nullptr;
			GDColor starting_color;
			switch (type) {
			case ColorTriggers::BG:
				starting_color = settings->m_background_color;
				color = &bg_color;
				break;
			case ColorTriggers::Obj:
				starting_color = settings->m_object_color;
				color = &obj_color;
				break;
			case ColorTriggers::DLine:
				starting_color = settings->m_3dl_color;
				color = &dl_color;
				break;
			case ColorTriggers::Col1:
				starting_color = settings->m_color1;
				color = &color1;
				break;
			case ColorTriggers::Col2:
				starting_color = settings->m_color2;
				color = &color2;
				break;
			case ColorTriggers::Col3:
				starting_color = settings->m_color3;
				color = &color3;
				break;
			case ColorTriggers::Col4:
				starting_color = settings->m_color4;
				color = &color4;
				break;
			default:
				continue;
			}
			*color = this->calculate_color(triggers, pos, starting_color);
		}

		(this->*m_current_color)[gd::CustomColorMode::Col1] = color1.blending;
		(this->*m_current_color)[gd::CustomColorMode::Col2] = color2.blending;
		(this->*m_current_color)[gd::CustomColorMode::Col3] = color3.blending;
		(this->*m_current_color)[gd::CustomColorMode::Col4] = color4.blending;

		this->backgroundSprite()->setColor(bg_color);

		auto gm = gd::GameManager::sharedState();

		GDColor p1_color(gm->colorForIdx(gm->getPlayerColor()), true);
		GDColor p2_color(gm->colorForIdx(gm->getPlayerColor2()), true);
		GDColor white(ccc3(255, 255, 255));

		auto lbg_color = calculate_lbg(bg_color, p1_color);

		auto sections = AwesomeArray<CCArray>(this->getLevelSections());
		for (auto objects : sections) {
			if (!objects) continue;
			for (auto object : AwesomeArray<gd::GameObject>(objects)) {
				if (!object || !object->getParent() || object->isSelected()) continue;

				if (object->getIsTintObject()) {
					object->setObjectColor(obj_color);
				}

				//if (object->getType() == gd::GameObjectType::kGameObjectTypeSolid) {
				//	from<CCSpriteBatchNode*>(object, 0xac)->setZOrder(100);
				//}

				//if (object->getType() == gd::GameObjectType::kGameObjectTypeSlope) {
				//	from<CCSpriteBatchNode*>(object, 0xac)->setZOrder(100);
				//}

				//if (object->getType() == gd::GameObjectType::kGameObjectTypeHazard) {
				//	from<CCSpriteBatchNode*>(object, 0xac)->setZOrder(100);
				//}

				auto mode = object->getColorMode();
				switch (mode) {
				case gd::CustomColorMode::DL:
					this->update_object_color(object, dl_color);
					break;
				case gd::CustomColorMode::Col1:
					this->update_object_color(object, color1);
					break;
				case gd::CustomColorMode::Col2:
					this->update_object_color(object, color2);
					break;
				case gd::CustomColorMode::Col3:
					this->update_object_color(object, color3);
					break;
				case gd::CustomColorMode::Col4:
					this->update_object_color(object, color4);
					break;
				case gd::CustomColorMode::LightBG:
					this->update_object_color(object, lbg_color);
					break;
				case gd::CustomColorMode::PlayerCol1:
					this->update_object_color(object, p1_color);
					break;
				case gd::CustomColorMode::PlayerCol2:
					this->update_object_color(object, p2_color);
					break;
				case gd::CustomColorMode::White:
					this->update_object_color(object, white);
					break; // bro
				default:;
				}
			}
		}
	}

	void reset_colors() {
		this->backgroundSprite()->setColor(ccc3(166, 166, 166));
		auto sections = AwesomeArray<CCArray>(this->getLevelSections());
		for (auto objects : sections) {
			if (!objects) continue;
			for (auto object : AwesomeArray<gd::GameObject>(objects)) {
				if (!object || object->isSelected()) continue;

				if (object->getIsTintObject())
					object->setObjectColor(ccc3(255, 255, 255));

				auto mode = object->getActiveColorMode();
				const std::unordered_map<gd::CustomColorMode, GDColor> default_colors = {
					{ gd::CustomColorMode::Default, GDColor(255,255,255, false)},
					{ gd::CustomColorMode::DL, GDColor(255,255,0, false)},
					{ gd::CustomColorMode::Col1, GDColor(255,150,255, false)},
					{ gd::CustomColorMode::Col2, GDColor(255,255,150, false)},
					{ gd::CustomColorMode::Col3, GDColor(150,255,255, false)},
					{ gd::CustomColorMode::Col4, GDColor(150,255,150, false)},
					{ gd::CustomColorMode::LightBG, GDColor(75,175,255, false)},
					{ gd::CustomColorMode::PlayerCol1, GDColor(175,150,255, false)},
					{ gd::CustomColorMode::PlayerCol2, GDColor(255,150,150, false)},
					{ gd::CustomColorMode::White, GDColor(255,255,255, false)}
				};
				this->update_object_color(object, default_colors.at(mode));
			}
		}
	}
};
MyEditorLayer* MyEditorLayer::s_instance = nullptr;

bool EditorPauseLayer_init(void* self, void* idc) {
	is_editor_paused = true;
	return matdash::orig<&EditorPauseLayer_init>(self, idc);
}

void EditorPauseLayer_dtor(void* self) {
	is_editor_paused = false;
	matdash::orig<&EditorPauseLayer_dtor>(self);
}

bool GameObject_shouldBlendColor(gd::GameObject* self) {
	if (gd::GameManager::sharedState()->getPlayLayer())
		return matdash::orig<&GameObject_shouldBlendColor>(self);
	else {
		auto editor = MyEditorLayer::s_instance;
		if (!editor) return false;

		switch (self->getColorMode()) {
		case gd::CustomColorMode::Col1:
		case gd::CustomColorMode::Col2:
		case gd::CustomColorMode::Col3:
		case gd::CustomColorMode::Col4:
		case gd::CustomColorMode::DL:
			return editor->is_color_blending(self->getColorMode());
		default:;
		}
		return false;
	}
}

void EditorUI_moveObject(gd::EditorUI* self, gd::GameObject* object, CCPoint to) {
	matdash::orig<&EditorUI_moveObject>(self, object, to);
	reinterpret_cast<MyEditorLayer*>(self->getParent())->move_trigger(object);
}

void EditorUI_deselectAll(gd::EditorUI* self) {
	const auto objs = self->getSelectedObjects();
	matdash::orig<&EditorUI_deselectAll>(self);
	if (!objs.empty() && setting().onEditorPreview) {
		if (auto editor = MyEditorLayer::s_instance; editor)
			editor->update_preview_mode();
	}
}

void EditorUI_updateZoom(gd::EditorUI* self, float amt) {
	matdash::orig<&EditorUI_updateZoom>(self, amt);
	if (auto editor = MyEditorLayer::s_instance; editor && setting().onEditorPreview) {
		// y does editor->*m_last_pos not work
		operator->*(editor, editor->m_last_pos) = -999;
	}
	preview_mode::showZoomText(self);
}

CCPoint* EditorUI_getLimitedPosition(CCPoint* retVal, CCPoint point) {
	*retVal = point;
	return retVal;
}

static bool g_holding_in_editor = false;

void EditorUI_onPlaytest(gd::EditorUI* self, void* btn) {
	if (!g_holding_in_editor)
		return matdash::orig<&EditorUI_onPlaytest>(self, btn);
}

void EditorUI_ccTouchBegan(gd::EditorUI* self, void* idc, void* idc2) {
	g_holding_in_editor = true;
	return matdash::orig<&EditorUI_ccTouchBegan>(self, idc, idc2);
}

void EditorUI_ccTouchEnded(gd::EditorUI* self, void* idc, void* idc2) {
	g_holding_in_editor = false;
	return matdash::orig<&EditorUI_ccTouchEnded>(self, idc, idc2);
}

void __fastcall LevelEditorLayer::onPlaytestH(gd::LevelEditorLayer* self) {
	LevelEditorLayer::onPlaytest(self);
	//self->getEditorUI()->updateZoom(1.f);
}

void EditorUI::Callback::onGoToBaseLayer(CCObject* sender) {
	from<int>(from<gd::EditorUI*>(sender, 0xFC)->getLevelEditorLayer(), 0x12C) = -1;
	from<CCLabelBMFont*>(from<gd::EditorUI*>(sender, 0xFC), 0x20C)->setString("All");
}

void EditorUI::Callback::onGoToNextFreeLayer(CCObject* sender) {
	auto leveleditor = from<gd::EditorUI*>(sender, 0xFC)->getLevelEditorLayer();
	bool foundClear = false;
	bool nextCycle = false;
	int currentLayer = 0;

	while (!foundClear && !nextCycle)
	{
		from<int>(leveleditor, 0x12C) = currentLayer;
		auto objs = CCArray::create();
		for (int i = 0; i < (leveleditor->getAllObjects()->count()); i++)
		{
			objs->addObjectsFromArray(reinterpret_cast<CCArray*>(leveleditor->getAllObjects()->objectAtIndex(i)));
		}

		auto objs2 = CCArray::create();
		for (int i = 0; i < (objs->count()); i++)
		{
			if (reinterpret_cast<gd::GameObject*>(objs->objectAtIndex(i))->getGroup() == leveleditor->getLayerGroup() || leveleditor->getLayerGroup() == -1)
				objs2->addObject(objs->objectAtIndex(i));
		}

		if (objs2->count() > 0) currentLayer++;
		else foundClear = true;

		if (leveleditor->getLayerGroup() == -1)
			nextCycle = true;
	}
	from<int>(from<gd::EditorUI*>(sender, 0xFC)->getLevelEditorLayer(), 0x12C) = currentLayer;
	from<CCLabelBMFont*>(from<gd::EditorUI*>(sender, 0xFC), 0x20C)->setString(std::to_string(currentLayer).c_str());
}

bool __fastcall EditorUI::dtor_H(gd::EditorUI* self) {
	editUI = nullptr;
	if (setting().onPersClip) savedClipboard = self->clipboard();
	EditorUI::dtor(self);
}

bool __fastcall EditorUI::init_H(gd::EditorUI* self, void*, gd::LevelEditorLayer* editor) {
	editUI = self;

	bool result = EditorUI::init(self, editor);

	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();

	auto moreObjInfoMenu = CCMenu::create();
	moreObjInfoMenu->setPosition({ 0, 0 });
	moreObjInfoMenu->setTag(8900);
	self->addChild(moreObjInfoMenu);

	auto objcolorid = CCLabelBMFont::create("", "chatFont.fnt");
	objcolorid->setString(CCString::createWithFormat("C: %s", 0)->getCString());
	objcolorid->setVisible(0);
	objcolorid->setTag(45011);
	objcolorid->setScale(0.66f);
	objcolorid->setPosition({ director->getScreenLeft() + 50, director->getScreenTop() - 60 });
	objcolorid->setAnchorPoint({ 0, 0.5f });
	moreObjInfoMenu->addChild(objcolorid);

	auto objgroupid = CCLabelBMFont::create("", "chatFont.fnt");
	objgroupid->setString(CCString::createWithFormat("G: %d")->getCString());
	objgroupid->setVisible(0);
	objgroupid->setTag(45012);
	objgroupid->setScale(0.66f);
	objgroupid->setPosition({ director->getScreenLeft() + 50, director->getScreenTop() - 70 });
	objgroupid->setAnchorPoint({ 0, 0.5f });
	moreObjInfoMenu->addChild(objgroupid);

	auto objrot = CCLabelBMFont::create("", "chatFont.fnt");
	objrot->setString(CCString::createWithFormat("Rot: %.0f%")->getCString());
	objrot->setVisible(0);
	objrot->setTag(45015);
	objrot->setScale(0.66f);
	objrot->setPosition({ director->getScreenLeft() + 50, director->getScreenTop() - 80 });
	objrot->setAnchorPoint({ 0, 0.5f });
	moreObjInfoMenu->addChild(objrot);

	auto objposx = CCLabelBMFont::create("", "chatFont.fnt");
	objposx->setString(CCString::createWithFormat("X: %.0f%")->getCString());
	objposx->setVisible(0);
	objposx->setTag(45013);
	objposx->setScale(0.66f);
	objposx->setPosition({ director->getScreenLeft() + 50, director->getScreenTop() - 90 });
	objposx->setAnchorPoint({ 0, 0.5f });
	moreObjInfoMenu->addChild(objposx);

	auto objposy = CCLabelBMFont::create("", "chatFont.fnt");
	objposy->setString(CCString::createWithFormat("Y: %.0f%")->getCString());
	objposy->setVisible(0);
	objposy->setTag(45014);
	objposy->setScale(0.66f);
	objposy->setPosition({ director->getScreenLeft() + 50, director->getScreenTop() - 100 });
	objposy->setAnchorPoint({ 0, 0.5f });
	moreObjInfoMenu->addChild(objposy);

	auto objid = CCLabelBMFont::create("", "chatFont.fnt");
	objid->setString(CCString::createWithFormat("ID: %d")->getCString());
	objid->setVisible(0);
	objid->setTag(45016);
	objid->setScale(0.66f);
	objid->setPosition({ director->getScreenLeft() + 50, director->getScreenTop() - 110 });
	objid->setAnchorPoint({ 0, 0.5f });
	moreObjInfoMenu->addChild(objid);

	gd::GameObject* selectedObject = from<gd::GameObject*>(self, 0x258);
	auto objaddr = CCLabelBMFont::create("", "chatFont.fnt");
	objaddr->setString(CCString::createWithFormat("Address: 0x%p", selectedObject)->getCString());
	objaddr->setVisible(0);
	objaddr->setTag(45017);
	objaddr->setAnchorPoint({ 0, 0.5f });
	objaddr->setPosition({ director->getScreenLeft() + 50, director->getScreenTop() - 120 });
	objaddr->setScale(0.66f);
	moreObjInfoMenu->addChild(objaddr);

	std::vector<gd::GameObject*> gameObjectVector = self->getSelectedObjects();
	auto objcounter = CCLabelBMFont::create("", "chatFont.fnt");
	objcounter->setString(CCString::createWithFormat("Objects: %d", gameObjectVector.size())->getCString());
	objcounter->setVisible(0);
	objcounter->setTag(45018);
	objcounter->setAnchorPoint({ 0, 0.5f });
	objcounter->setPosition(director->getScreenLeft() + 50, director->getScreenTop() - 50);
	objcounter->setScale(0.66f);
	moreObjInfoMenu->addChild(objcounter);

	auto custommenu = CCMenu::create();
	custommenu->setPosition({ director->getScreenLeft(), director->getScreenTop() });

	auto leftMenu = from<CCMenu*>(self->getRedoBtn(), 0xac);
	auto rightMenu = from<CCMenu*>(self->getDeselectBtn(), 0xac);
	auto leftGroupArrow = from<gd::CCMenuItemSpriteExtra*>(self, 0x210);
	auto rightGroupArrow = from<gd::CCMenuItemSpriteExtra*>(self, 0x214);
	auto redoBtn = self->getRedoBtn();
	auto editObjectBtn = from<gd::CCMenuItemSpriteExtra*>(self, 0x1c8);
	auto previewTogger = from<gd::CCMenuItemToggler*>(self, 0x1dc);

	leftGroupArrow->setPositionX(leftGroupArrow->getPositionX() - 10);
	rightGroupArrow->setPositionX(rightGroupArrow->getPositionX() - 10);
	previewTogger->setPositionX(previewTogger->getPositionX() - 20);

	CCLabelBMFont* groupLabel = from<CCLabelBMFont*>(self, 0x20c);
	groupLabel->setPositionX(groupLabel->getPositionX() - 10);

	auto onBaseLayerSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	auto onBaseLayerBtn = gd::CCMenuItemSpriteExtra::create(onBaseLayerSpr, nullptr, self, menu_selector(EditorUI::Callback::onGoToBaseLayer));
	onBaseLayerBtn->setTag(45028);
	onBaseLayerBtn->setPosition({ -90, -172 });
	onBaseLayerSpr->setScale(0.5f);
	onBaseLayerSpr->setOpacity(175);
	rightMenu->addChild(onBaseLayerBtn);

	auto onFreeLayerSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	auto onFreeLayerBtn = gd::CCMenuItemSpriteExtra::create(onFreeLayerSpr, nullptr, self, menu_selector(EditorUI::Callback::onGoToNextFreeLayer));
	onFreeLayerBtn->setTag(45029);
	onFreeLayerBtn->setPosition({ 10, -172 });
	onFreeLayerSpr->setScale(0.5f);
	onFreeLayerSpr->setFlipX(1);
	onFreeLayerSpr->setOpacity(175);
	rightMenu->addChild(onFreeLayerBtn);

	auto deletespr = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
	if (!deletespr->initWithFile("GJ_trashBtn_001.png")) {
		deletespr->createWithSpriteFrameName("edit_delBtn_001.png");
	}
	auto deletebtn = gd::CCMenuItemSpriteExtra::create(deletespr, nullptr, self, menu_selector(gd::EditorUI::onDeleteSelected));
	deletespr->setScale(0.925f);
	//deletebtn->setPosition({ -160 , 139.35 });
	deletebtn->setPositionX(redoBtn->getPositionX() + 50);
	deletebtn->setPositionY(redoBtn->getPositionY());
	deletebtn->setTag(45030);

	leftMenu->addChild(deletebtn);

	auto goToGroupSpr = CCSprite::create("GJ_button_04.png");
	if (!goToGroupSpr->initWithFile("GJ_goToGroupBtn_001.png")) {
		goToGroupSpr->create("GJ_button_04.png");
	}
	auto goToGroupBtn = gd::CCMenuItemSpriteExtra::create(goToGroupSpr, nullptr, self, 0);
	goToGroupBtn->setTag(45031);
	goToGroupSpr->setScale(0.75f);
	goToGroupBtn->setPosition({self->getDeselectBtn()->getPositionX() - 84, self->getDeselectBtn()->getPositionY() - 1});
	rightMenu->addChild(goToGroupBtn);
	goToGroupBtn->setVisible(0);
	goToGroupBtn->setEnabled(false);

	if (setting().onPersClip)
	{
		if (!savedClipboard.empty()) {
			self->clipboard() = savedClipboard;
			self->updateButtons();
		}
	}

	auto Slider = from<gd::Slider*>(self, 0x164);
	Slider->setAnchorPoint({ 0.f, 0.f });
	Slider->setPosition({ director->getScreenLeft() + 280, director->getScreenTop() - 19.5f});
	Slider->setScale(0.8f);

	return result;
}

void __fastcall EditorUI::scrollWheel_H(gd::EditorUI* _self, void* edx, float dy, float dx) {
	auto self = reinterpret_cast<gd::EditorUI*>(reinterpret_cast<uintptr_t>(_self) - 0xf8);
	auto layer = reinterpret_cast<gd::LevelEditorLayer*>(self->getParent())->gameLayer();
	auto zoom = layer->getScale();

	static_assert(offsetof(CCDirector, m_pKeyboardDispatcher) == 0x4c, "it wrong!");
	auto kb = CCDirector::sharedDirector()->m_pKeyboardDispatcher;
	if (kb->getControlKeyPressed()) {
		zoom = static_cast<float>(std::pow(2.71828182845904523536, std::log(std::max(zoom, 0.001f)) - dy * 0.01f));
		// zoom limit
		zoom = std::max(zoom, 0.01f);
		zoom = std::min(zoom, 1000000.f);
		self->updateZoom(zoom);
	}
	else if (kb->getShiftKeyPressed()) {
		layer->setPositionX(layer->getPositionX() - dy * 1.f);
	}
	else {
		EditorUI::scrollWheel(_self, dy, dx);
	}
}

void __fastcall EditorUI::updateGridNodeSizeH(gd::EditorUI* self) {
	//if (setting().onGridSize) {
	//	auto actualMode = self->selectedMode();
	//	self->selectedMode() == gd::EditorUI::Mode_Create;

	//	EditorUI::updateGridNodeSize(self);

	//	self->selectedMode() == actualMode;
	//}
	EditorUI::updateGridNodeSize(self);
}

CCPoint* __fastcall EditorUI::moveForCommandH(gd::EditorUI* self, void* edx, CCPoint* pos, gd::EditCommand com) {
	float gridSize = self->getGridSize();
	switch (com) // Taken from HJFod https://github.com/HJfod/BetterEdit/blob/v4/tools/EditTab/moveForCommand.cpp
	{
	case moveForCommand::kEditCommandHalfLeft:
		*pos = CCPoint(-1.f / 2.f, 0.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandHalfRight:
		*pos = CCPoint(1.f / 2.f, 0.0f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandHalfUp:
		*pos = CCPoint(0.f, 1.f / 2.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandHalfDown:
		*pos = CCPoint(0.f, -1.f / 2.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandQuarterLeft:
		*pos = CCPoint(-1.f / 4.f, 0.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandQuarterRight:
		*pos = CCPoint(1.f / 4.f, 0.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandQuarterUp:
		*pos = CCPoint(0.f, 1.f / 4.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandQuarterDown:
		*pos = CCPoint(0.f, -1.f / 4.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandEightLeft:
		*pos = CCPoint(-1.f / 8.f, 0.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandEightRight:
		*pos = CCPoint(1.f / 8.f, 0.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandEightUp:
		*pos = CCPoint(0.f, 1.f / 8.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandEightDown:
		*pos = CCPoint(0.f, -1.f / 8.f) * gridSize;
		return pos;
		break;
	case moveForCommand::kEditCommandSmallerLeft:
		*pos = CCPoint(-0.5f, 0.f);
		return pos;
		break;
	case moveForCommand::kEditCommandSmallerRight:
		*pos = CCPoint(0.5f, 0.f);
		return pos;
		break;
	case moveForCommand::kEditCommandSmallerUp:
		*pos = CCPoint(0.f, 0.5f);
		return pos;
		break;
	case moveForCommand::kEditCommandSmallerDown:
		*pos = CCPoint(0.f, -0.5f);
		return pos;
		break;
	case moveForCommand::kEditCommandQUnitLeft:
		*pos = CCPoint(-.1f, 0.f);
		return pos;
		break;
	case moveForCommand::kEditCommandQUnitRight:
		*pos = CCPoint(.1f, 0.f);
		return pos;
		break;
	case moveForCommand::kEditCommandQUnitUp:
		*pos = CCPoint(0.f, .1f);
		return pos;
		break;
	case moveForCommand::kEditCommandQUnitDown:
		*pos = CCPoint(0.f, -.1f);
		return pos;
		break;
	}

	EditorUI::moveForCommand(self, pos, com);
}

void __fastcall EditorUI::transformObjectH(gd::EditorUI* self, void* edx, gd::GameObject* obj, gd::EditCommand com, bool idk) {
	CCArray* selectedObjects = self->getSelectedObjectsOfCCArray();
	auto selectedObjCount = selectedObjects->count();
	switch (com)
	{
	case rotationForCommand::kEditCommandRotate45CW:
		self->rotateObjects(selectedObjects, (45.f / selectedObjCount), { 0,0 });
		break;
	case rotationForCommand::kEditCommandRotate45CCW:
		self->rotateObjects(selectedObjects, -(45.f / selectedObjCount), { 0,0 });
		break;
	case rotationForCommand::kEditCommandRotate265CW:
		self->rotateObjects(selectedObjects, (26.5f / selectedObjCount), { 0,0 });
		break;
	case rotationForCommand::kEditCommandRotate265CCW:
		self->rotateObjects(selectedObjects, -(26.5f / selectedObjCount), { 0,0 });
		break;
	}

	EditorUI::transformObject(self, obj, com, idk);
}

void EditorUI::Callback::moveObjectHalfUp(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandHalfUp);
}

void EditorUI::Callback::moveObjectHalfDown(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandHalfDown);
}

void EditorUI::Callback::moveObjectHalfLeft(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandHalfLeft);
}

void EditorUI::Callback::moveObjectHalfRight(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandHalfRight);
}

void EditorUI::Callback::moveObjectQuarterUp(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandQuarterUp);
}

void EditorUI::Callback::moveObjectQuarterDown(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandQuarterDown);
}

void EditorUI::Callback::moveObjectQuarterLeft(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandQuarterLeft);
}

void EditorUI::Callback::moveObjectQuarterRight(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandQuarterRight);
}

void EditorUI::Callback::moveObjectEightUp(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandEightUp);
}

void EditorUI::Callback::moveObjectEightDown(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandEightDown);
}

void EditorUI::Callback::moveObjectEightLeft(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandEightLeft);
}

void EditorUI::Callback::moveObjectEightRight(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandEightRight);
}

void EditorUI::Callback::moveObjectSmallerUp(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandSmallerUp);
}

void EditorUI::Callback::moveObjectSmallerDown(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandSmallerDown);
}

void EditorUI::Callback::moveObjectSmallerLeft(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandSmallerLeft);
}

void EditorUI::Callback::moveObjectSmallerRight(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandSmallerRight);
}

void EditorUI::Callback::moveObjectQUnitUp(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandQUnitUp);
}

void EditorUI::Callback::moveObjectQUnitDown(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandQUnitDown);
}

void EditorUI::Callback::moveObjectQUnitLeft(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandQUnitLeft);
}

void EditorUI::Callback::moveObjectQUnitRight(CCObject*) {
	editUI->moveObjectCall(moveForCommand::kEditCommandQUnitRight);
}

void EditorUI::Callback::rotate45CW(CCObject* sender) {
	editUI->transformObjectCall(rotationForCommand::kEditCommandRotate45CW);
}

void EditorUI::Callback::rotate45CCW(CCObject* sender) {
	editUI->transformObjectCall(rotationForCommand::kEditCommandRotate45CCW);
}

void EditorUI::Callback::rotate265CW(CCObject* sender) {
	editUI->transformObjectCall(rotationForCommand::kEditCommandRotate265CW);
}

void EditorUI::Callback::rotate265CCW(CCObject* sender) {
	editUI->transformObjectCall(rotationForCommand::kEditCommandRotate265CCW);
}

void __fastcall EditorUI::createMoveMenuH(gd::EditorUI* self) {
	EditorUI::createMoveMenu(self);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto boomScrollLayer = from<gd::BoomScrollLayer*>(self->editButtonBar(), 0xe8);
	auto extendLayer = from<CCLayer*>(boomScrollLayer, 0x158);
	auto buttonPage2 = reinterpret_cast<CCMenu*>(extendLayer->getChildren()->objectAtIndex(1));

	auto dotsNode = reinterpret_cast<CCLayer*>(boomScrollLayer->getChildren()->objectAtIndex(1));
	CCArray* dotsArray = from<CCArray*>(boomScrollLayer, 0x118);
	CCArray* pagesArray = from<CCArray*>(boomScrollLayer, 0x144);

	CCArray* movePageCCArray = CCArray::create();
	auto moveButtonPage = gd::ButtonPage::create(movePageCCArray);
	moveButtonPage->setPositionX(buttonPage2->getPositionX() + CCDirector::sharedDirector()->getWinSize().width / 2);
	auto movePageMenu = reinterpret_cast<CCMenu*>(moveButtonPage->getChildren()->objectAtIndex(0));
	auto pageDot = CCSprite::create("smallDot.png");

	extendLayer->addChild(moveButtonPage);
	pagesArray->insertObject(moveButtonPage, 2);
	dotsArray->addObject(pageDot);
	dotsNode->addChild(pageDot);

	auto moveHalfUp_spr = CCSprite::create("GJ_button_01.png");
	moveHalfUp_spr->setScale(0.9f);
	auto moveHalfUp_spr2 = CCSprite::createWithSpriteFrameName("edit_upBtn2_001.png");
	moveHalfUp_spr2->setPosition({ moveHalfUp_spr->getContentSize().width / 2, (moveHalfUp_spr->getContentSize().height / 2) + 2 });
	auto moveHalfUp_lbl = CCLabelBMFont::create("1/2", "bigFont.fnt");
	moveHalfUp_lbl->setScale(0.5f);
	moveHalfUp_lbl->setPosition({ moveHalfUp_spr->getContentSize().width / 2, (moveHalfUp_spr->getContentSize().height / 2) - 9 });
	moveHalfUp_spr->addChild(moveHalfUp_spr2);
	moveHalfUp_spr->addChild(moveHalfUp_lbl);
	auto moveHalfUp_btn = gd::CCMenuItemSpriteExtra::create(moveHalfUp_spr, moveHalfUp_spr, self, menu_selector(EditorUI::Callback::moveObjectHalfUp));
	moveHalfUp_btn->setPosition({ -105, -(winSize.height / 2) + 68});

	auto moveHalfDown_spr = CCSprite::create("GJ_button_01.png");
	moveHalfDown_spr->setScale(0.9f);
	auto moveHalfDown_spr2 = CCSprite::createWithSpriteFrameName("edit_downBtn2_001.png");
	moveHalfDown_spr2->setPosition({ moveHalfDown_spr->getContentSize().width / 2, (moveHalfDown_spr->getContentSize().height / 2) + 2 });
	auto moveHalfDown_lbl = CCLabelBMFont::create("1/2", "bigFont.fnt");
	moveHalfDown_lbl->setScale(0.5f);
	moveHalfDown_lbl->setPosition({ moveHalfDown_spr->getContentSize().width / 2, (moveHalfDown_spr->getContentSize().height / 2) - 9 });
	moveHalfDown_spr->addChild(moveHalfDown_spr2);
	moveHalfDown_spr->addChild(moveHalfDown_lbl);
	auto moveHalfDown_btn = gd::CCMenuItemSpriteExtra::create(moveHalfDown_spr, moveHalfDown_spr, self, menu_selector(EditorUI::Callback::moveObjectHalfDown));
	moveHalfDown_btn->setPosition({ -65, -(winSize.height / 2) + 68 });

	auto moveHalfLeft_spr = CCSprite::create("GJ_button_01.png");
	moveHalfLeft_spr->setScale(0.9f);
	auto moveHalfLeft_spr2 = CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
	moveHalfLeft_spr2->setPosition({ moveHalfLeft_spr->getContentSize().width / 2, (moveHalfLeft_spr->getContentSize().height / 2) + 2 });
	auto moveHalfLeft_lbl = CCLabelBMFont::create("1/2", "bigFont.fnt");
	moveHalfLeft_lbl->setScale(0.5f);
	moveHalfLeft_lbl->setPosition({ moveHalfLeft_spr->getContentSize().width / 2, (moveHalfLeft_spr->getContentSize().height / 2) - 9 });
	moveHalfLeft_spr->addChild(moveHalfLeft_spr2);
	moveHalfLeft_spr->addChild(moveHalfLeft_lbl);
	auto moveHalfLeft_btn = gd::CCMenuItemSpriteExtra::create(moveHalfLeft_spr, moveHalfLeft_spr, self, menu_selector(EditorUI::Callback::moveObjectHalfLeft));
	moveHalfLeft_btn->setPosition({ -25, -(winSize.height / 2) + 68 });

	auto moveHalfRight_spr = CCSprite::create("GJ_button_01.png");
	moveHalfRight_spr->setScale(0.9f);
	auto moveHalfRight_spr2 = CCSprite::createWithSpriteFrameName("edit_rightBtn2_001.png");
	moveHalfRight_spr2->setPosition({ moveHalfRight_spr->getContentSize().width / 2, (moveHalfRight_spr->getContentSize().height / 2) + 2 });
	auto moveHalfRight_lbl = CCLabelBMFont::create("1/2", "bigFont.fnt");
	moveHalfRight_lbl->setScale(0.5f);
	moveHalfRight_lbl->setPosition({ moveHalfRight_spr->getContentSize().width / 2, (moveHalfRight_spr->getContentSize().height / 2) - 9 });
	moveHalfRight_spr->addChild(moveHalfRight_spr2);
	moveHalfRight_spr->addChild(moveHalfRight_lbl);
	auto moveHalfRight_btn = gd::CCMenuItemSpriteExtra::create(moveHalfRight_spr, moveHalfRight_spr, self, menu_selector(EditorUI::Callback::moveObjectHalfRight));
	moveHalfRight_btn->setPosition({ 15, -(winSize.height / 2) + 68 });

	auto moveQuarterUp_spr = CCSprite::create("GJ_button_01.png");
	moveQuarterUp_spr->setScale(0.9f);
	auto moveQuarterUp_spr2 = CCSprite::createWithSpriteFrameName("edit_upBtn2_001.png");
	moveQuarterUp_spr2->setPosition({ moveQuarterUp_spr->getContentSize().width / 2, (moveQuarterUp_spr->getContentSize().height / 2) + 2 });
	auto moveQuarterUp_lbl = CCLabelBMFont::create("1/4", "bigFont.fnt");
	moveQuarterUp_lbl->setScale(0.5f);
	moveQuarterUp_lbl->setPosition({ moveQuarterUp_spr->getContentSize().width / 2, (moveQuarterUp_spr->getContentSize().height / 2) - 9 });
	moveQuarterUp_spr->addChild(moveQuarterUp_spr2);
	moveQuarterUp_spr->addChild(moveQuarterUp_lbl);
	auto moveQuarterUp_btn = gd::CCMenuItemSpriteExtra::create(moveQuarterUp_spr, moveQuarterUp_spr, self, menu_selector(EditorUI::Callback::moveObjectQuarterUp));
	moveQuarterUp_btn->setPosition({ 55, -(winSize.height / 2) + 68 });

	auto moveQuarterDown_spr = CCSprite::create("GJ_button_01.png");
	moveQuarterDown_spr->setScale(0.9f);
	auto moveQuarterDown_spr2 = CCSprite::createWithSpriteFrameName("edit_downBtn2_001.png");
	moveQuarterDown_spr2->setPosition({ moveQuarterDown_spr->getContentSize().width / 2, (moveQuarterDown_spr->getContentSize().height / 2) + 2 });
	auto moveQuarterDown_lbl = CCLabelBMFont::create("1/4", "bigFont.fnt");
	moveQuarterDown_lbl->setScale(0.5f);
	moveQuarterDown_lbl->setPosition({ moveQuarterDown_spr->getContentSize().width / 2, (moveQuarterDown_spr->getContentSize().height / 2) - 9 });
	moveQuarterDown_spr->addChild(moveQuarterDown_spr2);
	moveQuarterDown_spr->addChild(moveQuarterDown_lbl);
	auto moveQuarterDown_btn = gd::CCMenuItemSpriteExtra::create(moveQuarterDown_spr, moveQuarterDown_spr, self, menu_selector(EditorUI::Callback::moveObjectQuarterDown));
	moveQuarterDown_btn->setPosition({ 95, -(winSize.height / 2) + 68 });

	auto moveQuarterLeft_spr = CCSprite::create("GJ_button_01.png");
	moveQuarterLeft_spr->setScale(0.9f);
	auto moveQuarterLeft_spr2 = CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
	moveQuarterLeft_spr2->setPosition({ moveQuarterLeft_spr->getContentSize().width / 2, (moveQuarterLeft_spr->getContentSize().height / 2) + 2 });
	auto moveQuarterLeft_lbl = CCLabelBMFont::create("1/4", "bigFont.fnt");
	moveQuarterLeft_lbl->setScale(0.5f);
	moveQuarterLeft_lbl->setPosition({ moveQuarterLeft_spr->getContentSize().width / 2, (moveQuarterLeft_spr->getContentSize().height / 2) - 9 });
	moveQuarterLeft_spr->addChild(moveQuarterLeft_spr2);
	moveQuarterLeft_spr->addChild(moveQuarterLeft_lbl);
	auto moveQuarterLeft_btn = gd::CCMenuItemSpriteExtra::create(moveQuarterLeft_spr, moveQuarterLeft_spr, self, menu_selector(EditorUI::Callback::moveObjectQuarterLeft));
	moveQuarterLeft_btn->setPosition({ -105, -(winSize.height / 2) + 28 });

	auto moveQuarterRight_spr = CCSprite::create("GJ_button_01.png");
	moveQuarterRight_spr->setScale(0.9f);
	auto moveQuarterRight_spr2 = CCSprite::createWithSpriteFrameName("edit_rightBtn2_001.png");
	moveQuarterRight_spr2->setPosition({ moveQuarterRight_spr->getContentSize().width / 2, (moveQuarterRight_spr->getContentSize().height / 2) + 2 });
	auto moveQuarterRight_lbl = CCLabelBMFont::create("1/4", "bigFont.fnt");
	moveQuarterRight_lbl->setScale(0.5f);
	moveQuarterRight_lbl->setPosition({ moveQuarterRight_spr->getContentSize().width / 2, (moveQuarterRight_spr->getContentSize().height / 2) - 9 });
	moveQuarterRight_spr->addChild(moveQuarterRight_spr2);
	moveQuarterRight_spr->addChild(moveQuarterRight_lbl);
	auto moveQuarterRight_btn = gd::CCMenuItemSpriteExtra::create(moveQuarterRight_spr, moveQuarterRight_spr, self, menu_selector(EditorUI::Callback::moveObjectQuarterRight));
	moveQuarterRight_btn->setPosition({ -65, -(winSize.height / 2) + 28 });

	auto moveEightUp_spr = CCSprite::create("GJ_button_01.png");
	moveEightUp_spr->setScale(0.9f);
	auto moveEightUp_spr2 = CCSprite::createWithSpriteFrameName("edit_upBtn2_001.png");
	moveEightUp_spr2->setPosition({ moveEightUp_spr->getContentSize().width / 2, (moveEightUp_spr->getContentSize().height / 2) + 2 });
	auto moveEightUp_lbl = CCLabelBMFont::create("1/8", "bigFont.fnt");
	moveEightUp_lbl->setScale(0.5f);
	moveEightUp_lbl->setPosition({ moveEightUp_spr->getContentSize().width / 2, (moveEightUp_spr->getContentSize().height / 2) - 9 });
	moveEightUp_spr->addChild(moveEightUp_spr2);
	moveEightUp_spr->addChild(moveEightUp_lbl);
	auto moveEightUp_btn = gd::CCMenuItemSpriteExtra::create(moveEightUp_spr, moveEightUp_spr, self, menu_selector(EditorUI::Callback::moveObjectEightUp));
	moveEightUp_btn->setPosition({ -25, -(winSize.height / 2) + 28 });

	auto moveEightDown_spr = CCSprite::create("GJ_button_01.png");
	moveEightDown_spr->setScale(0.9f);
	auto moveEightDown_spr2 = CCSprite::createWithSpriteFrameName("edit_downBtn2_001.png");
	moveEightDown_spr2->setPosition({ moveEightDown_spr->getContentSize().width / 2, (moveEightDown_spr->getContentSize().height / 2) + 2 });
	auto moveEightDown_lbl = CCLabelBMFont::create("1/8", "bigFont.fnt");
	moveEightDown_lbl->setScale(0.5f);
	moveEightDown_lbl->setPosition({ moveEightDown_spr->getContentSize().width / 2, (moveEightDown_spr->getContentSize().height / 2) - 9 });
	moveEightDown_spr->addChild(moveEightDown_spr2);
	moveEightDown_spr->addChild(moveEightDown_lbl);
	auto moveEightDown_btn = gd::CCMenuItemSpriteExtra::create(moveEightDown_spr, moveEightDown_spr, self, menu_selector(EditorUI::Callback::moveObjectEightDown));
	moveEightDown_btn->setPosition({ 15, -(winSize.height / 2) + 28 });

	auto moveEightLeft_spr = CCSprite::create("GJ_button_01.png");
	moveEightLeft_spr->setScale(0.9f);
	auto moveEightLeft_spr2 = CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
	moveEightLeft_spr2->setPosition({ moveEightLeft_spr->getContentSize().width / 2, (moveEightLeft_spr->getContentSize().height / 2) + 2 });
	auto moveEightLeft_lbl = CCLabelBMFont::create("1/8", "bigFont.fnt");
	moveEightLeft_lbl->setScale(0.5f);
	moveEightLeft_lbl->setPosition({ moveEightLeft_spr->getContentSize().width / 2, (moveEightLeft_spr->getContentSize().height / 2) - 9 });
	moveEightLeft_spr->addChild(moveEightLeft_spr2);
	moveEightLeft_spr->addChild(moveEightLeft_lbl);
	auto moveEightLeft_btn = gd::CCMenuItemSpriteExtra::create(moveEightLeft_spr, moveEightLeft_spr, self, menu_selector(EditorUI::Callback::moveObjectEightLeft));
	moveEightLeft_btn->setPosition({ 55, -(winSize.height / 2) + 28 });

	auto moveEightRight_spr = CCSprite::create("GJ_button_01.png");
	moveEightRight_spr->setScale(0.9f);
	auto moveEightRight_spr2 = CCSprite::createWithSpriteFrameName("edit_rightBtn2_001.png");
	moveEightRight_spr2->setPosition({ moveEightRight_spr->getContentSize().width / 2, (moveEightRight_spr->getContentSize().height / 2) + 2 });
	auto moveEightRight_lbl = CCLabelBMFont::create("1/8", "bigFont.fnt");
	moveEightRight_lbl->setScale(0.5f);
	moveEightRight_lbl->setPosition({ moveEightRight_spr->getContentSize().width / 2, (moveEightRight_spr->getContentSize().height / 2) - 9 });
	moveEightRight_spr->addChild(moveEightRight_spr2);
	moveEightRight_spr->addChild(moveEightRight_lbl);
	auto moveEightRight_btn = gd::CCMenuItemSpriteExtra::create(moveEightRight_spr, moveEightRight_spr, self, menu_selector(EditorUI::Callback::moveObjectEightRight));
	moveEightRight_btn->setPosition({ 95, -(winSize.height / 2) + 28 });

	movePageMenu->addChild(moveHalfUp_btn);
	movePageMenu->addChild(moveHalfDown_btn);
	movePageMenu->addChild(moveHalfLeft_btn);
	movePageMenu->addChild(moveHalfRight_btn);

	movePageMenu->addChild(moveQuarterUp_btn);
	movePageMenu->addChild(moveQuarterDown_btn);
	movePageMenu->addChild(moveQuarterLeft_btn);
	movePageMenu->addChild(moveQuarterRight_btn);

	movePageMenu->addChild(moveEightUp_btn);
	movePageMenu->addChild(moveEightDown_btn);
	movePageMenu->addChild(moveEightLeft_btn);
	movePageMenu->addChild(moveEightRight_btn);

	CCArray* movePage2CCArray = CCArray::create();
	auto moveButtonPage2 = gd::ButtonPage::create(movePage2CCArray);
	moveButtonPage2->setPositionX(moveButtonPage->getPositionX() + CCDirector::sharedDirector()->getWinSize().width / 2);
	auto movePageMenu2 = reinterpret_cast<CCMenu*>(moveButtonPage2->getChildren()->objectAtIndex(0));
	auto pageDot2 = CCSprite::create("smallDot.png");

	extendLayer->addChild(moveButtonPage2);
	pagesArray->insertObject(moveButtonPage2, 3);
	dotsArray->addObject(pageDot2);
	dotsNode->addChild(pageDot2);

	auto moveSmallerUp_spr = CCSprite::create("GJ_button_01.png");
	moveSmallerUp_spr->setScale(0.9f);
	auto moveSmallerUp_spr2 = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
	moveSmallerUp_spr2->setPosition({ moveSmallerUp_spr->getContentSize().width / 2, (moveSmallerUp_spr->getContentSize().height / 2) + 2 });
	auto moveSmallerUp_lbl = CCLabelBMFont::create("0.5", "bigFont.fnt");
	moveSmallerUp_lbl->setScale(0.5f);
	moveSmallerUp_lbl->setPosition({ moveSmallerUp_spr->getContentSize().width / 2, (moveSmallerUp_spr->getContentSize().height / 2) - 9 });
	moveSmallerUp_spr->addChild(moveSmallerUp_spr2);
	moveSmallerUp_spr->addChild(moveSmallerUp_lbl);
	auto moveSmallerUp_btn = gd::CCMenuItemSpriteExtra::create(moveSmallerUp_spr, moveSmallerUp_spr, self, menu_selector(EditorUI::Callback::moveObjectSmallerUp));
	moveSmallerUp_btn->setPosition({ -105, -(winSize.height / 2) + 68 });

	auto moveSmallerDown_spr = CCSprite::create("GJ_button_01.png");
	moveSmallerDown_spr->setScale(0.9f);
	auto moveSmallerDown_spr2 = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
	moveSmallerDown_spr2->setPosition({ moveSmallerDown_spr->getContentSize().width / 2, (moveSmallerDown_spr->getContentSize().height / 2) + 2 });
	auto moveSmallerDown_lbl = CCLabelBMFont::create("0.5", "bigFont.fnt");
	moveSmallerDown_lbl->setScale(0.5f);
	moveSmallerDown_lbl->setPosition({ moveSmallerDown_spr->getContentSize().width / 2, (moveSmallerDown_spr->getContentSize().height / 2) - 9 });
	moveSmallerDown_spr->addChild(moveSmallerDown_spr2);
	moveSmallerDown_spr->addChild(moveSmallerDown_lbl);
	auto moveSmallerDown_btn = gd::CCMenuItemSpriteExtra::create(moveSmallerDown_spr, moveSmallerDown_spr, self, menu_selector(EditorUI::Callback::moveObjectSmallerDown));
	moveSmallerDown_btn->setPosition({ -65, -(winSize.height / 2) + 68 });

	auto moveSmallerLeft_spr = CCSprite::create("GJ_button_01.png");
	moveSmallerLeft_spr->setScale(0.9f);
	auto moveSmallerLeft_spr2 = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
	moveSmallerLeft_spr2->setPosition({ moveSmallerLeft_spr->getContentSize().width / 2, (moveSmallerLeft_spr->getContentSize().height / 2) + 2 });
	auto moveSmallerLeft_lbl = CCLabelBMFont::create("0.5", "bigFont.fnt");
	moveSmallerLeft_lbl->setScale(0.5f);
	moveSmallerLeft_lbl->setPosition({ moveSmallerLeft_spr->getContentSize().width / 2, (moveSmallerLeft_spr->getContentSize().height / 2) - 9 });
	moveSmallerLeft_spr->addChild(moveSmallerLeft_spr2);
	moveSmallerLeft_spr->addChild(moveSmallerLeft_lbl);
	auto moveSmallerLeft_btn = gd::CCMenuItemSpriteExtra::create(moveSmallerLeft_spr, moveSmallerLeft_spr, self, menu_selector(EditorUI::Callback::moveObjectSmallerLeft));
	moveSmallerLeft_btn->setPosition({ -25, -(winSize.height / 2) + 68 });

	auto moveSmallerRight_spr = CCSprite::create("GJ_button_01.png");
	moveSmallerRight_spr->setScale(0.9f);
	auto moveSmallerRight_spr2 = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
	moveSmallerRight_spr2->setPosition({ moveSmallerRight_spr->getContentSize().width / 2, (moveSmallerRight_spr->getContentSize().height / 2) + 2 });
	auto moveSmallerRight_lbl = CCLabelBMFont::create("0.5", "bigFont.fnt");
	moveSmallerRight_lbl->setScale(0.5f);
	moveSmallerRight_lbl->setPosition({ moveSmallerRight_spr->getContentSize().width / 2, (moveSmallerRight_spr->getContentSize().height / 2) - 9 });
	moveSmallerRight_spr->addChild(moveSmallerRight_spr2);
	moveSmallerRight_spr->addChild(moveSmallerRight_lbl);
	auto moveSmallerRight_btn = gd::CCMenuItemSpriteExtra::create(moveSmallerRight_spr, moveSmallerRight_spr, self, menu_selector(EditorUI::Callback::moveObjectSmallerRight));
	moveSmallerRight_btn->setPosition({ 15, -(winSize.height / 2) + 68 });

	auto moveQUnitUp_spr = CCSprite::create("GJ_button_01.png");
	moveQUnitUp_spr->setScale(0.9f);
	auto moveQUnitUp_spr2 = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
	moveQUnitUp_spr2->setPosition({ moveQUnitUp_spr->getContentSize().width / 2, (moveQUnitUp_spr->getContentSize().height / 2) + 2 });
	auto moveQUnitUp_lbl = CCLabelBMFont::create("Unit", "bigFont.fnt");
	moveQUnitUp_lbl->setScale(0.5f);
	moveQUnitUp_lbl->setPosition({ moveQUnitUp_spr->getContentSize().width / 2, (moveQUnitUp_spr->getContentSize().height / 2) - 9 });
	moveQUnitUp_spr->addChild(moveQUnitUp_spr2);
	moveQUnitUp_spr->addChild(moveQUnitUp_lbl);
	auto moveQUnitUp_btn = gd::CCMenuItemSpriteExtra::create(moveQUnitUp_spr, moveQUnitUp_spr, self, menu_selector(EditorUI::Callback::moveObjectQUnitUp));
	moveQUnitUp_btn->setPosition({ 55, -(winSize.height / 2) + 68 });

	auto moveQUnitDown_spr = CCSprite::create("GJ_button_01.png");
	moveQUnitDown_spr->setScale(0.9f);
	auto moveQUnitDown_spr2 = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
	moveQUnitDown_spr2->setPosition({ moveQUnitDown_spr->getContentSize().width / 2, (moveQUnitDown_spr->getContentSize().height / 2) + 2 });
	auto moveQUnitDown_lbl = CCLabelBMFont::create("Unit", "bigFont.fnt");
	moveQUnitDown_lbl->setScale(0.5f);
	moveQUnitDown_lbl->setPosition({ moveQUnitDown_spr->getContentSize().width / 2, (moveQUnitDown_spr->getContentSize().height / 2) - 9 });
	moveQUnitDown_spr->addChild(moveQUnitDown_spr2);
	moveQUnitDown_spr->addChild(moveQUnitDown_lbl);
	auto moveQUnitDown_btn = gd::CCMenuItemSpriteExtra::create(moveQUnitDown_spr, moveQUnitDown_spr, self, menu_selector(EditorUI::Callback::moveObjectQUnitDown));
	moveQUnitDown_btn->setPosition({ 95, -(winSize.height / 2) + 68 });

	auto moveQUnitLeft_spr = CCSprite::create("GJ_button_01.png");
	moveQUnitLeft_spr->setScale(0.9f);
	auto moveQUnitLeft_spr2 = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
	moveQUnitLeft_spr2->setPosition({ moveQUnitLeft_spr->getContentSize().width / 2, (moveQUnitLeft_spr->getContentSize().height / 2) + 2 });
	auto moveQUnitLeft_lbl = CCLabelBMFont::create("Unit", "bigFont.fnt");
	moveQUnitLeft_lbl->setScale(0.5f);
	moveQUnitLeft_lbl->setPosition({ moveQUnitLeft_spr->getContentSize().width / 2, (moveQUnitLeft_spr->getContentSize().height / 2) - 9 });
	moveQUnitLeft_spr->addChild(moveQUnitLeft_spr2);
	moveQUnitLeft_spr->addChild(moveQUnitLeft_lbl);
	auto moveQUnitLeft_btn = gd::CCMenuItemSpriteExtra::create(moveQUnitLeft_spr, moveQUnitLeft_spr, self, menu_selector(EditorUI::Callback::moveObjectQUnitLeft));
	moveQUnitLeft_btn->setPosition({ -105, -(winSize.height / 2) + 28 });

	auto moveQUnitRight_spr = CCSprite::create("GJ_button_01.png");
	moveQUnitRight_spr->setScale(0.9f);
	auto moveQUnitRight_spr2 = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
	moveQUnitRight_spr2->setPosition({ moveQUnitRight_spr->getContentSize().width / 2, (moveQUnitRight_spr->getContentSize().height / 2) + 2 });
	auto moveQUnitRight_lbl = CCLabelBMFont::create("Unit", "bigFont.fnt");
	moveQUnitRight_lbl->setScale(0.5f);
	moveQUnitRight_lbl->setPosition({ moveQUnitRight_spr->getContentSize().width / 2, (moveQUnitRight_spr->getContentSize().height / 2) - 9 });
	moveQUnitRight_spr->addChild(moveQUnitRight_spr2);
	moveQUnitRight_spr->addChild(moveQUnitRight_lbl);
	auto moveQUnitRight_btn = gd::CCMenuItemSpriteExtra::create(moveQUnitRight_spr, moveQUnitRight_spr, self, menu_selector(EditorUI::Callback::moveObjectQUnitRight));
	moveQUnitRight_btn->setPosition({ -65, -(winSize.height / 2) + 28 });

	auto rotate45CW_base = CCSprite::create("GJ_button_01.png");
	auto rotate45CW_second = CCSprite::createWithSpriteFrameName("edit_cwBtn_001.png");
	if (!rotate45CW_second->initWithFile("edit_rotate45rBtn_001.png")) {
		rotate45CW_second->initWithSpriteFrameName("edit_cwBtn_001.png");
	}
	rotate45CW_base->addChild(rotate45CW_second);
	rotate45CW_base->setScale(0.9f);
	rotate45CW_second->setPosition(rotate45CW_base->getContentSize() / 2);
	rotate45CW_second->setPositionY(rotate45CW_second->getPositionY() + 2);
	auto rotate45CW_btn = gd::CCMenuItemSpriteExtra::create(rotate45CW_base, rotate45CW_base, self, menu_selector(EditorUI::Callback::rotate45CW));
	rotate45CW_btn->setPosition({ -25, -(winSize.height / 2) + 28 });

	auto rotate45CCW_base = CCSprite::create("GJ_button_01.png");
	auto rotate45CCW_second = CCSprite::createWithSpriteFrameName("edit_ccwBtn_001.png");
	if (!rotate45CCW_second->initWithFile("edit_rotate45lBtn_001.png")) {
		rotate45CCW_second->initWithSpriteFrameName("edit_ccwBtn_001.png");
	}
	rotate45CCW_base->addChild(rotate45CCW_second);
	rotate45CCW_base->setScale(0.9f);
	rotate45CCW_second->setPosition(rotate45CCW_base->getContentSize() / 2);
	rotate45CCW_second->setPositionY(rotate45CCW_second->getPositionY() + 2);
	auto rotate45CCW_btn = gd::CCMenuItemSpriteExtra::create(rotate45CCW_base, rotate45CCW_base, self, menu_selector(EditorUI::Callback::rotate45CCW));
	rotate45CCW_btn->setPosition({ 15, -(winSize.height / 2) + 28 });

	auto rotate265CW_base = CCSprite::create("GJ_button_01.png");
	auto rotate265CW_second = CCSprite::createWithSpriteFrameName("edit_cwBtn_001.png");
	if (!rotate265CW_second->initWithFile("edit_rotate265rBtn_001.png")) {
		rotate265CW_second->initWithSpriteFrameName("edit_cwBtn_001.png");
	}
	rotate265CW_base->addChild(rotate265CW_second);
	rotate265CW_base->setScale(0.9f);
	rotate265CW_second->setPosition(rotate265CW_base->getContentSize() / 2);
	rotate265CW_second->setPositionY(rotate265CW_second->getPositionY() + 2);
	auto rotate265CW_btn = gd::CCMenuItemSpriteExtra::create(rotate265CW_base, rotate265CW_base, self, menu_selector(EditorUI::Callback::rotate265CW));
	rotate265CW_btn->setPosition({ 55, -(winSize.height / 2) + 28 });

	auto rotate265CCW_base = CCSprite::create("GJ_button_01.png");
	auto rotate265CCW_second = CCSprite::createWithSpriteFrameName("edit_ccwBtn_001.png");
	if (!rotate265CCW_second->initWithFile("edit_rotate265lBtn_001.png")) {
		rotate265CCW_second->initWithSpriteFrameName("edit_ccwBtn_001.png");
	}
	rotate265CCW_base->addChild(rotate265CCW_second);
	rotate265CCW_base->setScale(0.9f);
	rotate265CCW_second->setPosition(rotate265CCW_base->getContentSize() / 2);
	rotate265CCW_second->setPositionY(rotate265CCW_second->getPositionY() + 2);
	auto rotate265CCW_btn = gd::CCMenuItemSpriteExtra::create(rotate265CCW_base, rotate265CCW_base, self, menu_selector(EditorUI::Callback::rotate265CCW));
	rotate265CCW_btn->setPosition({ 95, -(winSize.height / 2) + 28 });

	movePageMenu2->addChild(moveSmallerUp_btn);
	movePageMenu2->addChild(moveSmallerDown_btn);
	movePageMenu2->addChild(moveSmallerLeft_btn);
	movePageMenu2->addChild(moveSmallerRight_btn);

	movePageMenu2->addChild(moveQUnitUp_btn);
	movePageMenu2->addChild(moveQUnitDown_btn);
	movePageMenu2->addChild(moveQUnitLeft_btn);
	movePageMenu2->addChild(moveQUnitRight_btn);

	movePageMenu2->addChild(rotate45CW_btn);
	movePageMenu2->addChild(rotate45CCW_btn);
	movePageMenu2->addChild(rotate265CW_btn);
	movePageMenu2->addChild(rotate265CCW_btn);
}

void __fastcall EditorUI::selectObjectH(gd::EditorUI* self, void* edx, gd::GameObject* object) {
	auto selectedCustomMode = gd::GameManager::sharedState()->getIntGameVariable("0005");
	if (selectedCustomMode != 3) gd::GameManager::sharedState()->setIntGameVariable("0006", 0);
	int selectFilterObject = gd::GameManager::sharedState()->getIntGameVariable("0006");

	bool selectFilter_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::SELECT_FILTER);


	if ((selectFilterObject != 0) && selectFilter_enabled) {
		if (object->getObjectID() == selectFilterObject) return EditorUI::selectObject(self, object);
	}
	else {
		EditorUI::selectObject(self, object);
	}
	
}

void __fastcall EditorUI::selectObjectsH(gd::EditorUI* self, void* edx, CCArray* objects) {
	auto selectedCustomMode = gd::GameManager::sharedState()->getIntGameVariable("0005");
	if (selectedCustomMode != 3) gd::GameManager::sharedState()->setIntGameVariable("0006", 0);
	int selectFilterObject = gd::GameManager::sharedState()->getIntGameVariable("0006");

	bool selectFilter_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::SELECT_FILTER);


	if ((selectFilterObject != 0) && selectFilter_enabled) {
		auto filteredObjects = CCArray::create();
		for (int i = 0; i < objects->count(); i++) {
			if (reinterpret_cast<gd::GameObject*>(objects->objectAtIndex(i))->getObjectID() == selectFilterObject) {
				filteredObjects->addObject(objects->objectAtIndex(i));
			}
		}
		return EditorUI::selectObjects(self, filteredObjects);
	}
	else return EditorUI::selectObjects(self, objects);

	EditorUI::selectObjects(self, objects);
}

void EditorPauseLayer::Callback::VanillaSelectAllButton(CCObject*)
{
	auto leveleditor = from<gd::LevelEditorLayer*>(editorPauseLayer, 0x1A8);
	auto editorUI = leveleditor->getEditorUI();

	auto objs = CCArray::create();
	for (int i = 0; i < (leveleditor->getAllObjects()->count()); i++)
	{
		objs->addObjectsFromArray(reinterpret_cast<CCArray*>(leveleditor->getAllObjects()->objectAtIndex(i)));
	}

	auto objs2 = CCArray::create();
	for (int i = 0; i < (objs->count()); i++)
	{
		if (reinterpret_cast<gd::GameObject*>(objs->objectAtIndex(i))->getGroup() == leveleditor->getLayerGroup() || leveleditor->getLayerGroup() == -1)
			objs2->addObject(objs->objectAtIndex(i));
	}
	editorUI->selectObjects(objs2);
	editorUI->updateButtons();
}

void EditorPauseLayer::Callback::PreviewModeToggler(CCObject*) {
	setting().onEditorPreview = !setting().onEditorPreview;
}

bool SEP = false;

void EditorPauseLayer::Callback::SmallEditorStepToggler(CCObject*) {
	gd::GameManager::sharedState()->toggleGameVariable("0035");
	//SEP = !SEP;
	//	if (SEP) {
	//		gd::GameManager::sharedState()->setGameVariable("0035", true);
	//	}
	//	else {
	//		gd::GameManager::sharedState()->setGameVariable("0035", false);
	//	}
}

void EditorPauseLayer::Callback::selectFilterToggle(CCObject*) {
	gd::GameManager::sharedState()->toggleGameVariable(GameVariable::SELECT_FILTER);
}

auto EPMTogglerSprite(CCSprite* toggleOn, CCSprite* toggleOff)
{
	return (setting().onEditorPreview) ? toggleOn : toggleOff;
}

auto SEPTogglerSprite(CCSprite* toggleOn, CCSprite* toggleOff)
{
	bool smallStep_enabled = gd::GameManager::sharedState()->getGameVariable("0035");
	return (smallStep_enabled) ? toggleOn : toggleOff;
	//return (SEP) ? toggleOn : toggleOff;
}

auto selectFilterTogglerSprite(CCSprite* toggleOn, CCSprite* toggleOff) {
	bool selectFilter_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::SELECT_FILTER);
	return (selectFilter_enabled) ? toggleOn : toggleOff;
}

class SaveLevelAlertProtocol : public gd::FLAlertLayerProtocol {
public:
	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override {
		if (btn2) {
			editorPauseLayer->saveLevel();
		}
	}
};

SaveLevelAlertProtocol saveLevelProtocol;

void EditorPauseLayer::Callback::onSaveLevel(CCObject* obj) {
	gd::FLAlertLayer::create(&saveLevelProtocol, "Save", "<cy>Save</c> the level?", "NO", "YES", 300.f, false, 140.f)->show();
}

void __fastcall EditorPauseLayer::customSetup_H(gd::EditorPauseLayer* self) {
	editorPauseLayer = self;
	EditorPauseLayer::customSetup(self);

	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();
	auto togglerMenu = CCMenu::create();
	togglerMenu->setPosition({ director->getScreenLeft(), director->getScreenBottom() });

	auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

	auto EPMButton = gd::CCMenuItemToggler::create(EPMTogglerSprite(toggleOn, toggleOff), EPMTogglerSprite(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::PreviewModeToggler));
	auto EPMLabel = CCLabelBMFont::create("Preview Mode", "bigFont.fnt");
	EPMButton->setScale(0.70f);
	EPMButton->setPosition({ 30, 180 });
	EPMLabel->setScale(0.35f);
	EPMLabel->setPosition({ 50,180 });
	EPMLabel->setAnchorPoint({ 0.f, 0.5f });
	togglerMenu->addChild(EPMButton);
	togglerMenu->addChild(EPMLabel);

	auto SFButton = gd::CCMenuItemToggler::create(selectFilterTogglerSprite(toggleOn, toggleOff), selectFilterTogglerSprite(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::selectFilterToggle));
	auto SFLabel = CCLabelBMFont::create("Select Filter", "bigFont.fnt");
	SFButton->setScale(0.70f);
	SFButton->setPosition({ 30, 150 });
	SFLabel->setScale(0.35f);
	SFLabel->setPosition({ 50,150 });
	SFLabel->setAnchorPoint({ 0.f, 0.5f });
	togglerMenu->addChild(SFButton);
	togglerMenu->addChild(SFLabel);

	auto SEPButton = gd::CCMenuItemToggler::create(SEPTogglerSprite(toggleOn, toggleOff), SEPTogglerSprite(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::SmallEditorStepToggler));
	auto SEPLabel = CCLabelBMFont::create("Small Editor Step", "bigFont.fnt");
	SEPButton->setScale(0.70f);
	SEPButton->setPosition({ 30, 120 });
	SEPLabel->setScale(0.35f);
	SEPLabel->setPosition({ 50, 120 });
	SEPLabel->setAnchorPoint({ 0.f, 0.5f });
	togglerMenu->addChild(SEPButton);
	togglerMenu->addChild(SEPLabel);

	constexpr auto pshandler = [](CCObject* self, CCObject*) {
		auto text = clipboard::read();
		auto editor = reinterpret_cast<gd::LevelEditorLayer*>(reinterpret_cast<CCNode*>(self)->getParent());
		editor->getEditorUI()->pasteObjects(text);
		};

	auto pssprite = gd::ButtonSprite::create("Paste\nString", 0x28, 0, 0.6f, true, "bigFont.fnt", "GJ_button_04.png", 30.0);
	auto psbutton = gd::CCMenuItemSpriteExtra::create(pssprite, nullptr, self, to_handler<SEL_MenuHandler, pshandler>);

	auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	auto optionsBtn = gd::CCMenuItemSpriteExtra::create(optionsSpr, nullptr, self, menu_selector(gd::MenuLayer::onOptions));
	optionsBtn->setPosition({ -38, -66 });
	optionsSpr->setScale(.66f);

	menu->setPosition({ director->getScreenRight(), director->getScreenTop() });
	psbutton->setPosition({ -50.f, -30.f });

	menu->addChild(psbutton);
	menu->addChild(optionsBtn);



	auto levellength = CCLabelBMFont::create("", "goldFont.fnt");
	levellength->setString(CCString::createWithFormat("", 0)->getCString());
	levellength->setTag(49001);
	levellength->setAnchorPoint({ 0, 0.5f });
	levellength->setPosition({ director->getScreenLeft() + 10, director->getScreenTop() - 30 });
	levellength->setScale(0.5f);
	self->addChild(levellength);

	if (levellength) {
		auto levelLength = levelEditorLayer->getLevel()->getLevelLength();
		switch (levelLength) {
		case 0:
			reinterpret_cast<CCLabelBMFont*>(levellength)->setString(CCString::createWithFormat("Tiny", levelLength)->getCString());
			break;
		case 1:
			reinterpret_cast<CCLabelBMFont*>(levellength)->setString(CCString::createWithFormat("Short", levelLength)->getCString());
			break;
		case 2:
			reinterpret_cast<CCLabelBMFont*>(levellength)->setString(CCString::createWithFormat("Medium", levelLength)->getCString());
			break;
		case 3:
			reinterpret_cast<CCLabelBMFont*>(levellength)->setString(CCString::createWithFormat("Long", levelLength)->getCString());
			break;
		case 4:
			reinterpret_cast<CCLabelBMFont*>(levellength)->setString(CCString::createWithFormat("Extra-Long", levelLength)->getCString());
			break;
		default:
			break;
		}
	}

	auto bottommenu = from<CCMenu*>(self->bpmButton(), 0xac);
	auto keysButton = (gd::CCMenuItemSpriteExtra*)bottommenu->getChildren()->objectAtIndex(7);
	keysButton->setPositionY(70);

	auto vanillaSelectAllSprite = gd::ButtonSprite::create("Select \nAll", 0x32, 0, 0.6f, true, "bigFont.fnt", "GJ_button_04.png", 30.0);
	auto vanillaSelectAllButton = gd::CCMenuItemSpriteExtra::create(vanillaSelectAllSprite, nullptr, self, menu_selector(EditorPauseLayer::Callback::VanillaSelectAllButton));
	vanillaSelectAllButton->setPosition({ keysButton->getPositionX(), 30 });

	bottommenu->addChild(vanillaSelectAllButton);

	self->addChild(togglerMenu);
	self->addChild(menu);

	auto mainMenu = reinterpret_cast<CCMenu*>(self->getChildren()->objectAtIndex(0));

	auto saveLevel_spr = gd::ButtonSprite::create("Save", 0xdc, 0, 1.f, true, "goldFont.fnt", "GJ_button_01.png", 30.f);
	auto saveLevel_btn = gd::CCMenuItemSpriteExtra::create(saveLevel_spr, nullptr, self, menu_selector(EditorPauseLayer::Callback::onSaveLevel));

	auto onResume_btn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(0));
	auto onSaveAndPlay_btn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(1));
	auto onSaveAndExit_btn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(2));
	auto onExitNoSave_btn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(3));

	onResume_btn->setPositionY(90.f);
	onSaveAndPlay_btn->setPositionY(45.f);
	onSaveAndExit_btn->setPositionY(0.f);
	saveLevel_btn->setPositionY(-45.f);
	onExitNoSave_btn->setPositionY(-90.f);

	auto bpmButton_on = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(bottommenu->getChildren()->objectAtIndex(0));
	auto helpButton = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(bottommenu->getChildren()->objectAtIndex(1));
	auto bpmButton_off = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(bottommenu->getChildren()->objectAtIndex(2));

	bpmButton_off->setPositionY(bpmButton_off->getPositionY() - 12);
	bpmButton_on->setPositionY(bpmButton_on->getPositionY() - 12);
	helpButton->setPositionY(helpButton->getPositionY() - 12);

	mainMenu->addChild(saveLevel_btn);
}

void __fastcall EditorPauseLayer::onSaveAndPlay_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
	auto lel = self->getEditorLayer();
	if (lel) {
		auto playerDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(124));
		auto objDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(125));
		playerDrawNode->clear();
		objDrawNode->clear();
	}
	editUI = nullptr;
	EditorPauseLayer::onSaveAndPlay(self, sender);
}

void __fastcall EditorPauseLayer::onSaveAndExit_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
	auto lel = self->getEditorLayer();
	if (lel) {
		auto playerDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(124));
		auto objDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(125));
		playerDrawNode->clear();
		objDrawNode->clear();
	}
	editUI = nullptr;
	EditorPauseLayer::onSaveAndExit(self, sender);
}

void __fastcall EditorPauseLayer::onExitNoSave_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
	auto lel = self->getEditorLayer();
	if (lel) {
		auto playerDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(124));
		auto objDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(125));
		playerDrawNode->clear();
		objDrawNode->clear();
	}
	editUI = nullptr;
	EditorPauseLayer::onExitNoSave(self, sender);
}

void __fastcall EditorPauseLayer::onExitEditor_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
	auto lel = self->getEditorLayer();
	if (lel) {
		auto playerDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(124));
		auto objDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(125));
		playerDrawNode->clear();
		objDrawNode->clear();
	}
	editUI = nullptr;
	EditorPauseLayer::onExitEditor(self, sender);
}

void __fastcall EditorPauseLayer::keyDown_H(gd::EditorPauseLayer* self, void* edx, enumKeyCodes key) {

	if (key == KEY_Escape) {
		editorPauseLayer->onResume(nullptr);
	}
	else {
		EditorPauseLayer::keyDown(self, key);
	}
}

//bool isEasedScrollLayer(gd::BoomScrollLayer* self) {
//	if (!self->getParent()) return false;
//
//	if (vtable_cast(self->getParent(), 0x11c804)) return false;
//
//	if (vtable_cast(self->getParent(), 0x114b90)) return false;
//
//	return true;
//}
//
//class BoomScrollLayer_CB : public gd::BoomScrollLayer {
//public:
//	void onGoToPage(CCObject* sender) {
//		auto p = sender->getTag();
//		if (isEasedScrollLayer(this)) this->moveToPage(p);
//		else {
//			this->instantMoveToPage(p - 1);
//			this->instantMoveToPage(p);
//		}
//	}
//};

void __fastcall Scheduler::update_H(CCScheduler* self, void* edx, float dt) {
	scheduler = self;
	Scheduler::update(self, dt);

	auto play_layer = gd::GameManager::sharedState()->getPlayLayer();
	const auto bar = gd::GameManager::sharedState()->getShowProgressBar();
	auto size = CCDirector::sharedDirector()->getWinSize();

	if (play_layer) {
		auto labelsMenu = reinterpret_cast<CCMenu*>(play_layer->getChildByTag(7900));
		auto percentLabel = reinterpret_cast<CCLabelBMFont*>(play_layer->getChildByTag(4571));
		auto cheatIndicator = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45072));

		if (percentLabel) {
			const auto value = play_layer->player1()->getPositionX() / play_layer->levelLength() * 100.f;

			percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
			percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });

			if (value < 100.0f) percentLabel->setString(CCString::createWithFormat("%.2f%%", value)->getCString());
			else percentLabel->setString(CCString::create("100.00%")->getCString());

			if (setting().onShowPercentage) {
				percentLabel->setVisible(1);
			}
			else {
				percentLabel->setVisible(0);
			}
		}

		if (cheatIndicator)
		{
			ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F04E9), &setting().CurrentNoclipByte, 1, 0);
			cheatIndicator->setColor({ 0, 255, 0 });

			if (setting().NoclipByte != setting().CurrentNoclipByte && setting().onNoclipOutOfMe == false) { setting().cheatsCount++; setting().beforeRestartCheatsCount++; setting().onNoclipOutOfMe = true; }
			else if (setting().NoclipByte == setting().CurrentNoclipByte && setting().onNoclipOutOfMe == true) { setting().cheatsCount--; setting().onNoclipOutOfMe = false; }

			//no cheats, no before restart cheats, no safe mode
			if (setting().cheatsCount == 0 &&
				setting().beforeRestartCheatsCount == 0 &&
				setting().NoclipByte == setting().CurrentNoclipByte &&
				!(setting().onSafeMode || setting().isSafeMode))
				cheatIndicator->setColor({ 0, 255, 0 });

			//no cheats, no before restart cheats, safe mode
			else if (setting().cheatsCount == 0 &&
				setting().beforeRestartCheatsCount == 0 &&
				setting().NoclipByte == setting().CurrentNoclipByte &&
				(setting().onSafeMode || setting().isSafeMode))
				cheatIndicator->setColor({ 255, 255, 0 });

			//no cheats
			else if (setting().cheatsCount == 0 &&
				setting().NoclipByte == setting().CurrentNoclipByte)
				cheatIndicator->setColor({ 255, 128, 0 });

			else if (setting().cheatsCount != 0 &&
				(setting().onSafeMode || setting().isSafeMode))
				cheatIndicator->setColor({ 255, 128, 0 });

			else cheatIndicator->setColor({ 255, 0, 0 });
		}
	}

	if (editUI) {
		auto lel = editUI->getLevelEditorLayer();
		auto moreObjInfoMenu = reinterpret_cast<CCMenu*>(editUI->getChildByTag(8900));
		auto objcolorid = moreObjInfoMenu->getChildByTag(45011);
		auto objgroupid = moreObjInfoMenu->getChildByTag(45012);
		auto objposx = moreObjInfoMenu->getChildByTag(45013);
		auto objposy = moreObjInfoMenu->getChildByTag(45014);
		auto objrot = moreObjInfoMenu->getChildByTag(45015);
		auto objid = moreObjInfoMenu->getChildByTag(45016);
		auto objaddr = moreObjInfoMenu->getChildByTag(45017);
		auto objcounter = moreObjInfoMenu->getChildByTag(45018);
		//auto customObj = moreObjInfoMenu->getChildByTag(45050);

		auto leftMenu = from<CCMenu*>(editUI->getRedoBtn(), 0xac);
		auto deleteBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(leftMenu->getChildByTag(45030));

		auto rightMenu = from<CCMenu*>(editUI->getDeselectBtn(), 0xac);
		auto onBaseLayerBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(rightMenu->getChildByTag(45028));

		if (lel) {
			auto playerDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(124));
			playerDrawNode->clear();
			if ((setting().onPlayerHitbox && setting().onHitboxes))
			{
				if (lel->player1())
				{
					Hitboxes::drawPlayerHitbox(lel->player1(), playerDrawNode);
				}
				if (lel->player2())
				{
					Hitboxes::drawPlayerHitbox(lel->player2(), playerDrawNode);
				}
			}
			auto secarr = lel->getLevelSections();
			auto arrcount = secarr->count();
			auto objectDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(125));
			objectDrawNode->clear();
			if (setting().onHitboxes) {
				//if (arrcount != 0) {
				auto layer = lel->gameLayer();
				float xp = -layer->getPositionX() / layer->getScale();
				if (setting().onSolidsHitbox) {
					for (int i = lel->sectionForPos(xp) - (5 / layer->getScale()); i < lel->sectionForPos(xp) + (6 / layer->getScale()); i++) {
						if (i < 0) continue;
						if (i >= arrcount) break;
						auto objAtInd = secarr->objectAtIndex(i);
						auto objarr = reinterpret_cast<CCArray*>(objAtInd);

						for (int j = 0; j < objarr->count(); j++) {
							auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
							Hitboxes::drawSolidsObjectHitbox(obj, objectDrawNode);
						}
					}
				}

				if (setting().onHazardsHitbox) {
					for (int i = lel->sectionForPos(xp) - (5 / layer->getScale()); i < lel->sectionForPos(xp) + (6 / layer->getScale()); i++) {
						if (i < 0) continue;
						if (i >= arrcount) break;
						auto objAtInd = secarr->objectAtIndex(i);
						auto objarr = reinterpret_cast<CCArray*>(objAtInd);

						for (int j = 0; j < objarr->count(); j++) {
							auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
							Hitboxes::drawHazardsObjectHitbox(obj, objectDrawNode);
						}
					}
				}

				if (setting().onSpecialsHitbox) {
					for (int i = lel->sectionForPos(xp) - (5 / layer->getScale()); i < lel->sectionForPos(xp) + (6 / layer->getScale()); i++) {
						if (i < 0) continue;
						if (i >= arrcount) break;
						auto objAtInd = secarr->objectAtIndex(i);
						auto objarr = reinterpret_cast<CCArray*>(objAtInd);

						for (int j = 0; j < objarr->count(); j++) {
							auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
							Hitboxes::drawSpecialsObjectHitbox(obj, objectDrawNode);
						}
					}
				}
				//}
			}
		}

		if (deleteBtn) {
			if (editUI->getSelectedObjectsOfCCArray()->count() == 0) {
				deleteBtn->setOpacity(175);
				deleteBtn->setColor({ 166, 166, 166 });
				deleteBtn->setEnabled(false);
			}
			else {
				deleteBtn->setOpacity(255);
				deleteBtn->setColor({ 255, 255, 255 });
				deleteBtn->setEnabled(true);
			}
		}

		if (onBaseLayerBtn) {
			if (editUI->getLevelEditorLayer()->getLayerGroup() == -1) {
				onBaseLayerBtn->setVisible(0);
				onBaseLayerBtn->setEnabled(false);
			}
			else {
				onBaseLayerBtn->setVisible(1);
				onBaseLayerBtn->setEnabled(true);
			}
		}
		
		if (objcolorid) {
			if (editUI->getSingleSelectedObj() == 0) objcolorid->setVisible(0);
			else
			{
				auto colorID = editUI->getSingleSelectedObj()->getObjectColor();
				switch (colorID) {
				case 0:
					reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: Default (0)", colorID)->getCString());
					break;
				case 1:
					reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: P-Col 1 (1)", colorID)->getCString());
					break;
				case 2:
					reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: P-Col 2 (2)", colorID)->getCString());
					break;
				case 3:
					reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: Col1 (3)", colorID)->getCString());
					break;
				case 4:
					reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: Col2 (4)", colorID)->getCString());
					break;
				case 5:
					reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: Light BG (5)", colorID)->getCString());
					break;
				case 6:
					reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: Col3 (6)", colorID)->getCString());
					break;
				case 7:
					reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: Col4 (7)", colorID)->getCString());
					break;
				case 8:
					reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: 3D-Line (8)", colorID)->getCString());
					break;
				case 9:
					reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: White (9)", colorID)->getCString());
					break;
				default:
					break;
				}
				objcolorid->setVisible(1);
			}
		}

		if (objgroupid) {
			if (editUI->getSingleSelectedObj() == 0) objgroupid->setVisible(0);
			else {
				reinterpret_cast<CCLabelBMFont*>(objgroupid)->setString(CCString::createWithFormat("G: %d", editUI->getSingleSelectedObj()->getObjectGroup())->getCString());
				objgroupid->setVisible(1);
			}
		}

		if (objrot) {
			if (editUI->getSingleSelectedObj() == 0) objrot->setVisible(0);
			else
			{
				reinterpret_cast<CCLabelBMFont*>(objrot)->setString(CCString::createWithFormat("Rot: %.01f%", editUI->getSingleSelectedObj()->getRotation())->getCString());
				objrot->setVisible(1);
			}
		}

		if (objposx) {
			if (editUI->getSingleSelectedObj() == 0) objposx->setVisible(0);
			else
			{
				reinterpret_cast<CCLabelBMFont*>(objposx)->setString(CCString::createWithFormat("X: %.01f%", editUI->getSingleSelectedObj()->getPositionX())->getCString());
				objposx->setVisible(1);
			}
		}

		if (objposy) {
			if (editUI->getSingleSelectedObj() == 0) objposy->setVisible(0);
			else
			{
				reinterpret_cast<CCLabelBMFont*>(objposy)->setString(CCString::createWithFormat("Y: %.01f%", editUI->getSingleSelectedObj()->getPositionY())->getCString());
				objposy->setVisible(1);
			}
		}

		if (objid) {
			if (editUI->getSingleSelectedObj() == 0) objid->setVisible(0);
			else {
				reinterpret_cast<CCLabelBMFont*>(objid)->setString(CCString::createWithFormat("ID: %d", editUI->getSingleSelectedObj()->getObjectID())->getCString());
				objid->setVisible(1);
			}
		}

		//if (customObj) {
		//	int selectFilterObject = gd::GameManager::sharedState()->getIntGameVariable("0005");
		//	reinterpret_cast<CCLabelBMFont*>(customObj)->setString(CCString::createWithFormat("%d", selectFilterObject)->getCString());
		//}

		if (objaddr)
		{
			if (editUI->getSingleSelectedObj() == 0) objaddr->setVisible(0);
			else {
				reinterpret_cast<CCLabelBMFont*>(objaddr)->setString(CCString::createWithFormat("Address: 0x%p", editUI->getSingleSelectedObj())->getCString());
				objaddr->setVisible(1);
			}
		}

		if (objcounter)
		{
			reinterpret_cast<CCLabelBMFont*>(objcounter)->setString(CCString::createWithFormat("Objects: %d", (editUI->getSelectedObjectsOfCCArray()->count()))->getCString());
			if (editUI->getSelectedObjectsOfCCArray()->count() == 0) objcounter->setVisible(0);
			else objcounter->setVisible(1);
		}
	}
}

void Scheduler::mem_init() {
	DWORD cocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");
	MH_CreateHook(
		reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("libcocos2d.dll"), "?update@CCScheduler@cocos2d@@UAEXM@Z")),
		Scheduler::update_H,
		reinterpret_cast<void**>(&Scheduler::update));
}

void LevelEditorLayer::mem_init() {
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x909f0), LevelEditorLayer::onPlaytestH, reinterpret_cast<void**>(&LevelEditorLayer::onPlaytest));
}

void EditorUI::mem_init() {
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x3fdc0),
		EditorUI::init_H,
		reinterpret_cast<void**>(&EditorUI::init));
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x3fb90),
		EditorUI::dtor_H,
		reinterpret_cast<void**>(&EditorUI::dtor));
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x4ee90),
		EditorUI::scrollWheel_H,
		reinterpret_cast<void**>(&EditorUI::scrollWheel));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41ae0), EditorUI::updateGridNodeSizeH, reinterpret_cast<void**>(&EditorUI::updateGridNodeSize));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x49d20), EditorUI::createMoveMenuH, reinterpret_cast<void**>(&EditorUI::createMoveMenu));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x47f10), EditorUI::selectObjectH, reinterpret_cast<void**>(&EditorUI::selectObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x47fa0), EditorUI::selectObjectsH, reinterpret_cast<void**>(&EditorUI::selectObjects));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4b040), EditorUI::moveForCommandH, reinterpret_cast<void**>(&EditorUI::moveForCommand));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4b7e0), EditorUI::transformObjectH, reinterpret_cast<void**>(&EditorUI::transformObject));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4e550), EditorUI::keyDown_H, reinterpret_cast<void**>(&EditorUI::keyDown));
	matdash::add_hook<&EditorUI_onPlaytest>(gd::base + 0x489c0);
	matdash::add_hook<&EditorUI_ccTouchBegan>(gd::base + 0x4d5e0);
	matdash::add_hook<&EditorUI_ccTouchEnded>(gd::base + 0x4de40);
}

void EditorPauseLayer::mem_init() {
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x3e3d0),
		EditorPauseLayer::customSetup_H,
		reinterpret_cast<void**>(&EditorPauseLayer::customSetup));
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x3f170),
		EditorPauseLayer::onSaveAndPlay_H,
		reinterpret_cast<void**>(&EditorPauseLayer::onSaveAndPlay));
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x3f340),
		EditorPauseLayer::onSaveAndExit_H,
		reinterpret_cast<void**>(&EditorPauseLayer::onSaveAndExit));
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x3f420),
		EditorPauseLayer::onExitNoSave_H,
		reinterpret_cast<void**>(&EditorPauseLayer::onExitNoSave));
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x3f570),
		EditorPauseLayer::keyDown_H,
		reinterpret_cast<void**>(&EditorPauseLayer::keyDown));
}

void preview_mode::init() {
	matdash::add_hook<&MyEditorLayer::updateVisibility, matdash::Thiscall>(gd::base + 0x8ef20);
	matdash::add_hook<&MyEditorLayer::addSpecial>(gd::base + 0x8ed10);
	matdash::add_hook<&MyEditorLayer::init>(gd::base + 0x8c2c0);
	matdash::add_hook<&MyEditorLayer::dtor>(gd::base + 0x8c080);
	matdash::add_hook<&EditorUI_moveObject>(gd::base + 0x4b410);
	matdash::add_hook<&MyEditorLayer::removeSpecial>(gd::base + 0x8ee30);

	matdash::add_hook<&EditorUI_deselectAll>(gd::base + 0x48380);
	matdash::add_hook<&EditorUI_updateZoom>(gd::base + 0x48c30);
	matdash::add_hook<&EditorUI_getLimitedPosition, matdash::Stdcall>(gd::base + 0x4b500);

	matdash::add_hook<&EditorPauseLayer_init>(gd::base + 0x3e2e0);
	matdash::add_hook<&EditorPauseLayer_dtor>(gd::base + 0x3e280);

	matdash::add_hook<&GameObject_shouldBlendColor>(gd::base + 0x6ece0);
}
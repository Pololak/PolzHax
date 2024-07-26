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

using namespace cocos2d;

//enum class gd::GJCustomColorMode {
//	Default = 0,
//	PlayerCol1 = 1,
//	PlayerCol2 = 2,
//	LightBG = 5,
//	Col1 = 3,
//	Col2 = 4,
//	Col3 = 6,
//	Col4 = 7,
//	DL = 8
//};

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
struct ExtendBase {
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

		menuh->setZOrder(100);
		menuh->setScale(.5f);
		menuh->setPosition({ director->getScreenLeft() + 20, director->getScreenTop() - 100.5f });


		menuh->addChild(hideui_btn);
		this->addChild(menuh);

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
			GDColor color_to = trigger;
			auto dist = time_between_pos(trigger->getPosition().x, pos) / trigger->triggerDuration();
			auto color_from = starting_color;
			if (bound > 1) {
				auto trigger = triggers[bound - 2];
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

				if (object->getIsTintObject())
					object->setObjectColor(obj_color);

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
}

CCPoint* EditorUI_getLimitedPosition(CCPoint* retVal, CCPoint point) {
	*retVal = point;
	return retVal;
}

bool __fastcall EditorUI::init_H(gd::EditorUI* self, void*, CCLayer* editor) {
	editUI = self;

	bool result = EditorUI::init(self, editor);

	auto sliderArr = from<CCArray*>(self, 0xA8);
	auto Slider = reinterpret_cast<gd::Slider*>(sliderArr->objectAtIndex(0));
	Slider->setPositionX(Slider->getPositionX() + 15);

	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();

	if (setting().onPersClip)
	{
		if (!savedClipboard.empty()) {
			self->clipboard() = savedClipboard;
			self->updateButtons();
		}
	}

	auto objcolorid = CCLabelBMFont::create("", "chatFont.fnt");
	objcolorid->setString(CCString::createWithFormat("C: %d", 0)->getCString());
	objcolorid->setVisible(0);
	objcolorid->setTag(45011);
	objcolorid->setScale(0.66f);
	objcolorid->setPosition({ director->getScreenLeft() + 80, director->getScreenTop() - 70 });
	objcolorid->setAnchorPoint({ 0, 0.5f });
	self->addChild(objcolorid);

	auto objgroupid = CCLabelBMFont::create("", "chatFont.fnt");
	objgroupid->setString(CCString::createWithFormat("G: %d")->getCString());
	objgroupid->setVisible(0);
	objgroupid->setTag(45012);
	objgroupid->setScale(0.66f);
	objgroupid->setPosition({ director->getScreenLeft() + 80, director->getScreenTop() - 80 });
	objgroupid->setAnchorPoint({ 0, 0.5f });
	self->addChild(objgroupid);

	auto objrot = CCLabelBMFont::create("", "chatFont.fnt");
	objrot->setString(CCString::createWithFormat("Rot: %.0f%")->getCString());
	objrot->setVisible(0);
	objrot->setTag(45015);
	objrot->setScale(0.66f);
	objrot->setPosition({ director->getScreenLeft() + 80, director->getScreenTop() - 90 });
	objrot->setAnchorPoint({ 0, 0.5f });
	self->addChild(objrot);

	auto objposx = CCLabelBMFont::create("", "chatFont.fnt");
	objposx->setString(CCString::createWithFormat("Pos X: %.0f%")->getCString());
	objposx->setVisible(0);
	objposx->setTag(45013);
	objposx->setScale(0.66f);
	objposx->setPosition({ director->getScreenLeft() + 80, director->getScreenTop() - 100 });
	objposx->setAnchorPoint({ 0, 0.5f });
	self->addChild(objposx);

	auto objposy = CCLabelBMFont::create("", "chatFont.fnt");
	objposy->setString(CCString::createWithFormat("Pos Y: %.0f%")->getCString());
	objposy->setVisible(0);
	objposy->setTag(45014);
	objposy->setScale(0.66f);
	objposy->setPosition({ director->getScreenLeft() + 80, director->getScreenTop() - 110 });
	objposy->setAnchorPoint({ 0, 0.5f });
	self->addChild(objposy);

	auto objid = CCLabelBMFont::create("", "chatFont.fnt");
	objid->setString(CCString::createWithFormat("ObjID: %d")->getCString());
	objid->setVisible(0);
	objid->setTag(45016);
	objid->setScale(0.66f);
	objid->setPosition({ director->getScreenLeft() + 80, director->getScreenTop() - 120 });
	objid->setAnchorPoint({ 0, 0.5f });
	self->addChild(objid);

	gd::GameObject* selectedObject = from<gd::GameObject*>(self, 0x258);
	auto objaddr = CCLabelBMFont::create("", "chatFont.fnt");
	objaddr->setString(CCString::createWithFormat("Addr: 0x%p", selectedObject)->getCString());
	objaddr->setVisible(0);
	objaddr->setTag(45017);
	objaddr->setAnchorPoint({ 0, 0.5f });
	objaddr->setPosition({ director->getScreenLeft() + 80, director->getScreenTop() - 130});
	objaddr->setScale(0.66f);
	self->addChild(objaddr);

	std::vector<gd::GameObject*> gameObjectVector = self->getSelectedObjects();
	auto objcounter = CCLabelBMFont::create("", "chatFont.fnt");
	objcounter->setString(CCString::createWithFormat("Objects: %d", gameObjectVector.size())->getCString());
	objcounter->setVisible(0);
	objcounter->setTag(45018);
	objcounter->setAnchorPoint({ 0, 0.5f });
	objcounter->setPosition(director->getScreenLeft() + 80, director->getScreenTop() - 60);
	objcounter->setScale(0.66f);
	self->addChild(objcounter);

	auto menu = CCMenu::create();
	menu->setPosition({ director->getScreenLeft(), director->getScreenTop() });

	auto deletespr = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
	if (!deletespr->initWithFile("GJ_trashBtn_001.png")) {
		deletespr->createWithSpriteFrameName("edit_delBtn_001.png");
	}
	auto deletebtn = gd::CCMenuItemSpriteExtra::create(deletespr, nullptr, self, menu_selector(gd::EditorUI::onDeleteSelected));
	deletespr->setScale(0.925f);
	deletebtn->setPosition({ 125.25 ,-20.75 });
	deletebtn->setTag(5901);
	deletebtn->setVisible(0);


	menu->addChild(deletebtn);
	self->addChild(menu);

	return result;
}

bool __fastcall EditorUI::dtor_H(gd::EditorUI* self) {
	editUI = nullptr;
	if (setting().onPersClip) savedClipboard = self->clipboard();
	EditorUI::dtor(self);
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

void __fastcall EditorUI::createMoveMenu_H(gd::EditorUI* self) {
	EditorUI::createMoveMenu(self);

	//auto bar_array = from<CCArray*>(self, 0xa8);

	//auto editButtonBar = from<gd::EditButtonBar*>(self, 0x160);
	//auto ccComponentContainer = from<CCComponentContainer*>(editButtonBar, 0xe4);
	//auto boomScrollLayer = from<CCNode*>(ccComponentContainer, 0x8);
	//auto array = from<CCArray*>(boomScrollLayer, 0xa8);

	//auto sprite = CCSprite::create("GJ_button_01.png");
	//auto btn = gd::CCMenuItemSpriteExtra::create(sprite, nullptr, self, 0);

	//array->addObject(sprite);

	//editButtonBar->addChild(sprite);
	

}

//void __fastcall EditorUI::onPause_H(gd::EditorUI* self, void*, CCObject* sender) {
//	editUI->onDeselectAll(sender);
//
//	EditorUI::onPause(self, sender);
//}

void __fastcall EditorPauseLayer::customSetup_H(gd::EditorPauseLayer* self) {
	editorPauseLayer = self;
	EditorPauseLayer::customSetup(self);

	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();

	constexpr auto handler = [](CCObject* self, CCObject*) {
		auto text = clipboard::read();
		auto editor = reinterpret_cast<gd::LevelEditorLayer*>(reinterpret_cast<CCNode*>(self)->getParent());
		editor->getEditorUI()->pasteObjects(text);
		};

	auto pssprite = gd::ButtonSprite::create("Paste\nString", 0x28, 0, 0.6f, true, "bigFont.fnt", "GJ_button_04.png", 30.0);
	auto psbutton = gd::CCMenuItemSpriteExtra::create(pssprite, nullptr, self, to_handler<SEL_MenuHandler, handler>);

	auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	auto optionsBtn = gd::CCMenuItemSpriteExtra::create(optionsSpr, nullptr, self, menu_selector(gd::MenuLayer::onOptions));
	optionsBtn->setPosition({ -38, -66 });
	optionsSpr->setScale(.66f);
	
	menu->setPosition({ director->getScreenRight(), director->getScreenTop() });
	psbutton->setPosition({ -50.f, -30.f });
	menu->addChild(psbutton);
	menu->addChild(optionsBtn);

	self->addChild(menu);
}

void __fastcall EditorPauseLayer::onSaveAndPlay_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
	EditorPauseLayer::onSaveAndPlay(self, sender);

	editUI = nullptr;
}

void __fastcall EditorPauseLayer::onSaveAndExit_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
	EditorPauseLayer::onSaveAndExit(self, sender);

	editUI = nullptr;
}

void __fastcall EditorPauseLayer::onExitNoSave_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
	EditorPauseLayer::onExitNoSave(self, sender);

	editUI = nullptr;
}

void __fastcall EditorPauseLayer::keyDown_H(gd::EditorPauseLayer* self, void* edx, enumKeyCodes key) {

	if (key == KEY_Escape) {
		editorPauseLayer->onResume(nullptr);
	}
	else {
		EditorPauseLayer::keyDown(self, key);
	}
}

void __fastcall Scheduler::update_H(CCScheduler* self, void* edx, float dt) {

	scheduler = self;

	auto play_layer = gd::GameManager::sharedState()->getPlayLayer();
	const auto bar = gd::GameManager::sharedState()->getShowProgressBar();
	auto size = CCDirector::sharedDirector()->getWinSize();

	if (play_layer) {
		auto percentLabel = reinterpret_cast<CCLabelBMFont*>(play_layer->getChildByTag(4571));

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
	}

	if (editUI) {
		auto objcolorid = editUI->getChildByTag(45011);
		auto objgroupid = editUI->getChildByTag(45012);
		auto objposx = editUI->getChildByTag(45013);
		auto objposy = editUI->getChildByTag(45014);
		auto objrot = editUI->getChildByTag(45015);
		auto objid = editUI->getChildByTag(45016);
		auto objaddr = editUI->getChildByTag(45017);
		auto objcounter = editUI->getChildByTag(45018);
		auto delbtn = editUI->getChildByTag(5901);

		if (objcolorid) {
			if (editUI->getSingleSelectedObj() == 0) objcolorid->setVisible(0);
			else
			{
				reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("C: %d", editUI->getSingleSelectedObj()->getObjectColor())->getCString());
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
				reinterpret_cast<CCLabelBMFont*>(objrot)->setString(CCString::createWithFormat("Rot: %.0f%", std::floor(editUI->getSingleSelectedObj()->getRotation()))->getCString());
				objrot->setVisible(1);
			}
		}

		if (objposx) {
			if (editUI->getSingleSelectedObj() == 0) objposx->setVisible(0);
			else
			{
				reinterpret_cast<CCLabelBMFont*>(objposx)->setString(CCString::createWithFormat("Pos X: %.0f%",
					std::floor(editUI->getSingleSelectedObj()->getPositionX()))->getCString());
				objposx->setVisible(1);
			}
		}

		if (objposy) {
			if (editUI->getSingleSelectedObj() == 0) objposy->setVisible(0);
			else
			{
				reinterpret_cast<CCLabelBMFont*>(objposy)->setString(CCString::createWithFormat("Pos Y: %.0f%",
					std::floor(editUI->getSingleSelectedObj()->getPositionY()))->getCString());
				objposy->setVisible(1);
			}
		}

		if (objid) {
			if (editUI->getSingleSelectedObj() == 0) objid->setVisible(0);
			else {
				reinterpret_cast<CCLabelBMFont*>(objid)->setString(CCString::createWithFormat("ObjID: %d", editUI->getSingleSelectedObj()->getObjectID())->getCString());
				objid->setVisible(1);
			}
		}

		if (objaddr)
		{
			if (editUI->getSingleSelectedObj() == 0) objaddr->setVisible(0);
			else {
				reinterpret_cast<CCLabelBMFont*>(objaddr)->setString(CCString::createWithFormat("Addr: 0x%p", editUI->getSingleSelectedObj())->getCString());
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

	Scheduler::update(self, dt);
}

void Scheduler::mem_init() {
	DWORD cocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");
	MH_CreateHook(
		reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("libcocos2d.dll"), "?update@CCScheduler@cocos2d@@UAEXM@Z")),
		Scheduler::update_H,
		reinterpret_cast<void**>(&Scheduler::update));
	/*MH_CreateHook(
		reinterpret_cast<void*>(cocosbase + 0xff970),
		Scheduler::update_H,
		reinterpret_cast<void**>(Scheduler::update));*/
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
	/*MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x411f0),
		EditorUI::onPause_H,
		reinterpret_cast<void**>(&EditorUI::onPause));*/
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x49d20),
		EditorUI::createMoveMenu_H,
		reinterpret_cast<void**>(&EditorUI::createMoveMenu));

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
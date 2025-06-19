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
#include "nodes.hpp"
#include "RotatingSaws.hpp"
#include "EditorSettingsLayer.hpp"
#include "RGBColorInputWidget.hpp"
#include "EditorObjectLayering.hpp"
#include "EditorLayerInput.h"
#include "ObjectIDEnterPopup.h"

using namespace cocos2d;

gd::EditorPauseLayer* editorPauseLayer;
gd::EditorUI* editUI;
std::string savedClipboard;
gd::LevelSettingsLayer* levelSettingsLayer;

CCLabelBMFont* m_gridSizeLabel = nullptr;

CCLabelBMFont* m_objectColor = nullptr;
CCLabelBMFont* m_objectGroup = nullptr; // Layer
CCLabelBMFont* m_objectRotation = nullptr;
CCLabelBMFont* m_objectXPos = nullptr;
CCLabelBMFont* m_objectYPos = nullptr;
CCLabelBMFont* m_objectKey = nullptr; // ID
CCLabelBMFont* m_objectAddress = nullptr;
CCLabelBMFont* m_objectType = nullptr;
CCLabelBMFont* m_objectsSelected = nullptr;
CCLabelBMFont* m_objectZ = nullptr;

FloatInputNode* m_fadeTime_input = nullptr;
float m_fadeTime;

RGBColorInputWidget* m_colorInputWidget = nullptr;

static std::array<float, 9> SNAP_GRID_SIZES{
	1.f, 2.f, 3.75f, 7.5f, 15.f, 30.f, 60.f, 90.f, 120.f
};

CCLayer* circleToolPopup;
static float m_angle;
static float m_step;
static float m_fat;
CCLabelBMFont* m_circleToolLabel = nullptr;
FloatInputNode* angle_input = nullptr;
FloatInputNode* step_input = nullptr;

EditorLayerInput* m_editorLayerInput = nullptr;

CCArray* m_hideableUIElements;

ObjectGroupInput* m_objectGroupInput = nullptr;

class CircleToolPopup : public CCLayer { // FINALLY
public:
	static CircleToolPopup* create() {
		CircleToolPopup* obj = new CircleToolPopup;
		if (obj && obj->init()) {
			obj->autorelease();
			return obj;
		}
		CC_SAFE_DELETE(obj);
		return nullptr;
	}

	bool init() {
		if (!CCLayer::init()) return false;
		const float width = 300, height = 220;
		CCLayerColor* CCLayerCol = CCLayerColor::create(ccc4(0, 0, 0, 0));
		CCLayerCol->setZOrder(1);
		CCLayerCol->setScale(10.f);
		this->addChild(CCLayerCol);
		auto actionColor = CCFadeTo::create(0.1f, 75);
		CCLayerCol->runAction(actionColor);

		auto touchDispatcher = CCDirector::sharedDirector()->m_pTouchDispatcher;
		touchDispatcher->incrementForcePrio();
		this->registerWithTouchDispatcher();
		setTouchEnabled(true);
		setKeypadEnabled(true);
		setMouseEnabled(true);

		auto director = CCDirector::sharedDirector();

		auto bgSprite = CCSprite::create("GJ_button_03.png");
		bgSprite->setScale(100.f);
		bgSprite->setOpacity(0);
		auto bgButton = gd::CCMenuItemSpriteExtra::create(bgSprite, nullptr, this, nullptr);
		auto bgMenu = CCMenu::create();
		bgMenu->addChild(bgButton);
		bgMenu->setZOrder(0);
		bgMenu->setPosition((CCDirector::sharedDirector()->getScreenRight()) - 25, (CCDirector::sharedDirector()->getScreenTop()) - 25);
		this->addChild(bgMenu); // DONT EVEN ASK WHAT IS THIS

		auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
		bg->setContentSize({ width, height });
		bg->setPosition(director->getWinSize().width / 2, director->getWinSize().height / 2);
		bg->setZOrder(2);
		this->addChild(bg);

		const CCPoint offset = director->getWinSize() / 2.f;

		auto appearAction = CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f), .6f);

		auto menu = CCMenu::create();
		menu->setZOrder(3);
		this->addChild(menu);

		auto closeBtn = gd::CCMenuItemSpriteExtra::create(
			(CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png")),
			nullptr,
			this,
			menu_selector(CircleToolPopup::onClose));
		closeBtn->setPosition(-((width / 2) - 5), (height / 2) - 5);

		menu->addChild(closeBtn);

		auto mainLabel = CCLabelBMFont::create("Circle Tool", "bigFont.fnt");
		mainLabel->setPosition(ccp(0.f, 95.f) + offset);
		mainLabel->setScale(0.75f);
		mainLabel->setZOrder(5);
		this->addChild(mainLabel);

		auto arcLabel = CCLabelBMFont::create("Arc:", "goldFont.fnt");
		arcLabel->setPosition(ccp(-60, 64) + offset);
		arcLabel->setScale(0.75f);
		arcLabel->setZOrder(5);
		this->addChild(arcLabel);
		angle_input = FloatInputNode::create(CCSize(60, 30), 2.f, "bigFont.fnt");
		angle_input->set_value(m_angle);
		angle_input->setPosition(ccp(-60, 38) + offset);
		angle_input->setZOrder(5);
		angle_input->callback = [&](FloatInputNode& input) {
			m_angle = input.get_value().value_or(m_angle);
			this->update_labels();
			};
		this->addChild(angle_input);

		auto stepLabel = CCLabelBMFont::create("Step:", "goldFont.fnt");
		stepLabel->setPosition(ccp(60, 64) + offset);
		stepLabel->setScale(0.75f);
		stepLabel->setZOrder(5);
		this->addChild(stepLabel);
		step_input = FloatInputNode::create(CCSize(60, 30), 2.f, "bigFont.fnt");
		step_input->set_value(m_step);
		step_input->setPosition(ccp(60, 38) + offset);
		step_input->setZOrder(5);
		step_input->callback = [&](FloatInputNode& input) {
			m_step = input.get_value().value_or(m_step);
			if (m_step == 0.f) m_step = 1.f;
			this->update_labels();
			};
		this->addChild(step_input);

		m_circleToolLabel = CCLabelBMFont::create("copies: 69\nobjects: 69420", "chatFont.fnt", 0.f, kCCTextAlignmentLeft);
		m_circleToolLabel->setPosition(ccp(-83, -41) + offset);
		m_circleToolLabel->setZOrder(5);
		this->addChild(m_circleToolLabel);
		this->update_labels();

		auto originalAuthorLbl = CCLabelBMFont::create("Original by Mat", "goldFont.fnt");
		originalAuthorLbl->setScale(0.5f);
		auto originalAuthor = gd::CCMenuItemSpriteExtra::create(originalAuthorLbl, nullptr, this, menu_selector(CircleToolPopup::originalAuthor));
		originalAuthor->setPosition(ccp(-92, -96));
		menu->addChild(originalAuthor);

		auto apply_btn = gd::CCMenuItemSpriteExtra::create(
			gd::ButtonSprite::create("Apply", 0, 0, 0.75f, false, "goldFont.fnt", "GJ_button_01.png", 25.f),
			nullptr,
			this, menu_selector(CircleToolPopup::onApply)
		);
		apply_btn->setPosition({ 0, -85 });
		menu->addChild(apply_btn);

		this->setScale(0.1f);
		this->runAction(appearAction);

		return true;
	}

	void keyBackClicked() override {
		this->removeFromParentAndCleanup(true);
		circleToolPopup = nullptr;
	}

	void onClose(CCObject*) {
		this->removeFromParentAndCleanup(true);
		circleToolPopup = nullptr;
	}

	void update_labels() {
		m_angle = angle_input->get_value().value_or(m_angle);
		m_step = step_input->get_value().value_or(m_step);
		auto objs = editUI->getSelectedObjectsOfCCArray();
		const auto amt = static_cast<size_t>(std::ceilf(m_angle / m_step) - 1.f);
		const auto obj_count = amt * objs->count();
		m_circleToolLabel->setString(("Copies: " + std::to_string(amt) + "\nObjects: " + std::to_string(obj_count)).c_str());
	}

	void onApply(CCObject*) {
		perform();
	}

	void perform() {
		CCArray* objs = CCArray::create();
		for (float i = 1; i * m_step < m_angle; i++) {
			editUI->onDuplicate(nullptr);
			auto selected = editUI->getSelectedObjectsOfCCArray();
			editUI->rotateObjects(selected, m_step, { 0.f, 0.f });

			const float angle = i * m_step;

			from<CCArray*>(editUI->m_editorLayer, 0x170)->removeLastObject();
			objs->addObjectsFromArray(selected);
		}
		from<CCArray*>(editUI->m_editorLayer, 0x170)->addObject(gd::UndoObject::createWithArray(objs, gd::UndoCommand::Paste));
		editUI->selectObjects(objs);
		this->keyBackClicked();
	}

	void originalAuthor(CCObject*) {
		CCApplication::sharedApplication()->openURL("https://github.com/matcool/small-gd-mods/blob/main/src/circle-tool.cpp");
	}

	void showCallback(CCObject* btn) {
		if (editUI->getSelectedObjectsOfCCArray()->count()) {
			auto director = CCDirector::sharedDirector();
			auto myCircleTool = CircleToolPopup::create();
			myCircleTool->setZOrder(9999);
			auto myLayer2 = CCDirector::sharedDirector()->getRunningScene();
			myLayer2->addChild(myCircleTool);
			circleToolPopup = myCircleTool;
		}
	}
};

void updateGridSizeLabel() {
	m_gridSizeLabel->setString(CCString::createWithFormat("%.02f%", setting().gridSize)->getCString());
}

class GridSizeCB : public gd::EditorUI {
public:
	void onIncrement(CCObject*) {
		auto next = std::upper_bound(SNAP_GRID_SIZES.begin(), SNAP_GRID_SIZES.end(), setting().gridSize);
		if (next == SNAP_GRID_SIZES.end()) next--;
		setting().gridSize = *next;
		updateGridSizeLabel();
		this->updateGridNodeSize();
	}
	void onDecrement(CCObject*) {
		auto next = std::lower_bound(SNAP_GRID_SIZES.begin(), SNAP_GRID_SIZES.end(), setting().gridSize);
		if (next != SNAP_GRID_SIZES.begin()) next--;
		setting().gridSize = *next;
		updateGridSizeLabel();
		this->updateGridNodeSize();
	}
};

gd::GameObject* g_lastObject = nullptr;
int g_bDontUpdateSlider = 0;

void updateLastObjectX(gd::LevelEditorLayer* lel, gd::GameObject* obj = nullptr);

float getLevelLength() {
	float screenEnd = CCDirector::sharedDirector()->getScreenRight() + 300.f;
	auto res = screenEnd;
	if (g_lastObject) {
		if (g_lastObject->retainCount() == 1u) {
			g_lastObject->release();
			g_lastObject = nullptr;
			updateLastObjectX(editUI->m_editorLayer);
		}
		else {
			res = g_lastObject->getPositionX() + 340.f;
		}
	}
	if (res < screenEnd)
		res = screenEnd;
	return res;
}

void updateLastObjectX(gd::LevelEditorLayer* lel, gd::GameObject* obj) {
	if (obj == nullptr) {
		CCARRAY_FOREACH_B_TYPE(lel->getAllObjects(), pObj, gd::GameObject) {
			if (!g_lastObject) {
				g_lastObject = pObj;
				continue;
			}
			if (pObj->getPositionX() < g_lastObject->getPositionX())
				g_lastObject = pObj;
		}
	}
	else {
		if (!g_lastObject)
			g_lastObject = obj;
		if (obj->getPositionX() > g_lastObject->getPositionX())
			g_lastObject = obj;
	}
	if (g_lastObject)
		g_lastObject->retain();
}

void handleObjectAddForSlider(gd::LevelEditorLayer* self, gd::GameObject* obj) {
	if (obj) updateLastObjectX(self, obj);
}

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

CCLayer* colorFilterPopup;
CCLabelBMFont* colorFilterLabel = nullptr;

class ColorFilterPopup : public CCLayer {
public:
	CCLabelBMFont* colorSelected = nullptr;

	static ColorFilterPopup* create() {
		ColorFilterPopup* obj = new ColorFilterPopup();
		if (obj && obj->init()) {
			obj->autorelease();
			return obj;
		}
		CC_SAFE_DELETE(obj);
		return nullptr;
	}

	bool init() {
		if (!CCLayer::init()) return false;
		const float width = 180, height = 110;
		CCLayerColor* CCLayerCol = CCLayerColor::create(ccc4(0, 0, 0, 0));
		CCLayerCol->setZOrder(1);
		CCLayerCol->setScale(10.f);
		this->addChild(CCLayerCol);
		auto actionColor = CCFadeTo::create(0.1f, 75);
		CCLayerCol->runAction(actionColor);

		auto touchDispatcher = CCDirector::sharedDirector()->m_pTouchDispatcher;
		touchDispatcher->incrementForcePrio();
		this->registerWithTouchDispatcher();
		setTouchEnabled(true);
		setKeypadEnabled(true);
		setMouseEnabled(true);

		auto director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();

		auto bgSprite = CCSprite::create("GJ_button_03.png");
		bgSprite->setScale(100.f);
		bgSprite->setOpacity(0);
		auto bgButton = gd::CCMenuItemSpriteExtra::create(bgSprite, nullptr, this, nullptr);
		auto bgMenu = CCMenu::create();
		bgMenu->addChild(bgButton);
		bgMenu->setZOrder(0);
		bgMenu->setPosition((CCDirector::sharedDirector()->getScreenRight()) - 25, (CCDirector::sharedDirector()->getScreenTop()) - 25);
		this->addChild(bgMenu); // DONT EVEN ASK WHAT IS THIS

		auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
		bg->setContentSize({ width, height });
		bg->setPosition(director->getWinSize().width / 2, director->getWinSize().height / 2);
		bg->setZOrder(2);
		this->addChild(bg);

		auto label = CCLabelBMFont::create("Color Filter", "goldFont.fnt");
		label->setPosition({ winSize.width / 2, winSize.height / 2 + 40 });
		label->setScale(0.7f);
		label->setZOrder(2);
		this->addChild(label);

		auto appearAction = CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f), .6f);

		auto menu = CCMenu::create();
		menu->setZOrder(3);
		this->addChild(menu);

		auto closeBtn = gd::CCMenuItemSpriteExtra::create(
			(CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png")),
			nullptr,
			this,
			menu_selector(ColorFilterPopup::onClose));
		closeBtn->setPosition(-((width / 2) - 5), (height / 2) - 5);

		menu->addChild(closeBtn);

		colorSelected = CCLabelBMFont::create("", "bigFont.fnt");
		update_label();
		colorSelected->setPosition(winSize.width / 2.f, winSize.height / 2 + 10);
		colorSelected->setZOrder(5);
		colorSelected->setScale(0.8f);
		this->addChild(colorSelected);

		auto button = gd::CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png"),
			nullptr,
			this,
			menu_selector(ColorFilterPopup::onIncrement));
		button->setPosition({ 40, 8 });

		auto button2 = gd::CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png"),
			nullptr,
			this,
			menu_selector(ColorFilterPopup::onDecrement));
		button2->setPosition({ -40, 8 });

		auto okButton = gd::CCMenuItemSpriteExtra::create(
			gd::ButtonSprite::create("OK", 0, 0, 1.f, false, "goldFont.fnt", "GJ_button_01.png", 28.f),
			nullptr,
			this,
			menu_selector(ColorFilterPopup::onClose)
		);
		okButton->setPositionY(-32);
		menu->addChild(okButton);

		menu->addChild(button);
		menu->addChild(button2);

		auto resetSprite = CCSprite::create("GJ_button_04.png");
		resetSprite->setScale(0.7f);
		auto dhaskjfhajklsdhflkah = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
		dhaskjfhajklsdhflkah->setPosition({ resetSprite->getContentSize() / 2.f });
		resetSprite->addChild(dhaskjfhajklsdhflkah);
		auto resetButton = gd::CCMenuItemSpriteExtra::create(
			resetSprite,
			nullptr,
			this,
			menu_selector(ColorFilterPopup::onReset)
		);
		resetButton->setPosition({ -70, -34 });
		menu->addChild(resetButton);

		this->setScale(0.1f);
		this->runAction(appearAction);

		return true;
	}

	void keyBackClicked() {
		this->onClose(nullptr);
	}

	void onClose(CCObject*) {
		update_label();
		this->removeFromParentAndCleanup(true);
		colorFilterPopup = nullptr;
	}

	void onIncrement(CCObject*) {
		gd::GameManager::sharedState()->setIntGameVariable(GameVariable::COLOR_FILTER, gd::GameManager::sharedState()->getIntGameVariable(GameVariable::COLOR_FILTER) + 1);
		if (gd::GameManager::sharedState()->getIntGameVariable(GameVariable::COLOR_FILTER) == 10) {
			gd::GameManager::sharedState()->setIntGameVariable(GameVariable::COLOR_FILTER, 9);
		}
		update_label();
	}

	void onDecrement(CCObject*) {
		gd::GameManager::sharedState()->setIntGameVariable(GameVariable::COLOR_FILTER, gd::GameManager::sharedState()->getIntGameVariable(GameVariable::COLOR_FILTER) - 1);
		if (gd::GameManager::sharedState()->getIntGameVariable(GameVariable::COLOR_FILTER) == -1) {
			gd::GameManager::sharedState()->setIntGameVariable(GameVariable::COLOR_FILTER, 0);
		}
		update_label();
	}

	void showCallback(CCObject*) {
		auto myLayer = ColorFilterPopup::create();
		myLayer->setZOrder(100);
		CCDirector::sharedDirector()->getRunningScene()->addChild(myLayer);
		colorFilterPopup = myLayer;
	}

	void onReset(CCObject*) {
		gd::GameManager::sharedState()->setIntGameVariable(GameVariable::COLOR_FILTER, 0);
		update_label();
	}

	void update_label() {
		int currentColor = gd::GameManager::sharedState()->getIntGameVariable(GameVariable::COLOR_FILTER);
		switch (currentColor)
		{
		case 0:
			colorSelected->setString("D");
			colorFilterLabel->setString("D");
			colorFilterLabel->setScale(.75f);
			break;
		case 1:
			colorSelected->setString("P1");
			colorFilterLabel->setString("P1");
			colorFilterLabel->setScale(.65f);
			break;
		case 2:
			colorSelected->setString("P2");
			colorFilterLabel->setString("P2");
			colorFilterLabel->setScale(.65f);
			break;
		case 3:
			colorSelected->setString("C1");
			colorFilterLabel->setString("C1");
			colorFilterLabel->setScale(.65f);
			break;
		case 4:
			colorSelected->setString("C2");
			colorFilterLabel->setString("C2");
			colorFilterLabel->setScale(.65f);
			break;
		case 5:
			colorSelected->setString("LBG");
			colorFilterLabel->setString("LBG");
			colorFilterLabel->setScale(.45f);
			break;
		case 6:
			colorSelected->setString("C3");
			colorFilterLabel->setString("C3");
			colorFilterLabel->setScale(.65f);
			break;
		case 7:
			colorSelected->setString("C4");
			colorFilterLabel->setString("C4");
			colorFilterLabel->setScale(.65f);
			break;
		case 8:
			colorSelected->setString("DL");
			colorFilterLabel->setString("DL");
			colorFilterLabel->setScale(.65f);
			break;
		case 9:
			colorSelected->setString("W");
			colorFilterLabel->setString("W");
			colorFilterLabel->setScale(.75f);
			break;
		}
	}
};

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
	GDColor(gd::GameObject* object) : GDColor(object->m_triggerColor, object->m_triggerBlending) {}
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
		//gd::GameManager::sharedState()->m_levelEditorLayer = nullptr;
	}

	bool init(gd::GJGameLevel* level) {
		if (!matdash::orig<&MyEditorLayer::init>(this, level)) return false;
		s_instance = this;
		//gd::GameManager::sharedState()->m_levelEditorLayer = this;

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
		zoomLabel->setZOrder(99);
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
		handleObjectAddForSlider(this, object);
		bool prevAnims_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::PREV_ANIMS);
		if (prevAnims_enabled && RotateSaws::objectIsSaw(object))
			RotateSaws::beginRotateSaw(object);
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
		handleObjectAddForSlider(this, object);
		bool prevAnims_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::PREV_ANIMS);
		if (prevAnims_enabled && RotateSaws::objectIsSaw(object))
			RotateSaws::beginRotateSaw(object);
		
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
		bool showObjInfo_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::SHOW_OBJ_INFO);
		if (showObjInfo_enabled)
			EditorUI::updateObjectInfo(this->m_uiLayer);
		else {
			auto objectInfo = static_cast<CCLabelBMFont*>(this->m_uiLayer->getChildByTag(3207));
			if (objectInfo) objectInfo->setString("");
		}

		auto objectDrawNode = reinterpret_cast<CCDrawNode*>(this->gameLayer()->getChildByTag(125));
		objectDrawNode->clear();
		if (setting().onHitboxes) {
			auto layer = this->gameLayer();
			float xp = -layer->getPositionX() / layer->getScale();
			for (int i = this->m_firstVisibleSection + 1; i <= this->m_lastVisibleSection - 1; i++) {
				if (i < 0) continue;
				if (i >= this->m_levelSections->count()) break;
				auto section = static_cast<CCArray*>(this->m_levelSections->objectAtIndex(i));
				for (int j = 0; j < section->count(); ++j) {
					auto obj = reinterpret_cast<gd::GameObject*>(section->objectAtIndex(j));
					if (setting().onSolidsHitbox)
						Hitboxes::drawSolidsObjectHitbox(obj, objectDrawNode);
					if (setting().onHazardsHitbox)
						Hitboxes::drawHazardsObjectHitbox(obj, objectDrawNode);
					if (setting().onSpecialsHitbox)
						Hitboxes::drawSpecialsObjectHitbox(obj, objectDrawNode);
				}
			}
		}

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
				starting_color = settings->m_backgroundColor;
				color = &bg_color;
				break;
			case ColorTriggers::Obj:
				starting_color = settings->m_objectColor;
				color = &obj_color;
				break;
			case ColorTriggers::DLine:
				starting_color = settings->m_dlColor;
				color = &dl_color;
				break;
			case ColorTriggers::Col1:
				starting_color = settings->m_customColor1;
				color = &color1;
				break;
			case ColorTriggers::Col2:
				starting_color = settings->m_customColor2;
				color = &color2;
				break;
			case ColorTriggers::Col3:
				starting_color = settings->m_customColor3;
				color = &color3;
				break;
			case ColorTriggers::Col4:
				starting_color = settings->m_customColor4;
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
		(this->*m_current_color)[gd::CustomColorMode::DL] = dl_color.blending;

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

				if (object->m_invisibleMode)
					object->setObjectColor(ccc3(255, 127, 0));
			}
		}
	}
};
MyEditorLayer* MyEditorLayer::s_instance = nullptr;

void __fastcall LevelEditorLayer::removeObjectH(gd::LevelEditorLayer* self, void*, gd::GameObject* obj, bool idk) {
	LevelEditorLayer::removeObject(self, obj, idk);
	g_lastObject = nullptr;
	handleObjectAddForSlider(self, obj);
	bool prevAnims_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::PREV_ANIMS);
	if (prevAnims_enabled && RotateSaws::objectIsSaw(obj))
		RotateSaws::stopRotateSaw(obj);

	std::cout << "LevelEditorLayer::removeObject called." << std::endl;
}

void __fastcall LevelEditorLayer::updateH(gd::LevelEditorLayer* self, void*, float dt) {
	LevelEditorLayer::update(self, dt);
	auto playerDrawNode = reinterpret_cast<CCDrawNode*>(self->gameLayer()->getChildByTag(124));
	playerDrawNode->clear();
	if ((setting().onPlayerHitbox && setting().onHitboxes))
	{
		if (self->player1())
		{
			Hitboxes::drawPlayerHitbox(self->player1(), playerDrawNode);
		}
		if (self->player2())
		{
			Hitboxes::drawPlayerHitbox(self->player2(), playerDrawNode);
		}
	}
}

void __fastcall LevelEditorLayer::flipGravityH(gd::LevelEditorLayer* self, void*, gd::PlayerObject* player, bool idc, bool idc2) {
	//if (self->m_dualMode && !self->m_levelSettings->m_twoPlayerMode) {
	//	LevelEditorLayer::flipGravity(self, player, idc, idc2);
	//}
	//else if (self->m_dualMode && self->m_levelSettings->m_twoPlayerMode)
	//		player->flipGravity(idc, idc2);
	////if (player->m_gravityFlipped != idc) {
	////	player->flipGravity(idc, idc2);
	////	if (self->m_dualMode) {
	////		if (!self->m_levelSettings->m_twoPlayerMode) {
	////			LevelEditorLayer::flipGravity(self, player, idc, idc2);
	////		}
	////	}
	////}
	LevelEditorLayer::flipGravity(self, player, idc, idc2);
}

bool EditorPauseLayer_init(void* self, void* idc) {
	is_editor_paused = true;
	return matdash::orig<&EditorPauseLayer_init>(self, idc);
}

void EditorPauseLayer_dtor(void* self) {
	is_editor_paused = false;
	editorPauseLayer = nullptr;
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

bool touchIntersectsInput(CCNode* input, CCTouch* touch) {
	if (!input)
		return false;

	auto inp = reinterpret_cast<gd::CCTextInputNode*>(input);
	auto isize = CCSize({ inp->getScaleX() * inp->getContentSize().width, inp->getScaleY() * inp->getContentSize().height });

	auto rect = cocos2d::CCRect{
		inp->getPositionX() - isize.width / 2,
		inp->getPositionY() - isize.height / 2,
		isize.width,
		isize.height
	};

	if (!rect.containsPoint(input->getParent()->convertTouchToNodeSpace(touch))) {
		reinterpret_cast<gd::CCTextInputNode*>(input)->getTextField()->detachWithIME();
		return false;
	}
	else
		return true;
}

void EditorUI_onPlaytest(gd::EditorUI* self, void* btn) {
	if (!g_holding_in_editor)
		return matdash::orig<&EditorUI_onPlaytest>(self, btn);
}

bool EditorUI_ccTouchBegan(gd::EditorUI* self, void* idc, void* idc2) {
	g_holding_in_editor = true;
	if (touchIntersectsInput(m_editorLayerInput->m_layerInput, reinterpret_cast<CCTouch*>(idc)))
		return true;
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
	this->m_editorLayer->m_groupIDFilter = -1;
	this->m_currentGroupLabel->setString("All");

	auto onBaseLayerBtn = static_cast<gd::CCMenuItemSpriteExtra*>(this->m_deselectBtn->getParent()->getChildByTag(45028));
	if (onBaseLayerBtn) {
		onBaseLayerBtn->setVisible(0);
		onBaseLayerBtn->setEnabled(false);
	}
	if (m_editorLayerInput)
		m_editorLayerInput->m_layerInput->setString(this->m_currentGroupLabel->getString());
}

void EditorUI::Callback::onGoToNextFreeLayer(CCObject* sender) {
	auto leveleditor = this->m_editorLayer;
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
	leveleditor->m_groupIDFilter = currentLayer;
	this->m_currentGroupLabel->setString(std::to_string(currentLayer).c_str());

	auto onBaseLayerBtn = static_cast<gd::CCMenuItemSpriteExtra*>(this->m_deselectBtn->getParent()->getChildByTag(45028));
	if (onBaseLayerBtn) {
		onBaseLayerBtn->setVisible(1);
		onBaseLayerBtn->setEnabled(true);
	}
	if (m_editorLayerInput)
		m_editorLayerInput->m_layerInput->setString(this->m_currentGroupLabel->getString());
}

bool __fastcall EditorUI::dtor_H(gd::EditorUI* self) {
	editUI = nullptr;
	if (gd::GameManager::sharedState()->getGameVariable(GameVariable::GLOBAL_CLIPBOARD)) savedClipboard = self->clipboard();
	g_lastObject = nullptr;
	m_objectColor = nullptr;
	m_objectGroup = nullptr;
	m_objectRotation = nullptr;
	m_objectXPos = nullptr;
	m_objectYPos = nullptr;
	m_objectKey = nullptr;
	m_objectAddress = nullptr;
	m_objectType = nullptr;
	m_objectsSelected = nullptr;
	m_editorLayerInput = nullptr;
	m_gridSizeLabel = nullptr;
	m_objectZ = nullptr;
	MyEditorLayer::s_instance = nullptr;
	EditorUI::dtor(self);
}

void EditorUI::Callback::onGoToGroup(CCObject* sender) {
	for (auto obj : CCArrayExt<gd::GameObject*>(this->getSelectedObjectsOfCCArray())) {
		if (obj) {
			int objectGroup = obj->m_editorGroup;
			this->m_editorLayer->m_groupIDFilter = objectGroup;
			this->m_currentGroupLabel->setString(std::to_string(objectGroup).c_str());

			auto onBaseLayerBtn = static_cast<gd::CCMenuItemSpriteExtra*>(this->m_deselectBtn->getParent()->getChildByTag(45028));
			if (onBaseLayerBtn) {
				onBaseLayerBtn->setVisible(1);
				onBaseLayerBtn->setEnabled(true);
			}
			if (m_editorLayerInput)
				m_editorLayerInput->m_layerInput->setString(this->m_currentGroupLabel->getString());
		}
	}
}

std::string colorToString(int id) {
	switch (id) {
	case 0: return "Default"; break;
	case 1: return "P-Col 1"; break;
	case 2: return "P-Col 2"; break;
	case 3: return "Col1"; break;
	case 4: return "Col2"; break;
	case 5: return "LBG"; break;
	case 6: return "Col3"; break;
	case 7: return "Col4"; break;
	case 8: return "3DL"; break;
	case 9: return "White"; break;
	default: return "Unknown"; break;
	}
}

std::string typeToString(gd::GameObjectType type) {
	switch (type) {
	case gd::GameObjectType::kGameObjectTypeSolid: return "Solid"; break;
	case gd::GameObjectType::kGameObjectTypeHazard: return "Hazard"; break;
	case gd::GameObjectType::kGameObjectTypeInverseGravityPortal: return "Inverse Gravity Portal"; break;
	case gd::GameObjectType::kGameObjectTypeNormalGravityPortal: return "Normal Gravity Portal"; break;
	case gd::GameObjectType::kGameObjectTypeShipPortal: return "Ship Portal"; break;
	case gd::GameObjectType::kGameObjectTypeCubePortal: return "Cube Portal"; break;
	case gd::GameObjectType::kGameObjectTypeDecoration: return "Decoration"; break;
	case gd::GameObjectType::kGameObjectTypePulseObjects: return "Pulse Object"; break;
	case gd::GameObjectType::kGameObjectTypeYellowJumpPad: return "Yellow Jump Pad"; break;
	case gd::GameObjectType::kGameObjectTypePinkJumpPad: return "Pink Jump Pad"; break;
	case gd::GameObjectType::kGameObjectTypeGravityPad: return "Gravity Pad"; break;
	case gd::GameObjectType::kGameObjectTypeYellowJumpRing: return "Yellow Jump Ring"; break;
	case gd::GameObjectType::kGameObjectTypePinkJumpRing: return "Pink Jump Ring"; break;
	case gd::GameObjectType::kGameObjectTypeGravityRing: return "Gravity Ring"; break;
	case gd::GameObjectType::kGameObjectTypeInverseMirrorPortal: return "Inverse Mirror Portal"; break;
	case gd::GameObjectType::kGameObjectTypeNormalMirrorPortal: return "Normal Mirror Portal"; break;
	case gd::GameObjectType::kGameObjectTypeBallPortal: return "Ball Portal"; break;
	case gd::GameObjectType::kGameObjectTypeRegularSizePortal: return "Regular Size Portal"; break;
	case gd::GameObjectType::kGameObjectTypeMiniSizePortal: return "Mini Size Portal"; break;
	case gd::GameObjectType::kGameObjectTypeUfoPortal: return "UFO Portal"; break;
	case gd::GameObjectType::kGameObjectTypeModifier: return "Modifier"; break;
	case gd::GameObjectType::kGameObjectTypeBreakable: return "Breakable"; break;
	case gd::GameObjectType::kGameObjectTypeSecretCoin: return "Secret Coin"; break;
	case gd::GameObjectType::kGameObjectTypeDualPortal: return "Dual Portal"; break;
	case gd::GameObjectType::kGameObjectTypeSoloPortal: return "Solo Portal"; break;
	case gd::GameObjectType::kGameObjectTypeSlope: return "Slope"; break;
	case gd::GameObjectType::kGameObjectTypeWavePortal: return "Wave Portal"; break;
	default: return "Unknown"; break;
	}
}

void EditorUI::updateObjectInfo(gd::EditorUI* self) {
	auto objectInfo = static_cast<CCLabelBMFont*>(self->getChildByTag(3207));
	if (objectInfo) {
		std::stringstream ss;

		if (self->m_selectedObject) {
			ss << "C: " << colorToString((int)self->m_selectedObject->m_defaultColorMode) << " (" << (int)self->m_selectedObject->m_defaultColorMode << ")" << "\n";
			ss << "G: " << self->m_selectedObject->m_editorGroup << "\n";
			ss << "Rot: " << self->m_selectedObject->getRotation() << "\n";
			ss << "X: " << self->m_selectedObject->getPositionX() << "\n";
			ss << "Y: " << self->m_selectedObject->getPositionY() << "\n";
			ss << "ID: " << self->m_selectedObject->m_objectID << "\n";
			ss << "Type: " << typeToString(self->m_selectedObject->m_objectType) << "\n";
			ss << "Addr: 0x" << std::hex << reinterpret_cast<uintptr_t>(self->m_selectedObject) << std::dec << "\n";

			objectInfo->setString(ss.str().c_str());
		}
		else if (self->m_selectedObjects->count() > 1) {
			std::stringstream ss;

			ss << "Objects: " << self->m_selectedObjects->count() << "\n";

			objectInfo->setString(ss.str().c_str());
		}
		else objectInfo->setString("");
	}
}

bool __fastcall EditorUI::init_H(gd::EditorUI* self, void*, gd::LevelEditorLayer* editor) {
	editUI = self;

	bool result = EditorUI::init(self, editor);

	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();

	gd::GameManager::sharedState()->setIntGameVariable(GameVariable::COLOR_FILTER, 0);
	colorFilterLabel->setString("D");

	// Object info

	auto objectInfo = CCLabelBMFont::create("", "chatFont.fnt");
	objectInfo->setScale(.6f);
	objectInfo->setAnchorPoint({ 0.f, 1.f });
	objectInfo->setPosition(director->getScreenLeft() + 50.f, director->getScreenTop() - 50.f);
	self->addChild(objectInfo, 0, 3207);

	//

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
	if (self->m_editorLayer->m_groupIDFilter == -1) {
		onBaseLayerBtn->setVisible(0);
		onBaseLayerBtn->setEnabled(false);
	}

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
	deletebtn->setPosition({ redoBtn->getPositionX() + 50, redoBtn->getPositionY() });
	deletebtn->setTag(45030);
	deletebtn->setOpacity(175);
	deletebtn->setColor({ 166, 166, 166 });
	deletebtn->setEnabled(false);

	leftMenu->addChild(deletebtn);

	auto goToGroupSpr = CCSprite::create("GJ_button_04.png");
	if (!goToGroupSpr->initWithFile("GJ_goToGroupBtn_001.png")) {
		goToGroupSpr->create("GJ_button_04.png");
	}
	auto goToGroupBtn = gd::CCMenuItemSpriteExtra::create(goToGroupSpr, nullptr, self, menu_selector(EditorUI::Callback::onGoToGroup));
	goToGroupBtn->setTag(45031);
	goToGroupSpr->setScale(0.75f);
	goToGroupBtn->setPosition({ self->getDeselectBtn()->getPositionX() - 84, self->getDeselectBtn()->getPositionY() - 1 });
	goToGroupBtn->setVisible(false);
	rightMenu->addChild(goToGroupBtn);

	if (gd::GameManager::sharedState()->getGameVariable(GameVariable::GLOBAL_CLIPBOARD))
	{
		if (!savedClipboard.empty()) {
			self->clipboard() = savedClipboard;
			self->updateButtons();
		}
	}

	auto Slider = from<gd::Slider*>(self, 0x164);
	Slider->setAnchorPoint({ 0.f, 0.f });
	Slider->setPosition({ director->getScreenLeft() + 280, director->getScreenTop() - 19.5f });
	Slider->setScale(0.8f);

	setting().gridSize = 30.f;
	auto offset = ccp((size.width / 2.f) + 140.f, (size.height) - 19.5f);
	if ((size.width / size.height) < 1.7f) offset = ccp(size.width / 2.f, director->getScreenTop() - 50.f);
	auto gridSizeMenu = CCMenu::create();
	gridSizeMenu->setPosition(offset);
	self->addChild(gridSizeMenu);
	auto gridMenuBG = extension::CCScale9Sprite::create("square02_small.png", { 0.f, 0.f, 0.f, 0.f });
	gridMenuBG->setZOrder(-1);
	gridMenuBG->setOpacity(75);
	gridMenuBG->setContentSize({ 96.f, 28.f });
	gridSizeMenu->addChild(gridMenuBG);
	auto incrementSpr = CCSprite::createWithSpriteFrameName("GJ_zoomInBtn_001.png");
	incrementSpr->setScale(0.5f);
	auto incrementBtn = gd::CCMenuItemSpriteExtra::create(incrementSpr, incrementSpr, self, menu_selector(GridSizeCB::onIncrement));
	incrementBtn->setPositionX(-35.f);
	auto decrementSpr = CCSprite::createWithSpriteFrameName("GJ_zoomOutBtn_001.png");
	decrementSpr->setScale(0.5f);
	auto decrementBtn = gd::CCMenuItemSpriteExtra::create(decrementSpr, decrementSpr, self, menu_selector(GridSizeCB::onDecrement));
	decrementBtn->setPositionX(35.f);
	gridSizeMenu->addChild(incrementBtn);
	gridSizeMenu->addChild(decrementBtn);
	m_gridSizeLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_gridSizeLabel->setString(CCString::createWithFormat("%.02f%", setting().gridSize)->getCString());
	m_gridSizeLabel->setScale(0.4f);

	gridSizeMenu->addChild(m_gridSizeLabel);
	bool prevAnims_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::PREV_ANIMS);

	updateLastObjectX(self->getLevelEditorLayer());

	m_editorLayerInput = EditorLayerInput::create(self);
	m_editorLayerInput->setPosition({ self->m_currentGroupLabel->getPositionX() - 1.f, self->m_currentGroupLabel->getPositionY() - 1.f });
	self->m_currentGroupLabel->setVisible(0);
	self->addChild(m_editorLayerInput);

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
	auto size = setting().gridSize;
	if (roundf(size) == 30.f) {
		return EditorUI::updateGridNodeSize(self);
	}
	auto actualMode = self->selectedMode();
	from<int>(self, 0x228) = gd::EditorUI::Mode_Create;
	EditorUI::updateGridNodeSize(self);
	from<int>(self, 0x228) = actualMode;
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
	default: return EditorUI::moveForCommand(self, pos, com);
	}
}

void __fastcall EditorUI::transformObjectH(gd::EditorUI* self, void* edx, gd::GameObject* obj, gd::EditCommand com, bool idk) {
	CCArray* selectedObjects = self->getSelectedObjectsOfCCArray();
	auto selectedObjCount = selectedObjects->count();
	if (obj->canRotateFree()) {
		switch (com)
		{
		case rotationForCommand::kEditCommandRotate45CW:
			self->rotateObjects(selectedObjects, (45.f / selectedObjCount), { 0,0 });
			break;
		case rotationForCommand::kEditCommandRotate45CCW:
			self->rotateObjects(selectedObjects, -(45.f / selectedObjCount), { 0,0 });
			break;
		case rotationForCommand::kEditCommandRotate265CW:
			self->rotateObjects(selectedObjects, (26.f / selectedObjCount), { 0,0 });
			break;
		case rotationForCommand::kEditCommandRotate265CCW:
			self->rotateObjects(selectedObjects, -(26.f / selectedObjCount), { 0,0 });
			break;
		}
	}

	EditorUI::transformObject(self, obj, com, idk);
}

void EditorUI::Callback::onCustomMoveObject(CCObject* obj) {
	auto buttonTag = static_cast<gd::CCMenuItemSpriteExtra*>(obj)->getTag();
	this->moveObjectCall(static_cast<gd::EditCommand>(buttonTag));
}

void EditorUI::Callback::onCustomRotateObject(CCObject* obj) {
	auto buttonTag = static_cast<gd::CCMenuItemSpriteExtra*>(obj)->getTag();
	this->transformObjectCall(static_cast<gd::EditCommand>(buttonTag));
}

void __fastcall EditorUI::createMoveMenuH(gd::EditorUI* self) {
	EditorUI::createMoveMenu(self);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto boomScrollLayer = from<gd::BoomScrollLayer*>(self->editButtonBar(), 0xe8);
	auto extendLayer = from<gd::ExtendedLayer*>(boomScrollLayer, 0x158);
	auto buttonPage2 = reinterpret_cast<gd::ButtonPage*>(extendLayer->getChildren()->objectAtIndex(1));
	auto buttonPage2_menu = reinterpret_cast<CCMenu*>(buttonPage2->getChildren()->objectAtIndex(0));

	auto dotsNode = reinterpret_cast<CCSpriteBatchNode*>(boomScrollLayer->getChildren()->objectAtIndex(1));
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

	// Half

	auto halfLabel = CCLabelBMFont::create("1/2", "bigFont.fnt");
	halfLabel->setScale(0.5f);
	halfLabel->setPosition({ 20.f, 10.f });

	auto halfUp = self->getSpriteButton("edit_upBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(halfUp->getChildren()->objectAtIndex(0))->addChild(halfLabel, 5);
	halfUp->setPosition({ -105, -(winSize.height / 2) + 68 });
	movePageMenu->addChild(halfUp, 0, (int)moveForCommand::kEditCommandHalfUp);

	auto halfDown = self->getSpriteButton("edit_downBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(halfDown->getChildren()->objectAtIndex(0))->addChild(halfLabel, 5);
	halfDown->setPosition({ -65, -(winSize.height / 2) + 68 });
	movePageMenu->addChild(halfDown, 0, (int)moveForCommand::kEditCommandHalfDown);

	auto halfLeft = self->getSpriteButton("edit_leftBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(halfLeft->getChildren()->objectAtIndex(0))->addChild(halfLabel, 5);
	halfLeft->setPosition({ -25, -(winSize.height / 2) + 68 });
	movePageMenu->addChild(halfLeft, 0, (int)moveForCommand::kEditCommandHalfLeft);

	auto halfRight = self->getSpriteButton("edit_rightBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(halfRight->getChildren()->objectAtIndex(0))->addChild(halfLabel, 5);
	halfRight->setPosition({ 15, -(winSize.height / 2) + 68 });
	movePageMenu->addChild(halfRight, 0, (int)moveForCommand::kEditCommandHalfRight);

	// Quarter

	auto quarterLabel = CCLabelBMFont::create("1/4", "bigFont.fnt");
	quarterLabel->setScale(0.5f);
	quarterLabel->setPosition({ 20.f, 10.f });

	auto quarterUp = self->getSpriteButton("edit_upBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(quarterUp->getChildren()->objectAtIndex(0))->addChild(quarterLabel, 5);
	quarterUp->setPosition({ 55, -(winSize.height / 2) + 68 });
	movePageMenu->addChild(quarterUp, 0, (int)moveForCommand::kEditCommandQuarterUp);

	auto quarterDown = self->getSpriteButton("edit_downBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(quarterDown->getChildren()->objectAtIndex(0))->addChild(quarterLabel, 5);
	quarterDown->setPosition({ 95, -(winSize.height / 2) + 68 });
	movePageMenu->addChild(quarterDown, 0, (int)moveForCommand::kEditCommandQuarterDown);

	auto quarterLeft = self->getSpriteButton("edit_leftBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(quarterLeft->getChildren()->objectAtIndex(0))->addChild(quarterLabel, 5);
	quarterLeft->setPosition({ -105, -(winSize.height / 2) + 28 });
	movePageMenu->addChild(quarterLeft, 0, (int)moveForCommand::kEditCommandQuarterLeft);

	auto quarterRight = self->getSpriteButton("edit_rightBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(quarterRight->getChildren()->objectAtIndex(0))->addChild(quarterLabel, 5);
	quarterRight->setPosition({ -65, -(winSize.height / 2) + 28 });
	movePageMenu->addChild(quarterRight, 0, (int)moveForCommand::kEditCommandQuarterRight);

	// Eight

	auto eightLabel = CCLabelBMFont::create("1/8", "bigFont.fnt");
	eightLabel->setScale(0.5f);
	eightLabel->setPosition({ 20.f, 10.f });

	auto eightUp = self->getSpriteButton("edit_upBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(eightUp->getChildren()->objectAtIndex(0))->addChild(eightLabel, 5);
	eightUp->setPosition({ -25, -(winSize.height / 2) + 28 });
	movePageMenu->addChild(eightUp, 0, (int)moveForCommand::kEditCommandEightUp);

	auto eightDown = self->getSpriteButton("edit_downBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(eightDown->getChildren()->objectAtIndex(0))->addChild(eightLabel, 5);
	eightDown->setPosition({ 15, -(winSize.height / 2) + 28 });
	movePageMenu->addChild(eightDown, 0, (int)moveForCommand::kEditCommandEightDown);

	auto eightLeft = self->getSpriteButton("edit_leftBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(eightLeft->getChildren()->objectAtIndex(0))->addChild(eightLabel, 5);
	eightLeft->setPosition({ 55, -(winSize.height / 2) + 28 });
	movePageMenu->addChild(eightLeft, 0, (int)moveForCommand::kEditCommandEightLeft);

	auto eightRight = self->getSpriteButton("edit_rightBtn2_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(eightRight->getChildren()->objectAtIndex(0))->addChild(eightLabel, 5);
	eightRight->setPosition({ 95, -(winSize.height / 2) + 28 });
	movePageMenu->addChild(eightRight, 0, (int)moveForCommand::kEditCommandEightRight);

	// 4th page

	CCArray* movePage2CCArray = CCArray::create();
	auto moveButtonPage2 = gd::ButtonPage::create(movePage2CCArray);
	moveButtonPage2->setPositionX(moveButtonPage->getPositionX() + CCDirector::sharedDirector()->getWinSize().width / 2);
	auto movePageMenu2 = reinterpret_cast<CCMenu*>(moveButtonPage2->getChildren()->objectAtIndex(0));
	auto pageDot2 = CCSprite::create("smallDot.png");

	extendLayer->addChild(moveButtonPage2);
	pagesArray->insertObject(moveButtonPage2, 3);
	dotsArray->addObject(pageDot2);
	dotsNode->addChild(pageDot2);

	// 0.5

	auto smallerLabel = CCLabelBMFont::create("0.5", "bigFont.fnt");
	smallerLabel->setScale(0.5f);
	smallerLabel->setPosition({ 20.f, 10.f });

	auto smallerUp = self->getSpriteButton("edit_upBtn_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(smallerUp->getChildren()->objectAtIndex(0))->addChild(smallerLabel, 5);
	smallerUp->setPosition({ -105, -(winSize.height / 2) + 68 });
	movePageMenu2->addChild(smallerUp, 0, (int)moveForCommand::kEditCommandSmallerUp);

	auto smallerDown = self->getSpriteButton("edit_downBtn_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(smallerDown->getChildren()->objectAtIndex(0))->addChild(smallerLabel, 5);
	smallerDown->setPosition({ -65, -(winSize.height / 2) + 68 });
	movePageMenu2->addChild(smallerDown, 0, (int)moveForCommand::kEditCommandSmallerDown);

	auto smallerLeft = self->getSpriteButton("edit_leftBtn_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(smallerLeft->getChildren()->objectAtIndex(0))->addChild(smallerLabel, 5);
	smallerLeft->setPosition({ -25, -(winSize.height / 2) + 68 });
	movePageMenu2->addChild(smallerLeft, 0, (int)moveForCommand::kEditCommandSmallerLeft);

	auto smallerRight = self->getSpriteButton("edit_rightBtn_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(smallerRight->getChildren()->objectAtIndex(0))->addChild(smallerLabel, 5);
	smallerRight->setPosition({ 15, -(winSize.height / 2) + 68 });
	movePageMenu2->addChild(smallerRight, 0, (int)moveForCommand::kEditCommandSmallerRight);

	// Unit

	auto unitLabel = CCLabelBMFont::create("0.1", "bigFont.fnt");
	unitLabel->setScale(0.5f);
	unitLabel->setPosition({ 20.f, 10.f });

	auto unitUp = self->getSpriteButton("edit_upBtn_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(unitUp->getChildren()->objectAtIndex(0))->addChild(unitLabel, 5);
	unitUp->setPosition({ 55, -(winSize.height / 2) + 68 });
	movePageMenu2->addChild(unitUp, 0, (int)moveForCommand::kEditCommandQUnitUp);

	auto unitDown = self->getSpriteButton("edit_downBtn_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(unitDown->getChildren()->objectAtIndex(0))->addChild(unitLabel, 5);
	unitDown->setPosition({ 95, -(winSize.height / 2) + 68 });
	movePageMenu2->addChild(unitDown, 0, (int)moveForCommand::kEditCommandQUnitDown);

	auto unitLeft = self->getSpriteButton("edit_leftBtn_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(unitLeft->getChildren()->objectAtIndex(0))->addChild(unitLabel, 5);
	unitLeft->setPosition({ -105, -(winSize.height / 2) + 28 });
	movePageMenu2->addChild(unitLeft, 0, (int)moveForCommand::kEditCommandQUnitLeft);

	auto unitRight = self->getSpriteButton("edit_rightBtn_001.png", menu_selector(EditorUI::Callback::onCustomMoveObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(unitRight->getChildren()->objectAtIndex(0))->addChild(unitLabel, 5);
	unitRight->setPosition({ -65, -(winSize.height / 2) + 28 });
	movePageMenu2->addChild(unitRight, 0, (int)moveForCommand::kEditCommandQUnitRight);

	// Rotate 45

	auto rotate45Label = CCLabelBMFont::create("45", "bigFont.fnt");
	rotate45Label->setScale(.45f);
	rotate45Label->setPosition({ 20.f, 22.f });

	auto rotate45CW = self->getSpriteButton("edit_cwBtn_001.png", menu_selector(EditorUI::Callback::onCustomRotateObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(rotate45CW->getChildren()->objectAtIndex(0))->addChild(rotate45Label, 5);
	rotate45CW->setPosition({ -25, -(winSize.height / 2) + 28 });
	movePageMenu2->addChild(rotate45CW, 0, (int)rotationForCommand::kEditCommandRotate45CW);

	auto rotate45CCW = self->getSpriteButton("edit_ccwBtn_001.png", menu_selector(EditorUI::Callback::onCustomRotateObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(rotate45CCW->getChildren()->objectAtIndex(0))->addChild(rotate45Label, 5);
	rotate45CCW->setPosition({ 15, -(winSize.height / 2) + 28 });
	movePageMenu2->addChild(rotate45CCW, 0, (int)rotationForCommand::kEditCommandRotate45CCW);

	// Rotate 26

	auto rotate26Label = CCLabelBMFont::create("26", "bigFont.fnt");
	rotate26Label->setScale(.45f);
	rotate26Label->setPosition({ 20.f, 22.f });

	auto rotate26CW = self->getSpriteButton("edit_cwBtn_001.png", menu_selector(EditorUI::Callback::onCustomRotateObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(rotate26CW->getChildren()->objectAtIndex(0))->addChild(rotate26Label, 5);
	rotate26CW->setPosition({ 55, -(winSize.height / 2) + 28 });
	movePageMenu2->addChild(rotate26CW, 0, (int)rotationForCommand::kEditCommandRotate265CW);

	auto rotate26CCW = self->getSpriteButton("edit_ccwBtn_001.png", menu_selector(EditorUI::Callback::onCustomRotateObject), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(rotate26CCW->getChildren()->objectAtIndex(0))->addChild(rotate26Label, 5);
	rotate26CCW->setPosition({ 95, -(winSize.height / 2) + 28 });
	movePageMenu2->addChild(rotate26CCW, 0, (int)rotationForCommand::kEditCommandRotate265CCW);

	// Circle Tool
	
	auto circleToolLabel = CCLabelBMFont::create("Circle\nTool", "bigFont.fnt", 0.f, CCTextAlignment::kCCTextAlignmentCenter);
	circleToolLabel->setScale(.35f);
	circleToolLabel->setPosition({ 20.f, 22.f });
	auto circleTool = self->getSpriteButton("edit_ccwBtn_001.png", menu_selector(CircleToolPopup::showCallback), nullptr, .9f);
	static_cast<gd::ButtonSprite*>(circleTool->getChildren()->objectAtIndex(0))->addChild(circleToolLabel, 5);
	circleTool->setPosition({ -105, -(winSize.height / 2) + 28 });

	buttonPage2_menu->addChild(circleTool);
}

void __fastcall EditorUI::selectObjectH(gd::EditorUI* self, void* edx, gd::GameObject* object) {
	auto selectedCustomMode = gd::GameManager::sharedState()->getIntGameVariable("0005");
	if (selectedCustomMode != 3) gd::GameManager::sharedState()->setIntGameVariable("0006", 0);
	int selectFilterObject = gd::GameManager::sharedState()->getIntGameVariable("0006");

	bool selectFilter_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::SELECT_FILTER);
	int colorFilter = gd::GameManager::sharedState()->getIntGameVariable(GameVariable::COLOR_FILTER);

	if ((selectFilterObject != 0) && selectFilter_enabled) {
		if (object->getObjectID() == selectFilterObject) return EditorUI::selectObject(self, object);
	}
	else if (colorFilter != 0) {
		if (object->getObjectColor() == colorFilter) return EditorUI::selectObject(self, object);
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
	int colorFilter = gd::GameManager::sharedState()->getIntGameVariable(GameVariable::COLOR_FILTER);

	if ((selectFilterObject != 0) && selectFilter_enabled) {
		auto filteredObjects = CCArray::create();
		for (int i = 0; i < objects->count(); i++) {
			if (reinterpret_cast<gd::GameObject*>(objects->objectAtIndex(i))->getObjectID() == selectFilterObject) {
				filteredObjects->addObject(objects->objectAtIndex(i));
			}
		}
		return EditorUI::selectObjects(self, filteredObjects);
	}
	else if (colorFilter != 0) {
		auto filteredObjects = CCArray::create();
		for (int i = 0; i < objects->count(); i++) {
			if (reinterpret_cast<gd::GameObject*>(objects->objectAtIndex(i))->getObjectColor() == colorFilter) {
				filteredObjects->addObject(objects->objectAtIndex(i));
			}
		}
		return EditorUI::selectObjects(self, filteredObjects);
	}
	else return EditorUI::selectObjects(self, objects);

	EditorUI::selectObjects(self, objects);
}

void __fastcall EditorUI::onCopyH(gd::EditorUI* self, void*, CCObject* sender) {
	EditorUI::onCopy(self, sender);
	bool copyString_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::COPY_STRING);
	if (copyString_enabled) {
		auto string = from<std::string>(self, 0x264);
		clipboard::write(string);
	}
}

void __fastcall EditorUI::sliderChangedH(gd::EditorUI* self, void*, CCObject* object) {
	EditorUI::sliderChanged(self, object);
}

void __fastcall EditorUI::setupDeleteMenuH(gd::EditorUI* self) {
	EditorUI::setupDeleteMenu(self);

	auto deleteMenu = from<CCMenu*>(self, 0x190);
	int colorFilter = gd::GameManager::sharedState()->getIntGameVariable(GameVariable::COLOR_FILTER);

	auto colorFilterSpr = CCSprite::create("GJ_button_04.png");
	colorFilterSpr->setScale(0.9f);
	auto onColorFilter = gd::CCMenuItemSpriteExtra::create(colorFilterSpr, nullptr, self, menu_selector(ColorFilterPopup::showCallback));
	onColorFilter->setPosition({ 33, -18 });
	colorFilterLabel = CCLabelBMFont::create("", "bigFont.fnt");
	colorFilterLabel->limitLabelWidth(16.f, .75f, .45f);
	colorFilterLabel->setPosition({ colorFilterSpr->getContentSize() / 2.f });
	colorFilterSpr->addChild(colorFilterLabel);

	deleteMenu->addChild(onColorFilter);
}

//void __fastcall EditorUI::keyDownH(gd::EditorUI* self, void*, enumKeyCodes key) {
//	static_assert(offsetof(CCDirector, m_pKeyboardDispatcher) == 0x4c, "it wrong!");
//	auto kb = CCDirector::sharedDirector()->m_pKeyboardDispatcher;
//
//	if (kb->getShiftKeyPressed() && key == KEY_E) {
//		EditorUI::Callback::rotate45CW(nullptr);
//		return;
//	}
//	else if (kb->getShiftKeyPressed() && key == KEY_Q) {
//		self->transformObjectCall(rotationForCommand::kEditCommandRotate45CCW);
//		return;
//	}
//	else {
//		EditorUI::keyDown(self, key);
//	}
//}

void __fastcall EditorUI::updateButtonsH(gd::EditorUI* self) {
	EditorUI::updateButtons(self);

	auto deleteBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(from<CCMenu*>(self->getRedoBtn(), 0xac)->getChildByTag(45030));

	if (deleteBtn) {
		if (self->getSelectedObjectsOfCCArray()->count()) {
			deleteBtn->setOpacity(255);
			deleteBtn->setColor({ 255, 255, 255 });
			deleteBtn->setEnabled(true);
		}
		else {
			deleteBtn->setOpacity(175);
			deleteBtn->setColor({ 166, 166, 166 });
			deleteBtn->setEnabled(false);
		}
	}

	auto onGoToGroup = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(from<CCMenu*>(self->getDeselectBtn(), 0xac)->getChildByTag(45031));

	if (onGoToGroup) {
		if (self->getSelectedObjectsOfCCArray()->count()/* == 1*/) {
			onGoToGroup->setVisible(1);
			onGoToGroup->setEnabled(true);
		}
		else {
			onGoToGroup->setVisible(0);
			onGoToGroup->setEnabled(false);
		}
	}
}

void __fastcall EditorUI::onGroupDownH(gd::EditorUI* self, void*, CCObject* obj) {
	auto onBaseLayerBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(from<CCMenu*>(self->getDeselectBtn(), 0xac)->getChildByTag(45028));
	if (onBaseLayerBtn) {
		if (from<int>(self->getLevelEditorLayer(), 0x12c) == 0) {
			onBaseLayerBtn->setVisible(0);
			onBaseLayerBtn->setEnabled(false);
		}
		if (self->m_editorLayer->m_groupIDFilter > 0) {
			onBaseLayerBtn->setVisible(1);
			onBaseLayerBtn->setEnabled(true);
		}
	}
	EditorUI::onGroupDown(self, obj);
	if (m_editorLayerInput)
		m_editorLayerInput->m_layerInput->setString(self->m_currentGroupLabel->getString());
}

void __fastcall EditorUI::onGroupUpH(gd::EditorUI* self, void*, CCObject* obj) {
	auto onBaseLayerBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(from<CCMenu*>(self->getDeselectBtn(), 0xac)->getChildByTag(45028));
	if (onBaseLayerBtn) {
		onBaseLayerBtn->setVisible(1);
		onBaseLayerBtn->setEnabled(true);
	}
	EditorUI::onGroupUp(self, obj);
	if (m_editorLayerInput)
		m_editorLayerInput->m_layerInput->setString(self->m_currentGroupLabel->getString());
}

void __fastcall EditorUI::moveObjectH(gd::EditorUI* self, void*, gd::GameObject* obj, CCPoint pos) {
	if (obj == nullptr) return; // rob is funny
	EditorUI::moveObject(self, obj, pos);
	reinterpret_cast<MyEditorLayer*>(self->getParent())->move_trigger(obj);
	updateLastObjectX(self->getLevelEditorLayer(), obj);
}

//CCPoint* __fastcall EditorUI::offsetForKeyH(gd::EditorUI* self, void*, CCPoint* offset, int objKey) {
//	return EditorUI::offsetForKey(self, 0, objKey);
//}

void EditorPauseLayer::Callback::VanillaSelectAllButton(CCObject*)
{
	auto leveleditor = from<gd::LevelEditorLayer*>(this, 0x1A8);
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
	if (MyEditorLayer::s_instance) {
		if (setting().onEditorPreview)
			MyEditorLayer::s_instance->update_preview_mode();
		else
			MyEditorLayer::s_instance->reset_colors();
	}
}

void EditorPauseLayer::Callback::SmallEditorStepToggler(CCObject*) {
	gd::GameManager::sharedState()->toggleGameVariable("0035");
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

void EditorPauseLayer::Callback::onCreateTriggers(CCObject* obj) {
	//auto lel = MyEditorLayer::s_instance;
	//lel->m_uiLayer->onDeselectAll(nullptr);
	//
	//auto pos = lel->m_gameLayer->convertToNodeSpace(CCDirector::sharedDirector()->getWinSize() / 2.f);
	//float y = pos.y;
	//auto objs = CCArray::create();

	//auto bgTrig = lel->createObject(29, { pos.x, y });
	//bgTrig->m_triggerColor = lel->m_levelSettings->m_backgroundColor->m_color;
	//bgTrig->m_triggerBlending = lel->m_levelSettings->m_backgroundColor->m_blending;
	//bgTrig->m_copyPlayerColor1 = lel->m_levelSettings->m_backgroundColor->m_custom == 1;
	//bgTrig->m_copyPlayerColor2 = lel->m_levelSettings->m_backgroundColor->m_custom == 2;

	//if (lel->m_groupIDFilter != -1)
	//	bgTrig->m_editorGroup = lel->m_groupIDFilter;

	//objs->addObject(bgTrig);

	//y += 30;

	//lel->m_uiLayer->selectObjects(objs);
	//objs->release();
	//lel->m_uiLayer->updateButtons();
}

float timeXPos(gd::LevelEditorLayer* self, float time) {
	auto l = self->getDrawGrid();
	return (l->timeForXPos(time));
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
	EPMButton->setPosition({ 30, 150 });
	EPMLabel->setScale(0.325f);
	EPMLabel->setPosition({ 50.25,150 });
	EPMLabel->setAnchorPoint({ 0.f, 0.5f });
	togglerMenu->addChild(EPMButton);
	togglerMenu->addChild(EPMLabel);

	auto SEPButton = gd::CCMenuItemToggler::create(SEPTogglerSprite(toggleOn, toggleOff), SEPTogglerSprite(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::SmallEditorStepToggler));
	auto SEPLabel = CCLabelBMFont::create("Small Editor Step", "bigFont.fnt");
	SEPButton->setScale(0.70f);
	SEPButton->setPosition({ 30, 120 });
	SEPLabel->setScale(0.325f);
	SEPLabel->setPosition({ 50.25, 120 });
	SEPLabel->setAnchorPoint({ 0.f, 0.5f });
	togglerMenu->addChild(SEPButton);
	togglerMenu->addChild(SEPLabel);

	constexpr auto pshandler = [](CCObject* self, CCObject*) {
		auto text = clipboard::read();
		auto editor = reinterpret_cast<gd::LevelEditorLayer*>(reinterpret_cast<CCNode*>(self)->getParent());
		editor->getEditorUI()->pasteObjects(text);
		editor->m_uiLayer->updateButtons();
		};

	auto pssprite = gd::ButtonSprite::create("Paste\nString", 0x28, 0, 0.6f, true, "bigFont.fnt", "GJ_button_04.png", 30.0);
	auto psbutton = gd::CCMenuItemSpriteExtra::create(pssprite, nullptr, self, to_handler<SEL_MenuHandler, pshandler>);

	auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	auto optionsBtn = gd::CCMenuItemSpriteExtra::create(optionsSpr, nullptr, self, menu_selector(gd::MenuLayer::onOptions));
	optionsBtn->setPosition({ -38, -65 });
	optionsSpr->setScale(.66f);

	auto options2Spr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
	options2Spr->setScale(0.85f);
	auto options2Btn = gd::CCMenuItemSpriteExtra::create(options2Spr, nullptr, self, menu_selector(EditorSettingsLayer::showCallback));
	options2Btn->setPosition({ -75, -65 });

	menu->setPosition({ director->getScreenRight(), director->getScreenTop() });
	psbutton->setPosition({ -50.f, -30.f });

	menu->addChild(psbutton);
	menu->addChild(optionsBtn);
	menu->addChild(options2Btn);

	auto levellength = CCLabelBMFont::create("", "goldFont.fnt");
	levellength->setString(CCString::createWithFormat("", 0)->getCString());
	levellength->setTag(49001);
	levellength->setAnchorPoint({ 0, 0.5f });
	levellength->setPosition({ director->getScreenLeft() + 10, director->getScreenTop() - 50 });
	levellength->setScale(0.5f);
	self->addChild(levellength);

	if (levellength) {
		if (MyEditorLayer::s_instance) {
			auto levelLength = MyEditorLayer::s_instance->m_level->m_levelLength;
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
	//mainMenu->setScale(.8f);
	//mainMenu->setAnchorPoint({ 0.f, 0.f });

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

	auto helpButton = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(bottommenu->getChildren()->objectAtIndex(1));

	self->m_audioOffBtn->setPositionY(self->m_audioOffBtn->getPositionY() - 12);
	self->m_audioOnBtn->setPositionY(self->m_audioOnBtn->getPositionY() - 12);
	helpButton->setPositionY(helpButton->getPositionY() - 12);

	mainMenu->addChild(saveLevel_btn);

	auto lengthLabel = CCLabelBMFont::create("Length:", "goldFont.fnt");
	lengthLabel->setPosition({ director->getScreenLeft() + 10, director->getScreenTop() - 30 });
	lengthLabel->setScale(0.5f);
	lengthLabel->setAnchorPoint({ 0, 0.5f });
	// time logic
	if (MyEditorLayer::s_instance) {
		float time = floorf(timeXPos(MyEditorLayer::s_instance, getLevelLength())); // fVar5
		int minutes = time / 60.f; // fVar10 = fvar5 / 60.0;
		float seconds = time - (minutes * 60); // iVar11 = fVar5 - (iVar1 * 60);
		std::string mins = CCString::createWithFormat("%i", minutes)->getCString();
		std::string secs = CCString::createWithFormat("%.00f%", seconds)->getCString();
		if (minutes < 1) {
			lengthLabel->setString(std::string("Length: " + secs + " seconds").c_str());
		}
		else {
			lengthLabel->setString(std::string("Length: " + mins + "min " + secs + "s").c_str());
		}
		self->addChild(lengthLabel);
	}
}

void __fastcall EditorPauseLayer::onSaveAndPlayH(gd::EditorPauseLayer* self, void*, CCObject* sender) {
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

void __fastcall EditorPauseLayer::onExitEditorH(gd::EditorPauseLayer* self, void*, CCObject* sender) {
	auto lel = self->getEditorLayer();
	if (lel) {
		auto playerDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(124));
		auto objDrawNode = reinterpret_cast<CCDrawNode*>(lel->gameLayer()->getChildByTag(125));
		playerDrawNode->clear();
		objDrawNode->clear();
	}
	editUI = nullptr;
	std::cout << "onExitEditor called" << std::endl;
	EditorPauseLayer::onExitEditor(self, sender);
}

void __fastcall EditorPauseLayer::keyDown_H(gd::EditorPauseLayer* self, void* edx, enumKeyCodes key) {
	if (key == KEY_Escape)
		reinterpret_cast<gd::EditorPauseLayer*>(reinterpret_cast<uintptr_t>(self) - 0xf4)->onResume(nullptr);
	else
		EditorPauseLayer::keyDown(self, key);
}

void __fastcall EditorPauseLayer::saveLevelH(gd::EditorPauseLayer* self) {
	if (setting().onResetEditPercentage) {
		std::cout << from<int>(self->getEditorLayer()->getLevel(), 0x1d8) << std::endl;
		if (from<gd::GJLevelType>(self->getEditorLayer()->getLevel(), 0x26c) == gd::GJLevelType::Editor) from<int>(self->getEditorLayer()->getLevel(), 0x1d8) = 0;
		std::cout << from<int>(self->getEditorLayer()->getLevel(), 0x1d8) << std::endl;
	}
	EditorPauseLayer::saveLevel(self);
}

void SetGroupIDLayer::Callback::onCurrentGroup(CCObject*) {
	CCArray* objs = editUI->getSelectedObjectsOfCCArray();
	int currentEditorGroup = editUI->getLevelEditorLayer()->getLayerGroup();

	auto setGroupIDLayer = CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(1);

	for (int i = 0; i < (objs->count()); i++)
	{
		auto obj = reinterpret_cast<gd::GameObject*>(objs->objectAtIndex(i));
		if (obj) {
			if (currentEditorGroup != -1) {
				from<int>(obj, 0x324) = currentEditorGroup;
				from<CCLabelBMFont*>(setGroupIDLayer, 0x1c4)->setString(std::to_string(currentEditorGroup).c_str());
				from<int>(setGroupIDLayer, 0x1c8) = currentEditorGroup;
			}
		}
	}

	if (m_objectGroupInput)
		m_objectGroupInput->m_layerInput->setString(from<CCLabelBMFont*>(this, 0x1c4)->getString());
}

bool __fastcall SetGroupIDLayer::initH(gd::SetGroupIDLayer* self, void* edx, gd::GameObject* obj, CCArray* arr) {
	if (!SetGroupIDLayer::init(self, obj, arr)) return false;

	auto menu = self->m_buttonMenu;
	auto onDown_btn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(1));
	auto onUp_btn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(2));
	onDown_btn->setSizeMult(1.f);
	onUp_btn->setSizeMult(1.f);

	auto setCurrentGroup_spr = gd::ButtonSprite::create("Current Group", 0x56, 0, 0.5f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
	auto setCurrentGroup_btn = gd::CCMenuItemSpriteExtra::create(setCurrentGroup_spr, setCurrentGroup_spr, self, menu_selector(SetGroupIDLayer::Callback::onCurrentGroup));
	setCurrentGroup_btn->setPosition({ -120.f, 0.f });

	menu->addChild(setCurrentGroup_btn);

	m_objectGroupInput = ObjectGroupInput::create(self);
	self->m_mainLayer->addChild(m_objectGroupInput, 5);
	m_objectGroupInput->setPosition(from<CCLabelBMFont*>(self, 0x1c4)->getPosition());
	from<CCLabelBMFont*>(self, 0x1c4)->setVisible(0);

	return true;
}

void __fastcall SetGroupIDLayer::updateGroupIDH(gd::SetGroupIDLayer* self) {
	SetGroupIDLayer::updateGroupID(self);
	if (m_objectGroupInput)
		m_objectGroupInput->m_layerInput->setString(from<CCLabelBMFont*>(self, 0x1c4)->getString());
}

void __fastcall SetGroupIDLayer::dtorH(gd::SetGroupIDLayer* self) {
	m_objectGroupInput = nullptr;
	SetGroupIDLayer::dtor(self);
}

std::string triggerIDxToTexture(int id) {
	switch (id)
	{
	case 29: return "edit_eTintBGBtn_001.png"; break;
	case 30: return "edit_eTintGBtn_001.png"; break;
	case 104: return "edit_eTintLBtn_001.png"; break;
	case 105: return "edit_eTintObjBtn_001.png"; break;
	case 744: return "edit_eTint3DLBtn_001.png"; break;
	case 221: return "edit_eTintCol01Btn_001.png"; break;
	case 717: return "edit_eTintCol02Btn_001.png"; break;
	case 718: return "edit_eTintCol03Btn_001.png"; break;
	case 743: return "edit_eTintCol04Btn_001.png"; break;
	default: return "edit_eTintBGBtn_001.png"; break;
	}
}

std::string colorIDToString(int id) {
	switch (id)
	{
	case 29: return "BG"; break;
	case 30: return "G"; break;
	case 104: return "L"; break;
	case 105: return "Obj"; break;
	case 744: return "3DL"; break;
	case 221: return "1"; break;
	case 717: return "2"; break;
	case 718: return "3"; break;
	case 743: return "3"; break;
	default: return "BG"; break;
	}
}

static std::array<int, 9> trigger_ids {
	29, 30, 104, 105, 744, 221, 717, 718, 743
};

int trigger_id_for_func = 0;

void updateCTypeLabel(gd::ColorSelectPopup* target, int id) {
	auto cTypeLabel = static_cast<CCLabelBMFont*>(target->getChildByTag(1703));
	if (cTypeLabel) {

	}
}

void updateTriggerTexture(gd::GameObject* obj, int id) {
	if (obj) {
		MyEditorLayer::s_instance->remove_trigger(obj);
		obj->m_objectID = id;
		auto newSpr = CCSprite::createWithSpriteFrameName(triggerIDxToTexture(obj->m_objectID).c_str());
		obj->m_textureName.clear();
		obj->m_textureName = triggerIDxToTexture(obj->m_objectID).c_str();
		obj->setTexture(newSpr->getTexture());
		obj->setTextureRect(newSpr->getTextureRect());
		MyEditorLayer::s_instance->insert_trigger(obj);
	}
	return;
}

void ColorSelectPopup::Callback::onTypeUp(CCObject*) {
	
}

void ColorSelectPopup::Callback::onTypeDown(CCObject*) {
	
}

bool __fastcall ColorSelectPopup::initH(gd::ColorSelectPopup* self, void*, gd::GameObject* obj, int color_id, int idk, int idk2) {
	setting().onShouldHue = true;
	if (!ColorSelectPopup::init(self, obj, color_id, idk, idk2)) return false;

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	if (self->m_durationLabel != nullptr) { // use this if you want to use on color triggers
		self->m_durationLabel->setVisible(0);

		auto fadeTimeLabel = CCLabelBMFont::create("FadeTime:", "goldFont.fnt");
		fadeTimeLabel->setPosition({ (winSize.width / 2) - 33, (winSize.height / 2) - 70 });
		self->m_mainLayer->addChild(fadeTimeLabel);

		m_fadeTime_input = FloatInputNode::create(CCSize(70, 35), 2.f, "bigFont.fnt");
		m_fadeTime_input->setScale(0.9f);
		m_fadeTime_input->set_value(self->m_duration);
		self->m_mainLayer->addChild(m_fadeTime_input);
		m_fadeTime_input->setPosition({ (winSize.width / 2) + 61, (winSize.height / 2) - 70 }); // 16:9: 346, 90

		//auto cTypeBG = extension::CCScale9Sprite::create("square02_small.png");
		//cTypeBG->setContentSize({ 40.f, 30.f });
		//cTypeBG->setOpacity(150);
		//cTypeBG->setPosition({ (winSize.width / 2.f) + 175.f, winSize.height / 2.f });
		//self->m_mainLayer->addChild(cTypeBG);

		//auto cTypeLabel = CCLabelBMFont::create("", "bigFont.fnt");
		//cTypeLabel->setString(colorIDToString(obj->m_objectID).c_str());
		//cTypeLabel->setScale(.6f);
		//cTypeLabel->setPosition({ (winSize.width / 2.f) + 175.f, winSize.height / 2.f });
		//self->m_mainLayer->addChild(cTypeLabel, 0, 1703);

		//auto decCTypeSpr = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
		//auto decCType = gd::CCMenuItemSpriteExtra::create(decCTypeSpr, nullptr, self, menu_selector(ColorSelectPopup::Callback::onTypeDown));
		//decCType->setPosition({ 175.f, 100.f });
		//self->m_buttonMenu->addChild(decCType);

		//auto incCTypeSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
		//auto incCType = gd::CCMenuItemSpriteExtra::create(incCTypeSpr, nullptr, self, menu_selector(ColorSelectPopup::Callback::onTypeUp));
		//incCType->setPosition({ 175.f, 160.f });
		//self->m_buttonMenu->addChild(incCType);
	}

	m_colorInputWidget = RGBColorInputWidget::create(self);

	m_colorInputWidget->setPosition({ CCDirector::sharedDirector()->getScreenLeft() + 67.5f, (winSize.height / 2.f) + 20.f });

	self->m_mainLayer->addChild(m_colorInputWidget);

	return true;
}

void __fastcall ColorSelectPopup::dtorH(gd::ColorSelectPopup* self) {
	setting().onShouldHue = false;
	ColorSelectPopup::dtor(self);
}

void __fastcall ColorSelectPopup::sliderChangedH(gd::ColorSelectPopup* self, void*, CCObject* sender) {
	auto slider = self->m_durationSlider;
	auto sliderValue = slider->getValue() * 10.f;
	m_fadeTime_input->set_value(floor(sliderValue * 100) / 100);
	std::cout << sliderValue << std::endl;
	ColorSelectPopup::sliderChanged(self, sender);
}

void __fastcall ColorSelectPopup::closeColorSelectH(gd::ColorSelectPopup* self, void*, CCObject* sender) {
	if (m_fadeTime_input != nullptr) {
		self->m_duration = m_fadeTime_input->get_value().value_or(m_fadeTime);
	}
	m_fadeTime_input = nullptr;
	m_colorInputWidget = nullptr;
	//m_hsvLabel = nullptr;
	//m_customFadeTimeInput = nullptr;
	ColorSelectPopup::closeColorSelect(self, sender);
}

void __fastcall ColorSelectPopup::colorValueChangedH(gd::ColorSelectPopup* self, void*, ccColor3B color) {
	ColorSelectPopup::colorValueChanged(self, color);
	if (m_colorInputWidget != nullptr) {
		m_colorInputWidget->update_labels(true, true, true);
	}
	//if (m_hsvLabel != nullptr) {
	//	//auto& colorPicker = self->m_colorPicker->getColorValue();
	//	auto jjj = color_utils::rgb_to_hsv({ color.r / 255.f, color.g / 255.f, color.b / 255.f });
	//	m_hsvLabel->setString(std::string("H: " + std::to_string(jjj.h) + " S: " + std::to_string(jjj.s) + " V: " + std::to_string(jjj.v)).c_str());
	//}
}

auto flipGravityToggle(CCSprite* toggleOn, CCSprite* toggleOff) {
	auto flipGravity_enabled = levelSettingsLayer->m_settingsObject->m_isFlipped;
	return (flipGravity_enabled) ? toggleOn : toggleOff;
}

bool __fastcall LevelSettingsLayer::initH(gd::LevelSettingsLayer* self, void*, gd::LevelSettingsObject* settingsObject) {
	levelSettingsLayer = self;
	if (!LevelSettingsLayer::init(self, settingsObject)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	if (from<gd::CustomSongWidget*>(self, 0x230) != nullptr) {
		auto flipGravityLabel = CCLabelBMFont::create("Start Flipped", "goldFont.fnt");
		flipGravityLabel->setScale(0.5f);
		flipGravityLabel->setAnchorPoint({ 0.f, 0.5f });
		flipGravityLabel->setPosition({ (winSize.width / 2.f) - 120.f, (winSize.height / 2.f) - 134.f });
		self->m_mainLayer->addChild(flipGravityLabel);

		auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

		auto menu = self->m_buttonMenu;
		auto flipGravityToggler = gd::CCMenuItemToggler::create(flipGravityToggle(toggleOn, toggleOff), flipGravityToggle(toggleOff, toggleOn), self, menu_selector(gd::LevelSettingsLayer::onGravityFlipped));
		flipGravityToggler->setScale(0.7f);
		flipGravityToggler->setPosition(menu->convertToNodeSpace({(winSize.width / 2.f) - 135.f, (winSize.height / 2.f) - 135.f}));
		menu->addChild(flipGravityToggler);
	}

	return true;
}

void __fastcall LevelSettingsLayer::dtorH(gd::LevelSettingsLayer* self) {
	levelSettingsLayer = nullptr;
	LevelSettingsLayer::dtor(self);
}

void __fastcall DrawGridLayer::addToSpeedObjectsH(gd::DrawGridLayer* self, void*, gd::GameObject* gameObject) {
	cocos2d::CCArray* speedObjects = from<CCArray*>(self, 0x150);
	bool bVar1 = speedObjects->containsObject(gameObject);
	if (!bVar1) {
		speedObjects->addObject(gameObject);
		from<int>(self, 0x17c) = 1;
	}
}

float timeBetweenPosition(float a, float b) {
	auto l = editUI->m_editorLayer->getDrawGrid();
	return std::abs(l->timeForXPos(a) - l->timeForXPos(b));
}

void __fastcall DrawGridLayer::drawH(gd::DrawGridLayer* self) {
	DrawGridLayer::draw(self);

	auto director = CCDirector::sharedDirector();

	gd::LevelEditorLayer* lel = from<gd::LevelEditorLayer*>(self, 0x124);

	if (setting().onEditorExtension) {
		glLineWidth(1);
		ccDrawColor4B(0xff, 0xff, 0xff, 0x96);
		ccDrawLine({ 0, 90 }, { 0, 1590 });
	}

	bool inPlaytest = self->m_levelEditorLayer->m_playerState == 1;

	bool drawDurationLines = true;

	for (int i = self->m_levelEditorLayer->m_firstVisibleSection + 1; i <= self->m_levelEditorLayer->m_lastVisibleSection - 1; i++) {
		if (i < 0) continue;
		if (i >= self->m_levelEditorLayer->m_levelSections->count()) break;

		auto objectAtIndex = self->m_levelEditorLayer->m_levelSections->objectAtIndex(i);
		auto objArr = reinterpret_cast<CCArray*>(objectAtIndex);

		for (int j = 0; j < objArr->count(); j++) {
			auto obj = reinterpret_cast<gd::GameObject*>(objArr->objectAtIndex(j));

			switch (obj->m_objectID) {
			case 29: case 30: case 104: case 105: case 744: case 221: case 717: case 718: case 743:
				auto triggerTimePos = self->timeForXPos(obj->getPositionX());
				auto triggerFadeEnd = self->xPosForTime(triggerTimePos + obj->m_triggerDuration);

				if (obj->m_triggerDuration > 0) {
					glLineWidth(2);
					ccDrawColor4B(100, 100, 100, 75);
					ccDrawLine(obj->getPosition(), { triggerFadeEnd, obj->getPositionY() });
				}
				break;
			}
		}
	}
}

//void __fastcall LevelEditorLayer::drawH(gd::LevelEditorLayer* self) {
//	LevelEditorLayer::draw(self);
//
//	auto director = CCDirector::sharedDirector();
//}

//bool isEasedScrollLayer(gd::BoomScrollLayer* self) {
//	if (!self->getParent())
//		return false;
//
//	if (vtable_cast(self->getParent(), 0x11c804))
//		return false;
//
//	if (vtable_cast(self->getParent(), 0x114b90))
//		return false;
//
//	return true;
//}
//
//class BoomScrollLayerCB : public gd::BoomScrollLayer {
//public:
//	void onGoToPage(CCObject* obj) {
//		auto p = static_cast<CCNode*>(obj)->getTag();
//		if (isEasedScrollLayer(this))
//			this->moveToPage(p);
//		else {
//			this->instantMoveToPage(p - 1);
//			this->instantMoveToPage(p);
//		}
//	}
//};
//
//void(__thiscall* BoomScrollLayer_updateDots)(gd::BoomScrollLayer*, float);
//void __fastcall BoomScrollLayer_updateDotsH(gd::BoomScrollLayer* self, void*, float width) {
//	BoomScrollLayer_updateDots(self, width);
//
//	if (editUI) {
//		if (self->m_dotsArray && self->m_dotsArray->count()) {
//			auto scale = static_cast<CCSprite*>(self->m_dotsArray->objectAtIndex(0))->getScale();
//			auto menu = self->getChildByTag(0x4490);
//			if (menu) {
//				menu->setPosition((self->getScaleX() * self->getContentSize().width) / 2.f, 5.f * scale);
//			}
//		}
//	}
//}
//
//bool(__thiscall* BoomScrollLayer_init)(gd::BoomScrollLayer*, CCArray*, int, bool, CCArray*, gd::DynamicScrollDelegate*);
//bool __fastcall BoomScrollLayer_initH(gd::BoomScrollLayer* self, void*, CCArray* pages, int idk, bool idk2, CCArray* idk3, gd::DynamicScrollDelegate* delegate) {
//	if (!BoomScrollLayer_init(self, pages, idk, idk2, idk3, delegate)) return false;
//
//	if (editUI) {
//		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x823e), "\x90\x90\x90\x90", 4, NULL);
//		auto menu = CCMenu::create();
//
//		auto i = 0;
//		for (auto dot : CCArrayExt<CCSprite*>(self->m_dotsArray)) {
//			dot->retain();
//			dot->removeFromParent();
//			dot->setPosition({ 0.f, 0.f });
//			auto dotBtn = gd::CCMenuItemSpriteExtra::create(dot, nullptr, self, menu_selector(BoomScrollLayerCB::onGoToPage));
//			dotBtn->setSizeMult(3.f);
//			dotBtn->setTag(i++);
//			dotBtn->setUserData(reinterpret_cast<void*>(idk2));
//			menu->addChild(dotBtn);
//			dot->release();
//		}
//
//		menu->alignItemsHorizontallyWithPadding((9.f - self->m_dotsArray->count()) / 2.2f);
//		menu->setPosition((self->getScaleX() * self->getContentSize().width) / 2.f, 8.f);
//		menu->setTag(0x4490);
//		self->addChild(menu, 5);
//	}
//
//	return true;
//}

void(__thiscall* GameObject_customSetup)(gd::GameObject*);
void __fastcall GameObject_customSetupH(gd::GameObject* self) {
	GameObject_customSetup(self);

	if (editUI && gd::GameManager::sharedState()->getGameVariable(GameVariable::EXPERIMENTAL_LAYERING))
		EditorObjectLayering::updateObjLayering(self);
}

//gd::CCMenuItemSpriteExtra* __fastcall EditorUI::getSpriteButtonH(gd::EditorUI* self, void*, const char* sprite, SEL_MenuHandler callback, CCMenu* menu, float scale, int buttonID, CCPoint idk) {
//	return EditorUI::getSpriteButton(self, sprite, callback, menu, scale, 1, {0,-10});
//}

void __fastcall Scheduler::update_H(CCScheduler* self, void* edx, float dt) {
	Scheduler::update(self, dt);

	auto play_layer = gd::GameManager::sharedState()->getPlayLayer();

	if (play_layer) {
		auto labelsMenu = reinterpret_cast<CCMenu*>(play_layer->getChildByTag(7900));
		auto cheatIndicator = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45072));

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
}

void Scheduler::mem_init() {
	MH_CreateHook(
		reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("libcocos2d.dll"), "?update@CCScheduler@cocos2d@@UAEXM@Z")),
		Scheduler::update_H,
		reinterpret_cast<void**>(&Scheduler::update));
}

void LevelEditorLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8e180), LevelEditorLayer::removeObjectH, reinterpret_cast<void**>(&LevelEditorLayer::removeObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x91620), LevelEditorLayer::updateH, reinterpret_cast<void**>(&LevelEditorLayer::update));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x905b0), LevelEditorLayer::flipGravityH, reinterpret_cast<void**>(&LevelEditorLayer::flipGravity));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xee5e0), LevelEditorLayer::drawH, reinterpret_cast<void**>(&LevelEditorLayer::draw));
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
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41ae0), EditorUI::updateGridNodeSizeH, reinterpret_cast<void**>(&EditorUI::updateGridNodeSize));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x49d20), EditorUI::createMoveMenuH, reinterpret_cast<void**>(&EditorUI::createMoveMenu));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x47f10), EditorUI::selectObjectH, reinterpret_cast<void**>(&EditorUI::selectObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x47fa0), EditorUI::selectObjectsH, reinterpret_cast<void**>(&EditorUI::selectObjects));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4b040), EditorUI::moveForCommandH, reinterpret_cast<void**>(&EditorUI::moveForCommand));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4b7e0), EditorUI::transformObjectH, reinterpret_cast<void**>(&EditorUI::transformObject));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4e550), EditorUI::keyDownH, reinterpret_cast<void**>(&EditorUI::keyDown));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41850), EditorUI::sliderChangedH, reinterpret_cast<void**>(&EditorUI::sliderChanged));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x42080), EditorUI::setupDeleteMenuH, reinterpret_cast<void**>(&EditorUI::setupDeleteMenu));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x490c0), EditorUI::onCopyH, reinterpret_cast<void**>(&EditorUI::onCopy));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41450), EditorUI::updateButtonsH, reinterpret_cast<void**>(&EditorUI::updateButtons));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4afc0), EditorUI::onGroupDownH, reinterpret_cast<void**>(&EditorUI::onGroupDown));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4af50), EditorUI::onGroupUpH, reinterpret_cast<void**>(&EditorUI::onGroupUp));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4b410), EditorUI::moveObjectH, reinterpret_cast<void**>(&EditorUI::moveObject));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4efe0), EditorUI::offsetForKeyH, reinterpret_cast<void**>(&EditorUI::offsetForKey));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4ae20), EditorUI::editObjectH, reinterpret_cast<void**>(&EditorUI::editObject));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x48e70), EditorUI::onDuplicateH, reinterpret_cast<void**>(&EditorUI::onDuplicate));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41790), EditorUI::getSpriteButtonH, reinterpret_cast<void**>(&EditorUI::getSpriteButton));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4cbb0), EditorUI::drawH, reinterpret_cast<void**>(&EditorUI::draw));
	matdash::add_hook<&EditorUI_onPlaytest>(gd::base + 0x489c0);
	matdash::add_hook<&EditorUI_ccTouchBegan>(gd::base + 0x4d5e0);
	matdash::add_hook<&EditorUI_ccTouchEnded>(gd::base + 0x4de40);

	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8110), BoomScrollLayer_updateDotsH, reinterpret_cast<void**>(&BoomScrollLayer_updateDots));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x79b0), BoomScrollLayer_initH, reinterpret_cast<void**>(&BoomScrollLayer_init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6ee50), GameObject_customSetupH, reinterpret_cast<void**>(&GameObject_customSetup));
}

void EditorPauseLayer::mem_init() {
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x3e3d0),
		EditorPauseLayer::customSetup_H,
		reinterpret_cast<void**>(&EditorPauseLayer::customSetup));
	//MH_CreateHook(
	//	reinterpret_cast<void*>(gd::base + 0x3f340),
	//	EditorPauseLayer::onSaveAndExit_H,
	//	reinterpret_cast<void**>(&EditorPauseLayer::onSaveAndExit));
	//MH_CreateHook(
	//	reinterpret_cast<void*>(gd::base + 0x3f420),
	//	EditorPauseLayer::onExitNoSave_H,
	//	reinterpret_cast<void**>(&EditorPauseLayer::onExitNoSave)); // i need to hook flalert clicked instead
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x3f570),
		EditorPauseLayer::keyDown_H,
		reinterpret_cast<void**>(&EditorPauseLayer::keyDown));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3eec0), EditorPauseLayer::saveLevelH, reinterpret_cast<void**>(&EditorPauseLayer::saveLevel));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3f380), EditorPauseLayer::onExitEditorH, reinterpret_cast<void**>(&EditorPauseLayer::onExitEditor));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3f170),	EditorPauseLayer::onSaveAndPlayH,	reinterpret_cast<void**>(&EditorPauseLayer::onSaveAndPlay));
}

void preview_mode::init() {
	matdash::add_hook<&MyEditorLayer::updateVisibility, matdash::Thiscall>(gd::base + 0x8ef20);
	matdash::add_hook<&MyEditorLayer::addSpecial>(gd::base + 0x8ed10);
	matdash::add_hook<&MyEditorLayer::init>(gd::base + 0x8c2c0);
	matdash::add_hook<&MyEditorLayer::dtor>(gd::base + 0x8c080);
	matdash::add_hook<&MyEditorLayer::removeSpecial>(gd::base + 0x8ee30);

	matdash::add_hook<&EditorUI_deselectAll>(gd::base + 0x48380);
	matdash::add_hook<&EditorUI_updateZoom>(gd::base + 0x48c30);
	matdash::add_hook<&EditorUI_getLimitedPosition, matdash::Stdcall>(gd::base + 0x4b500);

	matdash::add_hook<&EditorPauseLayer_init>(gd::base + 0x3e2e0);
	matdash::add_hook<&EditorPauseLayer_dtor>(gd::base + 0x3e280);

	matdash::add_hook<&GameObject_shouldBlendColor>(gd::base + 0x6ece0);

	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x833e0), HardStreak_updateStrokeH, reinterpret_cast<void**>(&HardStreak_updateStroke));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe1270), PlayerObject_placeStreakPointH, reinterpret_cast<void**>(&PlayerObject_placeStreakPoint));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd9b50), PlayerObject_updateH, reinterpret_cast<void**>(&PlayerObject_update));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0de0), PlayerObject_fadeOutStreak2H, reinterpret_cast<void**>(&PlayerObject_fadeOutStreak2));
}

void SetGroupIDLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf78d0), SetGroupIDLayer::initH, reinterpret_cast<void**>(&SetGroupIDLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf7e80), SetGroupIDLayer::updateGroupIDH, reinterpret_cast<void**>(&SetGroupIDLayer::updateGroupID));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf7760), SetGroupIDLayer::dtorH, reinterpret_cast<void**>(&SetGroupIDLayer::dtor));
}

void ColorSelectPopup::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x29db0), ColorSelectPopup::initH, reinterpret_cast<void**>(&ColorSelectPopup::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x2b050), ColorSelectPopup::dtorH, reinterpret_cast<void**>(&ColorSelectPopup::dtor));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x2aec0), ColorSelectPopup::closeColorSelectH, reinterpret_cast<void**>(&ColorSelectPopup::closeColorSelect));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x2ae00), ColorSelectPopup::sliderChangedH, reinterpret_cast<void**>(&ColorSelectPopup::sliderChanged));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x2af60), ColorSelectPopup::colorValueChangedH, reinterpret_cast<void**>(&ColorSelectPopup::colorValueChanged));
}

// 26.11.24 holy fuck.
// 21.12.24 holy fucking fuck
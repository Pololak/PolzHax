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

void reorderObjectSection(gd::LevelEditorLayer* lel, gd::GameObject* obj) {
	auto sObj = lel->m_levelSections->objectAtIndex(obj->m_sectionIdx);
	lel->m_levelSections->removeObject((CCObject*)sObj, true);
	lel->m_objectCount -= 1;
	lel->addToSection(obj);
	return;
}

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
		if (showObjInfo_enabled) {
			EditorUI::updateObjectInfo();
		}

		auto secarr = this->getLevelSections();
		auto arrcount = secarr->count();
		auto objectDrawNode = reinterpret_cast<CCDrawNode*>(this->gameLayer()->getChildByTag(125));
		objectDrawNode->clear();
		//auto playerDrawNode = reinterpret_cast<CCDrawNode*>(this->gameLayer()->getChildByTag(124));
		//playerDrawNode->clear();
		if (setting().onHitboxes) {
			//if (arrcount != 0) {
			auto layer = this->gameLayer();
			float xp = -layer->getPositionX() / layer->getScale();
			if (setting().onSolidsHitbox) {
				for (int i = this->sectionForPos(xp) - (5 / layer->getScale()); i < this->sectionForPos(xp) + (6 / layer->getScale()); i++) {
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
				for (int i = this->sectionForPos(xp) - (5 / layer->getScale()); i < this->sectionForPos(xp) + (6 / layer->getScale()); i++) {
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
				for (int i = this->sectionForPos(xp) - (5 / layer->getScale()); i < this->sectionForPos(xp) + (6 / layer->getScale()); i++) {
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
		}

		if (circleToolPopup) {
			m_angle = angle_input->get_value().value_or(m_angle);
			m_step = step_input->get_value().value_or(m_step);
			auto objs = editUI->getSelectedObjectsOfCCArray();
			const auto amt = static_cast<size_t>(std::ceilf(m_angle / m_step) - 1.f);
			const auto obj_count = amt * objs->count();
			m_circleToolLabel->setString(("Copies: " + std::to_string(amt) + "\nObjects: " + std::to_string(obj_count)).c_str());
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
				if (gd::GameManager::sharedState()->getGameVariable(GameVariable::EXPERIMENTAL_LAYERING))
					EditorObjectLayering::updateObjLayering(this, object);
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

	if (self->m_player) {
		if (self->m_player->m_hardStreak) {
			std::cout << self->m_player->m_hardStreak->m_pointArray << std::endl;
		}
	}
}

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
	if (object == nullptr)
		return;
	matdash::orig<&EditorUI_moveObject>(self, object, to);
	reinterpret_cast<MyEditorLayer*>(self->getParent())->move_trigger(object);
	updateLastObjectX(self->getLevelEditorLayer(), object);
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
	from<int>(from<gd::EditorUI*>(sender, 0xFC)->getLevelEditorLayer(), 0x12C) = -1;
	from<CCLabelBMFont*>(from<gd::EditorUI*>(sender, 0xFC), 0x20C)->setString("All");

	auto onBaseLayerBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(from<CCMenu*>(from<gd::EditorUI*>(sender, 0xFC)->getDeselectBtn(), 0xac)->getChildByTag(45028));
	if (onBaseLayerBtn) {
		onBaseLayerBtn->setVisible(0);
		onBaseLayerBtn->setEnabled(false);
	}
	if (m_editorLayerInput)
		m_editorLayerInput->m_layerInput->setString(from<gd::EditorUI*>(sender, 0xFC)->m_currentGroupLabel->getString());
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

	auto onBaseLayerBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(from<CCMenu*>(from<gd::EditorUI*>(sender, 0xFC)->getDeselectBtn(), 0xac)->getChildByTag(45028));
	if (onBaseLayerBtn) {
		onBaseLayerBtn->setVisible(1);
		onBaseLayerBtn->setEnabled(true);
	}
	if (m_editorLayerInput)
		m_editorLayerInput->m_layerInput->setString(leveleditor->m_uiLayer->m_currentGroupLabel->getString());
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
	levelSettingsLayer = nullptr;
	MyEditorLayer::s_instance = nullptr;
	EditorUI::dtor(self);
}

void EditorUI::Callback::onGoToGroup(CCObject* sender) {
	for (auto obj : CCArrayExt<gd::GameObject*>(this->getSelectedObjectsOfCCArray())) {
		if (obj) {
			int objectGroup = obj->m_editorGroup;
			from<int>(from<gd::EditorUI*>(sender, 0xFC)->getLevelEditorLayer(), 0x12C) = objectGroup;
			from<CCLabelBMFont*>(from<gd::EditorUI*>(sender, 0xFC), 0x20C)->setString(std::to_string(objectGroup).c_str());

			auto onBaseLayerBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(from<CCMenu*>(from<gd::EditorUI*>(sender, 0xFC)->getDeselectBtn(), 0xac)->getChildByTag(45028));
			if (onBaseLayerBtn) {
				onBaseLayerBtn->setVisible(1);
				onBaseLayerBtn->setEnabled(true);
			}
			if (m_editorLayerInput)
				m_editorLayerInput->m_layerInput->setString(this->m_currentGroupLabel->getString());
		}
	}
}

bool __fastcall EditorUI::init_H(gd::EditorUI* self, void*, gd::LevelEditorLayer* editor) {
	editUI = self;

	bool result = EditorUI::init(self, editor);

	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();

	gd::GameManager::sharedState()->setIntGameVariable(GameVariable::COLOR_FILTER, 0);
	colorFilterLabel->setString("D");

	auto leftInfoSide = director->getScreenLeft() + 50.f;

	m_objectsSelected = CCLabelBMFont::create("Objects:", "chatFont.fnt");
	m_objectsSelected->setVisible(0);
	m_objectsSelected->setAnchorPoint({ 0.f, 0.5f });
	m_objectsSelected->setScale(0.66f);
	m_objectsSelected->setPosition({ leftInfoSide, director->getScreenTop() - 50.f });
	self->addChild(m_objectsSelected);

	m_objectColor = CCLabelBMFont::create("C:", "chatFont.fnt");
	m_objectColor->setVisible(0);
	m_objectColor->setAnchorPoint({ 0.f, 0.5f });
	m_objectColor->setScale(0.66f);
	m_objectColor->setPosition({ leftInfoSide, director->getScreenTop() - 60.f });
	self->addChild(m_objectColor);

	m_objectGroup = CCLabelBMFont::create("G:", "chatFont.fnt");
	m_objectGroup->setVisible(0);
	m_objectGroup->setAnchorPoint({ 0.f, 0.5f });
	m_objectGroup->setScale(0.66f);
	m_objectGroup->setPosition({ leftInfoSide, director->getScreenTop() - 70.f });
	self->addChild(m_objectGroup);

	m_objectRotation = CCLabelBMFont::create("Rot:", "chatFont.fnt");
	m_objectRotation->setVisible(0);
	m_objectRotation->setAnchorPoint({ 0.f, 0.5f });
	m_objectRotation->setScale(0.66f);
	m_objectRotation->setPosition({ leftInfoSide, director->getScreenTop() - 80.f });
	self->addChild(m_objectRotation);

	m_objectXPos = CCLabelBMFont::create("X:", "chatFont.fnt");
	m_objectXPos->setVisible(0);
	m_objectXPos->setAnchorPoint({ 0.f, 0.5f });
	m_objectXPos->setScale(0.66f);
	m_objectXPos->setPosition({ leftInfoSide, director->getScreenTop() - 90.f });
	self->addChild(m_objectXPos);

	m_objectYPos = CCLabelBMFont::create("Y:", "chatFont.fnt");
	m_objectYPos->setVisible(0);
	m_objectYPos->setAnchorPoint({ 0.f, 0.5f });
	m_objectYPos->setScale(0.66f);
	m_objectYPos->setPosition({ leftInfoSide, director->getScreenTop() - 100.f });
	self->addChild(m_objectYPos);

	m_objectKey = CCLabelBMFont::create("ObjID:", "chatFont.fnt");
	m_objectKey->setVisible(0);
	m_objectKey->setAnchorPoint({ 0.f, 0.5f });
	m_objectKey->setScale(0.66f);
	m_objectKey->setPosition({ leftInfoSide, director->getScreenTop() - 110.f });
	self->addChild(m_objectKey);

	m_objectAddress = CCLabelBMFont::create("Addr:", "chatFont.fnt");
	m_objectAddress->setVisible(0);
	m_objectAddress->setAnchorPoint({ 0.f, 0.5f });
	m_objectAddress->setScale(0.66f);
	m_objectAddress->setPosition({ leftInfoSide, director->getScreenTop() - 120.f });
	self->addChild(m_objectAddress);

	m_objectType = CCLabelBMFont::create("Type:", "chatFont.fnt");
	m_objectType->setVisible(0);
	m_objectType->setAnchorPoint({ 0.f, 0.5f });
	m_objectType->setScale(0.66f);
	m_objectType->setPosition({ leftInfoSide, director->getScreenTop() - 130.f });
	self->addChild(m_objectType);

	m_objectZ = CCLabelBMFont::create("Object Z:", "chatFont.fnt");
	m_objectZ->setVisible(0);
	m_objectZ->setAnchorPoint({ 0.f, 0.5f });
	m_objectZ->setScale(0.66f);
	m_objectZ->setPosition({ leftInfoSide, director->getScreenTop() - 140.f });
	self->addChild(m_objectZ);

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
	//deletebtn->setPosition({ -160 , 139.35 });
	deletebtn->setPositionX(redoBtn->getPositionX() + 50);
	deletebtn->setPositionY(redoBtn->getPositionY());
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
	if (prevAnims_enabled)
		RotateSaws::beginRotations(self->getLevelEditorLayer());

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
			self->rotateObjects(selectedObjects, (26.5f / selectedObjCount), { 0,0 });
			break;
		case rotationForCommand::kEditCommandRotate265CCW:
			self->rotateObjects(selectedObjects, -(26.5f / selectedObjCount), { 0,0 });
			break;
		}
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
	auto rotate45CW_second = CCLabelBMFont::create("45", "bigFont.fnt");
	rotate45CW_second->setScale(0.45f); // 45 lol
	rotate45CW_second->setPosition({ rotate45CW_base->getContentSize().width / 2, (rotate45CW_base->getContentSize().height / 2) + 2 });
	rotate45CW_second->setZOrder(2);
	auto rotate45CW_spr2 = CCSprite::createWithSpriteFrameName("edit_cwBtn_001.png");
	rotate45CW_spr2->setPosition({ rotate45CW_base->getContentSize().width / 2, (rotate45CW_base->getContentSize().height / 2) + 2 });
	rotate45CW_base->addChild(rotate45CW_second);
	rotate45CW_base->addChild(rotate45CW_spr2);
	rotate45CW_base->setScale(0.9f);
	auto rotate45CW_btn = gd::CCMenuItemSpriteExtra::create(rotate45CW_base, rotate45CW_base, self, menu_selector(EditorUI::Callback::rotate45CW));
	rotate45CW_btn->setPosition({ -25, -(winSize.height / 2) + 28 });

	auto rotate45CCW_base = CCSprite::create("GJ_button_01.png");
	auto rotate45CCW_second = CCLabelBMFont::create("45", "bigFont.fnt");
	rotate45CCW_second->setScale(0.45f); // 45 lol
	rotate45CCW_second->setPosition({ rotate45CCW_base->getContentSize().width / 2, (rotate45CCW_base->getContentSize().height / 2) + 2 });
	rotate45CCW_second->setZOrder(2);
	auto rotate45CCW_spr2 = CCSprite::createWithSpriteFrameName("edit_ccwBtn_001.png");
	rotate45CCW_spr2->setPosition({ rotate45CCW_base->getContentSize().width / 2, (rotate45CCW_base->getContentSize().height / 2) + 2 });
	rotate45CCW_base->addChild(rotate45CCW_second);
	rotate45CCW_base->addChild(rotate45CCW_spr2);
	rotate45CCW_base->setScale(0.9f);
	auto rotate45CCW_btn = gd::CCMenuItemSpriteExtra::create(rotate45CCW_base, rotate45CCW_base, self, menu_selector(EditorUI::Callback::rotate45CCW));
	rotate45CCW_btn->setPosition({ 15, -(winSize.height / 2) + 28 });

	auto rotate265CW_base = CCSprite::create("GJ_button_01.png");
	auto rotate265CW_second = CCLabelBMFont::create("26.5", "bigFont.fnt");
	rotate265CW_second->setScale(0.45f);
	rotate265CW_second->setPosition({ rotate265CW_base->getContentSize().width / 2, (rotate265CW_base->getContentSize().height / 2) + 2 });
	rotate265CW_second->setZOrder(2);
	auto rotate265CW_spr2 = CCSprite::createWithSpriteFrameName("edit_cwBtn_001.png");
	rotate265CW_spr2->setPosition({ rotate265CW_base->getContentSize().width / 2, (rotate265CW_base->getContentSize().height / 2) + 2 });
	rotate265CW_base->addChild(rotate265CW_second);
	rotate265CW_base->addChild(rotate265CW_spr2);
	rotate265CW_base->setScale(0.9f);
	auto rotate265CW_btn = gd::CCMenuItemSpriteExtra::create(rotate265CW_base, rotate265CW_base, self, menu_selector(EditorUI::Callback::rotate265CW));
	rotate265CW_btn->setPosition({ 55, -(winSize.height / 2) + 28 });

	auto rotate265CCW_base = CCSprite::create("GJ_button_01.png");
	auto rotate265CCW_second = CCLabelBMFont::create("26.5", "bigFont.fnt");
	rotate265CCW_second->setScale(0.45f);
	rotate265CCW_second->setPosition({ rotate265CCW_base->getContentSize().width / 2, (rotate265CCW_base->getContentSize().height / 2) + 2 });
	rotate265CCW_second->setZOrder(2);
	auto rotate265CCW_spr2 = CCSprite::createWithSpriteFrameName("edit_ccwBtn_001.png");
	rotate265CCW_spr2->setPosition({ rotate265CCW_base->getContentSize().width / 2, (rotate265CCW_base->getContentSize().height / 2) + 2 });
	rotate265CCW_base->addChild(rotate265CCW_second);
	rotate265CCW_base->addChild(rotate265CCW_spr2);
	rotate265CCW_base->setScale(0.9f);
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

	auto circleToolBase_spr = CCSprite::create("GJ_button_01.png");
	auto circleToolSecond_spr = CCSprite::createWithSpriteFrameName("edit_cwBtn_001.png");
	auto circleToolLabel = CCLabelBMFont::create("Circle\ntool", "bigFont.fnt", 0.f, CCTextAlignment::kCCTextAlignmentCenter);
	circleToolBase_spr->addChild(circleToolSecond_spr);
	circleToolSecond_spr->setPosition({ (circleToolBase_spr->getContentSize().width) / 2, ((circleToolBase_spr->getContentSize().height) / 2) + 2 });
	circleToolBase_spr->setScale(0.9f);
	circleToolBase_spr->addChild(circleToolLabel);
	circleToolLabel->setScale(0.35f);
	circleToolLabel->setPosition({ (circleToolBase_spr->getContentSize().width) / 2, ((circleToolBase_spr->getContentSize().height) / 2) + 2 });
	auto circleToolBtn = gd::CCMenuItemSpriteExtra::create(circleToolBase_spr, circleToolBase_spr, self, menu_selector(CircleToolPopup::showCallback));
	circleToolBtn->setPosition({ -105, -(winSize.height / 2) + 28 });

	buttonPage2_menu->addChild(circleToolBtn);
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
	std::cout << getLevelLength() << std::endl;
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

void EditorUI::updateObjectInfo() {
	if (editUI) {
		if (m_objectColor) {
			if (editUI->m_selectedObject == 0) m_objectColor->setVisible(0);
			else {
				m_objectColor->setVisible(1);
				auto colorID = static_cast<int>(editUI->m_selectedObject->m_customColorMode);
				switch (colorID) {
				case 0:
					m_objectColor->setString(CCString::createWithFormat("C: Default (0)", colorID)->getCString());
					break;
				case 1:
					m_objectColor->setString(CCString::createWithFormat("C: P-Col 1 (1)", colorID)->getCString());
					break;
				case 2:
					m_objectColor->setString(CCString::createWithFormat("C: P-Col 2 (2)", colorID)->getCString());
					break;
				case 3:
					m_objectColor->setString(CCString::createWithFormat("C: Col1 (3)", colorID)->getCString());
					break;
				case 4:
					m_objectColor->setString(CCString::createWithFormat("C: Col2 (4)", colorID)->getCString());
					break;
				case 5:
					m_objectColor->setString(CCString::createWithFormat("C: Light BG (5)", colorID)->getCString());
					break;
				case 6:
					m_objectColor->setString(CCString::createWithFormat("C: Col3 (6)", colorID)->getCString());
					break;
				case 7:
					m_objectColor->setString(CCString::createWithFormat("C: Col4 (7)", colorID)->getCString());
					break;
				case 8:
					m_objectColor->setString(CCString::createWithFormat("C: 3D-Line (8)", colorID)->getCString());
					break;
				case 9:
					m_objectColor->setString(CCString::createWithFormat("C: White (9)", colorID)->getCString());
					break;
				}
			}
		}
		if (m_objectGroup) {
			if (editUI->m_selectedObject == 0) m_objectGroup->setVisible(0);
			else {
				m_objectGroup->setVisible(1);
				m_objectGroup->setString(CCString::createWithFormat("G: %i", editUI->m_selectedObject->m_editorGroup)->getCString());
			}
		}
		if (m_objectRotation) {
			if (editUI->m_selectedObject == 0) m_objectRotation->setVisible(0);
			else {
				m_objectRotation->setVisible(1);
				m_objectRotation->setString(CCString::createWithFormat("Rot: %.01f%", editUI->m_selectedObject->getRotation())->getCString());
			}
		}
		if (m_objectXPos) {
			if (editUI->m_selectedObject == 0) m_objectXPos->setVisible(0);
			else {
				m_objectXPos->setVisible(1);
				m_objectXPos->setString(CCString::createWithFormat("X: %.01f%", editUI->m_selectedObject->getPositionX())->getCString());
			}
		}
		if (m_objectYPos) {
			if (editUI->m_selectedObject == 0) m_objectYPos->setVisible(0);
			else {
				m_objectYPos->setVisible(1);
				m_objectYPos->setString(CCString::createWithFormat("Y: %.01f%", editUI->m_selectedObject->getPositionY())->getCString());
			}
		}
		if (m_objectKey) {
			if (editUI->m_selectedObject == 0) m_objectKey->setVisible(0);
			else {
				m_objectKey->setVisible(1);
				m_objectKey->setString(CCString::createWithFormat("ID: %i", editUI->m_selectedObject->m_objectID)->getCString());
			}
		}
		if (m_objectAddress) {
			if (editUI->m_selectedObject == 0) m_objectAddress->setVisible(0);
			else {
				m_objectAddress->setVisible(1);
				m_objectAddress->setString(CCString::createWithFormat("Addr: 0x%p", editUI->m_selectedObject)->getCString());
			}
		}
		if (m_objectType) {
			if (editUI->m_selectedObject == 0) m_objectType->setVisible(0);
			else {
				m_objectType->setVisible(1);
				auto objectType = editUI->m_selectedObject->m_objectType;
				switch (objectType)
				{
				case gd::GameObjectType::kGameObjectTypeSolid:
					m_objectType->setString("Type: Solid (0)");
					break;
				case gd::GameObjectType::kGameObjectTypeHazard:
					m_objectType->setString("Type: Hazard (2)");
					break;
				case gd::GameObjectType::kGameObjectTypeInverseGravityPortal:
					m_objectType->setString("Type: InverseGravityPortal (3)");
					break;
				case gd::GameObjectType::kGameObjectTypeNormalGravityPortal:
					m_objectType->setString("Type: NormalGravityPortal (4)");
					break;
				case gd::GameObjectType::kGameObjectTypeShipPortal:
					m_objectType->setString("Type: ShipPortal (5)");
					break;
				case gd::GameObjectType::kGameObjectTypeCubePortal:
					m_objectType->setString("Type: CubePortal (6)");
					break;
				case gd::GameObjectType::kGameObjectTypeDecoration:
					m_objectType->setString("Type: Decoration (7)");
					break;
				case gd::GameObjectType::kGameObjectTypePulseObjects:
					m_objectType->setString("Type: PulseObjects (8)");
					break;
				case gd::GameObjectType::kGameObjectTypeYellowJumpPad:
					m_objectType->setString("Type: YellowJumpPad (9)");
					break;
				case gd::GameObjectType::kGameObjectTypePinkJumpPad:
					m_objectType->setString("Type: PinkJumpPad (10)");
					break;
				case gd::GameObjectType::kGameObjectTypeGravityPad:
					m_objectType->setString("Type: GravityPad (11)");
					break;
				case gd::GameObjectType::kGameObjectTypeYellowJumpRing:
					m_objectType->setString("Type: YellowJumpRing (12)");
					break;
				case gd::GameObjectType::kGameObjectTypePinkJumpRing:
					m_objectType->setString("Type: PinkJumpRing (13)");
					break;
				case gd::GameObjectType::kGameObjectTypeGravityRing:
					m_objectType->setString("Type: GravityRing (14)");
					break;
				case gd::GameObjectType::kGameObjectTypeInverseMirrorPortal:
					m_objectType->setString("Type: InverseMirrorPortal (15)");
					break;
				case gd::GameObjectType::kGameObjectTypeNormalMirrorPortal:
					m_objectType->setString("Type: NormalMirrorPortal (16)");
					break;
				case gd::GameObjectType::kGameObjectTypeBallPortal:
					m_objectType->setString("Type: BallPortal (17)");
					break;
				case gd::GameObjectType::kGameObjectTypeRegularSizePortal:
					m_objectType->setString("Type: RegularSizePortal (18)");
					break;
				case gd::GameObjectType::kGameObjectTypeMiniSizePortal:
					m_objectType->setString("Type: MiniSizePortal (19)");
					break;
				case gd::GameObjectType::kGameObjectTypeUfoPortal:
					m_objectType->setString("Type: UfoPortal (20)");
					break;
				case gd::GameObjectType::kGameObjectTypeModifier:
					m_objectType->setString("Type: Modifier (21)");
					break;
				case gd::GameObjectType::kGameObjectTypeBreakable:
					m_objectType->setString("Type: Breakable (22)");
					break;
				case gd::GameObjectType::kGameObjectTypeSecretCoin:
					m_objectType->setString("Type: SecretCoin (23)");
					break;
				case gd::GameObjectType::kGameObjectTypeDualPortal:
					m_objectType->setString("Type: DualPortal (24)");
					break;
				case gd::GameObjectType::kGameObjectTypeSoloPortal:
					m_objectType->setString("Type: SoloPortal (25)");
					break;
				case gd::GameObjectType::kGameObjectTypeSlope:
					m_objectType->setString("Type: Slope (26)");
					break;
				case gd::GameObjectType::kGameObjectTypeWavePortal:
					m_objectType->setString("Type: WavePortal (27)");
					break;
				}
			}
		}
		if (m_objectZ) {
			if (editUI->m_selectedObject == 0) m_objectZ->setVisible(0);
			else {
				m_objectZ->setString(CCString::createWithFormat("Object Z: %i", editUI->m_selectedObject->m_objectZ)->getCString());
				m_objectZ->setVisible(1);
			}
		}
		if (m_objectsSelected) {
			if (editUI->getSelectedObjectsOfCCArray()->count() == 0) m_objectsSelected->setVisible(0);
			else m_objectsSelected->setVisible(1);
			m_objectsSelected->setString(CCString::createWithFormat("Objects: %i", editUI->getSelectedObjectsOfCCArray()->count())->getCString());
		}
	}
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

void __fastcall EditorUI::editObjectH(gd::EditorUI* self, void*, CCObject* obj) {

	//for (auto obj : CCArrayExt<gd::GameObject*>(self->getSelectedObjectsOfCCArray())) {
	//	gd::ColorSelectPopup::create(obj, 0, 0, 0);
	//}

	EditorUI::editObject(self, obj);
}

void __fastcall EditorUI::onDuplicateH(gd::EditorUI* self, void*, CCObject* obj) {
}

//CCPoint __fastcall EditorUI::offsetForKeyH(gd::EditorUI* self, void*, CCPoint* pos, int objID) {
//	return EditorUI::offsetForKey(self, pos, objID);
//}

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
	if (MyEditorLayer::s_instance) {
		if (setting().onEditorPreview)
			MyEditorLayer::s_instance->update_preview_mode();
		else
			MyEditorLayer::s_instance->reset_colors();
	}
}

bool SEP = false;

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
	if (key == KEY_Escape) {
		self->onResume(nullptr);
	}
	else {
		EditorPauseLayer::keyDown(self, key);
	}
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
		m_fadeTime_input->callback = [&](FloatInputNode& input) {
			//std::cout << "Duration: " << input.get_value().value_or(self->m_duration) << std::endl;
			//if (self->m_durationSlider) {
			//	self->m_durationSlider->setValue(0.5f);
			//	self->m_durationSlider->updateBar();
			//}
			//self->m_durationSlider->setValue(input.get_value().value_or(self->m_duration) * 10);
			//self->m_durationSlider->updateBar();
			//self->m_duration = input.get_value().value_or(self->m_duration);
			//self->setFadeTime(m_fadeTime_input->get_value().value_or(m_fadeTime));
			//m_fadeTime = input.get_value().value_or(m_fadeTime);
			//self->setFadeTime(m_fadeTime_input->get_value().value_or(m_fadeTime));
			};
		self->m_mainLayer->addChild(m_fadeTime_input);
		m_fadeTime_input->setPosition({ (winSize.width / 2) + 61, (winSize.height / 2) - 70 }); // 16:9: 346, 90

		//self->m_durationSlider->setValue(1);
		//self->m_durationSlider->updateBar();
		//std::cout << self->m_durationSlider->getValue() << std::endl;

		//m_customFadeTimeInput = CustomFadeTimeInput::create(self);

		//self->m_mainLayer->addChild(m_customFadeTimeInput);
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
	////if (m_customFadeTimeInput) {
	////	m_customFadeTimeInput->updateInput();
	////}
	ColorSelectPopup::sliderChanged(self, sender);
}

void __fastcall ColorSelectPopup::closeColorSelectH(gd::ColorSelectPopup* self, void*, CCObject* sender) {
	if (m_fadeTime_input != nullptr) {
		self->m_duration = m_fadeTime_input->get_value().value_or(m_fadeTime);
	}
	m_fadeTime_input = nullptr;
	m_colorInputWidget = nullptr;
	//m_customFadeTimeInput = nullptr;
	ColorSelectPopup::closeColorSelect(self, sender);
}

void __fastcall ColorSelectPopup::colorValueChangedH(gd::ColorSelectPopup* self, void*, ccColor3B color) {
	ColorSelectPopup::colorValueChanged(self, color);
	if (m_colorInputWidget != nullptr) {
		m_colorInputWidget->update_labels(true, true);
	}
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

	//glLineWidth(2);
	//ccDrawColor4B(0x32, 0x32, 0x32, 0x32);
	//ccDrawLine({ director->getWinSize().width / 2.f, director->getScreenTop() }, { director->getWinSize().width / 2.f, director->getScreenBottom() });

	//auto secarr = lel->getLevelSections();
	//auto arrcount = secarr->count();
	//auto layer = lel->gameLayer();
	//float xp = -layer->getPositionX() / layer->getScale();
	//for (int i = lel->sectionForPos(xp) - (5 / layer->getScale()); i < lel->sectionForPos(xp) + (6 / layer->getScale()); i++) {
	//	if (i < 0) continue;
	//	if (i >= arrcount) break;
	//	auto objAtInd = secarr->objectAtIndex(i);
	//	auto objarr = reinterpret_cast<CCArray*>(objAtInd);

	//	for (int j = 0; j < objarr->count(); j++) {
	//		auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
	//		switch (obj->getObjectID())
	//		{
	//		case 29:
	//			break;
	//		}
	//	}
	//}	
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

	if (editUI && setting().onHitboxBugFix) {
		if (self->canRotateFree() && self->m_objectRadius == 0) {
			self->m_isOriented = true;
			self->updateOrientedBox();
		}
	}
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
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41850), EditorUI::sliderChangedH, reinterpret_cast<void**>(&EditorUI::sliderChanged));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x42080), EditorUI::setupDeleteMenuH, reinterpret_cast<void**>(&EditorUI::setupDeleteMenu));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x490c0), EditorUI::onCopyH, reinterpret_cast<void**>(&EditorUI::onCopy));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41450), EditorUI::updateButtonsH, reinterpret_cast<void**>(&EditorUI::updateButtons));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4afc0), EditorUI::onGroupDownH, reinterpret_cast<void**>(&EditorUI::onGroupDown));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4af50), EditorUI::onGroupUpH, reinterpret_cast<void**>(&EditorUI::onGroupUp));
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
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6ee50), GameObject_customSetupH, reinterpret_cast<void**>(&GameObject_customSetup));
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
	matdash::add_hook<&EditorUI_moveObject>(gd::base + 0x4b410);
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
#include "ColorSelectPopup.hpp"
#include "LevelEditorLayer.hpp"
#include "PulseBuilderPopup.hpp"
#include "RGBColorInputWidget.hpp"
#include "LiveColorEdit.hpp"
#include "LevelSettingsLayer.hpp"
#include "hsv.hpp"

class FadeTimeInput : public cocos2d::CCLayer, gd::TextInputDelegate {
protected:
	gd::ColorSelectPopup* m_parent;

	virtual bool init(gd::ColorSelectPopup* parent) {
		m_parent = parent;
		if (!CCLayer::init()) return false;

		auto bg = extension::CCScale9Sprite::create("square02_small.png");
		bg->setContentSize({ 60.f, 28.f });
		bg->setOpacity(100);
		this->addChild(bg, -1);

		m_input = gd::CCTextInputNode::create(55.f, 15.f, "Dur", this, "bigFont.fnt");
		m_input->setMaxLabelScale(.9f);
		m_input->setCharLimit(5);
		m_input->setString(CCString::createWithFormat("%.02f%", m_parent->m_duration)->getCString());
		m_input->setLabelPlaceholderScale(.65f);
		m_input->setLabelPlaceholderColor({100, 100, 100});
		m_input->setAllowedChars("0123456789.");
		m_input->setDelegate(this);
		this->addChild(m_input);

		return true;
	}

	virtual void textChanged(gd::CCTextInputNode* input) {
		m_parent->m_duration = std::atof(input->getString().c_str());
		m_parent->m_durationSlider->setValue(m_parent->m_duration / 10.f);
	}

public:
	gd::CCTextInputNode* m_input;

	static FadeTimeInput* create(gd::ColorSelectPopup* parent) {
		FadeTimeInput* ret = new FadeTimeInput();
		if (ret && ret->init(parent)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	void updateInput() {
		m_input->setString(CCString::createWithFormat("%.02f%", m_parent->m_duration)->getCString());
	}
};

void ColorSelectPopup::Callback::onDefault(CCObject*) {
	auto editorLayer = LevelEditorLayer::get();
	
	if (editorLayer == nullptr) return;

	switch (this->m_targetObject->m_objectID) {
	case 29: this->m_colorPicker->setColorValue(editorLayer->m_levelSettings->m_backgroundColor->m_color); break;
	case 30: this->m_colorPicker->setColorValue(editorLayer->m_levelSettings->m_groundColor->m_color); break;
	case 104: this->m_colorPicker->setColorValue(editorLayer->m_levelSettings->m_lineColor->m_color); break;
	case 105: this->m_colorPicker->setColorValue(editorLayer->m_levelSettings->m_objectColor->m_color); break;
	case 744: this->m_colorPicker->setColorValue(editorLayer->m_levelSettings->m_3DLineColor->m_color); break;
	case 221: this->m_colorPicker->setColorValue(editorLayer->m_levelSettings->m_customColor01->m_color); break;
	case 717: this->m_colorPicker->setColorValue(editorLayer->m_levelSettings->m_customColor02->m_color); break;
	case 718: this->m_colorPicker->setColorValue(editorLayer->m_levelSettings->m_customColor03->m_color); break;
	case 743: this->m_colorPicker->setColorValue(editorLayer->m_levelSettings->m_customColor04->m_color); break;
	}
}

void ColorSelectPopup::Callback::onPulseHelper(CCObject* sender) {
	auto pulseBuilder = PulseBuilderPopup::create();
	pulseBuilder->setUserData(this);
	pulseBuilder->show();
}

void ColorSelectPopup::Callback::onLiveEdit(CCObject*) {
	if (LevelEditorLayer::get()) {
		auto liveColorEdit = static_cast<LiveColorEdit*>(LevelEditorLayer::get()->m_uiLayer->getChildByTag(9095));
		if (liveColorEdit) {
			liveColorEdit->closeColorEdit(nullptr);
		}

		auto newLiveColorEdit = LiveColorEdit::create(this->m_targetObject, this->getTag());
		newLiveColorEdit->setPosition(CCDirector::sharedDirector()->getScreenLeft() + 110.f, CCDirector::sharedDirector()->getScreenTop() - 140.f);
		LevelEditorLayer::get()->m_uiLayer->addChild(newLiveColorEdit, 105, 9095);
	}

	this->closeColorSelect(nullptr);
	if (LevelSettingsLayer::get()) {
		LevelSettingsLayer::get()->onClose(nullptr);
	}
}

ccColor3B getLightBGColor(ccColor3B bgColor) {
	std::cout << "allo" << std::endl;
	auto hsv = extension::CCControlUtils::HSVfromRGB(extension::RGBA((int)bgColor.r, (int)bgColor.g, (int)bgColor.b, 255));
	std::cout << hsv.h << std::endl;
	std::cout << hsv.s << std::endl;
	std::cout << hsv.v << std::endl;
	hsv.s -= 20;

	auto gm = gd::GameManager::sharedState();

	auto rgb = extension::CCControlUtils::RGBfromHSV(hsv);

	return ccc3(rgb.r, rgb.g, rgb.b);
}

void ColorSelectPopup::Callback::onLBGInfo(CCObject*) {
	auto gm = gd::GameManager::sharedState();
	//auto lbgColor = getLightBGColor(this->m_colorPicker->getColorValue());

	//std::stringstream info;

	//info << "Color value: " << lbgColor.r << " " << lbgColor.g << " " << lbgColor.b;
	//gd::FLAlertLayer::create("Light BG", info.str().c_str(), "OK")->show();
}

bool __fastcall ColorSelectPopup::initH(gd::ColorSelectPopup* self, void*, gd::GameObject* object, int colorID, int playerColor, int blending) {
	if (!ColorSelectPopup::init(self, object, colorID, playerColor, blending)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	if (self->m_durationSlider) {
		self->m_durationLabel->setVisible(false);

		auto fadeTimeLabel = CCLabelBMFont::create("FadeTime:", "goldFont.fnt");
		fadeTimeLabel->setPosition(winSize.width / 2.f - 30.f, winSize.height / 2.f - 70.f);
		self->addChild(fadeTimeLabel);

		auto fadeTimeInput = FadeTimeInput::create(self);
		fadeTimeInput->setPosition(winSize.width / 2.f + 62.f, winSize.height / 2.f - 70.f);
		self->addChild(fadeTimeInput, 0, 301);

		auto onDefaultSpr = gd::ButtonSprite::create("Default", 0x28, 0, .6f, true, "goldFont.fnt", "GJ_button_04.png", 30.f);
		auto onDefault = gd::CCMenuItemSpriteExtra::create(onDefaultSpr, self, menu_selector(ColorSelectPopup::Callback::onDefault));
		onDefault->setPosition(self->m_buttonMenu->convertToNodeSpace({ director->getScreenRight() - 50.f, director->getScreenTop() - 110.f }));
		self->m_buttonMenu->addChild(onDefault);

		auto onPulseSpr = gd::ButtonSprite::create("Pulse", 0x28, 0, .6f, true, "goldFont.fnt", "GJ_button_04.png", 30.f);
		auto onPulseHelper = gd::CCMenuItemSpriteExtra::create(onPulseSpr, self, menu_selector(ColorSelectPopup::Callback::onPulseHelper));
		onPulseHelper->setPosition(self->m_buttonMenu->convertToNodeSpace({ director->getScreenRight() - 50.f, director->getScreenTop() - 150.f }));
		self->m_buttonMenu->addChild(onPulseHelper);
	}

	auto colorInputWidget = RGBColorInputWidget::create(self);
	colorInputWidget->setPosition(director->getScreenLeft() + 67.5f, winSize.height / 2.f + 20.f);
	self->m_mainLayer->addChild(colorInputWidget, 0, 211);

	auto onLiveEditSpr = CCSprite::create("GJ_editHSVBtn2_001.png");
	auto onLiveEdit = gd::CCMenuItemSpriteExtra::create(onLiveEditSpr, self, menu_selector(ColorSelectPopup::Callback::onLiveEdit));
	onLiveEdit->setPosition(-130.f, 260.f);
	self->m_buttonMenu->addChild(onLiveEdit);

	if (LevelEditorLayer::get()) {
		auto liveColorEdit = static_cast<LiveColorEdit*>(LevelEditorLayer::get()->m_uiLayer->getChildByTag(9095));
		if (liveColorEdit) {
			liveColorEdit->closeColorEdit(nullptr);
		}
	}

	//if ((object && object->m_objectID == 29) || ((colorID == 0) && !self->m_durationSlider)) {
	//	auto onLBGInfoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	//	onLBGInfoSpr->setScale(.75f);
	//	auto onLBGInfo = gd::CCMenuItemSpriteExtra::create(onLBGInfoSpr, self, menu_selector(ColorSelectPopup::Callback::onLBGInfo));
	//	onLBGInfo->setPosition(135.f, 265.f);
	//	self->m_buttonMenu->addChild(onLBGInfo);
	//}

	return true;
}

void __fastcall ColorSelectPopup::sliderChangedH(gd::ColorSelectPopup* self, void*, CCObject* sender) {
	ColorSelectPopup::sliderChanged(self, sender);

	auto fadeTimeInput = static_cast<FadeTimeInput*>(self->getChildByTag(301));
	if (fadeTimeInput) {
		fadeTimeInput->updateInput();
	}
}

void __fastcall ColorSelectPopup::colorValueChangedH(gd::ColorSelectPopup* _self, void*, ccColor3B color) {
	ColorSelectPopup::colorValueChanged(_self, color);
	auto self = reinterpret_cast<gd::ColorSelectPopup*>(reinterpret_cast<uintptr_t>(_self) - 0x1bc);

	auto colorInputWidget = static_cast<RGBColorInputWidget*>(self->m_mainLayer->getChildByTag(211));
	if (colorInputWidget) {
		colorInputWidget->update_labels(true, true);
	}

	//std::cout << (int)getLightBGColor(color).r << std::endl;
}

void ColorSelectPopup::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x29db0), ColorSelectPopup::initH, reinterpret_cast<void**>(&ColorSelectPopup::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x2ae00), ColorSelectPopup::sliderChangedH, reinterpret_cast<void**>(&ColorSelectPopup::sliderChanged));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x2af60), ColorSelectPopup::colorValueChangedH, reinterpret_cast<void**>(&ColorSelectPopup::colorValueChanged));
}
#include "LiveColorEdit.hpp"
#include "LevelEditorLayer.hpp"

gd::SettingsColorObject* LiveColorEdit::tagToColor(gd::LevelSettingsObject* settings) {
	switch (m_colorTag) {
	case 0: return settings->m_backgroundColor; break;
	case 1: return settings->m_groundColor; break;
	case 2: return settings->m_lineColor; break;
	case 3: return settings->m_objectColor; break;
	case 8: return settings->m_3DLineColor; break;
	case 4: return settings->m_customColor01; break;
	case 5: return settings->m_customColor02; break;
	case 6: return settings->m_customColor03; break;
	case 7: return settings->m_customColor04; break;
	default: return nullptr; break;
	}
}

std::string LiveColorEdit::defineLabel() {
	if (m_targetObject) {
		switch (m_targetObject->m_objectID) {
		case 29: return "BG"; break;
		case 30: return "Ground"; break;
		case 104: return "Line"; break;
		case 105: return "Obj"; break;
		case 744: return "3DL"; break;
		case 221: return "Col1"; break;
		case 717: return "Col2"; break;
		case 718: return "Col3"; break;
		case 743: return "Col4"; break;
		default: return "Unknown"; break;
		}
	}
	else {
		switch (m_colorTag) {
		case 0: return "BG"; break;
		case 1: return "Ground"; break;
		case 2: return "Line"; break;
		case 3: return "Obj"; break;
		case 8: return "3DL"; break;
		case 4: return "Col1"; break;
		case 5: return "Col2"; break;
		case 6: return "Col3"; break;
		case 7: return "Col4"; break;
		default: return "Unknown"; break;
		}
	}
}

bool LiveColorEdit::init(gd::GameObject* obj, int tag) {
	this->m_targetObject = obj;
	this->m_colorTag = tag;
	if (LevelEditorLayer::get()) {
		this->m_levelSettings = LevelEditorLayer::get()->m_levelSettings;
	}
	if (!CCLayer::init()) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto menu = CCMenu::create();
	menu->setPosition({ 0.f, 0.f });
	this->addChild(menu);

	auto bg = extension::CCScale9Sprite::create("square02_001.png");
	bg->setContentSize({ 100.f, 130.f });
	bg->setOpacity(150);
	this->addChild(bg, -1);

	auto title = CCLabelBMFont::create("", "goldFont.fnt");
	title->setString(defineLabel().c_str());
	title->setScale(.6f);
	title->setPositionY(75.f);
	this->addChild(title);

	auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	closeSpr->setScale(.4f);
	auto onClose = gd::CCMenuItemSpriteExtra::create(closeSpr, this, menu_selector(LiveColorEdit::closeColorEdit));
	onClose->setPosition(-50.f, 65.f);
	menu->addChild(onClose);

	// HUE

	auto hueBar = CCSprite::create("hueBar.png");
	hueBar->setScaleX(.7f);
	hueBar->setScaleY(.377f);
	hueBar->setPositionX(-35.f);
	this->addChild(hueBar);

	m_hueSlider = gd::Slider::create(this, menu_selector(LiveColorEdit::sliderChanged), "sliderBar.png", "slidergroove_02.png", "GJ_colorThumbBtn.png", "GJ_colorThumbSBtn.png", 1.f);
	m_hueSlider->m_sliderBar->setVisible(false);
	m_hueSlider->m_sliderBar->getParent()->setVisible(false);

	m_hueSlider->setScale(.55f);
	from<CCSprite*>(m_hueSlider->m_touchLogic->m_thumb, 0x108)->setScale(1.45f);
	from<CCSprite*>(m_hueSlider->m_touchLogic->m_thumb, 0x108)->setAnchorPoint({ .5f, .5f });
	from<CCSprite*>(m_hueSlider->m_touchLogic->m_thumb, 0x108)->setPosition({ 15.f, 15.f });
	from<CCSprite*>(m_hueSlider->m_touchLogic->m_thumb, 0x10c)->setScale(1.45f);
	from<CCSprite*>(m_hueSlider->m_touchLogic->m_thumb, 0x10c)->setAnchorPoint({ .5f, .5f });
	from<CCSprite*>(m_hueSlider->m_touchLogic->m_thumb, 0x10c)->setPosition({ 15.f, 15.f });

	m_hueSlider->setRotation(-90.f);
	m_hueSlider->setAnchorPoint({ 0.f, 0.f });
	m_hueSlider->setPosition(hueBar->getPosition());

	this->addChild(m_hueSlider);

	// Saturation

	m_saturationBar = CCSprite::create("saturationBarBottom.png");
	auto saturationBarTop = CCSprite::create("saturationBarTop.png");
	saturationBarTop->setPosition({ 10.f, 146.f });
	m_saturationBar->addChild(saturationBarTop);
	m_saturationBar->setScaleX(.7f);
	m_saturationBar->setScaleY(.377f);
	this->addChild(m_saturationBar);

	m_saturationSlider = gd::Slider::create(this, menu_selector(LiveColorEdit::sliderChanged), "sliderBar.png", "slidergroove_02.png", "GJ_colorThumbBtn.png", "GJ_colorThumbSBtn.png", 1.f);
	m_saturationSlider->m_sliderBar->setVisible(false);
	m_saturationSlider->m_sliderBar->getParent()->setVisible(false);

	m_saturationSlider->setScale(.55f);
	from<CCSprite*>(m_saturationSlider->m_touchLogic->m_thumb, 0x108)->setScale(1.45f);
	from<CCSprite*>(m_saturationSlider->m_touchLogic->m_thumb, 0x108)->setAnchorPoint({ .5f, .5f });
	from<CCSprite*>(m_saturationSlider->m_touchLogic->m_thumb, 0x108)->setPosition({ 15.f, 15.f });
	from<CCSprite*>(m_saturationSlider->m_touchLogic->m_thumb, 0x10c)->setScale(1.45f);
	from<CCSprite*>(m_saturationSlider->m_touchLogic->m_thumb, 0x10c)->setAnchorPoint({ .5f, .5f });
	from<CCSprite*>(m_saturationSlider->m_touchLogic->m_thumb, 0x10c)->setPosition({ 15.f, 15.f });

	m_saturationSlider->setRotation(-90.f);
	m_saturationSlider->setAnchorPoint({ 0.f, 0.f });
	m_saturationSlider->setPosition(m_saturationBar->getPosition());

	this->addChild(m_saturationSlider);

	// Value

	m_valueBar = CCSprite::create("lightnessBar.png");
	m_valueBar->setScaleX(.7f);
	m_valueBar->setScaleY(.377f);
	m_valueBar->setPositionX(35.f);
	this->addChild(m_valueBar);

	m_valueSlider = gd::Slider::create(this, menu_selector(LiveColorEdit::sliderChanged), "sliderBar.png", "slidergroove_02.png", "GJ_colorThumbBtn.png", "GJ_colorThumbSBtn.png", 1.f);
	m_valueSlider->m_sliderBar->setVisible(false);
	m_valueSlider->m_sliderBar->getParent()->setVisible(false);

	m_valueSlider->setScale(.55f);
	from<CCSprite*>(m_valueSlider->m_touchLogic->m_thumb, 0x108)->setScale(1.45f);
	from<CCSprite*>(m_valueSlider->m_touchLogic->m_thumb, 0x108)->setAnchorPoint({ .5f, .5f });
	from<CCSprite*>(m_valueSlider->m_touchLogic->m_thumb, 0x108)->setPosition({ 15.f, 15.f });
	from<CCSprite*>(m_valueSlider->m_touchLogic->m_thumb, 0x10c)->setScale(1.45f);
	from<CCSprite*>(m_valueSlider->m_touchLogic->m_thumb, 0x10c)->setAnchorPoint({ .5f, .5f });
	from<CCSprite*>(m_valueSlider->m_touchLogic->m_thumb, 0x10c)->setPosition({ 15.f, 15.f });

	m_valueSlider->setRotation(-90.f);
	m_valueSlider->setAnchorPoint({ 0.f, 0.f });
	m_valueSlider->setPosition(m_valueBar->getPosition());

	this->addChild(m_valueSlider);

	ccColor3B color;

	if (m_targetObject) {
		color = m_targetObject->m_triggerColor;
	}
	else {
		color = this->tagToColor(m_levelSettings)->m_color;
	}

	// amazing lmao
	m_hsv = color_utils::rgb_to_hsv({ (double)color.r, (double)color.g, (double)color.b });
	auto rgbFromHsv = color_utils::hsv_to_rgb({ m_hsv.h, 1, 255 });
	m_saturationBar->setColor(ccc3(rgbFromHsv.r, rgbFromHsv.g, rgbFromHsv.b));
	auto uhh = color_utils::hsv_to_rgb({ m_hsv.h, m_hsv.s, m_hsv.v });
	m_valueBar->setColor(ccc3(uhh.r, uhh.g, uhh.b));

	m_hueSlider->setValue(m_hsv.h / 360.f);
	m_saturationSlider->setValue(m_hsv.s);
	m_valueSlider->setValue(m_hsv.v / 255.f);

	// ColorRefSpr

	auto prevColor = CCSprite::createWithSpriteFrameName("whiteSquare20_001.png");
	prevColor->setPosition({ 60.f, -50.f });
	prevColor->setScale(1.5f);
	prevColor->setColor(color);

	m_newColorPreview = CCSprite::createWithSpriteFrameName("whiteSquare20_001.png");
	m_newColorPreview->setPosition({ 60.f, -35.f });
	m_newColorPreview->setScale(1.5f);
	m_newColorPreview->setColor(color);

	this->addChild(prevColor);
	this->addChild(m_newColorPreview);

	m_infoLabel = CCLabelBMFont::create("", "chatFont.fnt");
	m_infoLabel->setScale(.5f);
	m_infoLabel->setPositionY(-60.f);
	this->addChild(m_infoLabel, -1);

	this->updateDevLabel();

	return true;
}

void LiveColorEdit::updateDevLabel() {
	if (m_infoLabel) {
		m_infoLabel->setString(CCString::createWithFormat("H: %.0f S: %.2f V: %.0f", m_hsv.h, m_hsv.s, m_hsv.v)->getCString());
	}
}

void LiveColorEdit::sliderChanged(CCObject* sender) { // in geode version this func was bigger and ass lmao
	auto sliderThumb = static_cast<gd::SliderThumb*>(sender);
	if (sliderThumb) {
		auto slider = static_cast<gd::Slider*>(sliderThumb->getParent()->getParent()); // wacky

		if (slider == m_hueSlider) {
			m_hsv.h = sliderThumb->getValue() * 360.f;
		}
		else if (slider == m_saturationSlider) {
			m_hsv.s = sliderThumb->getValue();
		}
		else if (slider == m_valueSlider) {
			m_hsv.v = sliderThumb->getValue() * 255.f;
		}

		if (m_targetObject) {
			auto color = color_utils::hsv_to_rgb(m_hsv);
			m_targetObject->m_triggerColor = ccc3(color.r, color.g, color.b);
		}
		else {
			auto color = color_utils::hsv_to_rgb(m_hsv);
			this->tagToColor(m_levelSettings)->m_color = ccc3(color.r, color.g, color.b);
		}

		auto color = color_utils::hsv_to_rgb(m_hsv);
		m_newColorPreview->setColor(ccc3(color.r, color.g, color.b));
		auto saturationColor = color_utils::hsv_to_rgb({ m_hsv.h, 1, 255 });
		m_saturationBar->setColor(ccc3(saturationColor.r, saturationColor.g, saturationColor.b));
		auto valueColor = color_utils::hsv_to_rgb({ m_hsv.h, m_hsv.s, 255 });
		m_valueBar->setColor(ccc3(valueColor.r, valueColor.g, valueColor.b));

		this->updateDevLabel();
		LevelEditorLayer::updatePreviewMode();
	}
}

void LiveColorEdit::closeColorEdit(CCObject*) {
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}
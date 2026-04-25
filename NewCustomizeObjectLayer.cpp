#include "NewCustomizeObjectLayer.hpp"
#include "LevelEditorLayer.hpp"
#include "EditorUI.hpp"
#include "utils.hpp"
#include "Setting.hpp"

NewCustomizeObjectLayer* m_newCustomizeObjectLayer;

NewCustomizeObjectLayer* NewCustomizeObjectLayer::get() {
	return m_newCustomizeObjectLayer;
}

const char* NewCustomizeObjectLayer::colorToString(int colorID) {
	switch (colorID) {
	case 0: return "Default"; break;
	case 1: return "P1"; break;
	case 2: return "P2"; break;
	case 3: return "Col1"; break;
	case 4: return "Col2"; break;
	case 5: return "LBG"; break;
	case 6: return "Col3"; break;
	case 7: return "Col4"; break;
	case 8: return "3DL"; break;
	case 9: return "White"; break;
	default: break;
	}
}

gd::SettingsColorObject* NewCustomizeObjectLayer::intToSettings(int colorID) {
	auto editorLayer = LevelEditorLayer::get();
	if (editorLayer) {
		auto settings = editorLayer->m_levelSettings;

		switch (colorID) {
		case 8: return settings->m_3DLineColor; break;
		case 4: return settings->m_customColor01; break;
		case 5: return settings->m_customColor02; break;
		case 6: return settings->m_customColor03; break;
		case 7: return settings->m_customColor04; break;
		default: return nullptr; break;
		}
	}
}

bool NewCustomizeObjectLayer::init(gd::GameObject* object, CCArray* objects) {
	m_newCustomizeObjectLayer = this;
	if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

	if (object) {
		m_selectedObject = object;
	}
	else {
		m_selectedObjects = objects;
		m_selectedObjects->retain();
	}

	if (m_selectedObject == nullptr) {
		int compareColor = 0;
		m_selectedColorIdx = -1;
		for (auto obj : CCArrayExt<gd::GameObject*>(m_selectedObjects)) {
			if (m_selectedColorIdx == -1) {
				m_selectedColorIdx = static_cast<int>(obj->m_customColorMode);
			}
			else {
				compareColor = static_cast<int>(obj->m_customColorMode);
				if (m_selectedColorIdx != compareColor) {
					m_selectedColorIdx = 0;
				}
			}
		}
		if (m_selectedColorIdx == -1) {
			m_selectedColorIdx = 0;
		}
	}
	else {
		m_selectedColorIdx = static_cast<int>(m_selectedObject->m_customColorMode);
	}

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();
	director->getTouchDispatcher()->incrementForcePrio();
	this->registerWithTouchDispatcher();

	auto layer = CCLayer::create();
	auto menu = CCMenu::create();
	menu->setPosition(winSize.width / 2.f, winSize.height / 2.f - 75.f);
	this->m_mainLayer = layer;
	this->m_buttonMenu = menu;

	layer->addChild(menu, 1);
	this->addChild(layer);

	auto gm = gd::GameManager::sharedState();
	auto editorLayer = LevelEditorLayer::get();

	auto title = CCLabelBMFont::create("Select Color", "bigFont.fnt");
	title->setPosition(winSize.width / 2.f, winSize.height / 2.f + 75.f);
	title->setScale(.8f);
	layer->addChild(title);

	auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
	bg->setContentSize({ 240.f, 200.f });
	bg->setPosition(winSize / 2.f);
	layer->addChild(bg, -1);

	auto onCloseSpr = gd::ButtonSprite::create("OK", 0, 0, 1.f, false, "goldFont.fnt", "GJ_button_01.png", 30.f);
	auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(NewCustomizeObjectLayer::onClose));
	menu->addChild(onClose);

	auto bgbg = extension::CCScale9Sprite::create("square02_small.png");
	bgbg->setContentSize({ 170.f, 110.f });
	bgbg->setPosition(winSize / 2.f);
	bgbg->setOpacity(75);
	layer->addChild(bgbg);

	auto separator = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
	separator->setScaleY(1.15f);
	separator->setPosition(winSize / 2.f);
	layer->addChild(separator);

	m_selectSprite = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	layer->addChild(m_selectSprite);

	m_selectedColorLabel = CCLabelBMFont::create("", "goldFont.fnt");
	m_selectedColorLabel->setAnchorPoint({ 1.f, .5f });
	m_selectedColorLabel->setPosition(winSize.width / 2.f + 75.f, winSize.height / 2.f - 75.f);
	m_selectedColorLabel->setScale(.65f);
	layer->addChild(m_selectedColorLabel);

	m_selectedColorSprite = ColorChannelSprite::create();
	auto onEditColor = gd::CCMenuItemSpriteExtra::create(m_selectedColorSprite, this, menu_selector(NewCustomizeObjectLayer::editColor));
	onEditColor->setPosition(96.f, 0.f);
	menu->addChild(onEditColor);

	auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
	toggleOff->setScale(.8f);
	auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	toggleOn->setScale(.8f);

	auto liveColorsLabel = CCLabelBMFont::create("Live\nColors", "bigFont.fnt");
	liveColorsLabel->setAnchorPoint({ 0.f, .5f });
	liveColorsLabel->setScale(.35f);
	liveColorsLabel->setPosition(winSize.width / 2.f - 78.f, winSize.height / 2.f - 75.f);
	layer->addChild(liveColorsLabel);
	auto toggleLiveColor = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(NewCustomizeObjectLayer::toggleLiveColor));
	toggleLiveColor->toggle(setting().m_liveColorEnabled);
	toggleLiveColor->setPositionX(-96.f);
	menu->addChild(toggleLiveColor);

	toggleOff->setColor(setting().onPreviewMode ? ccWHITE : ccGRAY);
	toggleOn->setColor(setting().onPreviewMode ? ccWHITE : ccGRAY);
	toggleLiveColor->setEnabled(setting().onPreviewMode);
	liveColorsLabel->setColor(setting().onPreviewMode ? ccWHITE : ccGRAY);

	m_buttonsArray = CCArray::create();
	m_buttonsArray->retain();
	m_colorSprites = CCArray::create();
	m_colorSprites->retain();

	// Special Colors

	auto onDefaultSpr = ColorChannelSprite::create("D");
	onDefaultSpr->updateValues(ccGRAY);
	auto onDefault = gd::CCMenuItemSpriteExtra::create(onDefaultSpr, this, menu_selector(NewCustomizeObjectLayer::onSelectColor));
	onDefault->setPosition(-60.f, 110.f);
	menu->addChild(onDefault, 0, 0);

	auto onLightBGSpr = ColorChannelSprite::create("LBG");
	auto onLightBG = gd::CCMenuItemSpriteExtra::create(onLightBGSpr, this, menu_selector(NewCustomizeObjectLayer::onSelectColor));
	onLightBG->setPosition(-25.f, 110.f);
	menu->addChild(onLightBG, 0, 5);
	onLightBGSpr->setTag(5);

	auto onP1Spr = ColorChannelSprite::create("P1");
	onP1Spr->updateValues(gm->colorForIdx(gm->m_playerColor), true);
	auto onP1 = gd::CCMenuItemSpriteExtra::create(onP1Spr, this, menu_selector(NewCustomizeObjectLayer::onSelectColor));
	onP1->setPosition(-60.f, 75.f);
	menu->addChild(onP1, 0, 1);

	auto onP2Spr = ColorChannelSprite::create("P2");
	onP2Spr->updateValues(gm->colorForIdx(gm->m_playerColor2), true);
	auto onP2 = gd::CCMenuItemSpriteExtra::create(onP2Spr, this, menu_selector(NewCustomizeObjectLayer::onSelectColor));
	onP2->setPosition(-25.f, 75.f);
	menu->addChild(onP2, 0, 2);

	auto onWhiteSpr = ColorChannelSprite::create("W");
	auto onWhite = gd::CCMenuItemSpriteExtra::create(onWhiteSpr, this, menu_selector(NewCustomizeObjectLayer::onSelectColor));
	onWhite->setPosition(-60.f, 40.f);
	menu->addChild(onWhite, 0, 9);

	auto on3DLSpr = ColorChannelSprite::create("3DL");
	auto on3DL = gd::CCMenuItemSpriteExtra::create(on3DLSpr, this, menu_selector(NewCustomizeObjectLayer::onSelectColor));
	on3DL->setPosition(-25.f, 40.f);
	menu->addChild(on3DL, 0, 8);
	on3DLSpr->setTag(8);

	// Color Channels

	auto onCol1Spr = ColorChannelSprite::create("1");
	auto onCol1 = gd::CCMenuItemSpriteExtra::create(onCol1Spr, this, menu_selector(NewCustomizeObjectLayer::onSelectColor));
	onCol1->setPosition(25.f, 92.5f);
	menu->addChild(onCol1, 0, 3);
	onCol1Spr->setTag(3);

	auto onCol2Spr = ColorChannelSprite::create("2");
	auto onCol2 = gd::CCMenuItemSpriteExtra::create(onCol2Spr, this, menu_selector(NewCustomizeObjectLayer::onSelectColor));
	onCol2->setPosition(60.f, 92.5f);
	menu->addChild(onCol2, 0, 4);
	onCol2Spr->setTag(4);

	auto onCol3Spr = ColorChannelSprite::create("3");
	auto onCol3 = gd::CCMenuItemSpriteExtra::create(onCol3Spr, this, menu_selector(NewCustomizeObjectLayer::onSelectColor));
	onCol3->setPosition(25.f, 57.5f);
	menu->addChild(onCol3, 0, 6);
	onCol3Spr->setTag(6);

	auto onCol4Spr = ColorChannelSprite::create("4");
	auto onCol4 = gd::CCMenuItemSpriteExtra::create(onCol4Spr, this, menu_selector(NewCustomizeObjectLayer::onSelectColor));
	onCol4->setPosition(60.f, 57.5f);
	menu->addChild(onCol4, 0, 7);
	onCol4Spr->setTag(7);

	m_buttonsArray->addObject(onDefault);
	m_buttonsArray->addObject(onLightBG);
	m_buttonsArray->addObject(onP1);
	m_buttonsArray->addObject(onP2);
	m_buttonsArray->addObject(onWhite);
	m_buttonsArray->addObject(on3DL);
	m_buttonsArray->addObject(onCol1);
	m_buttonsArray->addObject(onCol2);
	m_buttonsArray->addObject(onCol3);
	m_buttonsArray->addObject(onCol4);

	m_colorSprites->addObject(onCol1Spr);
	m_colorSprites->addObject(onCol2Spr);
	m_colorSprites->addObject(onCol3Spr);
	m_colorSprites->addObject(onCol4Spr);
	m_colorSprites->addObject(on3DLSpr);
	m_colorSprites->addObject(onLightBGSpr);

	this->updateColorSprites();
	this->updateSelectedColorSprite();
	this->updateSelectedColorLabel();
	this->updateSelectPosition();

	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

	return true;
}

void NewCustomizeObjectLayer::keyBackClicked() {
	if (m_selectedObjects) {
		m_selectedObjects->release();
	}
	if (m_buttonsArray) {
		m_buttonsArray->release();
	}
	if (m_colorSprites) {
		m_colorSprites->release();
	}
	if (m_newCustomizeObjectLayer) {
		m_newCustomizeObjectLayer = nullptr;
	}

	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void NewCustomizeObjectLayer::onClose(CCObject*) {
	this->keyBackClicked();
}

void NewCustomizeObjectLayer::colorSelectClosed(gd::ColorSelectPopup* colorSelect) {
	auto editorLayer = LevelEditorLayer::get();
	if (editorLayer) {
		auto settings = editorLayer->m_levelSettings;

		switch (colorSelect->getTag()) {
		case 4:
			settings->m_customColor01->m_color = colorSelect->getColorValue();
			settings->m_customColor01->m_playerColor = colorSelect->m_custom;
			settings->m_customColor01->m_blending = colorSelect->m_custom2;
			break;
		case 5:
			settings->m_customColor02->m_color = colorSelect->getColorValue();
			settings->m_customColor02->m_playerColor = colorSelect->m_custom;
			settings->m_customColor02->m_blending = colorSelect->m_custom2;
			break;
		case 6:
			settings->m_customColor03->m_color = colorSelect->getColorValue();
			settings->m_customColor03->m_playerColor = colorSelect->m_custom;
			settings->m_customColor03->m_blending = colorSelect->m_custom2;
			break;
		case 7:
			settings->m_customColor04->m_color = colorSelect->getColorValue();
			settings->m_customColor04->m_playerColor = colorSelect->m_custom;
			settings->m_customColor04->m_blending = colorSelect->m_custom2;
			break;
		case 8:
			settings->m_3DLineColor->m_color = colorSelect->getColorValue();
			settings->m_3DLineColor->m_playerColor = colorSelect->m_custom;
			settings->m_3DLineColor->m_blending = colorSelect->m_custom2;
			break;
		default: break;
		}
	}

	this->updateColorSprites();
	this->updateSelectedColorSprite();
}

void NewCustomizeObjectLayer::updateSelectedColorLabel() {
	if (m_selectedColorLabel) {
		m_selectedColorLabel->setString(this->colorToString(m_selectedColorIdx));
	}
}

void NewCustomizeObjectLayer::updateSelectPosition() {
	if (m_selectSprite) {
		for (auto btn : CCArrayExt<gd::CCMenuItemSpriteExtra*>(m_buttonsArray)) {
			if (btn && btn->getTag() == m_selectedColorIdx) {
				m_selectSprite->setPosition(btn->getParent()->convertToWorldSpace(btn->getPosition()));
			}
		}
	}
}

void NewCustomizeObjectLayer::updateSelectedColorSprite() {
	if (m_selectedColorSprite) {
		switch (m_selectedColorIdx) {
		case 0:
		case 1:
		case 2:
		case 5:
		case 9:
			m_selectedColorSprite->getParent()->setVisible(false); break;
		default:
			m_selectedColorSprite->getParent()->setVisible(true); break;
		}

		auto editorLayer = LevelEditorLayer::get();
		if (editorLayer) {
			auto levelSettings = editorLayer->m_levelSettings;

			if (setting().m_liveColorEnabled && setting().onPreviewMode) {
				for (auto colorSprite : CCArrayExt<ColorChannelSprite*>(m_colorSprites)) {
					if (colorSprite && colorSprite->getTag() == m_selectedColorIdx) {
						m_selectedColorSprite->updateValues(colorSprite->getColor(), colorSprite->m_blendingDot->isVisible());
					}
				}
			}
			else {
				switch (m_selectedColorIdx) {
				case 3: m_selectedColorSprite->updateValues(levelSettings->m_customColor01->m_color, levelSettings->m_customColor01->m_blending); break;
				case 4: m_selectedColorSprite->updateValues(levelSettings->m_customColor02->m_color, levelSettings->m_customColor02->m_blending); break;
				case 6: m_selectedColorSprite->updateValues(levelSettings->m_customColor03->m_color, levelSettings->m_customColor03->m_blending); break;
				case 7: m_selectedColorSprite->updateValues(levelSettings->m_customColor04->m_color, levelSettings->m_customColor04->m_blending); break;
				case 8: m_selectedColorSprite->updateValues(levelSettings->m_3DLineColor->m_color, levelSettings->m_3DLineColor->m_blending); break;
				default: m_selectedColorSprite->updateValues(ccWHITE); break;
				}
			}
		}

		if (m_selectedColorLabel) {
			auto winSize = CCDirector::sharedDirector()->getWinSize();
			m_selectedColorLabel->setPositionX(m_selectedColorSprite->getParent()->isVisible() ? winSize.width / 2.f + 75.f : winSize.width / 2.f + 110.f);
		}
	}
}

void NewCustomizeObjectLayer::updateColorSprites() {
	auto editorLayer = LevelEditorLayer::get();
	auto levelSettings = editorLayer->m_levelSettings;

	for (auto colorSprite : CCArrayExt<ColorChannelSprite*>(m_colorSprites)) {
		if (colorSprite) {
			if (editorLayer) {
				if (setting().m_liveColorEnabled && setting().onPreviewMode) {
					for (auto colorSprite : CCArrayExt<ColorChannelSprite*>(m_colorSprites)) {
						if (colorSprite) {
							switch (colorSprite->getTag()) {
							case 3: colorSprite->updateValues(LevelEditorLayer::getColor01(), LevelEditorLayer::getColor01().blending); break;
							case 4: colorSprite->updateValues(LevelEditorLayer::getColor02(), LevelEditorLayer::getColor02().blending); break;
							case 5: colorSprite->updateValues(getLightBGColor(editorLayer->m_backgroundSprite->getColor(), gd::GameManager::sharedState()->colorForIdx(gd::GameManager::sharedState()->m_playerColor)), true); break;
							case 6: colorSprite->updateValues(LevelEditorLayer::getColor03(), LevelEditorLayer::getColor03().blending); break;
							case 7: colorSprite->updateValues(LevelEditorLayer::getColor04(), LevelEditorLayer::getColor04().blending); break;
							case 8: colorSprite->updateValues(LevelEditorLayer::getColor3DL(), LevelEditorLayer::getColor3DL().blending); break;
							default: break;
							}
						}
					}
				}
				else {
					switch (colorSprite->getTag()) {
					case 3: colorSprite->updateValues(levelSettings->m_customColor01->m_color, levelSettings->m_customColor01->m_blending); break;
					case 4: colorSprite->updateValues(levelSettings->m_customColor02->m_color, levelSettings->m_customColor02->m_blending); break;
					case 5: colorSprite->updateValues(getLightBGColor(editorLayer->m_levelSettings->m_backgroundColor->m_color, gd::GameManager::sharedState()->colorForIdx(gd::GameManager::sharedState()->m_playerColor)), true); break;
					case 6: colorSprite->updateValues(levelSettings->m_customColor03->m_color, levelSettings->m_customColor03->m_blending); break;
					case 7: colorSprite->updateValues(levelSettings->m_customColor04->m_color, levelSettings->m_customColor04->m_blending); break;
					case 8: colorSprite->updateValues(levelSettings->m_3DLineColor->m_color, levelSettings->m_3DLineColor->m_blending); break;
					default: break;
					}
				}
			}
		}
	}
}

void NewCustomizeObjectLayer::onSelectColor(CCObject* sender) {
	auto btn = static_cast<gd::CCMenuItemSpriteExtra*>(sender);

	this->m_selectedColorIdx = btn->getTag();

	int iVar5 = 1 - m_selectedColorIdx;
	if (1 < m_selectedColorIdx) {
		iVar5 = 0;
	}
	if (m_selectedObject == nullptr) {
		for (auto object : CCArrayExt<gd::GameObject*>(m_selectedObjects)) {
			if (object) {
				if (iVar5 == 0) {
					object->updateCustomColorMode(static_cast<gd::GJCustomColorMode>(m_selectedColorIdx), false);
				}
				else {
					object->resetCustomColorMode();
				}
			}
		}
	}
	else if (iVar5 == 0) {
		m_selectedObject->updateCustomColorMode(static_cast<gd::GJCustomColorMode>(m_selectedColorIdx), false);
	}
	else {
		m_selectedObject->resetCustomColorMode();
	}

	this->updateSelectedColorLabel();
	this->updateSelectPosition();
	this->updateSelectedColorSprite();

	if (EditorUI::get()) {
		EditorUI::updateObjectInfoLabel(EditorUI::get());
	}
}

void NewCustomizeObjectLayer::editColor(CCObject*) {
	int colorSelectCID = 0;
	
	if (m_selectedColorIdx == 3 || m_selectedColorIdx == 4) {
		colorSelectCID = m_selectedColorIdx + 1;
	}
	else {
		colorSelectCID = m_selectedColorIdx;
	}

	auto colorObject = this->intToSettings(colorSelectCID);

	if (colorObject) {
		auto colorSelectPopup = gd::ColorSelectPopup::create(0, colorSelectCID, colorObject->m_playerColor, colorObject->m_blending);
		colorSelectPopup->selectColor(colorObject->m_color);
		colorSelectPopup->m_delegate = this;
		colorSelectPopup->show();
	}
}

void NewCustomizeObjectLayer::toggleLiveColor(CCObject* sender) {
	auto toggle = static_cast<gd::CCMenuItemToggler*>(sender);

	if (!setting().onPreviewMode) {
		toggle->toggle(true);
		gd::FLAlertLayer::create("Error", "You need to turn on Preview Mode first.", "OK")->show();
		return;
	}

	setting().m_liveColorEnabled = !setting().m_liveColorEnabled;

	this->updateColorSprites();
	this->updateSelectedColorSprite();
}
#include "preview-mode.hpp"

bool __fastcall LevelSettingsLayer::initH(gd::LevelSettingsLayer* self, void*, gd::LevelSettingsObject* settingsObject) {
	if (!LevelSettingsLayer::init(self, settingsObject)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	if (self->m_customSongWidget) {
		auto flipGravityLabel = CCLabelBMFont::create("Start Flipped", "goldFont.fnt");
		flipGravityLabel->setScale(0.5f);
		flipGravityLabel->setAnchorPoint({ 0.f, 0.5f });
		flipGravityLabel->setPosition({ (winSize.width / 2.f) - 120.f, (winSize.height / 2.f) - 134.f });
		self->m_mainLayer->addChild(flipGravityLabel);

		auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

		auto menu = self->m_buttonMenu;
		auto flipGravityToggler = gd::CCMenuItemToggler::create(toggleOff, toggleOn, self, menu_selector(gd::LevelSettingsLayer::onGravityFlipped));
		flipGravityToggler->toggle(self->m_settingsObject->m_isFlipped);
		flipGravityToggler->setScale(0.7f);
		flipGravityToggler->setPosition(menu->convertToNodeSpace({ (winSize.width / 2.f) - 135.f, (winSize.height / 2.f) - 135.f }));
		menu->addChild(flipGravityToggler);

		// Blending dots

		CCPoint dotOffset = ccp(1.f, 51.f);

		auto line_blending = CCLabelBMFont::create(".", "bigFont.fnt");
		line_blending->setAnchorPoint({ 0.f, 1.f });
		line_blending->setPosition(dotOffset);
		self->m_lBtnSpr->addChild(line_blending); // lmao why not, even tho you can't change this

		auto dl_blending = CCLabelBMFont::create(".", "bigFont.fnt");
		dl_blending->setAnchorPoint({ 0.f, 1.f });
		dl_blending->setPosition(dotOffset);
		self->m_dlBtnSpr->addChild(dl_blending);

		auto col1_blending = CCLabelBMFont::create(".", "bigFont.fnt");
		col1_blending->setAnchorPoint({ 0.f, 1.f });
		col1_blending->setPosition(dotOffset);
		self->m_col1BtnSpr->addChild(col1_blending);

		auto col2_blending = CCLabelBMFont::create(".", "bigFont.fnt");
		col2_blending->setAnchorPoint({ 0.f, 1.f });
		col2_blending->setPosition(dotOffset);
		self->m_col2BtnSpr->addChild(col2_blending);

		auto col3_blending = CCLabelBMFont::create(".", "bigFont.fnt");
		col3_blending->setAnchorPoint({ 0.f, 1.f });
		col3_blending->setPosition(dotOffset);
		self->m_col3BtnSpr->addChild(col3_blending);

		auto col4_blending = CCLabelBMFont::create(".", "bigFont.fnt");
		col4_blending->setAnchorPoint({ 0.f, 1.f });
		col4_blending->setPosition(dotOffset);
		self->m_col4BtnSpr->addChild(col4_blending);

		dl_blending->setVisible(self->m_settingsObject->m_dlColor->m_blending);
		col1_blending->setVisible(self->m_settingsObject->m_customColor1->m_blending);
		col2_blending->setVisible(self->m_settingsObject->m_customColor2->m_blending);
		col3_blending->setVisible(self->m_settingsObject->m_customColor3->m_blending);
		col4_blending->setVisible(self->m_settingsObject->m_customColor4->m_blending);
	}

	return true;
}

void __fastcall LevelSettingsLayer::colorSelectClosedH(gd::LevelSettingsLayer* _self, void*, gd::ColorSelectPopup* colorSelect) {
	LevelSettingsLayer::colorSelectClosed(_self, colorSelect);
	gd::LevelSettingsLayer* self = reinterpret_cast<gd::LevelSettingsLayer*>(reinterpret_cast<uintptr_t>(_self) - 0x1bc);

	auto dl_blending = static_cast<CCLabelBMFont*>(self->m_dlBtnSpr->getChildren()->objectAtIndex(0));
	auto col1_blending = static_cast<CCLabelBMFont*>(self->m_col1BtnSpr->getChildren()->objectAtIndex(0));
	auto col2_blending = static_cast<CCLabelBMFont*>(self->m_col2BtnSpr->getChildren()->objectAtIndex(0));
	auto col3_blending = static_cast<CCLabelBMFont*>(self->m_col3BtnSpr->getChildren()->objectAtIndex(0));
	auto col4_blending = static_cast<CCLabelBMFont*>(self->m_col4BtnSpr->getChildren()->objectAtIndex(0));

	if (dl_blending && col1_blending && col2_blending && col3_blending && col4_blending) {
		dl_blending->setVisible(self->m_settingsObject->m_dlColor->m_blending);
		col1_blending->setVisible(self->m_settingsObject->m_customColor1->m_blending);
		col2_blending->setVisible(self->m_settingsObject->m_customColor2->m_blending);
		col3_blending->setVisible(self->m_settingsObject->m_customColor3->m_blending);
		col4_blending->setVisible(self->m_settingsObject->m_customColor4->m_blending);
	}
}

void LevelSettingsLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x97050), LevelSettingsLayer::initH, reinterpret_cast<void**>(&LevelSettingsLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9a0c0), LevelSettingsLayer::colorSelectClosedH, reinterpret_cast<void**>(&LevelSettingsLayer::colorSelectClosed));
}
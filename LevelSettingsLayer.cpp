#include "LevelSettingsLayer.hpp"
#include "LevelEditorLayer.hpp"

gd::LevelSettingsLayer* m_levelSettingsLayer;

gd::LevelSettingsLayer* LevelSettingsLayer::get() {
	return m_levelSettingsLayer;
}

bool __fastcall LevelSettingsLayer::initH(gd::LevelSettingsLayer* self, void*, gd::LevelSettingsObject* settingsObject) {
	m_levelSettingsLayer = self;
	if (!LevelSettingsLayer::init(self, settingsObject)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	if (self->m_customSongWidget) {
		CCSprite* toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
		toggleOff->setScale(.7f);
		CCSprite* toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		toggleOn->setScale(.7f);

		auto onStartFlipped = gd::CCMenuItemToggler::create(toggleOff, toggleOn, self, menu_selector(gd::LevelSettingsLayer::onGravityFlipped));
		onStartFlipped->toggle(settingsObject->m_isFlipped);
		onStartFlipped->setPosition(self->m_buttonMenu->convertToNodeSpace({ winSize.width / 2.f - 135.f, winSize.height / 2.f - 135.f }));
		self->m_buttonMenu->addChild(onStartFlipped);
		auto onStartFlippedLabel = CCLabelBMFont::create("Start Flipped", "goldFont.fnt");
		onStartFlippedLabel->setAnchorPoint({ 0.f, .5f });
		onStartFlippedLabel->setScale(.5f);
		onStartFlippedLabel->setPosition(winSize.width / 2.f - 120.f, winSize.height / 2.f - 135.f);
		self->m_mainLayer->addChild(onStartFlippedLabel);

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

		dl_blending->setVisible(self->m_settingsObject->m_3DLineColor->m_blending);
		col1_blending->setVisible(self->m_settingsObject->m_customColor01->m_blending);
		col2_blending->setVisible(self->m_settingsObject->m_customColor02->m_blending);
		col3_blending->setVisible(self->m_settingsObject->m_customColor03->m_blending);
		col4_blending->setVisible(self->m_settingsObject->m_customColor04->m_blending);
	}

	return true;
}

void __fastcall LevelSettingsLayer::colorSelectClosedH(gd::LevelSettingsLayer* _self, void*, gd::ColorSelectPopup* colorSelect) {
	LevelSettingsLayer::colorSelectClosed(_self, colorSelect);
	auto self = reinterpret_cast<gd::LevelSettingsLayer*>(reinterpret_cast<uintptr_t>(_self) - 0x1bc);

	auto dl_blending = static_cast<CCLabelBMFont*>(self->m_dlBtnSpr->getChildren()->objectAtIndex(0));
	auto col1_blending = static_cast<CCLabelBMFont*>(self->m_col1BtnSpr->getChildren()->objectAtIndex(0));
	auto col2_blending = static_cast<CCLabelBMFont*>(self->m_col2BtnSpr->getChildren()->objectAtIndex(0));
	auto col3_blending = static_cast<CCLabelBMFont*>(self->m_col3BtnSpr->getChildren()->objectAtIndex(0));
	auto col4_blending = static_cast<CCLabelBMFont*>(self->m_col4BtnSpr->getChildren()->objectAtIndex(0));

	if (dl_blending && col1_blending && col2_blending && col3_blending && col4_blending) {
		dl_blending->setVisible(self->m_settingsObject->m_3DLineColor->m_blending);
		col1_blending->setVisible(self->m_settingsObject->m_customColor01->m_blending);
		col2_blending->setVisible(self->m_settingsObject->m_customColor02->m_blending);
		col3_blending->setVisible(self->m_settingsObject->m_customColor03->m_blending);
		col4_blending->setVisible(self->m_settingsObject->m_customColor04->m_blending);
	}
}

void __fastcall LevelSettingsLayer::selectArtClosedH(gd::LevelSettingsLayer* _self, void*, gd::SelectArtLayer* selectArtLayer) {
	LevelSettingsLayer::selectArtClosed(_self, selectArtLayer);
	LevelEditorLayer::updatePreviewMode();
}

void __fastcall LevelSettingsLayer::destructorH(gd::LevelSettingsLayer* self) {
	LevelSettingsLayer::destructor(self);
	m_levelSettingsLayer = nullptr;
}

void LevelSettingsLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x97050), LevelSettingsLayer::initH, reinterpret_cast<void**>(&LevelSettingsLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9a0c0), LevelSettingsLayer::colorSelectClosedH, reinterpret_cast<void**>(&LevelSettingsLayer::colorSelectClosed));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9a890), LevelSettingsLayer::selectArtClosedH, reinterpret_cast<void**>(&LevelSettingsLayer::selectArtClosed));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x96e90), LevelSettingsLayer::destructorH, reinterpret_cast<void**>(&LevelSettingsLayer::destructor));
}
#include "GJGarageLayer.hpp"
#include "IconPreviewPopup.hpp"
#include "Icons.hpp"

CCMenu* m_iconTypeMenu = nullptr;

gd::CCMenuItemToggler* m_dartBtn = nullptr;
cocos2d::CCSprite* m_dartSelector = nullptr;
cocos2d::CCMenu* m_dartPage = nullptr;
cocos2d::CCMenu* m_arrowMenu = nullptr;
int m_totalWavePages = 0;
int m_wavePage = 0;

void GJGarageLayer::Callback::onIconPreview(CCObject*) {
	IconPreviewPopup::create()->show();
}

void GJGarageLayer::Callback::onGarageRopeBack(CCObject*) {
	return;
}

bool __fastcall GJGarageLayer::initH(gd::GJGarageLayer* self) {
	m_iconTypeMenu = nullptr;
	if (!GJGarageLayer::init(self)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto gm = gd::GameManager::sharedState();

	auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
	demonIcon->setPosition({ director->getScreenRight() - 40.f, director->getScreenTop() - 96.f });
	self->addChild(demonIcon);

	auto demonCountLabel = CCLabelBMFont::create("", "bigFont.fnt");
	demonCountLabel->setString(CCString::createWithFormat("%i", gd::GameStatsManager::sharedState()->getStat("5"))->getCString());
	demonCountLabel->setScale(.5f);
	demonCountLabel->setAnchorPoint({ 1.f, .5f });
	demonCountLabel->setPosition(director->getScreenRight() - 58.f, director->getScreenTop() - 96.f);
	self->addChild(demonCountLabel);

	auto iconPreviewMenu = CCMenu::create();
	iconPreviewMenu->setPosition(self->m_playerPreview->getPositionX() + 65.f, self->m_playerPreview->getPositionY());
	self->addChild(iconPreviewMenu);

	auto onIconPreviewSpr00 = CCSprite::create("GJ_button_01.png");
	onIconPreviewSpr00->setScale(.8f);
	auto onIconPreviewSpr01 = gd::SimplePlayer::create(gm->m_playerFrame);
	onIconPreviewSpr01->setScale(.9f);
	onIconPreviewSpr01->setPosition({ 20.f, 20.f });
	onIconPreviewSpr00->addChild(onIconPreviewSpr01);
	auto onIconPreview = gd::CCMenuItemSpriteExtra::create(onIconPreviewSpr00, self, menu_selector(GJGarageLayer::Callback::onIconPreview));
	iconPreviewMenu->addChild(onIconPreview);

	return true;
}

void __fastcall GJGarageLayer::setupIconSelectH(gd::GJGarageLayer* self) {
	GJGarageLayer::setupIconSelect(self);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto gm = gd::GameManager::sharedState();

	if (gm->m_playerIconType == gd::IconType::Wave) {
		self->m_playerPreview->updatePlayerFrame(setting().selectedDartIdx, gd::IconType::Wave);
	}

	if (m_iconTypeMenu) {
		std::cout << m_iconTypeMenu << std::endl;

		auto dartOff = CCSprite::createWithSpriteFrameName("gj_dartBtn_on_001.png");
		auto dartOn = CCSprite::createWithSpriteFrameName("gj_dartBtn_off_001.png");

		m_dartBtn = gd::CCMenuItemToggler::create(dartOn, dartOff, self, menu_selector(gd::GJGarageLayer::onSelectTab));
		m_dartBtn->setSizeMult(1.2f);
		m_iconTypeMenu->addChild(m_dartBtn, 0, 4);

		m_iconTypeMenu->alignItemsHorizontallyWithPadding(0.f);
		m_iconTypeMenu->setPositionX(m_iconTypeMenu->getPositionX() + 19.2f);

		auto specialNewX = m_dartBtn->getPositionX() + 1.2f;
		m_dartBtn->setPositionX(self->m_tabToggleSpecial->getPositionX());
		self->m_tabToggleSpecial->setPositionX(specialNewX);

		//m_dartPage = gd::GaragePage::create(gd::IconType::Wave, self, menu_selector(GJGarageLayer::Callback::onDartIcon));
		//self->addChild(m_dartPage, 100);
		//self->m_pagesArray->addObject(m_dartPage);

		m_dartPage = CCMenu::create();

		m_dartSelector = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
		m_dartSelector->setScale(.85f);

		int count = Icons::getCount("dart", "001");
		int totalWaves = 0;
		float pages = (ceilf(count / 36) + 1);
		self->m_pagesArray->insertObject(m_dartPage, 4);
		for (int a = 0; a < pages; a++) {
			auto dartMenu = CCMenu::create();
			float rows = -1;
			
			for (int i = 0; i < 36; i++) {
				if (totalWaves > count - 1) break;

				auto base = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("dart_%02d_001.png", i + 1)->getCString());
				auto extra = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("dart_%02d_2_001.png", i + 1)->getCString());
				base->addChild(extra, -1);
				base->setScale(.8f);
				base->setColor(gm->colorForIdx(17));

				base->setPosition(base->getContentSize() / 2.f);
				extra->setPosition(base->getContentSize() / 2.f);
				extra->setColor(gm->colorForIdx(12));

				float posLimit = i % 12;
				if (posLimit == 0) {
					rows++;
				}

				auto btn = gd::CCMenuItemSpriteExtra::create(base, self, menu_selector(GJGarageLayer::Callback::onDartIcon));
				totalWaves++;
				btn->setPosition(-165 + (posLimit * 30), -6 - (rows * 30));
				if (i + 1 == setting().selectedDartIdx) m_dartSelector->setPosition(btn->getPosition());

				btn->setTag(totalWaves);
				dartMenu->addChild(btn);
			}

			m_dartPage->addChild(dartMenu);
			dartMenu->setPosition(0, 0);
		}

		m_totalWavePages = pages;
		m_dartPage->addChild(m_dartSelector);

		if (pages > 1) {
			m_arrowMenu = CCMenu::create();

			auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
			nextSpr->setFlipX(true);
			nextSpr->setScale(.8f);

			auto nextBtn = gd::CCMenuItemSpriteExtra::create(nextSpr, self, menu_selector(GJGarageLayer::Callback::onNext));
			nextBtn->setPosition( 220.f, -36.f );

			auto prevSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
			prevSpr->setScale(.8f);

			auto prevBtn = gd::CCMenuItemSpriteExtra::create(prevSpr, self, menu_selector(GJGarageLayer::Callback::onPrev));
			prevBtn->setPosition( -220.f, -36.f );

			m_arrowMenu->addChild(nextBtn);
			m_arrowMenu->addChild(prevBtn);
			m_arrowMenu->setPosition( 0.f, 0.f );

			m_dartPage->addChild(m_arrowMenu);
		}

		self->addChild(m_dartPage);

		self->selectPage(gd::IconType::Cube);
		GJGarageLayer::switchToPage(ceilf(setting().selectedDartIdx / 36));
	}
}

void GJGarageLayer::Callback::onDartIcon(CCObject* sender) {
	auto btn = static_cast<gd::CCMenuItemSpriteExtra*>(sender);

	setting().selectedDartIdx = btn->getTag();

	Icons::patchDart(Icons::getCount("dart", "001"), btn->getTag());

	this->m_playerPreview->updatePlayerFrame(btn->getTag(), gd::IconType::Wave);

	gd::GameManager::sharedState()->m_playerIconType = gd::IconType::Wave;

	m_dartSelector->setPosition(btn->getPosition());
}

void GJGarageLayer::Callback::onNext(CCObject*) {
	m_wavePage++;
	if (m_wavePage > m_totalWavePages - 1) m_wavePage = 0;
	switchToPage(m_wavePage);
}

void GJGarageLayer::Callback::onPrev(CCObject*) {
	m_wavePage--;
	if (m_wavePage > 0) m_wavePage = m_totalWavePages - 1;
	switchToPage(m_wavePage);
}

void GJGarageLayer::switchToPage(int page) {
	for (int i = 0; i < m_totalWavePages; i++) {
		auto node = static_cast<CCNode*>(m_dartPage->getChildren()->objectAtIndex(i));
		if (i == page) node->setVisible(true);
		else node->setVisible(false);
	}
	float selectedWavePage = ceilf(setting().selectedDartIdx / 36);
	if (selectedWavePage == page) m_dartSelector->setVisible(true);
	else m_dartSelector->setVisible(false);
}

void __fastcall GJGarageLayer::selectPageH(gd::GJGarageLayer* self, void*, gd::IconType iconType) {
	int page = 0;
	switch (iconType) {
	case gd::IconType::Special: page = 5; break;
	default: page = static_cast<int>(iconType); break;
	}

	for (int i = 0; i < self->m_pagesArray->count(); i++) {
		auto node = static_cast<CCNode*>(self->m_pagesArray->objectAtIndex(i));
		if (i == page) node->setVisible(true);
		else node->setVisible(false);
	}

	self->m_tabToggleCube->toggle(iconType == gd::IconType::Cube);
	self->m_tabToggleShip->toggle(iconType == gd::IconType::Ship);
	self->m_tabToggleBall->toggle(iconType == gd::IconType::Ball);
	self->m_tabToggleUfo->toggle(iconType == gd::IconType::UFO);
	m_dartBtn->toggle(iconType == gd::IconType::Wave);
	self->m_tabToggleSpecial->toggle(iconType == gd::IconType::Special);

	self->m_tabToggleCube->setEnabled(iconType != gd::IconType::Cube);
	self->m_tabToggleShip->setEnabled(iconType != gd::IconType::Ship);
	self->m_tabToggleBall->setEnabled(iconType != gd::IconType::Ball);
	self->m_tabToggleUfo->setEnabled(iconType != gd::IconType::UFO);
	m_dartBtn->setEnabled(iconType != gd::IconType::Wave);
	self->m_tabToggleSpecial->setEnabled(iconType != gd::IconType::Special);
}

void __fastcall GJGarageLayer::iconTypeMenuH() {
	__asm {
		mov m_iconTypeMenu, eax
	}
	GJGarageLayer::iconTypeMenu();
}

void __fastcall GJGarageLayer::destructorH(gd::GJGarageLayer* self) {
	GJGarageLayer::destructor(self);
	m_iconTypeMenu = nullptr;
	m_dartBtn = nullptr;
	m_dartSelector = nullptr;
	m_dartPage = nullptr;
	m_arrowMenu = nullptr;
	m_totalWavePages = 0;
	m_wavePage = 0;
}

void GJGarageLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7c5c0), GJGarageLayer::initH, reinterpret_cast<void**>(&GJGarageLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7d790), GJGarageLayer::setupIconSelectH, reinterpret_cast<void**>(&GJGarageLayer::setupIconSelect));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7de70), GJGarageLayer::selectPageH, reinterpret_cast<void**>(&GJGarageLayer::selectPage));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7d93e), GJGarageLayer::iconTypeMenuH, reinterpret_cast<void**>(&GJGarageLayer::iconTypeMenu));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7c4b0), GJGarageLayer::destructorH, reinterpret_cast<void**>(&GJGarageLayer::destructor));
}
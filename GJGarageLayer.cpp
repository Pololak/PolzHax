#include "GJGarageLayer.h"
#include "utils.hpp"
#include "Icons.h"
#include "state.h"

#include "GameVariables.hpp"

Fields* fields = nullptr;
gd::GJGarageLayer* garageLayer;
CCLayer* iconKitPreview;

CCMenu* m_backButtonMenu = nullptr;

class IconKitPreview : public CCLayer {
public:
	static IconKitPreview* create() {
		IconKitPreview* obj = new IconKitPreview;
		if (obj && obj->init()) {
			obj->autorelease();
			return obj;
		}
		CC_SAFE_DELETE(obj);
		return nullptr;
	}

	bool init() {
		if (!CCLayer::init()) return false;
		const float width = 300, height = 85;
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

		auto bgbg = extension::CCScale9Sprite::create("square02_small.png");
		bgbg->setContentSize({ 250, 50 });
		bgbg->setPosition(director->getWinSize().width / 2, director->getWinSize().height / 2);
		bgbg->setOpacity(75);
		bgbg->setZOrder(2);
		this->addChild(bgbg);

		auto appearAction = CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f), .6f);

		auto menu = CCMenu::create();
		menu->setZOrder(3);
		this->addChild(menu);

		auto closeBtn = gd::CCMenuItemSpriteExtra::create(
			(CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png")),
			nullptr,
			this,
			menu_selector(IconKitPreview::onClose));
		closeBtn->setPosition(-((width / 2) - 5), (height / 2) - 5);

		menu->addChild(closeBtn);

		auto gm = gd::GameManager::sharedState();

		auto playerIcon = gd::SimplePlayer::create(gd::GameManager::sharedState()->getPlayerFrame());
		playerIcon->baseSpr()->setColor(gm->colorForIdx(gm->getPlayerColor()));
		playerIcon->colorSpr()->setColor(gm->colorForIdx(gm->getPlayerColor2()));
		playerIcon->glowSpr()->setColor(gm->colorForIdx(gm->getPlayerColor2()));
		if (gm->getPlayerGlow() == 2) playerIcon->glowSpr()->setVisible(1);
		playerIcon->setPosition({ -100, 0 });
		menu->addChild(playerIcon);

		auto shipIcon = gd::SimplePlayer::create(gd::GameManager::sharedState()->getPlayerShip());
		shipIcon->updatePlayerFrame(gd::GameManager::sharedState()->getPlayerShip(), gd::IconType::Ship);
		shipIcon->baseSpr()->setColor(gm->colorForIdx(gm->getPlayerColor()));
		shipIcon->colorSpr()->setColor(gm->colorForIdx(gm->getPlayerColor2()));
		shipIcon->glowSpr()->setColor(gm->colorForIdx(gm->getPlayerColor2()));
		if (gm->getPlayerGlow() == 2) shipIcon->glowSpr()->setVisible(1);
		shipIcon->setPosition({ -50, 0 });
		menu->addChild(shipIcon);

		auto rollIcon = gd::SimplePlayer::create(gd::GameManager::sharedState()->getPlayerBall());
		rollIcon->updatePlayerFrame(gd::GameManager::sharedState()->getPlayerBall(), gd::IconType::Ball);
		rollIcon->baseSpr()->setColor(gm->colorForIdx(gm->getPlayerColor()));
		rollIcon->colorSpr()->setColor(gm->colorForIdx(gm->getPlayerColor2()));
		rollIcon->glowSpr()->setColor(gm->colorForIdx(gm->getPlayerColor2()));
		if (gm->getPlayerGlow() == 2) rollIcon->glowSpr()->setVisible(1);
		rollIcon->setPosition({ 0, 0 });
		menu->addChild(rollIcon);

		auto birdIcon = gd::SimplePlayer::create(gd::GameManager::sharedState()->getPlayerBird());
		birdIcon->updatePlayerFrame(gd::GameManager::sharedState()->getPlayerBird(), gd::IconType::UFO);
		birdIcon->baseSpr()->setColor(gm->colorForIdx(gm->getPlayerColor()));
		birdIcon->colorSpr()->setColor(gm->colorForIdx(gm->getPlayerColor2()));
		birdIcon->glowSpr()->setColor(gm->colorForIdx(gm->getPlayerColor2()));
		if (gm->getPlayerGlow() == 2) birdIcon->glowSpr()->setVisible(1);
		birdIcon->setPosition({ 50, 0 });
		menu->addChild(birdIcon);

		auto waveIcon = gd::SimplePlayer::create(setting().selected_dart);
		waveIcon->updatePlayerFrame(setting().selected_dart, gd::IconType::Wave);
		waveIcon->baseSpr()->setColor(gm->colorForIdx(gm->getPlayerColor()));
		waveIcon->colorSpr()->setColor(gm->colorForIdx(gm->getPlayerColor2()));
		waveIcon->glowSpr()->setColor(gm->colorForIdx(gm->getPlayerColor2()));
		if (gm->getPlayerGlow() == 2) waveIcon->glowSpr()->setVisible(1);
		waveIcon->setPosition({ 100, 0 });
		menu->addChild(waveIcon);

		auto userIDlabel = CCLabelBMFont::create("UserID:", "chatFont.fnt");
		userIDlabel->setPosition({ -145, -31 });
		userIDlabel->setScale(0.75f);
		userIDlabel->setString(CCString::createWithFormat("UserID: %d", gm->getUserID())->getCString());
		userIDlabel->setColor({ 0,0,0 });
		userIDlabel->setAnchorPoint({ 0.f, 0.5f });
		userIDlabel->setOpacity(75);
		menu->addChild(userIDlabel);

		this->setScale(0.1f);
		this->runAction(appearAction);

		return true;
	}

	void keyBackClicked() override {
		this->removeFromParentAndCleanup(true);
		iconKitPreview = nullptr;
	}

	void onClose(CCObject*) {
		this->removeFromParentAndCleanup(true);
		iconKitPreview = nullptr;
	}

	void showCallback(CCObject* btn) {
		auto director = CCDirector::sharedDirector();
		auto myIconLayer = IconKitPreview::create();
		myIconLayer->setZOrder(100);
		auto myLayer2 = static_cast<CCLayer*>(static_cast<CCNode*>(btn)->getParent()->getParent());
		myLayer2->addChild(myIconLayer);
		iconKitPreview = myIconLayer;
	}
};

void GJGarageLayer::Callback::onGarageRopeBack(CCObject*) {
	auto lastScene = static_cast<int>(gd::GameManager::sharedState()->m_lastScene2);
	gd::GJGameLevel* level = static_cast<gd::GJGameLevel*>(gd::GameManager::sharedState()->m_premiumPopup);
	if (lastScene == 99) {
		auto ellScene = gd::EditLevelLayer::scene(level);
		CCScene* scene = CCTransitionMoveInB::create(0.5, ellScene);
		CCDirector::sharedDirector()->pushScene(scene);
	}
	else if (lastScene == 98) {
		auto lilScene = gd::LevelInfoLayer::scene(level);
		CCScene* scene = CCTransitionMoveInB::create(0.5, lilScene);
		CCDirector::sharedDirector()->pushScene(scene);
	}
}

bool __fastcall GJGarageLayer::init_H(gd::GJGarageLayer* self) {
	garageLayer = self;
	delete fields;
	fields = new Fields;
	if (!GJGarageLayer::init_(self)) return false;

	//auto menugamelayer = gd::MenuGameLayer::create();
	//menugamelayer->setTag(-5);
	//self->addChild(menugamelayer);

	auto director = CCDirector::sharedDirector();

	auto iconKitSpr = CCSprite::createWithSpriteFrameName("GJ_rotationControlBtn01_001.png");
	iconKitSpr->setScale(1.25);
	auto iconKitSpr2 = CCSprite::createWithSpriteFrameName("player_04_001.png");
	iconKitSpr2->setScale(0.5);
	iconKitSpr2->setPosition({ 14.5f, 15.5f });
	auto iconKitSpr3 = CCSprite::createWithSpriteFrameName("player_04_2_001.png");
	iconKitSpr3->setPosition({ 15, 15 });
	iconKitSpr3->setZOrder(-1);
	iconKitSpr->addChild(iconKitSpr2);
	iconKitSpr2->addChild(iconKitSpr3);

	auto iconKitBtn = gd::CCMenuItemSpriteExtra::create(iconKitSpr, iconKitSpr, self, menu_selector(IconKitPreview::showCallback));
	auto simplePlayer = from<gd::SimplePlayer*>(self, 0x12c);
	iconKitBtn->setPosition({ simplePlayer->getPositionX() + 80, simplePlayer->getPositionY()});

	auto menu = CCMenu::create();

	menu->addChild(iconKitBtn);
	menu->setPosition({ director->getScreenLeft(), director->getScreenBottom() });
	
	self->addChild(menu);
	
	auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
	auto hammerIcon = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");

	demonIcon->setPosition({ director->getScreenRight() - 40.f, director->getScreenTop() - 96.f });
	hammerIcon->setPosition({ director->getScreenRight() - 40.f, director->getScreenTop() - 124.f });

	self->addChild(demonIcon);
	//self->addChild(hammerIcon);

	auto demonCount = CCLabelBMFont::create("", "bigFont.fnt");
	demonCount->setString(CCString::createWithFormat("%i", gd::GameStatsManager::sharedState()->getStat("5"))->getCString());
	demonCount->setScale(0.5f);
	demonCount->setAnchorPoint({ 1.f, 0.5f });
	demonCount->setPosition({ director->getScreenRight() - 58.f, director->getScreenTop() - 96.f });
	self->addChild(demonCount);

	//auto hammerCount = CCLabelBMFont::create("", "bigFont.fnt");
	//hammerCount->setString(CCString::createWithFormat("%i", gd::GameStatsManager::sharedState()->getStat("5"))->getCString());
	//hammerCount->setScale(0.5f);
	//hammerCount->setAnchorPoint({ 1.f, 0.5f });
	//hammerCount->setPosition({ director->getScreenRight() - 58.f, director->getScreenTop() - 124.f });
	//self->addChild(hammerCount);

	auto lastScene = static_cast<int>(gd::GameManager::sharedState()->m_lastScene2);

	if (lastScene >= 98) {
		auto garageRopeBackSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
		auto garageRopeBackBtn = gd::CCMenuItemSpriteExtra::create(garageRopeBackSpr, nullptr, self, menu_selector(GJGarageLayer::Callback::onGarageRopeBack));
		static_cast<gd::CCMenuItemSpriteExtra*>(m_backButtonMenu->getChildren()->objectAtIndex(0))->setVisible(0);
		m_backButtonMenu->addChild(garageRopeBackBtn);
	}

	return true;
}

void __fastcall GJGarageLayer::setupIconSelectH(gd::GJGarageLayer* self) {
	GJGarageLayer::setupIconSelect(self);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto GM = gd::GameManager::sharedState();

	auto itemMenu = cocos::getChildOfType<CCMenu>(self, 0);

	auto dartOff = CCSprite::createWithSpriteFrameName("gj_dartBtn_on_001.png");
	auto dartOn = CCSprite::createWithSpriteFrameName("gj_dartBtn_off_001.png");

	fields->m_dartBtn = gd::CCMenuItemToggler::create(dartOn, dartOff, self, menu_selector(gd::GJGarageLayer::onSelectTab));
	fields->m_dartBtn->setSizeMult(1.2f);
	itemMenu->addChild(fields->m_dartBtn);
	fields->m_dartBtn->setTag(4);

	itemMenu->alignItemsHorizontallyWithPadding(0.f);

	fields->m_dartPage = CCMenu::create();

	auto specialBtn = from<gd::CCMenuItemToggler*>(self, 0x16c);
	auto specialNewX = fields->m_dartBtn->getPositionX() + 1.2;
	fields->m_dartBtn->setPositionX(specialBtn->getPositionX());
	specialBtn->setPositionX(specialNewX);

	fields->m_dartSelector = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	fields->m_dartSelector->setScale(0.85);

	//fields->m_pagesArray = CCArray::create();
	int count = Icons::getCount("dart", "001");
	int totalWaves = 0;
	float pages = (ceilf(count / 36) + 1);
	self->getGaragePageArray()->insertObject(fields->m_dartPage, 4);
	for (int a = 0; a < pages; a++) {
		auto dartMenu = CCMenu::create();
		float rows = -1;
		for (int i = 0; i < 36; i++) {

			//if (i > 35) break;
			if (totalWaves > count - 1) break;

			//std::cout << totalWaves << " " << i << " " << count << " " << a << std::endl;

			auto base = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("dart_%02d_001.png", i + 1)->getCString());
			auto extra = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("dart_%02d_2_001.png", i + 1)->getCString());
			base->addChild(extra, -1);
			base->setScale(0.8);
			base->setColor(GM->colorForIdx(17));

			base->setPosition(base->getContentSize() / 2);
			extra->setPosition(base->getContentSize() / 2);
			extra->setColor(GM->colorForIdx(12));


			float posLimit = i % 12;
			if (posLimit == 0)
				rows++;

			//player->updatePlayerFrame(i + 1, gd::IconType::Wave);
			auto btn = gd::CCMenuItemSpriteExtra::create(base, nullptr, self, menu_selector(GJGarageLayer::Callback::onDartIcon));
			totalWaves++;
			btn->setPosition({ -165 + (posLimit * 30), -6 - (rows * 30) }); //guh
			if (i + 1 == setting().selected_dart) fields->m_dartSelector->setPosition(btn->getPosition());

			btn->setTag(totalWaves);
			dartMenu->addChild(btn);
		}
		//std::cout << "page: " << a << std::endl;
		//dartMenu->setPositionX(dartMenu->getContentSize().width * (a));
		fields->m_dartPage->addChild(dartMenu);
		dartMenu->setPosition({ 0, 0 });
		//fields->m_pagesArray->addObject(dartMenu);
	}
	fields->m_totalWavePages = pages;
	fields->m_dartPage->addChild(fields->m_dartSelector);

	if (pages > 1) {
		fields->m_arrowMenu = CCMenu::create();

		auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
		nextSpr->setFlipX(true);
		nextSpr->setScale(0.8);

		auto nextBtn = gd::CCMenuItemSpriteExtra::create(nextSpr, nullptr, self, menu_selector(GJGarageLayer::Callback::onNext));
		nextBtn->setPosition({ 220, -36 });

		auto prevSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
		prevSpr->setScale(0.8);

		auto prevBtn = gd::CCMenuItemSpriteExtra::create(prevSpr, nullptr, self, menu_selector(GJGarageLayer::Callback::onPrev));
		prevBtn->setPosition({ -220, -36 });

		fields->m_arrowMenu->addChild(nextBtn);
		fields->m_arrowMenu->addChild(prevBtn);
		fields->m_arrowMenu->setPosition({ 0, 0 });

		fields->m_dartPage->addChild(fields->m_arrowMenu);
	}

	itemMenu->setPosition(winSize.width * 0.5 - 115, itemMenu->getPositionY());
	self->addChild(fields->m_dartPage);
	//fields->m_dartMenu->alignItemsHorizontallyWithPadding(10.f);

	//auto dartMenu = gd::GaragePage::create(gd::IconType::Ball, this, menu_selector(GJGarageLayer::onDartIcon));

	GJGarageLayer::selectPage(self, 0);
	GJGarageLayer::switchToPage((ceilf(setting().selected_dart / 36)));
}

void GJGarageLayer::Callback::onDartIcon(CCObject* sender) {
	auto newSender = static_cast<CCNode*>(sender);
	setting().selected_dart = newSender->getTag();
	Icons::patchDart(Icons::getCount("dart", "001"), newSender->getTag());
	auto playerIcon = garageLayer->getPlayerIcon();
	playerIcon->updatePlayerFrame(newSender->getTag(), gd::IconType::Wave);
	gd::GameManager::sharedState()->setPlayerType(gd::IconType::Wave);
	fields->m_dartSelector->setPosition(newSender->getPosition());
}

void GJGarageLayer::Callback::onNext(CCObject* sender) {
	fields->m_wavePage++;
	if (fields->m_wavePage > fields->m_totalWavePages - 1) fields->m_wavePage = 0;
	//std::cout << fields->m_wavePage << " " << fields->m_totalWavePages << std::endl;
	GJGarageLayer::switchToPage(fields->m_wavePage);
}

void GJGarageLayer::Callback::onPrev(CCObject* sender) {
	fields->m_wavePage--;
	if (fields->m_wavePage < 0) fields->m_wavePage = fields->m_totalWavePages - 1;
	//std::cout << fields->m_wavePage << " " << fields->m_totalWavePages << std::endl;
	GJGarageLayer::switchToPage(fields->m_wavePage);
}

void GJGarageLayer::switchToPage(int page) {
	//std::cout << page << " " << fields->m_totalWavePages - 1 << std::endl;
	for (int i = 0; i < fields->m_totalWavePages; i++) {
		auto node = static_cast<CCNode*>(fields->m_dartPage->getChildren()->objectAtIndex(i));
		if (i == page) node->setVisible(true);
		else node->setVisible(false);
	}
	float selectedWavePage = (ceilf(setting().selected_dart / 36));
	if (selectedWavePage == page) fields->m_dartSelector->setVisible(true);
	else fields->m_dartSelector->setVisible(false);
}

void __fastcall GJGarageLayer::selectPageH(gd::GJGarageLayer* self, void* edx, int iconType) {
	int page = 0;
	switch (iconType) {
	case 99: page = 5; break; // replace to 4 if you want to bring back
	default: page = iconType; break;
	}
	for (int i = 0; i < self->getGaragePageArray()->count(); i++) {
		auto node = static_cast<CCNode*>(self->getGaragePageArray()->objectAtIndex(i));
		if (i == page) node->setVisible(true);
		else node->setVisible(false);
	}
	self->getCubeBtn()->toggle(iconType == 0);
	self->getShipBtn()->toggle(iconType == 1);
	self->getBallBtn()->toggle(iconType == 2);
	self->getBirdBtn()->toggle(iconType == 3);
	fields->m_dartBtn->toggle(iconType == 4);
	self->getSpecialBtn()->toggle(iconType == 99);

	self->getCubeBtn()->setEnabled(iconType != 0);
	self->getShipBtn()->setEnabled(iconType != 1);
	self->getBallBtn()->setEnabled(iconType != 2);
	self->getBirdBtn()->setEnabled(iconType != 3);
	fields->m_dartBtn->setEnabled(iconType != 4);
	self->getSpecialBtn()->setEnabled(iconType != 99);
}

void __fastcall GJGarageLayer::backButtonMenuH() {
	__asm {
		mov m_backButtonMenu, eax
	}
	GJGarageLayer::backButtonMenu();
}

void GJGarageLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7c5c0), GJGarageLayer::init_H, reinterpret_cast<void**>(&GJGarageLayer::init_));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7d790), GJGarageLayer::setupIconSelectH, reinterpret_cast<void**>(&GJGarageLayer::setupIconSelect));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7de70), GJGarageLayer::selectPageH, reinterpret_cast<void**>(&GJGarageLayer::selectPage));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7cef5), GJGarageLayer::backButtonMenuH, reinterpret_cast<void**>(&GJGarageLayer::backButtonMenu));
}
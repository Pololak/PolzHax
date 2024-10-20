#include "GJGarageLayer.h"
#include "utils.hpp"
#include "Icons.h"
#include "state.h"

Fields* fields = nullptr;
gd::GJGarageLayer* garageLayer;

bool __fastcall GJGarageLayer::init_H(gd::GJGarageLayer* self) {
	garageLayer = self;
	delete fields;
	fields = new Fields;
	if (!GJGarageLayer::init_(self)) return false;
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

			std::cout << totalWaves << " " << i << " " << count << " " << a << std::endl;

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
		std::cout << "page: " << a << std::endl;
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
	std::cout << page << " " << fields->m_totalWavePages - 1 << std::endl;
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

void GJGarageLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7c5c0), GJGarageLayer::init_H, reinterpret_cast<void**>(&GJGarageLayer::init_));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7d790), GJGarageLayer::setupIconSelectH, reinterpret_cast<void**>(&GJGarageLayer::setupIconSelect));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7de70), GJGarageLayer::selectPageH, reinterpret_cast<void**>(&GJGarageLayer::selectPage));
}
#include "LevelSearchLayer.hpp"
#include "Setting.hpp"

void LevelSearchLayer::Callback::onSearchPlayer(CCObject*) {
	if (!this->m_searchInput->getString().empty()) {
		std::cout << this->m_searchInput->getString() << std::endl;
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, gd::LevelBrowserLayer::scene(gd::GJSearchObject::create(gd::SearchType::UsersLevels, this->m_searchInput->getString()))));
	}
}

bool __fastcall LevelSearchLayer::initH(gd::LevelSearchLayer* self) {
	if (!LevelSearchLayer::init(self)) return false;

	auto menu = CCMenu::create();
	self->addChild(menu);

	auto onSearchPlayerSpr = CCSprite::create("GJ_button_01.png");
	onSearchPlayerSpr->setScale(.7f);
	auto onSearchPlayerSpr2 = gd::SimplePlayer::create(4);
	onSearchPlayerSpr2->updatePlayerFrame(4, gd::IconType::Cube);
	onSearchPlayerSpr2->setScale(.9f);
	onSearchPlayerSpr2->setPosition(onSearchPlayerSpr->getContentSize() / 2.f);
	onSearchPlayerSpr->addChild(onSearchPlayerSpr2);
	auto onSearchPlayer = gd::CCMenuItemSpriteExtra::create(onSearchPlayerSpr, self, menu_selector(LevelSearchLayer::Callback::onSearchPlayer));
	onSearchPlayer->setPosition(200.f, 120.f);
	menu->addChild(onSearchPlayer);

	return true;
}

void __fastcall LevelSearchLayer::clearFiltersH(gd::LevelSearchLayer* self) {
	LevelSearchLayer::clearFilters(self);

	setting().onExtraLong = false;
}

void LevelSearchLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9f830), LevelSearchLayer::initH, reinterpret_cast<void**>(&LevelSearchLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xa1000), LevelSearchLayer::clearFiltersH, reinterpret_cast<void**>(&LevelSearchLayer::clearFilters));
}
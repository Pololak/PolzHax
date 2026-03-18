#include "LeaderboardsLayer.hpp"

void LeaderboardsLayer::Callback::onRefresh(CCObject*) {
	auto glm = gd::GameLevelManager::sharedState();

	glm->updateUserScore();
	glm->resetTimerForKey("leaderboard_top");
	glm->resetTimerForKey("leaderboard_week");
	glm->resetTimerForKey("leaderboard_global");
	glm->resetTimerForKey("leaderboard_creator");
}

bool __fastcall LeaderboardsLayer::initH(gd::LeaderboardsLayer* self, void*, gd::LeaderboardState state) {
	if (!LeaderboardsLayer::init(self, state)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto refreshMenu = CCMenu::create();
	refreshMenu->setPosition(director->getScreenRight(), director->getScreenBottom());
	self->addChild(refreshMenu, 2);

	auto onRefreshSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	auto onRefresh = gd::CCMenuItemSpriteExtra::create(onRefreshSpr, self, menu_selector(LeaderboardsLayer::Callback::onRefresh));
	onRefresh->setPosition(-30.f, 30.f);
	refreshMenu->addChild(onRefresh);

	return true;
}

void LeaderboardsLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x87fc0), LeaderboardsLayer::initH, reinterpret_cast<void**>(&LeaderboardsLayer::init));
}
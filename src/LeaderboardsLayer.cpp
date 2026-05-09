#include "LeaderboardsLayer.hpp"

void PolzLeaderboardsLayer::onRefresh(CCObject*) {
    auto glm = GameLevelManager::sharedState();

    glm->updateUserScore();
    glm->resetTimerForKey("leaderboard_top");
    glm->resetTimerForKey("leaderboard_week");
    glm->resetTimerForKey("leaderboard_global");
    glm->resetTimerForKey("leaderboard_creator");

    LeaderboardState currentState = this->m_leaderboardState;
    this->m_leaderboardState = static_cast<LeaderboardState>(5);
    this->selectLeaderboard(currentState);
}

bool PolzLeaderboardsLayer::init(LeaderboardState state) {
    if (!LeaderboardsLayer::init(state)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto refreshMenu = CCMenu::create();
    refreshMenu->setPosition(director->getScreenRight(), director->getScreenBottom());
    this->addChild(refreshMenu, 2);

    auto onRefreshSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    auto onRefresh = CCMenuItemSpriteExtra::create(onRefreshSpr, this, menu_selector(PolzLeaderboardsLayer::onRefresh));
    onRefresh->setPosition(-30.f, 30.f);
    refreshMenu->addChild(onRefresh);

    return true;
}
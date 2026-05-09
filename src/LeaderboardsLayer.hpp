#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/LeaderboardsLayer.hpp>

using namespace geode::prelude;

class PolzLeaderboardsLayer : public geode::Modify<PolzLeaderboardsLayer, LeaderboardsLayer> {
public:
    bool init(LeaderboardState);

    void onRefresh(CCObject*);
};
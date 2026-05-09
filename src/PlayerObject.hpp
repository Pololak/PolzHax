#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class PolzPlayerObject : public geode::Modify<PolzPlayerObject, PlayerObject> {
public:
    bool init(int, int, CCLayer*);
    void updatePlayerFrame(int);
    void updatePlayerRollFrame(int);
    void toggleDartMode(bool);
    void togglePlayerScale(bool);
    void loadFromCheckpoint(PlayerCheckpoint*);
    void runBallRotation2();
    void updateJump(float);

    inline void updateSwing(const float);
};
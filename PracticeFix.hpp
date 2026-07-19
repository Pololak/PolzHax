#pragma once
#include "pch.h"
#include <unordered_map>
#include "PolzBot.hpp"
#include <map>

struct CheckpointStorage {
    gd::GameObject* m_cameraPortal;
    gd::GameObject* m_dualPortal;

    cocos2d::CCPoint m_player1RealPos;
    cocos2d::CCPoint m_player2RealPos;

    double m_yVelocity;
    double m_yVelocityP2;
    float m_rotation;
    float m_rotationP2;

    std::unordered_map<gd::GameObject*, std::pair<bool, bool>> m_activatedObjects;

    int m_currentFrame;
    unsigned int m_frameOffset;
    std::vector<PolzBot::Event> m_replayEventsVec;
};
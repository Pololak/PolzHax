#pragma once
#include "pch.h"
#include <unordered_map>

//inline std::unordered_map<gd::GameObject*, std::pair<bool, bool>> m_activatedObjects;

struct CheckpointStorage {
    gd::GameObject* m_cameraPortal;
    gd::GameObject* m_dualPortal;

    float m_rotationX;
    float m_rotationXP2;
    float m_rotationY;
    float m_rotationYP2;
    float m_slopeYVelocity;
    float m_slopeYVelocityP2;
    double m_yVelocity;
    double m_yVelocityP2;

    std::unordered_map<gd::GameObject*, std::pair<bool, bool>> m_activatedObjects;
};
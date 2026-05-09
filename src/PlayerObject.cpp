#include "PlayerObject.hpp"
#include "Setting.hpp"
#include "LevelEditorLayer.hpp"

bool PolzPlayerObject::init(int p0, int p1, CCLayer* p3) {
    if (!PlayerObject::init(p0, p1, p3)) return false;

    auto gm = GameManager::sharedState();

    if (setting().onMiniCubeIcon || setting().onNoMiniIcon) {
        this->updatePlayerFrame(p0);
    }

    return true;
}

void PolzPlayerObject::updatePlayerFrame(int frameID) {
    auto gm = GameManager::sharedState();

    if (gm->m_playLayer || PolzLevelEditorLayer::get()) {
        if (setting().onMiniCubeIcon) return PlayerObject::updatePlayerFrame(0);

        if (setting().onNoMiniIcon) return PlayerObject::updatePlayerFrame(gm->m_playerFrame);
    }

    PlayerObject::updatePlayerFrame(frameID);
}

void PolzPlayerObject::updatePlayerRollFrame(int frameID) {
    auto gm = GameManager::sharedState();

    if (gm->m_playLayer || PolzLevelEditorLayer::get()) {
        if (setting().onMiniCubeIcon) return PlayerObject::updatePlayerRollFrame(0);

        if (setting().onNoMiniIcon) return PlayerObject::updatePlayerRollFrame(gm->m_playerBall);
    }

    PlayerObject::updatePlayerRollFrame(frameID);
}

void PolzPlayerObject::toggleDartMode(bool p0) {
    PlayerObject::toggleDartMode(p0);

    if (setting().onNoWaveTrailBehind) {
        this->m_playerStreak->stopStroke();
    }
}

void PolzPlayerObject::togglePlayerScale(bool p0) {
    PlayerObject::togglePlayerScale(p0);

    if (setting().onWaveTrailBugFix) {
        this->placeStreakPoint();
    }
}

void PolzPlayerObject::loadFromCheckpoint(PlayerCheckpoint* p0) {
    PlayerObject::loadFromCheckpoint(p0);

    if (setting().onWaveTrailBugFix) {
        this->placeStreakPoint();
    }
}

void PolzPlayerObject::runBallRotation2() {
    if (setting().onBallRotationBugFix) {
        this->stopActionByTag(1);
    }
    PlayerObject::runBallRotation2();
}

void PolzPlayerObject::updateSwing(const float delta) { // https://github.com/adafcaefc/SwingCopter/blob/master/SwingCopter/main.cpp
    const auto direction = this->m_gravityFlipped ? -1.f : 1.f;

    const auto size = (this->getScale() != 1.f) ? .85f : 1.f;

    const auto gravity = (std::fabs(this->m_yVelocity) > 6.) ? this->m_gravity + 1. : this->m_gravity;

    const auto modifier = -.5 * gravity * delta * direction / size;
    const auto yAcceleration = this->m_yVelocity + modifier;

    if (std::fabs(this->m_yVelocity) <= 6. || std::fabs(yAcceleration) <= std::fabs(this->m_yVelocity)) {
        this->m_yVelocity = yAcceleration;
    }

    if (this->m_upKeyDown && this->m_upKeyPressed) {
        this->m_upKeyPressed = false;
        this->m_upKeyDown = false;
        this->flipGravity(!this->m_gravityFlipped, true);
    }
}

void PolzPlayerObject::updateJump(float dt) {
    if (!this->m_flyMode || !setting().onShipcopter) return PlayerObject::updateJump(dt);

    return this->updateSwing(dt);
}
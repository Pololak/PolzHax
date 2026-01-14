#include "PlayerObject.hpp"
#include "Setting.hpp"

bool __fastcall PlayerObject::initH(gd::PlayerObject* self, void*, int frameID, int iconType, CCLayer* gameLayer) {
	if (!PlayerObject::init(self, frameID, iconType, gameLayer)) return false;

	self->updatePlayerFrame(frameID);

	return true;
}

void __fastcall PlayerObject::updatePlayerFrameH(gd::PlayerObject* self, void*, int frameID) {
	auto gm = gd::GameManager::sharedState();

	if (setting().onMiniCubeIcon) return PlayerObject::updatePlayerFrame(self, 0);

	if (setting().onNoMiniIcon) return PlayerObject::updatePlayerFrame(self, gm->m_playerFrame);

	PlayerObject::updatePlayerFrame(self, frameID);
}

void __fastcall PlayerObject::updatePlayerRollFrameH(gd::PlayerObject* self, void*, int frameID) {
	auto gm = gd::GameManager::sharedState();

	if (setting().onMiniCubeIcon) return PlayerObject::updatePlayerRollFrame(self, 0);

	if (setting().onNoMiniIcon) return PlayerObject::updatePlayerRollFrame(self, gm->m_playerFrame);

	PlayerObject::updatePlayerRollFrame(self, frameID);
}

void __fastcall PlayerObject::toggleDartModeH(gd::PlayerObject* self, void*, bool p0) {
	PlayerObject::toggleDartMode(self, p0);
	if (setting().onNoWaveTrailBehind) {
		self->m_playerStreak->stopStroke();
	}
}

void __fastcall PlayerObject::runBallRotation2H(gd::PlayerObject* self) {
	if (setting().onBallRotationBugFix) {
		self->stopActionByTag(1);
	}
	PlayerObject::runBallRotation2(self);
}

void PlayerObject::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd8ca0), PlayerObject::initH, reinterpret_cast<void**>(&PlayerObject::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdfff0), PlayerObject::updatePlayerFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0430), PlayerObject::updatePlayerRollFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerRollFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdee80), PlayerObject::toggleDartModeH, reinterpret_cast<void**>(&PlayerObject::toggleDartMode));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdad10), PlayerObject::runBallRotation2H, reinterpret_cast<void**>(&PlayerObject::runBallRotation2));
}
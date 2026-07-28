#include "PlayerObject.hpp"
#include "PlayLayer.hpp"
#include "Setting.hpp"
#include "Icons.hpp"
#include "LevelEditorLayer.hpp"

void PlayerObject::updateSwing(gd::PlayerObject* self, const float delta) { // https://github.com/adafcaefc/SwingCopter/blob/master/SwingCopter/main.cpp
	const auto direction = self->m_gravityFlipped ? -1.f : 1.f;

	const auto size = (self->getScale() != 1.f) ? .85f : 1.f;

	const auto gravity = (std::fabs(self->m_yVelocity) > 6.) ? self->m_gravity + 1. : self->m_gravity;

	const auto modifier = -.5 * gravity * delta * direction / size;
	const auto yAcceleration = self->m_yVelocity + modifier;

	if (std::fabs(self->m_yVelocity) <= 6. || std::fabs(yAcceleration) <= std::fabs(self->m_yVelocity)) {
		self->m_yVelocity = yAcceleration;
	}

	if (self->m_upKeyDown && self->m_upKeyPressed) {
		self->m_upKeyPressed = false;
		self->m_upKeyDown = false;
		self->flipGravity(!self->m_gravityFlipped, true);
	}
}

void PlayerObject::updatePlayerSpriteExtra(gd::PlayerObject* self, std::string frameName) { // Fuck you Capeling.
	auto playerSpriteExtra = static_cast<CCSprite*>(self->m_playerFrame->getChildByTag(6556));
	auto spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName.c_str());
	if (spriteFrame) {
		playerSpriteExtra->setDisplayFrame(spriteFrame);
		playerSpriteExtra->setVisible(true);
		playerSpriteExtra->setPosition(self->m_playerFrame->getContentSize() / 2.f);
	}
	else {
		playerSpriteExtra->setVisible(false);
	}
}

void PlayerObject::updateShipSpriteExtra(gd::PlayerObject* self, std::string frameName) {
	auto shipSpriteExtra = static_cast<CCSprite*>(self->m_vehicleFrame->getChildByTag(6556));
	auto spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName.c_str());
	if (spriteFrame) {
		shipSpriteExtra->setDisplayFrame(spriteFrame);
		shipSpriteExtra->setVisible(true);
		shipSpriteExtra->setPosition(self->m_vehicleFrame->getContentSize() / 2.f);
	}
	else {
		shipSpriteExtra->setVisible(false);
	}
}

bool __fastcall PlayerObject::initH(gd::PlayerObject* self, void*, int playerFrameID, int vehicleFrameID, CCLayer* gameLayer) {
	if (!PlayerObject::init(self, playerFrameID, vehicleFrameID, gameLayer)) return false;
	
	auto playerSpriteExtra = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("player_%02d_001.png", playerFrameID)->getCString());
	self->m_playerFrame->addChild(playerSpriteExtra, 2, 6556);
	playerSpriteExtra->setPosition(self->m_playerFrame->convertToNodeSpace(self->m_playerFrame->getContentSize()));
	
	auto playerExtraFrameName = CCString::createWithFormat("player_%02d_extra_001.png", playerFrameID);
	PlayerObject::updatePlayerSpriteExtra(self, playerExtraFrameName->getCString());

	auto shipSpriteExtra = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("ship_%02d_001.png", vehicleFrameID)->getCString());
	self->m_vehicleFrame->addChild(shipSpriteExtra, 2, 6556);
	shipSpriteExtra->setPosition(self->m_vehicleFrame->convertToNodeSpace(self->m_vehicleFrame->getContentSize()));

	auto shipExtraFrameName = CCString::createWithFormat("ship_%02d_extra_001.png", vehicleFrameID);
	PlayerObject::updateShipSpriteExtra(self, shipExtraFrameName->getCString());

	auto gm = gd::GameManager::sharedState();

	if (gm->getPlayLayer() || LevelEditorLayer::get()) {
		if (setting().onMiniCubeIcon) self->updatePlayerFrame(gm->m_playerFrame);
	}

	return true;
}

void __fastcall PlayerObject::updatePlayerFrameH(gd::PlayerObject* self, void*, int frameID) {
	auto gm = gd::GameManager::sharedState();

	if (gm->getPlayLayer() || LevelEditorLayer::get()) {
		if (setting().onMiniCubeIcon) frameID = 0;

		if (setting().onNoMiniIcon) frameID = gm->m_playerFrame;
	}

	PlayerObject::updatePlayerFrame(self, frameID);

	auto extraFrameName = CCString::createWithFormat("player_%02d_extra_001.png", frameID);
	PlayerObject::updatePlayerSpriteExtra(self, extraFrameName->getCString());
}

void __fastcall PlayerObject::updatePlayerShipFrameH(gd::PlayerObject* self, void*, int frameID) {
	PlayerObject::updatePlayerShipFrame(self, frameID);

	auto extraFrameName = CCString::createWithFormat("ship_%02d_extra_001.png", frameID);
	PlayerObject::updateShipSpriteExtra(self, extraFrameName->getCString());
}

void __fastcall PlayerObject::updatePlayerRollFrameH(gd::PlayerObject* self, void*, int frameID) {
	auto gm = gd::GameManager::sharedState();

	if (gm->getPlayLayer() || LevelEditorLayer::get()) {
		if (setting().onMiniCubeIcon) frameID = 0;

		if (setting().onNoMiniIcon) frameID = gm->m_playerBall;
	}

	PlayerObject::updatePlayerRollFrame(self, frameID);

	auto extraFrameName = CCString::createWithFormat("player_ball_%02d_extra_001.png", frameID);
	PlayerObject::updatePlayerSpriteExtra(self, extraFrameName->getCString());
}

void __fastcall PlayerObject::updatePlayerBirdFrameH(gd::PlayerObject* self, void*, int frameID) {
	PlayerObject::updatePlayerBirdFrame(self, frameID);

	auto extraFrameName = CCString::createWithFormat("bird_%02d_extra_001.png", frameID);
	PlayerObject::updateShipSpriteExtra(self, extraFrameName->getCString());
}

void __fastcall PlayerObject::updatePlayerDartFrameH(gd::PlayerObject* self, void*, int frameID) { // it's funny how frameID is there but unused
	PlayerObject::updatePlayerDartFrame(self, frameID);

	auto extraFrameName = CCString::createWithFormat("dart_%02d_extra_001.png", setting().selectedDartIdx);
	PlayerObject::updatePlayerSpriteExtra(self, extraFrameName->getCString());
}

void __fastcall PlayerObject::toggleDartModeH(gd::PlayerObject* self, void*, bool p0) {
	PlayerObject::toggleDartMode(self, p0);

	if (setting().onNoWaveTrailBehind) {
		self->m_playerStreak->stopStroke();
	}
}

void __fastcall PlayerObject::togglePlayerScaleH(gd::PlayerObject* self, void*, bool p0) {
	PlayerObject::togglePlayerScale(self, p0);

	if (setting().onModernParticles) {
		self->m_playerGroundParticles->setScale(1.f);
		self->m_trailingParticles->setScale(1.f);
		self->m_shipClickParticles->setScale(1.f);
		self->m_vehicleGroundParticles->setScale(1.f);
		self->m_ufoClickParticles->setScale(1.f);
		self->m_landParticles->setScale(1.f);
		self->m_landParticles2->setScale(1.f);

		if (p0) {
			self->m_playerGroundParticles->m_fStartSize = self->m_playerGroundParticles->m_fStartSize * self->m_playerScale;
			self->m_playerGroundParticles->m_fStartSizeVar = self->m_playerGroundParticles->m_fStartSizeVar * self->m_playerScale;

			self->m_trailingParticles->m_fStartSize = self->m_trailingParticles->m_fStartSize * self->m_playerScale;
			self->m_trailingParticles->m_fStartSizeVar = self->m_trailingParticles->m_fStartSizeVar * self->m_playerScale;

			self->m_shipClickParticles->m_fStartSize = self->m_shipClickParticles->m_fStartSize * .4f;
			self->m_shipClickParticles->m_fStartSizeVar = self->m_shipClickParticles->m_fStartSizeVar * .4f;

			self->m_vehicleGroundParticles->m_fStartSize = self->m_vehicleGroundParticles->m_fStartSize * self->m_playerScale;
			self->m_vehicleGroundParticles->m_fStartSizeVar = self->m_vehicleGroundParticles->m_fStartSizeVar * self->m_playerScale;

			self->m_ufoClickParticles->m_fStartSize = self->m_ufoClickParticles->m_fStartSize * self->m_playerScale;	
			self->m_ufoClickParticles->m_fStartSizeVar = self->m_ufoClickParticles->m_fStartSizeVar * self->m_playerScale;

			self->m_landParticles->m_fStartSize = self->m_landParticles->m_fStartSize * self->m_playerScale;
			self->m_landParticles->m_fStartSizeVar = self->m_landParticles->m_fStartSizeVar * self->m_playerScale;

			self->m_landParticles2->m_fStartSize = self->m_landParticles2->m_fStartSize * self->m_playerScale;
			self->m_landParticles2->m_fStartSizeVar = self->m_landParticles2->m_fStartSizeVar * self->m_playerScale;
		}
		else {
			self->m_playerGroundParticles->m_fStartSize = 4.f;
			self->m_playerGroundParticles->m_fStartSizeVar = 3.f;

			self->m_trailingParticles->m_fStartSize = 4.f;
			self->m_trailingParticles->m_fStartSizeVar = 3.f;

			self->m_shipClickParticles->m_fStartSize = 6.f;
			self->m_shipClickParticles->m_fStartSizeVar = 4.5f;

			self->m_vehicleGroundParticles->m_fStartSize = 3.f;
			self->m_vehicleGroundParticles->m_fStartSizeVar = 2.f;

			self->m_ufoClickParticles->m_fStartSize = 8.f;
			self->m_ufoClickParticles->m_fStartSizeVar = 2.f;

			self->m_landParticles->m_fStartSize = 5.f;
			self->m_landParticles->m_fStartSizeVar = 3.f;

			self->m_landParticles2->m_fStartSize = 5.f;
			self->m_landParticles2->m_fStartSizeVar = 3.f;
		}
	}

	if (setting().onWaveTrailBugFix) {
		self->placeStreakPoint();
	}

	if (setting().onTrailBugFix && !p0 && (gd::GameManager::sharedState()->m_playerStreak == 2)) {
		self->m_playerStreak->m_fMinSeg = 14.f;
	}
}

void __fastcall PlayerObject::runBallRotation2H(gd::PlayerObject* self) {
	if (setting().onBallRotationBugFix) {
		self->stopActionByTag(1);
	}
	PlayerObject::runBallRotation2(self);
}

void __fastcall PlayerObject::loadFromCheckpointH(gd::PlayerObject* self, void*, gd::PlayerCheckpoint* playerCheckpoint) {
	PlayerObject::loadFromCheckpoint(self, playerCheckpoint);

	if (setting().onWaveTrailBugFix) {
		self->placeStreakPoint();
	}
}

void __fastcall PlayerObject::updateJumpH(gd::PlayerObject* self, void*) {
	float delta = 0.f;
	__asm movss delta, xmm1;

	if (!self->m_flyMode || !setting().onShipcopter) return PlayerObject::updateJump(self, delta);

	return updateSwing(self, delta);
}

void __fastcall PlayerObject::activateStreakH(gd::PlayerObject* self) {
	PlayerObject::activateStreak(self);

	if (self->m_dartMode && setting().onNoWaveTrailBehind) {
		self->m_playerStreak->stopStroke();
	}

	if (setting().onTrailBugFix && (self->m_playerScale == 1.f) && (gd::GameManager::sharedState()->m_playerStreak == 2)) {
		self->m_playerStreak->m_fMinSeg = 14.f;
	}
}

void __fastcall PlayerObject::playerDestroyedH(gd::PlayerObject* self, void*, bool p0) {
	PlayerObject::playerDestroyed(self, p0);

	CCSprite* playerExtraSprite = static_cast<CCSprite*>(self->m_playerFrame->getChildByTag(6556));
	CCSprite* vehicleExtraSprite = static_cast<CCSprite*>(self->m_vehicleFrame->getChildByTag(6556));

	if (!setting().onNoDeathEffect) {
		if (playerExtraSprite) {
			playerExtraSprite->runAction(CCFadeTo::create(.05f, 0));
		}
		if (vehicleExtraSprite) {
			vehicleExtraSprite->runAction(CCFadeTo::create(.05f, 0));
		}
	}
}

void __fastcall PlayerObject::resetObjectH(gd::PlayerObject* self) {
	PlayerObject::resetObject(self);

	CCSprite* playerExtraSprite = static_cast<CCSprite*>(self->m_playerFrame->getChildByTag(6556));
	CCSprite* vehicleExtraSprite = static_cast<CCSprite*>(self->m_vehicleFrame->getChildByTag(6556));

	if (playerExtraSprite) {
		playerExtraSprite->setOpacity(255);
	}
	if (vehicleExtraSprite) {
		vehicleExtraSprite->setOpacity(255);
	}
}

void __fastcall PlayerObject::destructorH(gd::PlayerObject* self) {
	PlayerObject::destructor(self);
}

void PlayerObject::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd8ca0), PlayerObject::initH, reinterpret_cast<void**>(&PlayerObject::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdfff0), PlayerObject::updatePlayerFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0140), PlayerObject::updatePlayerShipFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerShipFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0430), PlayerObject::updatePlayerRollFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerRollFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0290), PlayerObject::updatePlayerBirdFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerBirdFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0580), PlayerObject::updatePlayerDartFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerDartFrame));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdfc80), PlayerObject::updateGlowColorH, reinterpret_cast<void**>(&PlayerObject::updateGlowColor));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0f40), PlayerObject::updatePlayerGlowH, reinterpret_cast<void**>(&PlayerObject::updatePlayerGlow));

	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdea20), PlayerObject::toggleFlyModeH, reinterpret_cast<void**>(&PlayerObject::toggleFlyMode));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdf490), PlayerObject::toggleRollModeH, reinterpret_cast<void**>(&PlayerObject::toggleRollMode));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4dec0), PlayerObject::toggleBirdModeH, reinterpret_cast<void**>(&PlayerObject::toggleBirdMode));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdee80), PlayerObject::toggleDartModeH, reinterpret_cast<void**>(&PlayerObject::toggleDartMode));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe12e0), PlayerObject::togglePlayerScaleH, reinterpret_cast<void**>(&PlayerObject::togglePlayerScale));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdad10), PlayerObject::runBallRotation2H, reinterpret_cast<void**>(&PlayerObject::runBallRotation2));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe19c0), PlayerObject::loadFromCheckpointH, reinterpret_cast<void**>(&PlayerObject::loadFromCheckpoint));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xda1a0), PlayerObject::updateJumpH, reinterpret_cast<void**>(&PlayerObject::updateJump));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0d10), PlayerObject::activateStreakH, reinterpret_cast<void**>(&PlayerObject::activateStreak));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xde1c0), PlayerObject::ringJumpH, reinterpret_cast<void**>(&PlayerObject::ringJump));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdc510), PlayerObject::collidedWithObjectH, reinterpret_cast<void**>(&PlayerObject::collidedWithObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xddda0), PlayerObject::playerDestroyedH, reinterpret_cast<void**>(&PlayerObject::playerDestroyed));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd9f80), PlayerObject::resetObjectH, reinterpret_cast<void**>(&PlayerObject::resetObject));

	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd8a30), PlayerObject::destructorH, reinterpret_cast<void**>(&PlayerObject::destructor));
}
#include "PlayerObject.hpp"
#include "PlayLayer.hpp"
#include "Setting.hpp"
#include "Icons.hpp"

int cubeIcon, shipIcon, rollIcon, birdIcon, dartIcon;

void PlayerObject::setCubeIcon(int val) {
	cubeIcon = val;
}

void PlayerObject::setShipIcon(int val) {
	shipIcon = val;
}

void PlayerObject::setRollIcon(int val) {
	rollIcon = val;
}

void PlayerObject::setBirdIcon(int val) {
	birdIcon = val;
}

void PlayerObject::setDartIcon(int val) {
	dartIcon = val;
}

void PlayerObject::newExtraFrame(gd::PlayerObject* playerObject, const char* playerFrame, const char* shipFrame) {
	return;

	//CCSprite* playerExtraSpr = static_cast<CCSprite*>(playerObject->m_playerFrame->getChildByTag(69));
	//CCSprite* vehicleExtraSpr = static_cast<CCSprite*>(playerObject->m_vehicleFrame->getChildByTag(69));
	//auto spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//if (!playerExtraSpr || !vehicleExtraSpr) return;

	//if (spriteFrameCache->spriteFrameByName(playerFrame) && spriteFrameCache->spriteFrameByName(shipFrame)) {
	//	playerExtraSpr->setPosition(playerObject->m_playerFrameSecondary->getPosition());
	//	playerExtraSpr->setDisplayFrame(spriteFrameCache->spriteFrameByName(playerFrame));
	//	playerExtraSpr->setVisible(true);

	//	vehicleExtraSpr->setPosition(playerObject->m_vehicleFrameSecondary->getPosition());
	//	vehicleExtraSpr->setDisplayFrame(spriteFrameCache->spriteFrameByName(shipFrame));
	//	vehicleExtraSpr->setVisible(true);
	//}
	//else {
	//	playerExtraSpr->setVisible(false);
	//	vehicleExtraSpr->setVisible(false);
	//}
}

bool __fastcall PlayerObject::initH(gd::PlayerObject* self, void*, int frameID, int iconType, CCLayer* gameLayer) {
	if (!PlayerObject::init(self, frameID, iconType, gameLayer)) return false;

	auto gm = gd::GameManager::sharedState();

	//int limit = Icons::getCount("player", "001");
	//if (frameID > limit) frameID = limit;

	//CCSprite* playerExtraSprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("player_%02d_glow_001.png", frameID)->getCString());
	//playerExtraSprite->setTag(69);
	//playerExtraSprite->setVisible(false);
	//playerExtraSprite->setZOrder(99);
	//playerExtraSprite->setPosition(self->m_playerFrameSecondary->getPosition());
	//self->m_playerFrame->addChild(playerExtraSprite);

	//CCSprite* vehicleExtraSprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("ship_%02d_glow_001.png", gm->m_playerShip)->getCString());
	//vehicleExtraSprite->setTag(69);
	//vehicleExtraSprite->setVisible(false);
	//vehicleExtraSprite->setZOrder(99);
	//vehicleExtraSprite->setPosition(self->m_vehicleFrameSecondary->getPosition());
	//self->m_vehicleFrame->addChild(vehicleExtraSprite);

	//PlayerObject::newExtraFrame(self, CCString::createWithFormat("player_%02d_extra_001.png", frameID)->getCString(), CCString::createWithFormat("ship_%02d_extra_001.png", gm->m_playerShip)->getCString());

	if (setting().onNoMiniIcon || setting().onMiniCubeIcon) {
		self->updatePlayerFrame(frameID);
	}

	return true;
}

void __fastcall PlayerObject::updatePlayerFrameH(gd::PlayerObject* self, void*, int frameID) {
	auto gm = gd::GameManager::sharedState();

	PlayerObject::updatePlayerFrame(self, frameID);

	//int limit = Icons::getCount("player", "001");
	//if (frameID > limit) frameID = limit;
	//PlayerObject::newExtraFrame(self, CCString::createWithFormat("player_%02d_extra_001.png", frameID)->getCString(), CCString::createWithFormat("ship_%02d_extra_001.png", gm->m_playerShip)->getCString());

	if (setting().onIconRandomizer && setting().onRandomizeCube) return PlayerObject::updatePlayerFrame(self, frameID);

	if (setting().onMiniCubeIcon) return PlayerObject::updatePlayerFrame(self, 0);

	if (setting().onNoMiniIcon) return PlayerObject::updatePlayerFrame(self, gm->m_playerFrame);
}

void __fastcall PlayerObject::updatePlayerShipFrameH(gd::PlayerObject* self, void*, int frameID) {
	auto gm = gd::GameManager::sharedState();

	PlayerObject::updatePlayerShipFrame(self, frameID);

	int limit = Icons::getCount("ship", "001");
	if (frameID > limit) frameID = limit;
	PlayerObject::newExtraFrame(self, CCString::createWithFormat("ship_%02d_extra_001.png", frameID)->getCString(), CCString::createWithFormat("ship_%02d_extra_001.png", gm->m_playerShip)->getCString());
}

void __fastcall PlayerObject::updatePlayerRollFrameH(gd::PlayerObject* self, void*, int frameID) {
	auto gm = gd::GameManager::sharedState();

	PlayerObject::updatePlayerRollFrame(self, frameID);

	//int limit = Icons::getCount("player_ball", "001");
	//if (frameID > limit) frameID = limit;
	//PlayerObject::newExtraFrame(self, CCString::createWithFormat("player_ball_%02d_extra_001.png", frameID)->getCString(), CCString::createWithFormat("ship_%02d_extra_001.png", gm->m_playerShip)->getCString());

	if (setting().onMiniCubeIcon) return PlayerObject::updatePlayerRollFrame(self, 0);

	if (setting().onNoMiniIcon) return PlayerObject::updatePlayerRollFrame(self, gm->m_playerBall);
}

void __fastcall PlayerObject::updatePlayerBirdFrameH(gd::PlayerObject* self, void*, int frameID) {
	auto gm = gd::GameManager::sharedState();

	PlayerObject::updatePlayerBirdFrame(self, frameID);

	int limit = Icons::getCount("bird", "001");
	if (frameID > limit) frameID = limit;
	PlayerObject::newExtraFrame(self, CCString::createWithFormat("bird_%02d_extra_001.png", frameID)->getCString(), CCString::createWithFormat("ship_%02d_extra_001.png", gm->m_playerShip)->getCString());
}

void __fastcall PlayerObject::updatePlayerDartFrameH(gd::PlayerObject* self) {
	auto gm = gd::GameManager::sharedState();

	PlayerObject::updatePlayerDartFrame(self);

	PlayerObject::newExtraFrame(self, CCString::createWithFormat("bird_%02d_extra_001.png", setting().selectedDartIdx)->getCString(), CCString::createWithFormat("ship_%02d_extra_001.png", gm->m_playerShip)->getCString());
}

void __fastcall PlayerObject::toggleFlyModeH(gd::PlayerObject* self, void*, bool p0) {
	PlayerObject::toggleFlyMode(self, p0);

	if (!gd::GameManager::sharedState()->getPlayLayer()) return;

	if (setting().onIconRandomizer && setting().onRandomizeShip && p0) {
		self->updatePlayerShipFrame(shipIcon);
	}
}

void __fastcall PlayerObject::toggleRollModeH(gd::PlayerObject* self, void*, bool p0) {
	PlayerObject::toggleRollMode(self, p0);

	if (!gd::GameManager::sharedState()->getPlayLayer()) return;

	if (setting().onIconRandomizer && setting().onRandomizeBall && p0) {
		self->updatePlayerRollFrame(rollIcon);
	}
}

void __fastcall PlayerObject::toggleBirdModeH(gd::PlayerObject* self, void*, bool p0) {
	PlayerObject::toggleBirdMode(self, p0);

	if (!gd::GameManager::sharedState()->getPlayLayer()) return;

	if (setting().onIconRandomizer && setting().onRandomizeUFO && p0) {
		self->updatePlayerBirdFrame(birdIcon);
	}
}

void __fastcall PlayerObject::toggleDartModeH(gd::PlayerObject* self, void*, bool p0) {
	PlayerObject::toggleDartMode(self, p0);
	if (setting().onNoWaveTrailBehind) {
		self->m_playerStreak->stopStroke();
	}

	if (!gd::GameManager::sharedState()->getPlayLayer()) return;

	if (setting().onIconRandomizer) {
		if (setting().onRandomizeDart && p0) {
			self->updatePlayerDartFrame(dartIcon);
		}
		else if (!self->m_dartMode) {
			self->updatePlayerFrame(cubeIcon);
		}
	}
	else {
		if (!p0 && !self->m_dartMode) {
			self->updatePlayerFrame(gd::GameManager::sharedState()->m_playerFrame);
		}
	}
}

void __fastcall PlayerObject::togglePlayerScaleH(gd::PlayerObject* self, void*, bool p0) {
	PlayerObject::togglePlayerScale(self, p0);

	if (!gd::GameManager::sharedState()->getPlayLayer()) return;

	//if (p0 && (!setting().onNoMiniIcon || setting().onMiniCubeIcon)) {
	//	self->m_playerFrame->getChildByTag(69)->setVisible(false);
	//}

	//self->m_vehicleFrame->getChildByTag(69)->setVisible(true);

	if (setting().onIconRandomizer && setting().onRandomizeCube && !self->m_flyMode && !self->m_rollMode && !self->m_birdMode && !self->m_dartMode) {
		if (!p0) {
			self->updatePlayerFrame(cubeIcon);
		}
		else if (p0 && setting().onNoMiniIcon) {
			self->updatePlayerFrame(cubeIcon);
		}
		else if (p0 && !setting().onNoMiniIcon) {
			self->updatePlayerFrame(0);
		}
	}

	if (setting().onWaveTrailBugFix) {
		self->placeStreakPoint();
	}
}

void __fastcall PlayerObject::runBallRotation2H(gd::PlayerObject* self) {
	if (setting().onBallRotationBugFix) {
		self->stopActionByTag(1);
	}
	PlayerObject::runBallRotation2(self);
}

void __fastcall PlayerObject::collidedWithObjectH(gd::PlayerObject* self, void*, gd::GameObject* object, CCRect rect) {
	PlayerObject::collidedWithObject(self, object, rect);

	std::cout << "collidedWithObject: " << object << std::endl;
	PlayLayer::setDeathObject(object);
}

void __fastcall PlayerObject::loadFromCheckpointH(gd::PlayerObject* self, void*, gd::PlayerCheckpoint* playerCheckpoint) {
	PlayerObject::loadFromCheckpoint(self, playerCheckpoint);

	if (setting().onWaveTrailBugFix) {
		self->placeStreakPoint();
	}
}

void PlayerObject::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd8ca0), PlayerObject::initH, reinterpret_cast<void**>(&PlayerObject::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdfff0), PlayerObject::updatePlayerFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerFrame));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0140), PlayerObject::updatePlayerShipFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerShipFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0430), PlayerObject::updatePlayerRollFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerRollFrame));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0290), PlayerObject::updatePlayerBirdFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerBirdFrame));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0580), PlayerObject::updatePlayerDartFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerDartFrame));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdea20), PlayerObject::toggleFlyModeH, reinterpret_cast<void**>(&PlayerObject::toggleFlyMode));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdf490), PlayerObject::toggleRollModeH, reinterpret_cast<void**>(&PlayerObject::toggleRollMode));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4dec0), PlayerObject::toggleBirdModeH, reinterpret_cast<void**>(&PlayerObject::toggleBirdMode));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdee80), PlayerObject::toggleDartModeH, reinterpret_cast<void**>(&PlayerObject::toggleDartMode));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe12e0), PlayerObject::togglePlayerScaleH, reinterpret_cast<void**>(&PlayerObject::togglePlayerScale));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdad10), PlayerObject::runBallRotation2H, reinterpret_cast<void**>(&PlayerObject::runBallRotation2));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe19c0), PlayerObject::loadFromCheckpointH, reinterpret_cast<void**>(&PlayerObject::loadFromCheckpoint));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdc510), PlayerObject::collidedWithObjectH, reinterpret_cast<void**>(&PlayerObject::collidedWithObject));
}
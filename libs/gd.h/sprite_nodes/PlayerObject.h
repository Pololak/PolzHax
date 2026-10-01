#ifndef __PLAYEROBJECT_H__
#define __PLAYEROBJECT_H__

#include <gd.h>

namespace gd {
	class GameObject;
	enum class GhostType;
	class GhostTrailEffect;
	class HardStreak;
	class CheckpointObject;
	class GameplayDelegate;

	class PlayerObject : public GameObject {
	public:
		double m_slopeStartTime; // 0x328
		bool m_placedStreakPoint; // 0x330
		GameObject* m_collidedObject; // 0x334
		cocos2d::CCDictionary* m_collisionLogTop; // 0x338
		cocos2d::CCDictionary* m_collisionLogBottom; // 0x33c
		int m_lastCollisionIdBottom; // 0x340
		int m_lastCollisionIdTop; // 0x344
		bool m_isRotating; // 0x348
		bool m_checkCollisions; // 0x349 used in checkCollisions and preSlopeCollision idk
		GameObject* m_nextSlope; // 0x34c
		GameObject* m_collidingSlope; // 0x350
		float m_slopeAngle; // 0x354
		int m_collidingWithSlopeId; // 0x358
		bool m_slopeFlipGravityRelated; // 0x35c
		cocos2d::CCArray* m_particleSystems; // 0x360
		int m_iUnused1; // 0x364
		GhostType m_ghostEffect; // 0x368
		GhostTrailEffect* m_ghostTrailEffect; // 0x36c
		cocos2d::CCSprite* m_playerFrame; // 0x370
		cocos2d::CCSprite* m_playerFrameSecondary; // 0x374
		cocos2d::CCSprite* m_playerFrameGlow; // 0x378
		cocos2d::CCSprite* m_vehicleFrame; // 0x37c
		cocos2d::CCSprite* m_vehicleFrameSecondary; // 0x380
		cocos2d::CCSprite* m_vehicleFrameThird; // 0x384
		cocos2d::CCSprite* m_vehicleFrameGlow; // 0x388
		cocos2d::CCSprite* m_unkCCSprite; // 0x38c
		cocos2d::CCMotionStreak* m_playerStreak; // 0x390
		HardStreak* m_hardStreak; // 0x394
		double m_speed; // 0x398
		double m_yStart; // 0x3a0
		double m_gravity; // 0x3a8
		float m_trailingParticleLife; // 0x3b0
		float m_gameModeChangedTime; // 0x3b4
		bool m_bUnknown4; // 0x3b8
		bool m_bUnknown5; // 0x3b9
		bool m_placedJumpCheckpoint; // 0x3ba
		bool m_hasPlayLayer; // 0x3bb
		bool m_bUnknown6; // 0x3bc
		bool m_particlesActive; // 0x3bd
		bool m_flyFallParticlesActive; // 0x3be
		bool m_inBallFall; // 0x3bf
		bool m_bUnknown8; // 0x3c0
		double m_lastCheckpointTime; // 0x3cc
		double m_lastJumpTime; // 0x3d4
		double m_lastFlipTime; // 0x3dc
		bool m_bUnknown9; // 0x3e4
		bool m_bUnknown10; // 0x3e5
		double m_accelerationOrSpeed; // 0x3e8
		double m_snapDistance; // 0x3f0
		GameObject* m_snapToObject; // 0x3f8
		CheckpointObject* m_pendingCheckpoint; // 0x3fc
		int m_onFlyCheckpointTries; // 0x400
		cocos2d::CCPoint m_lastUpdatePos; // 0x404
		cocos2d::CCParticleSystemQuad* m_playerGroundParticles; // 0x40c
		cocos2d::CCParticleSystemQuad* m_trailingParticles; // 0x410
		cocos2d::CCParticleSystemQuad* m_shipClickParticles; // 0x414
		cocos2d::CCParticleSystemQuad* m_vehicleGroundParticles; // 0x418
		cocos2d::CCParticleSystemQuad* m_ufoClickParticles; // 0x41c
		bool m_useLandParticles; // 0x420
		cocos2d::CCParticleSystemQuad* m_landParticles; // 0x424
		cocos2d::CCParticleSystemQuad* m_landParticles2; // 0x428
		float m_landParticlesAngle; // 0x42c
		float m_landParticleRelatedY; // 0x430
		int m_streakType; // 0x434
		float m_slopeRotation; // 0x438
		float m_currentSlopeYVelocity; // 0x43c
		float m_lastPlayerYPosOnSlope; // 0x440 it changes when you're colliding with slope
		bool m_inBoost; // 0x444
		bool m_isCurrentSlopeTop; // 0x445
		float m_updateCollideBottom; // 0x448
		float m_updateCollideTop; // 0x44c
		bool m_hardStreakActive; // 0x450
		bool m_canPlaceCheckpoint; // 0x451
		bool m_tookDamage; // 0x452
		bool m_upKeyDown; // 0x453
		bool m_upKeyPressed; // 0x454
		double m_yVelocity; // 0x458
		bool m_onSlope; // 0x460
		bool m_wasOnSlope; // 0x461
		float m_slopeYVel; // 0x464
		bool m_flyMode; // 0x468
		bool m_birdMode; // 0x469
		bool m_rollMode; // 0x46a
		bool m_dartMode; // 0x46b
		bool m_gravityFlipped; // 0x46c
		bool m_isDead; // 0x46d
		bool m_canJump; // 0x46e
		float m_playerScale; // 0x470
		float m_timeMod; // 0x474
		cocos2d::CCPoint m_lastPos; // 0x478
		cocos2d::CCPoint m_portalPos; // 0x480
		cocos2d::CCLayer* m_gameLayer; // 0x488
		bool m_onGround; // 0x48c
		bool m_isJumping; // 0x48d
		bool m_isLocked; // 0x48e
		bool m_allowRollJump; // 0x48f
		cocos2d::CCPoint m_lastGroundPos; // 0x490
		GameObject* m_touchedRing; // 0x498
		GameObject* m_portalObject; // 0x49c
		bool m_hasJumped; // 0x4a0
		bool m_hasRingJumped; // 0x4a1
		cocos2d::ccColor3B m_glowColor1; // 0x4a2
		cocos2d::ccColor3B m_glowColor2; // 0x4a5
		cocos2d::CCPoint m_realPlayerPos; // 0x4a8
		bool m_isSecondPlayer; // 0x4b0
		bool m_dualMode; // 0x4b1
		double m_clkTimer; // 0x4b8
		bool m_disableEffects; // 0x4c0
		GameplayDelegate* m_gameDelegate; // 0x4c4
		float m_audioScale; // 0x4c8
		float m_groundHeight; // 0x4cc

		static PlayerObject* create(int player, int ship, cocos2d::CCLayer* layer) {
			auto ret = reinterpret_cast<PlayerObject * (__fastcall*)(int, int, cocos2d::CCLayer*)>(base + 0xd8bf0)(player, ship, layer);

			__asm add esp, 0x4

			return ret;
		}

		virtual void setPosition(cocos2d::CCPoint const& position) {
			return reinterpret_cast<void(__thiscall*)(PlayerObject*, cocos2d::CCPoint const&)>(base + 0xdf8c0)(this, position);
		}

		void flipGravity(bool flip, bool effect) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, bool, bool)>(base + 0xde660)(this, flip, effect);
		}

		void updatePlayerFrame(int frameID) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, int)>(base + 0xdfff0)(this, frameID);
		}

		void updatePlayerShipFrame(int frameID) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, int)>(base + 0xe0140)(this, frameID);
		}

		void updatePlayerRollFrame(int frameID) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, int)>(base + 0xe0430)(this, frameID);
		}

		void updatePlayerBirdFrame(int frameID) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, int)>(base + 0xe0290)(this, frameID);
		}

		void updatePlayerDartFrame(int frameID) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, int)>(base + 0xe0580)(this, frameID);
		}

		OBB2D* getOrientedBox() {
			return reinterpret_cast<OBB2D * (__fastcall*)(PlayerObject*)>(base + 0xe1260)(this);
		}

		virtual void setColor(cocos2d::_ccColor3B const& color) {
			return reinterpret_cast<void(__thiscall*)(PlayerObject*, cocos2d::_ccColor3B const&)>(base + 0xdfc40)(this, color);
		}

		void setSecondColor(cocos2d::_ccColor3B const& color) {
			this->m_playerFrameSecondary->setColor(color);
			this->m_vehicleFrameSecondary->setColor(color);
		}

		void updateGlowColor() {
			reinterpret_cast<void(__thiscall*)(PlayerObject*)>(base + 0xdfc80)(this);
		}

		void placeStreakPoint() {
			reinterpret_cast<void(__fastcall*)(PlayerObject*)>(base + 0xe1270)(this);
		}

		void pushButton(int p0) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, int)>(base + 0xde080)(this, p0);
		}

		void releaseButton(int p0) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, int)>(base + 0xde150)(this, p0);
		}

		void toggleFlyMode(bool p0) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, bool)>(base + 0xdea20)(this, p0);
		}

		void toggleRollMode(bool p0) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, bool)>(base + 0xdf490)(this, p0);
		}

		void toggleBirdMode(bool p0) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, bool)>(base + 0xdec10)(this, p0);
		}

		void toggleDartMode(bool p0) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, bool)>(base + 0xdee80)(this, p0);
		}

		void runRotateAction(bool ground) {
			reinterpret_cast<void(__fastcall*)(PlayerObject*, bool)>(base + 0xdab30)(this, ground);
		}
	};
}

#endif
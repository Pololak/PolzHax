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
		double m_slopeTime; // 0x328
		PAD(24)
		bool m_isRotating; // 0x348
		PAD(3)
		GameObject* m_nextSlope; // 0x34c
		GameObject* m_collidingSlope; // 0x350
		PAD(12)
		cocos2d::CCArray* m_particleSystems; // 0x360
		PAD(4)
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
		PAD(10)
		bool m_placedJumpCheckpoint; // 0x3ba
		bool m_hasPlayLayer; // 0x3bb
		PAD(1)
		bool m_particlesActive; // 0x3bd
		bool m_flyFallParticlesActive; // 0x3be
		bool m_inBallFall; // 0x3bf
		PAD(12)
		double m_lastJumpTime; // 0x3cc
		PAD(26)
		GameObject* m_snapToObject; // 0x3f8
		CheckpointObject* m_pendingCheckpoint; // 0x3fc
		PAD(4)
		cocos2d::CCPoint m_lastUpdatePos; // 0x404
		cocos2d::CCParticleSystemQuad* m_dragParticle; // 0x40c
		cocos2d::CCParticleSystemQuad* m_birdDragParticle; // 0x410
		cocos2d::CCParticleSystemQuad* m_dragParticle2; // 0x414
		cocos2d::CCParticleSystemQuad* m_shipDragParticle; // 0x418
		cocos2d::CCParticleSystemQuad* m_burstParticle; // 0x41c
		PAD(4)
		cocos2d::CCParticleSystemQuad* m_landParticle; // 0x424
		cocos2d::CCParticleSystemQuad* m_landParticle2; // 0x424
		PAD(8)
		int m_streakType; // 0x434
		PAD(12)
		bool m_inBoost; // 0x444
		PAD(12)
		bool m_hardStreakActive; // 0x450
		PAD(1)
		bool m_tookDamage; // 0x452
		bool m_upKeyDown; // 0x453
		bool m_upKeyPressed; // 0x454
		double m_yVelocity; // 0x458
		bool m_onSlope; // 0x460
		bool m_wasOnSlope; // 0x461
		PAD(2)
		float m_slopeYVel; // 0x464
		bool m_flyMode; // 0x468
		bool m_birdMode; // 0x469
		bool m_rollMode; // 0x46a
		bool m_dartMode; // 0x46b
		bool m_gravityFlipped; // 0x46c
		bool m_isDead; // 0x46d
		bool m_canJump; // 0x46e
		PAD(1)
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
		PAD(2)
		cocos2d::ccColor3B m_glowColor1; // 0x4a4
		cocos2d::ccColor3B m_glowColor2; // 0x4a8
		cocos2d::CCPoint m_realPlayerPos; // 0x4ac
		bool m_isSecondPlayer; // 0x4b4
		bool m_dualMode; // 0x4b5
		PAD(2)
		double m_clkTimer; // 0x4b8
		bool m_disableEffects; // 0x4c0
		PAD(3)
		GameplayDelegate* m_gameDelegate; // 0x4c4
		float m_audioScale; // 0x4c8
		float m_groundHeight; // 0x4cc

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
	};
}

#endif
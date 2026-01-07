#ifndef __PLAYEROBJECT_H__
#define __PLAYEROBJECT_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class GameObject;
	class HardStreak;
	class PlayLayer;
	class OBB2D;
	class GhostTrailEffect;
	enum class GhostType {

	};
	class CheckpointObject;
	class PlayerCheckpoint : public cocos2d::CCNode {};
	class GameplayDelegate;

	enum class PlayerButton {
		Jump = 1,
	};

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

		virtual void setVisible(bool visible) {
			return reinterpret_cast<void(__thiscall*)(PlayerObject*, bool)>(
				base + 0x1FA860
				)(this, visible);
		}
		virtual void addAllParticles() {
			return reinterpret_cast<void(__thiscall*)(PlayerObject*)>(
				base + 0xd7db0
				)(this);
		}
		bool collidedWithObject(float fl, gd::GameObject* obj, cocos2d::CCRect* rect) {
			return reinterpret_cast<bool(__thiscall*)(PlayerObject*, float fl, gd::GameObject* obj, cocos2d::CCRect*)>(
				base + 0xdaa60
				)(this, fl, obj, rect);
		}
		void pushButton(PlayerButton p0) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, PlayerButton)>(base + 0xde080)(this, p0);
		}
		void releaseButton(PlayerButton p0) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, PlayerButton)>(base + 0xde150)(this, p0);
		}

		bool isFly() {
			return from<bool>(this, 0x468);
		}

		bool isBird() {
			return from<bool>(this, 0x469);
		}

		bool isRoll() {
			return from<bool>(this, 0x46a);
		}

		bool isDart() {
			return from<bool>(this, 0x46b);
		}
		
		//bool collidedWithObject(float fl, gd::GameObject* obj)
		//{
		//	int myRect = 0;
		//	auto rect = obj->getObjectRect(0);
		//	return collidedWithObject(fl, obj, rect);
		//}

		static auto create(int idk1, int idk2, cocos2d::CCLayer* layer) {
			return reinterpret_cast<PlayerObject * (__fastcall*)(int, int, cocos2d::CCLayer*)>(
				base + 0xd8bf0)(idk1, idk2, layer);
		}

		auto getOBB2D() {
			return from<OBB2D*>(this, 0x1D0);
		}
		auto getOrientedBox() {
			return reinterpret_cast<OBB2D*(__fastcall*)(PlayerObject*)>(base + 0xe1260)(this);
		}
		auto getFirstColor() {
			return from<cocos2d::CCSprite*>(this, 0x370)->getColor();
		}
		auto setFirstColor(cocos2d::ccColor3B color) {
			from<cocos2d::CCSprite*>(this, 0x370)->setColor(color);
		}

		auto getSecondColor() {
			return from<cocos2d::CCSprite*>(this, 0x374)->getColor();
		}
		auto setSecondColor(cocos2d::ccColor3B color) {
			from<cocos2d::CCSprite*>(this, 0x374)->setColor(color);
		}

		auto getGlow() {
			return from<cocos2d::CCSprite*>(this, 0x378);
		}
		auto getGlowColor() {
			return from<cocos2d::CCSprite*>(this, 0x378)->getColor();
		}
		auto setGlowColor(cocos2d::ccColor3B color) {
			from<cocos2d::CCSprite*>(this, 0x378)->setColor(color);
		}

		auto& position() {
			return from<cocos2d::CCPoint>(this, 0x4a8);
		}

		/*auto getColor3() {
			return from<cocos2d::CCSprite*>(this, 0x37C)->getColor();
		}
		auto setColor3(cocos2d::ccColor3B color) {
			from<cocos2d::CCSprite*>(this, 0x37C)->setColor(color);
		}*/

		auto getColorSecondVehicle() {
			return from<cocos2d::CCSprite*>(this, 0x380)->getColor();
		}
		auto setColorSecondVehicle(cocos2d::ccColor3B color) {
			from<cocos2d::CCSprite*>(this, 0x380)->setColor(color);
		}

		auto getRealPlayerPosX() {
			return from<float>(this, 0x234);
		}

		auto getRealPlayerPosY() {
			return from<float>(this, 0x238);
		}

		auto getVelocity() {
			return from<double>(this, 0x45c);
		}

		auto getPlayerSize() {
			return from<float>(this, 0x25c);
		}

		auto getPlayerSpeed() {
			return from<float>(this, 0x398);
		}
		/*auto getColor5() {
			return from<cocos2d::CCSprite*>(this, 0x384)->getColor();
		}
		auto setColor5(cocos2d::ccColor3B color) {
			from<cocos2d::CCSprite*>(this, 0x384)->setColor(color);
		}*/

		cocos2d::CCPoint getPoint1() {
			auto f1 = from<float>(this, 0x234);
			auto f2 = from<float>(this, 0x238);
			cocos2d::CCPoint point = { f1, f2 };
			return point;
		}

		auto getGlowVehicle() {
			return from<cocos2d::CCSprite*>(this, 0x388);
		}
		auto getColorGlowVehicle() {
			return from<cocos2d::CCSprite*>(this, 0x388)->getColor();
		}
		auto setColorGlowVehicle(cocos2d::ccColor3B color) {
			from<cocos2d::CCSprite*>(this, 0x388)->setColor(color);
		}

		/*auto getColor7() {
			return from<cocos2d::CCSprite*>(this, 0x38C)->getColor();
		}
		auto setColor7(cocos2d::ccColor3B color) {
			from<cocos2d::CCSprite*>(this, 0x38C)->setColor(color);
		}*/

		auto getWaveTrailColor() {
			return from<cocos2d::CCMotionStreak*>(this, 0x394)->getColor();
		}
		auto setWaveTrailColor(cocos2d::ccColor3B color) {
			from<cocos2d::CCMotionStreak*>(this, 0x394)->setColor(color);
		}

		float getAudioScale() {
			return from<float>(this, 0x4C8);
		}

		void setAudioScale(float scale) {
			from<float>(this, 0x4C8) = scale;
		}

		bool getIsDead() {
			return from<bool>(this, 0x46D);
		}

		auto getCubeSprite() {
			return from<CCSprite*>(this, 0x370);
		}

		HardStreak* waveTrail() {
			return from<HardStreak*>(this, 0x394);
		}

		auto getYVelocity() {
			return from<double>(this, 0x458);
		}

		void setYVelocity(double yVel) {
			from<double>(this, 0x458) = yVel;
		}

		auto getRealRotation() {
			return from<float>(this, 0x18);
		}

		void setRealRotation(float rot) {
			from<float>(this, 0x18) = rot;
		}

		cocos2d::CCMotionStreak* motionStreak() {
			return from<cocos2d::CCMotionStreak*>(this, 0x390);
		}

		void flipGravity(bool p0, bool p1) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, bool, bool)>(base + 0xde660)(this, p0, p1);
		}
	};
}

// addAllParticles(PlayerObject* this) base + 0xd7db0
// updateGlowColor(PlayerObject* this) base + 0xde1d0
// playerDestroyed(PlayerObject* this, bool/char ???) base + 0xdc2f0
// PlayLayer::getOtherPlayer(PlayLayer* this, PlayerObject* param_1) base + 0xee170
// 
#endif
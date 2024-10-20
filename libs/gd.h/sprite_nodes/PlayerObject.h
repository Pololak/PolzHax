#ifndef __PLAYEROBJECT_H__
#define __PLAYEROBJECT_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class GameObject;
	class HardStreak;
	class PlayLayer;
	class OBB2D;

	enum class PlayerButton {
		Jump = 1,
	};

	class PlayerObject : public GameObject {
	public:

		HardStreak* m_waveTrail;
		float m_yVelocity;
		bool m_isDead;

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
				base + 0xd71b0)(idk1, idk2, layer);
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
	};
}

// addAllParticles(PlayerObject* this) base + 0xd7db0
// updateGlowColor(PlayerObject* this) base + 0xde1d0
// playerDestroyed(PlayerObject* this, bool/char ???) base + 0xdc2f0
// PlayLayer::getOtherPlayer(PlayLayer* this, PlayerObject* param_1) base + 0xee170
// 
#endif
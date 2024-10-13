#ifndef __PLAYLAYER_H__
#define __PLAYLAYER_H__

#include <gd.h>

namespace gd {

	class GJBaseGameLayer;

	class UILayer : public cocos2d::CCLayerColor {
	public:
		PAD(8)
			cocos2d::CCMenu* m_pCheckPointMenu;		// 0x1a0

		void onCheck(CCObject* pSender) {
			reinterpret_cast<void(__thiscall*)(UILayer*, CCObject*)>(
				base + 0x25fb60
				)(this, pSender);
		}

		void onDeleteCheck(CCObject* pSender) {
			reinterpret_cast<void(__thiscall*)(UILayer*, CCObject*)>(
				base + 0x25fc90
				)(this, pSender);
		}

		void onPause(CCObject* pSender) {
			reinterpret_cast<void(__thiscall*)(UILayer*, CCObject*)>(
				base + 0x25fad0
				)(this, pSender);
		}
	};

	class PlayLayer : public GJBaseGameLayer {
	public:

		StartPosObject* m_startPos; // 0x32C

		void resetLevel() {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0xf1f20)(this); //0xf1f20 - GHS 1.92
		}
		void destroyPlayer(PlayerObject* selfig) {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*, PlayerObject*)>(base + 0xf04a0)(this, selfig); //ee990 1.91
		}
		void resume() {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0xf39b0)(this); //f1e10 1.91
		}
		void onQuit() {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0xf3b80)(this); //f1fe0 1.91
		}

		void updateCustomColorBlend(int colorID, bool isBlend) {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*, int, bool)>(base + 0xecf30)(this, colorID, isBlend);
		}

		auto layer() {
			return from<cocos2d::CCLayer*>(this, 0x2D4);
		}

		auto getFirstVisibleSection() {
			return from<int>(this, 0x1BC);
		}
		auto getLastVisibleSection() {
			return from<int>(this, 0x1C0);
		}

		auto getSections() {
			return from<cocos2d::CCArray*>(this, 0x170);
		}

		auto getObjects() {
			return from<cocos2d::CCArray*>(this, 0x184);
		}

		auto getLevelSettings() {
			return from <gd::LevelSettingsObject*>(this, 0x14C);
		}

		auto getStartPosObject() {
			return from <gd::StartPosObject*>(this, 0x148);
		}
		void setStartPosObject(gd::StartPosObject* startPos) {
			from <gd::StartPosObject*>(this, 0x148) = startPos;
		}

		auto getPlayerStartPosition() {
			return from<cocos2d::CCPoint>(this, 0x2E0);
		}
		void setPlayerStartPosition(cocos2d::CCPoint playerStartPosition) {
			from<cocos2d::CCPoint>(this, 0x2E0) = playerStartPosition;
		}


		/*auto getScene() {
			return from<cocos2d::CCScene*>(this, 0xa8);
		}*/
		// TODO: make these actual members
		auto player1() {
			return from<PlayerObject*>(this, 0x2a4); //2a4
		}
		auto player2() {
			return from<PlayerObject*>(this, 0x2a8); //2a8
		}
		auto levelLength() {
			return from<float>(this, 0x1d0); //1d0
		}
		auto attemptsLabel() {
			return from<cocos2d::CCLabelBMFont*>(this, 0x1d8); //1d8
		}
		auto attemptsCount() {
			return from<int>(this, 0x2e8); 
		}
		auto jumpsCount() {
			return from<int>(this, 0x2ec); 
		}
		auto getStartPos() {
			return from<cocos2d::CCPoint*>(this, 0x2e0);
		}
		auto getLastRunPercent() {
			return from<int>(this, 0x2fc);
		}
		auto getClkTimer() {
			return from<float>(this, 0x2f4);
		}
		void setAttempts(int atts) {
			from<int>(this, 0x2e8) = atts; 
		}
		bool getPracticeMode() {
			return from<bool>(this, 0x2b9);
		}
		bool hasLevelCompleteMenu() {
			return from<bool>(this, 0x2f8);
		}
		bool hasCompletedLevel() {
			return from<bool>(this, 0x2f9);
		}
		bool isDead() {
			return from<bool>(this, 0x1b4);
		}
		auto getAudioEffectsLayer() {
			return from<gd::AudioEffectsLayer*>(this, 0x198);
		}
		auto getBackgroundSprite() {
			return from<cocos2d::CCSprite*>(this, 0x164);
		}
		auto getGroundBottom() {
			return from<cocos2d::CCLayer*>(this, 0x1a4);
		}
		auto getGroundTop() {
			return from<cocos2d::CCLayer*>(this, 0x1a8);
		}
	};
}

#endif
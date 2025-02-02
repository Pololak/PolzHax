#ifndef __PLAYLAYER_H__
#define __PLAYLAYER_H__

#include <gd.h>

namespace gd {

	class GJBaseGameLayer;
	class GJGameLevel;
	class EndPortalObject {};

	class GJGroundLayer : public cocos2d::CCLayer {
	public:
		static GJGroundLayer* create(int p0) {
			return reinterpret_cast<GJGroundLayer * (__fastcall*)(int)>(base + 0x81140)(p0);
		}

		void hideShadows(bool hide) {
			if (hide) {
				reinterpret_cast<cocos2d::CCSprite*>(this->getChildren()->objectAtIndex(2))->setVisible(0);
				reinterpret_cast<cocos2d::CCSprite*>(this->getChildren()->objectAtIndex(3))->setVisible(0);
			}
		}

		auto groundSprite() {
			return from<cocos2d::CCSprite*>(this, 0x118);
		}

		auto lineSprite() {
			return from<cocos2d::CCSprite*>(this, 0x120);
		}
	};

	class UILayer : public cocos2d::CCLayerColor {
	public:
		PAD(0x8)
			cocos2d::CCMenu* m_checkpointMenu;
			CCMenuItemSpriteExtra* m_pauseBtn;

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

	class PlayLayer : public cocos2d::CCLayer {
	public:
		PAD(12)
		cocos2d::CCDrawNode* m_drawNode; // 0x124
		PAD(32)
		StartPosObject* m_startPosObject; // 0x148
		LevelSettingsObject* m_levelSettings; // 0x14c
		EndPortalObject* m_endPortalObject; // 0x150
		cocos2d::CCArray* m_checkpoints; // 0x154
		cocos2d::CCArray* m_activatedSpeedObjects; // 0x158
		cocos2d::CCArray* m_levelSpeedObjects; // 0x15c
		int m_activeEnterEffect; // 0x160
		cocos2d::CCSprite* m_backgroundSprite; // 0x164
		PAD(0x8)
		cocos2d::CCArray* m_levelSections; // 0x170
		PAD(0x4)
		cocos2d::CCArray* m_activeObjects; // 0x178
		PAD(0x8)
		cocos2d::CCArray* m_objects; // 0x184
		cocos2d::CCArray* m_stateObjects; // 0x188
		cocos2d::CCParticleSystemQuad* m_glitterParticleSystem; // 0x18c
		PAD(0x4)
		cocos2d::CCArray* m_effectObjects; // 0x194
		AudioEffectsLayer* m_audioEffectsLayer; // 0x198
		PAD(0x8)
		GJGroundLayer* m_bottomGround; // 0x1a4
		GJGroundLayer* m_topGround; // 0x1a8
		PAD(0x10)
		int m_firstVisibleSection; // 0x1bc
		int m_lastVisibleSection; // 0x1c0
		PAD(0x8)
		float m_levelLength; // 0x1d0
		PAD(0x4)
		cocos2d::CCLabelBMFont* m_attemptsLabel; // 0x1d8
		PAD(0x18)
		cocos2d::CCDictionary* m_particlesDictionary; // 0x1f4
		PAD(0x4)
		cocos2d::CCArray* m_particles; // 0x1fc
		cocos2d::CCNode* m_backgroundFlash; // 0x200 // bg white flash when entering sizing portals
		cocos2d::CCSprite* m_lineColorRef; // 0x204
		cocos2d::CCSprite* m_objColorRef; // 0x208
		cocos2d::CCSprite* m_gColorRef; // 0x20c
		cocos2d::CCSprite* m_3DLineColorRef; // 0x210
		cocos2d::CCSprite* m_custom01ColorRef; // 0x214
		cocos2d::CCSprite* m_custom02ColorRef; // 0x218
		cocos2d::CCSprite* m_custom03ColorRef; // 0x21c
		cocos2d::CCSprite* m_custom04ColorRef; // 0x220
		cocos2d::CCSprite* m_sliderGroove; // 0x224
		cocos2d::CCSprite* m_sliderBar; // 0x228
		PAD(0x44)
		double m_levelTime; // 0x270
		bool m_needsReorderColor01; // 0x278
		bool m_needsReorderColor02; // 0x279
		bool m_needsReorderColor03; // 0x27a
		bool m_needsReorderColor04; // 0x27b
		bool m_needsReorderColorDL; // 0x27c
		PAD(0x20)
		UILayer* m_uiLayer; // 0x2a0
		PlayerObject* m_player; // 0x2a4
		PlayerObject* m_player2; // 0x2a8
		GJGameLevel* m_level; // 0x2ac
		cocos2d::CCPoint m_cameraPos; // 0x2b0
		bool m_testMode; // 0x2b8
		bool m_practiceMode; // 0x2b9
		bool m_isResetting; // 0x2ba
		cocos2d::CCSpriteBatchNode* m_batchNode; // 0x2bc
		cocos2d::CCSpriteBatchNode* m_batchNodeBottom; // 0x2c0
		cocos2d::CCSpriteBatchNode* m_batchNodeAdd; // 0x2c4
		cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom; // 0x2c8
		cocos2d::CCSpriteBatchNode* m_batchNodePlayer; // 0x2cc
		cocos2d::CCSpriteBatchNode* m_batchNodeAddPlayer; // 0x2d0
		cocos2d::CCLayer* m_gameLayer; // 0x2d4
		cocos2d::CCArray* m_bigActionContainer; // 0x2d8
		bool m_cleanReset; // 0x2dc
		cocos2d::CCPoint m_playerStartPos; // 0x2e0
		int m_attempts; // 0x2e8
		int m_jumps; // 0x2ec
		bool m_didJump; // 0x2f0
		float m_clkTime; // 0x2f4
		bool m_showingEndLayer; // 0x2f8
		bool m_endTriggered; // 0x2f9
		bool m_resetQueued; // 0x2fa
		int m_lastRunPercent; // 0x2fc
		bool m_didAwardStars; // 0x300
		ColorAction* m_activeBGColorAction; // 0x304
		ColorAction* m_activeGColorAction; // 0x308
		ColorAction* m_activeLineColorAction; // 0x30c
		ColorAction* m_activeObjColorAction; // 0x310
		ColorAction* m_active3DLineColorAction; // 0x314
		ColorAction* m_activeColor01ColorAction; // 0x318
		ColorAction* m_activeColor02ColorAction; // 0x31c
		ColorAction* m_activeColor03ColorAction; // 0x320
		ColorAction* m_activeColor04ColorAction; // 0x324
		bool m_shouldRestartAfterStopping; // 0x328
		bool m_customColor01Blend; // 0x329
		bool m_customColor02Blend; // 0x32a
		bool m_customColor03Blend; // 0x32b
		bool m_customColor04Blend; // 0x32c
		bool m_customColorDLBlend; // 0x32d

		static PlayLayer* create(GJGameLevel* lvl) {
			return reinterpret_cast<PlayLayer * (__fastcall*)(GJGameLevel*)>(
				base + 0xe3530
				)(lvl);
		}

		void resetLevel() {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0xf1f20)(this); //0xf1f20 - GHS 1.92
		}
		void destroyPlayer(PlayerObject* selfig) { // self ig, nice job
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

		void updateVisibility(float dt) {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*, float)>(base + 0xeb3f0)(this, dt);
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
		GJGameLevel* getGameLevel() {
			return from<GJGameLevel*>(this, 0x2ac);
		}

		UILayer* getUILayer() {
			return from<UILayer*>(this, 0x2a0);
		}
		auto playerGlowNode() {
			return from<cocos2d::CCSpriteBatchNode*>(this, 0x2d0);
		}
		void pushButton(int p0, bool push) {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*, int, bool)>(base + 0xf0a00)(this, p0, push);
		}
		void releaseButton(int p0, bool push) {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*, int, bool)>(base + 0xf0af0)(this, p0, push);
		}
		void showEndLayer() {
			reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0xe6a80)(this);
		}
		void pickupItem(GameObject* p0) {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*, GameObject*)>(base + 0xee080)(this, p0);
		}
		void togglePracticeMode(bool p0) {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*, bool)>(base + 0xf3610)(this, p0);
		}
		cocos2d::CCSpriteBatchNode* getPlayerBatchNode() {
			return from<cocos2d::CCSpriteBatchNode*>(this, 0x2cc);
		}
		cocos2d::CCArray* getCheckpoints() {
			return from<cocos2d::CCArray*>(this, 0x154);
		}
	};
}

#endif
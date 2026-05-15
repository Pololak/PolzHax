#ifndef __PLAYLAYER_H__
#define __PLAYLAYER_H__

#include <gd.h>

namespace gd {
	class CCCircleWaveDelegate;
	class GameplayDelegate;
	class StartPosObject;
	class LevelSettingsObject;
	class EndPortalObject;
	class AudioEffectsLayer;
	class GJGroundLayer;
	class GameObject;
	class UILayer;
	class PlayerObject;
	class GJGameLevel;
	class ColorAction;

	class PlayLayer : public cocos2d::CCLayer, CCCircleWaveDelegate, GameplayDelegate {
	public:
		bool m_drawDebug; // 0x120
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
		PAD(0x4)
		cocos2d::CCArray* m_spawnObjects; // 0x180
		cocos2d::CCArray* m_objects; // 0x184
		cocos2d::CCArray* m_stateObjects; // 0x188
		cocos2d::CCParticleSystemQuad* m_glitterParticleSystem; // 0x18c
		PAD(0x4)
		cocos2d::CCArray* m_effectObjects; // 0x194
		AudioEffectsLayer* m_audioEffectsLayer; // 0x198
		PAD(0x8)
		GJGroundLayer* m_bottomGround; // 0x1a4
		GJGroundLayer* m_topGround; // 0x1a8
		PAD(0x8)
		bool m_isDead; // 0x1b4
		PAD(0x1)
		bool m_cameraMovingX; // 0x1b6
		bool m_cameraMovingY; // 0x1b7
		PAD(0x4)
		int m_firstVisibleSection; // 0x1bc
		int m_lastVisibleSection; // 0x1c0
		PAD(0x8)
		bool m_groundMoving; // 0x1cc
		float m_levelLength; // 0x1d0
		float m_realLevelLength; // 0x1d4
		cocos2d::CCLabelBMFont* m_attemptsLabel; // 0x1d8
		PAD(24)
		cocos2d::CCDictionary* m_particlesDict; // 0x1f4
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
		PAD(40)
		std::string m_replayString; // 0x254
		cocos2d::CCArray* m_replayActions; // 0x26c
		double m_levelTime; // 0x270
		bool m_needsReorderColor01; // 0x278
		bool m_needsReorderColor02; // 0x279
		bool m_needsReorderColor03; // 0x27a
		bool m_needsReorderColor04; // 0x27b
		bool m_needsReorderColor3D; // 0x27c
		PAD(0x3)
		PAD(0x8)
		bool m_inCameraFlip; // 0x288
		bool m_meteringEnabled; // 0x289
		bool m_playbackMode; // 0x28a
		GameObject* m_cameraPortal; // 0x28c
		GameObject* m_dualModeCamera; // 0x290
		bool m_isFlipped; // 0x294
		float m_flipValue; // 0x298
		bool m_dualMode; // 0x29c
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
		float m_clkTimer; // 0x2f4
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

		void resetLevel() {
			reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0xf1f20)(this);
		}

		void togglePracticeMode(bool practice) {
			reinterpret_cast<void(__thiscall*)(PlayLayer*, bool)>(base + 0xf3610)(this, practice);
		}

		void pickupItem(GameObject* obj) {
			reinterpret_cast<void(__thiscall*)(PlayLayer*, GameObject*)>(base + 0xee080)(this, obj);
		}

		void destroyPlayer(PlayerObject* player) {
			reinterpret_cast<void(__thiscall*)(PlayLayer*, PlayerObject*)>(base + 0xf04a0)(this, player);
		}

		void onQuit() {
			reinterpret_cast<void(__fastcall*)(PlayLayer*)>(base + 0xf3b80)(this);
		}

		void updateCustomColorBlend(int channel, bool blend) {
			switch (channel) {
			case 3:
				if (this->m_customColor01Blend != blend) {
					this->m_customColor01Blend = blend;
					this->m_needsReorderColor01 = true;
				}
				break;
			case 4:
				if (this->m_customColor02Blend != blend) {
					this->m_customColor02Blend = blend;
					this->m_needsReorderColor02 = true;
				}
				break;
			case 6:
				if (this->m_customColor03Blend != blend) {
					this->m_customColor03Blend = blend;
					this->m_needsReorderColor03 = true;
				}
				break;
			case 7:
				if (this->m_customColor04Blend != blend) {
					this->m_customColor04Blend = blend;
					this->m_needsReorderColor04 = true;
				}
				break;
			case 8:
				if (this->m_customColorDLBlend != blend) {
					this->m_customColorDLBlend = blend;
					this->m_needsReorderColor3D = true;
				}
				break;
			}
		}

		void setStartPosObject(StartPosObject* startPos) {
			if (startPos != m_startPosObject) {
				if (startPos) {
					startPos->retain();
				}
				if (m_startPosObject) {
					m_startPosObject->release();
				}
				m_startPosObject = startPos;
			}
		}

		void pushButton(int p0, bool p1) {
			reinterpret_cast<void(__thiscall*)(PlayLayer*, int, bool)>(base + 0xf0a00)(this, p0, p1);
		}

		void releaseButton(int p0, bool p1) {
			reinterpret_cast<void(__thiscall*)(PlayLayer*, int, bool)>(base + 0xf0af0)(this, p0, p1);
		}

		void delayedResetLevel() {
			reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0xf1f10)(this);
		}

		void updateColor(cocos2d::ccColor3B const& toColor, float duration, int colorID) {
			__asm movss xmm2, duration

			reinterpret_cast<void(__thiscall*)(PlayLayer*, cocos2d::ccColor3B const&, int)>(base + 0xeca90)(this, toColor, colorID);
		}

		cocos2d::ccColor3B getBGColor() {
			return this->m_backgroundSprite->getColor();
		}

		cocos2d::ccColor3B getGColor() {
			return this->m_bottomGround->m_groundSprite->getColor();
		}

		cocos2d::ccColor3B getLineColor() {
			return this->m_bottomGround->m_line->getColor();
		}

		cocos2d::ccColor3B getObjColor() {
			return this->m_objColorRef->getColor();
		}

		cocos2d::ccColor3B getCustom01Color() {
			return this->m_custom01ColorRef->getColor();
		}

		cocos2d::ccColor3B getCustom02Color() {
			return this->m_custom02ColorRef->getColor();
		}

		cocos2d::ccColor3B getCustom03Color() {
			return this->m_custom03ColorRef->getColor();
		}

		cocos2d::ccColor3B getCustom04Color() {
			return this->m_custom04ColorRef->getColor();
		}

		cocos2d::ccColor3B get3DLineColor() {
			return this->m_3DLineColorRef->getColor();
		}
	};
}

#endif
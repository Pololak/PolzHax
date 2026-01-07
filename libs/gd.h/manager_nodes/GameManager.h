#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include <gd.h>

namespace gd {
	enum IconType {
		Cube = 0,
		Ship = 1,
		Ball = 2,
		UFO = 3,
		Wave = 4,
		Special = 99
	};
	
	enum UnlockType {
		kUnlockTypeUnknown = 0,
		kUnlockTypeCube = 1,
		kUnlockTypeColor1 = 2,
		kUnlockTypeColor2 = 3,
		kUnlockTypeShip = 4,
		kUnlockTypeBall = 5,
		kUnlockTypeUfo = 6,
		kUnlockTypeWave = 7,
		kUnlockTypeRobot = 8,
		kUnlockTypeSpider = 9,
		kUnlockTypeSpecial = 10,
		kUnlockTypeDeathEffect = 11,
		kUnlockTypeGlow = 12,
	};

	enum LastGameScene {

	};

	class PlayLayer;
	class LevelSelectLayer {}; // todo

	class GameManager : public GManager {
	public:
		bool m_switchMode; // 0x104
		bool m_toFullScreen; // 0x105
		bool m_reloading; // 0x106
		PAD(5)
		cocos2d::CCDictionary* m_valueKeeper; // 0x10c
		double m_adTimer; // 0x110
		double m_adCache; // 0x118
		PAD(4)
		bool m_loaded; // 0x128
		bool m_didSyncAchiviements; // 0x129
		PAD(2)
		gd::string m_editorCopyString; // 0x12c
		PAD(4)
		PlayLayer* m_playLayer; // 0x144
		LevelSelectLayer* m_levelSelectLayer; // 0x148
		bool m_inMenuLayer; // 0x14c
		PAD(3)
		void* m_premiumPopup; // 0x150 // unused // used in polzhax
		bool m_firstSetup; // 0x154
		bool m_showedMenu; // 0x155
		PAD(2)
		gd::string m_playerUDID; // 0x158
		gd::string m_playerName; // 0x170
		int m_playerUserID; // 0x188
		bool m_playerScoreValid; // 0x18c
		PAD(3)
		float m_bgVolume; // 0x190
		float m_sfxVolume; // 0x194
		int m_timeOffset; // 0x198
		bool m_ratedGame; // 0x19c
		bool m_clickedFacebook; // 0x19d
		bool m_clickedTwitter; // 0x19e
		bool m_clickedYouTube; // 0x19f
		bool m_didPauseBGMusic; // 0x1a0
		bool m_wasHigh; // 0x1a1
		bool m_editorEnabled; // 0x1a2
		PAD(1)
		LastGameScene m_lastScene; // 0x1a4
		LastGameScene m_lastScene2; // 0x1a8
		bool m_searchObjectBool; // 0x1ac
		int m_playerFrame; // 0x1b0
		int m_playerShip; // 0x1b4
		int m_playerBall; // 0x1b8
		int m_playerBird; // 0x1bc
		int m_playerColor; // 0x1c0
		int m_playerColor2; // 0x1c4
		int m_playerStreak; // 0x1c8
		IconType m_playerIconType; // 0x1cc
		bool m_everyPlaySetup; // 0x1d0
		bool m_showSongMarkers; // 0x1d1
		bool m_showBPMMarkers; // 0x1d2
		bool m_recordGameplay; // 0x1d3
		bool m_showProgressBar; // 0x1d4
		bool m_performanceMode; // 0x1d5
		bool m_commentSortRecent; // 0x1d6
		bool m_showedPirateMessage; // 0x1d7
		bool m_clickedGarage; // 0x1d8
		bool m_clickedEditor; // 0x1d9
		bool m_clickedName; // 0x1da
		bool m_clickedPractice; // 0x1db
		bool m_showedEditorGuide; // 0x1dc
		bool m_showedRateDiffDialog; // 0x1dd
		bool m_showedRateStarDialog; // 0x1de
		bool m_showedLowDetailDialog; // 0x1df
		GameRateDelegate* m_rateDelegate; // 0x1e0
		PAD(4)
		int m_lastLevelID; // 0x1e8
		int m_loadedBgID; // 0x1ec
		int m_loadedGroundID; // 0x1f0
		int m_totalAttempts; // 0x1f4
		int m_bootups; // 0x1f8
		bool m_hasRatedGame; // 0x1fc
		bool m_didPlayGame; // 0x1fd
		bool m_isResetting; // 0x1fe
		PAD(1)
		int m_resolution; // 0x200
		cocos2d::TextureQuality m_texQuality; // 0x204

		std::string getPlayerUDID() {
			return from<std::string>(this, 0x158);
		}
		
		bool isColorUnlocked(int _id, bool _type) {
			return reinterpret_cast<bool(__thiscall*)(
				GameManager*, int, bool
			)>(
				base + 0xc53f0
			)(
				this, _id, _type
			);
		}

		bool isIconUnlocked(int _id, IconType _type) {
			return reinterpret_cast<bool(__thiscall*)(
				GameManager*, int, IconType
			)>(
				base + 0xc4fc0
			)(
				this, _id, _type
			);
		}

		cocos2d::ccColor3B colorForIdx(int colorID) {
			cocos2d::ccColor3B out;
			reinterpret_cast<cocos2d::ccColor3B* (__thiscall*)(GameManager*, cocos2d::ccColor3B*, int)>(base + 0x6a410)(this, &out, colorID);
			return out;
		}

		/*cocos2d::ccColor3B colorForIdx(int colorID) {
			cocos2d::ccColor3B out;
			reinterpret_cast<cocos2d::ccColor3B* (__thiscall*)(GameManager*, cocos2d::ccColor3B*, int)>(base + 0x6a410)(this, &out, colorID);
			return out;
		}*/

		static GameManager* sharedState() {
			return reinterpret_cast<GameManager * (__stdcall*)()>(base + 0x667d0)();
		}

		int getPlayerColor() {
			return from<int>(this, 0x1c0);
		}

		int getPlayerColor2() {
			return from<int>(this, 0x1c4);
		}

		int getPlayerGlow() {
			return from<int>(this, 0x1c8);
		}

		bool getShowProgressBar() {
			return from<bool>(this, 0x1d4); //0x1d4 - ghs 1.92
		}
		void reloadAll(bool bSwitch, bool bFullscreen, bool bReloadedInSession) {
			return reinterpret_cast<void(__thiscall*)(GameManager*, bool, bool, bool)>(
				base + 0x6c990
				)(this, bSwitch, bFullscreen, bReloadedInSession);
		}
		bool getGameVariable(const char* key) {
			return reinterpret_cast<bool(__thiscall*)(GameManager*, const char*)>(
				base + 0x6ace0
				)(this, key);
		}
		void setGameVariable(const char* key, bool value) {
			return reinterpret_cast<void(__thiscall*)(GameManager*, const char*, bool)>(
				base + 0x6abe0
				)(this, key, value);
		}
		int getIntGameVariable(const char* key) {
			return reinterpret_cast<int(__thiscall*)(GameManager*, const char*)>(
				base + 0x6afb0
				)(this, key);
		}
		void setIntGameVariable(const char* key, int value) {
			reinterpret_cast<void(__thiscall*)(GameManager*, const char*, int)>(base + 0x6aed0)(this, key, value);
		}
		bool getUGV(const char* key) {
			return reinterpret_cast<bool(__thiscall*)(GameManager*, const char*)>(
				base + 0xCA0D0
				)(this, key);
		}
		void fadeInMusic(const char* filename) {
			return reinterpret_cast<void(__thiscall*)(GameManager*, const char*)>(
				base + 0xC4BD0
				)(this, filename);
		}
		static void toggleGameVariable(const char* key) {
			reinterpret_cast<void(__stdcall*)(const char*)>(base + 0x6add0)(key);
		}

		PlayLayer* getPlayLayer() {
			return from<PlayLayer*>(this, 0x144);
		}

		int getUserID() {
			return from<int>(gd::GameManager::sharedState(), 0x188);
		}

		int getPlayerFrame() {
			return from<int>(this, 0x1b0);
		}

		int getPlayerShip() {
			return from<int>(this, 0x1b4);
		}

		int getPlayerBall() {
			return from<int>(this, 0x1b8);
		}

		int getPlayerBird() {
			return from<int>(this, 0x1bc);
		}

		void setPlayerType(IconType type) {
			from<IconType>(this, 0x2b4) = type;
		}
	};
}

#endif
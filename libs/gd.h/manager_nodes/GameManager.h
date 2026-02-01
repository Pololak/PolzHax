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

	class PlayLayer;
	class LevelSelectLayer {};
	enum class LastGameScene {
		MenuLayer = 0,
		CreatorLayer = 1,
		MyLevelsLayer = 2,
		LevelInfoLayerOrEditLevelLayer = 3,
		AnotherMenuLayer = 7,
		LevelSelectLayer = 9
	};
	class PremiumPopup : public cocos2d::CCNode {};
	class GameRateDelegate;

	class GameManager : public GManager {
	public:
		bool m_switchMode; // 0x104
		bool m_toFullScreen; // 0x105
		bool m_reloading; // 0x106
		PAD(5)
		cocos2d::CCDictionary* m_valueKeeper; // 0x10c
		double m_adTimer; // 0x110
		double m_adCache; // 0x118
		PAD(8)
		bool m_loaded; // 0x128
		bool m_didSyncAchiviements; // 0x129
		std::string m_editorCopyString; // 0x12c
		PlayLayer* m_playLayer; // 0x144
		LevelSelectLayer* m_levelSelectLayer; // 0x148
		bool m_inMenuLayer; // 0x14c
		PremiumPopup* m_premiumPopup; // 0x150
		bool m_firstSetup; // 0x154
		bool m_showedMenu; // 0x155
		std::string m_playerUDID; // 0x158
		std::string m_playerName; // 0x170
		int m_playerUserID; // 0x188
		bool m_playerScoreValid; // 0x18c
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
		int m_resolution; // 0x200
		cocos2d::TextureQuality m_texQuality; // 0x204

		static GameManager* sharedState() {
			return reinterpret_cast<GameManager*(__stdcall*)()>(base + 0x667d0)();
		}

		bool getGameVariable(char const* gv) {
			return reinterpret_cast<bool(__thiscall*)(GameManager*, char const*)>(base + 0x6ace0)(this, gv);
		}

		void setGameVariable(char const* gv, bool val) {
			reinterpret_cast<void(__thiscall*)(GameManager*, char const*, bool)>(base + 0x6abe0)(this, gv, val);
		}

		int getIntGameVariable(char const* gv) {
			return reinterpret_cast<int(__thiscall*)(GameManager*, char const*)>(base + 0x6afb0)(this, gv);
		}

		void setIntGameVariable(char const* gv, int val) {
			reinterpret_cast<void(__thiscall*)(GameManager*, char const*, int)>(base + 0x6aed0)(this, gv, val);
		}

		void toggleGameVariable(char const* gv) {
			reinterpret_cast<void(__thiscall*)(GameManager*, char const*)>(base + 0x6add0)(this, gv);
		}

		cocos2d::ccColor3B colorForIdx(int id) {
			return reinterpret_cast<cocos2d::ccColor3B(__thiscall*)(GameManager*, int)>(base + 0x6a410)(this, id);
		}

		void reloadAll(bool swtch, bool fscreen, bool inSession) {
			reinterpret_cast<void(__thiscall*)(GameManager*, bool, bool, bool)>(base + 0x6c990)(this, swtch, fscreen, inSession);
		}

		void returnToLastScene(GJGameLevel* level) {
			reinterpret_cast<void(__thiscall*)(GameManager*, GJGameLevel*)>(base + 0x6c700)(this, level);
		}

		PlayLayer* getPlayLayer() {
			return m_playLayer;
		}
	};
}

#endif
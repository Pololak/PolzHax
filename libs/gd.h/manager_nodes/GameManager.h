#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include <gd.h>

#define PLAYER_ICON_FUNC(x) void setPlayer##x(int v) { m_nPlayer##x = v; __STR_CAT__(m_nPlayer##x,Rand1) = v + __STR_CAT__(m_nPlayer##x,Rand2); }



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

	class PlayLayer;

	class GameManager : public GManager {
	protected:
		bool m_bSwitchModes;
		bool m_bToFullscreen;
		bool m_bReloading;
		bool m_bUnknown;
		PAD(4);
		cocos2d::CCDictionary* m_pValueKeeper;
		cocos2d::CCDictionary* m_pUnlockValueKeeper;
		cocos2d::CCDictionary* m_pCustomObjectDict;
		PAD(4);
		double m_dUnknown;
		PAD(16);
		double m_dUnknown2;
		PAD(8);
		bool m_bLoaded; //?
		std::string m_sUnknown;
		PlayLayer* m_pPlayLayer;
		LevelEditorLayer* m_pLevelEditorLayer;
		PAD(20);
		std::string m_sPlayerUDID;
		std::string m_sPlayerName;
		bool m_bCommentsEnabled;
		int m_nPlayerUserIDRand1;
		int m_nPlayerUserIDRand2;
		int m_nPlayerUserID;
		float m_fBackgroundMusicVolume;
		float m_fEffectsVolume;
		int m_nTimeOffset;
		PAD(28);
		int m_nPlayerFrameRand1;
		int m_nPlayerFrameRand2;
		int m_nPlayerFrame;
		int m_nPlayerShipRand1;
		int m_nPlayerShipRand2;
		int m_nPlayerShip;
		int m_nPlayerBallRand1;
		int m_nPlayerBallRand2;
		int m_nPlayerBall;
		int m_nPlayerBirdRand1;
		int m_nPlayerBirdRand2;
		int m_nPlayerBird;
		int m_nPlayerDartRand1;
		int m_nPlayerDartRand2;
		int m_nPlayerDart;
		int m_nPlayerRobotRand1;
		int m_nPlayerRobotRand2;
		int m_nPlayerRobot;
		int m_nPlayerSpiderRand1;
		int m_nPlayerSpiderRand2;
		int m_nPlayerSpider;
		int m_nPlayerColorRand1;
		int m_nPlayerColorRand2;
		//int m_nPlayerColor;
		int m_nPlayerColor2Rand1;
		int m_nPlayerColor2Rand2;
		//int m_nPlayerColor2;
		int m_nPlayerStreakRand1;
		int m_nPlayerStreakRand2;
		int m_nPlayerStreak;
		int m_nPlayerDeathEffectRand1;
		int m_nPlayerDeathEffectRand2;
		int m_nPlayerDeathEffect;
		PAD(8);
		int m_nSecretNumberRand1; //? may be named differently
		int m_nSecretNumberRand2;
		bool m_bPlayerGlow;
		IconType m_nPlayerIconType;
		bool m_bUnknown2;
		bool m_bShowSongMarkers;
		bool m_bShowBPMMarkers;
		bool m_bRecordGameplay;
		bool m_bShowProgressBar;
		bool m_bPerformanceMode;
		bool m_bClickedGarage;
		bool m_bClickedEditor;
		bool m_bClickedName;
		bool m_bClickedPractice;
		bool m_bShowedEditorGuide;
		bool m_bShowedRateDiffDialog;
		bool m_bShowedRateStarDialog;
		bool m_bShowedLowDetailDialog;
		PAD(48);
		int m_nBootups;
		bool m_bHasRatedGame;
		PAD(3);
		bool m_bGameCenterEnabled;
		bool m_bSmoothFix;
		PAD(16);
		int m_nResolution;
		cocos2d::TextureQuality m_eQuality; //more after that i havent re'd

	public:
		//int getPlayerFrame() { return m_nPlayerFrame; }
		int getPlayerShip() { return m_nPlayerShip; }
		//int getPlayerBall() { return m_nPlayerBall; }
		int getPlayerBird() { return m_nPlayerBird; }
		int getPlayerDart() { return m_nPlayerDart; }
		int getPlayerRobot() { return m_nPlayerRobot; }
		int getPlayerSpider() { return m_nPlayerSpider; }
		int getPlayerStreak() { return m_nPlayerStreak; }
		int getPlayerDeathEffect() { return m_nPlayerDeathEffect; }
		bool getPlayerGlow() { return m_bPlayerGlow; }
		//int getPlayerColor() { return m_nPlayerColor; }
		//int getPlayerColor2() { return m_nPlayerColor2; }
		IconType getPlayerIconType() { return m_nPlayerIconType; }
		std::string getPlayerUDID() {
			return from<std::string>(this, 0x158);
		}

		PLAYER_ICON_FUNC(Frame)
		PLAYER_ICON_FUNC(Ship)
		PLAYER_ICON_FUNC(Ball)
		PLAYER_ICON_FUNC(Bird)
		PLAYER_ICON_FUNC(Dart)
		PLAYER_ICON_FUNC(Robot)
		PLAYER_ICON_FUNC(Spider)
		PLAYER_ICON_FUNC(Streak)
		PLAYER_ICON_FUNC(DeathEffect)
		//PLAYER_ICON_FUNC(Color)
		//PLAYER_ICON_FUNC(Color2)
		void setPlayerGlow(bool v) { m_bPlayerGlow = v; }
		void setPlayerIconType(IconType v) { m_nPlayerIconType = v; }

		#undef FUCK_THIS
		
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

		bool getShowProgressBar() {
			return from<bool>(this, 0x1d4); //0x1d4 - ghs 1.92
		}
		void reloadAll(bool bSwitch, bool bFullscreen, bool bReloadedInSession) {
			return reinterpret_cast<void(__thiscall*)(GameManager*, bool, bool, bool)>(
				base + 0x6c990
				)(this, bSwitch, bFullscreen, bReloadedInSession);
		}
		void setQuality(cocos2d::TextureQuality quality) {
			m_eQuality = quality;
		}
		bool getGameVariable(const char* key) {
			return reinterpret_cast<bool(__thiscall*)(GameManager*, const char*)>(
				base + 0x6ade0
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
		//PlayLayer* getPlayLayer() { return m_pPlayLayer; }
		LevelEditorLayer* getEditorLayer() { return m_pLevelEditorLayer; }

		int getPlayerBall() {
			return from<int>(this, 0x1b8);
		}

		int getPlayerFrame() {
			return from<int>(this, 0x1b0);
		}

		void setPlayerType(IconType type) {
			from<IconType>(this, 0x2b4) = type;
		}
	};
}

#endif
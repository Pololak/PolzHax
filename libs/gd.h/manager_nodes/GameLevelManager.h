#ifndef __GAMELEVELMANAGER_H__
#define __GAMELEVELMANAGER_H__

#include <gd.h>

namespace gd {
	class LevelManagerDelegate;
	class LevelDownloadDelegate;
	class LevelCommentDelegate;
	class CommentUploadDelegate;
	class LevelUploadDelegate;
	class LevelUpdateDelegate;
	class LeaderboardManagerDelegate;
	class LevelDeleteDelegate;
	enum class SearchType;
	class GJGameLevel;
	class GJSearchObject;

	class GameLevelManager : public cocos2d::CCNode {
	public:
		cocos2d::CCDictionary* m_mainLevels; // 0xe8
		cocos2d::CCDictionary* m_valueDict; // 0xec
		cocos2d::CCDictionary* m_savedLevelsDict; // 0xf0
		cocos2d::CCDictionary* m_timerDict; // 0xf4
		cocos2d::CCDictionary* m_userValues; // 0xf8
		cocos2d::CCDictionary* m_storedLevels; // 0xfc
		cocos2d::CCDictionary* m_pageInfo; // 0x100
		cocos2d::CCDictionary* m_unknownDict; // 0x104
		cocos2d::CCDictionary* m_savedPacks; // 0x108
		cocos2d::CCDictionary* m_activeDownloads; // 0x10c
		cocos2d::CCDictionary* m_activeUploads; // 0x110
		std::string m_lastSearchKey; // 0x114
		std::string m_lastSearchKey2; // 0x12c
		int m_lastLeaderboard; // 0x144
		bool m_createdNewLevel; // 0x148
		LevelManagerDelegate* m_levelManagerDelegate; // 0x14c
		LevelDownloadDelegate* m_levelDownloadDelegate; // 0x150
		LevelCommentDelegate* m_levelCommentDelegate; // 0x154
		CommentUploadDelegate* m_commentUploadDelegate; // 0x158
		LevelUploadDelegate* m_levelUploadDelegate; // 0x15c
		LevelUpdateDelegate* m_levelUpdateDelegate; // 0x160
		LeaderboardManagerDelegate* m_leaderboardDelegate; // 0x164
		LevelDeleteDelegate* m_levelDeleteDelegate; // 0x168
		SearchType m_lastSearchType; // 0x16c
		int m_lastMapPackID; // 0x170
		std::string m_tempSave; // 0x174

		static GameLevelManager* sharedState() {
			return reinterpret_cast<GameLevelManager*(__stdcall*)()>(base + 0x55850)();
		}

		static cocos2d::CCDictionary* responseToDict(std::string p0, bool p1) {
			return reinterpret_cast<cocos2d::CCDictionary * (__stdcall*)(std::string, bool)>(base + 0x61000)(p0, p1);
		}

		GJGameLevel* createNewLevel() {
			return reinterpret_cast<GJGameLevel*(__thiscall*)(GameLevelManager*)>(base + 0x56590)(this);
		}

		void resetTimerForKey(char const* key) {
			reinterpret_cast<void(__thiscall*)(GameLevelManager*, char const*)>(gd::base + 0x5440)(this, key);
		}

		int getTimeLeft(char const* key, float delay) {
			__asm movss xmm2, delay
			return reinterpret_cast<int(__thiscall*)(GameLevelManager*, char const*)>(base + 0x585d0)(this, key);
		}

		void makeTimeStamp(char const* key) {
			reinterpret_cast<void(__thiscall*)(GameLevelManager*, char const*)>(base + 0x58350)(this, key);
		}

		void updateUserScore() {
			reinterpret_cast<void(__thiscall*)(GameLevelManager*)>(base + 0x5c950)(this);
		}

		gd::string getLengthStr(bool isTiny, bool isShort, bool isMedium, bool isLong) {
			return reinterpret_cast<gd::string(__thiscall*)(bool, bool, bool, bool)>(base + 0x599b0)(isTiny, isShort, isMedium, isLong);
		}

		cocos2d::CCArray* getSavedLevels() {
			return reinterpret_cast<cocos2d::CCArray * (__fastcall*)(GameLevelManager*)>(base + 0x57640)(this);
		}

		void getOnlineLevels(GJSearchObject* searchObject) {
			reinterpret_cast<void(__thiscall*)(GameLevelManager*, GJSearchObject*)>(base + 0x59cb0)(this, searchObject);
		}

		cocos2d::CCArray* getStoredOnlineLevels(char const* key) {
			return reinterpret_cast<cocos2d::CCArray * (__thiscall*)(GameLevelManager*, char const*)>(base + 0x57ef0)(this, key);
		}
	};
}

#endif // !__GAMELEVELMANAGER_H__

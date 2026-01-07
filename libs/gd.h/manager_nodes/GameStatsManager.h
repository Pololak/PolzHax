#ifndef __GAMESTATSMANAGER_H__
#define __GAMESTATSMANAGER_H__

#include <gd.h>

namespace gd {
	class GJGameLevel;

	class GameStatsManager : public cocos2d::CCNode {
	public:
		int m_savedStars; // 0xe8
		int m_savedDemons; // 0xec
		int m_savedCompletedLevels; // 0xf0
		cocos2d::CCDictionary* m_valueDict; // 0xf4
		cocos2d::CCDictionary* m_completedLevels; // 0xf8
		cocos2d::CCDictionary* m_liteAchievementsDict; // 0xfc

		static GameStatsManager* sharedState() {
			return reinterpret_cast<GameStatsManager*(__stdcall*)()>(base + 0x76ec0)();
		}

		bool hasCompletedLevel(GJGameLevel* gameLevel) {
			return reinterpret_cast<bool(__thiscall*)(GameStatsManager*, GJGameLevel*)>(base + 0x78220)(this, gameLevel);
		}

		int getStat(const char* index) {
			return reinterpret_cast<int(__thiscall*)(GameStatsManager*, const char*)>(base + 0x770c0)(this, index);
		}

		void setStat(const char* index, int amount) {
			reinterpret_cast<void(__thiscall*)(GameStatsManager*, const char*, int)>(base + 0x77180)(this, index, amount);
		}

		const char* getDemonLevelKey(int levelID) {
			return cocos2d::CCString::createWithFormat("demon_%i", levelID)->getCString();
		}

		const char* getStarLevelKey(int levelID) {
			return cocos2d::CCString::createWithFormat("star_%i", levelID)->getCString();
		}
	};
}

#endif // !__GAMESTATSMANAGER_H__

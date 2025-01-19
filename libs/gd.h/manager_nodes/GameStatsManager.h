#ifndef __GAMESTATSMANAGER_H__
#define __GAMESTATSMANAGER_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class GJGameLevel;

	class GameStatsManager : public GManager {
	public:
		PAD(0x8)
		cocos2d::CCDictionary* m_completedLevels;

		static GameStatsManager* sharedState() {
			return reinterpret_cast<GameStatsManager * (__stdcall*)()>(
				base + 0x76ec0 //76770
				)();
		}

		int getStat(const char* index) {
			return reinterpret_cast<int(__thiscall*)(GameStatsManager*, const char*)>(
				base + 0x770c0 //4ED0
				)(this, index);
		}
		void setStat(const char* index, int amount) {
			return reinterpret_cast<void(__thiscall*)(GameStatsManager*, const char*, int)>(base + 0x77180)(this, index, amount);
		}

		bool hasCompletedLevel(GJGameLevel* gameLevel) {
			return reinterpret_cast<bool(__thiscall*)(GameStatsManager*, GJGameLevel*)>(base + 0x78220)(this, gameLevel);
		}

		cocos2d::CCDictionary* completedLevels() {
			return from<cocos2d::CCDictionary*>(this, 0xf8);
		}
		/*
		1 - Jumps
		2 - Attempts
		3 - Completed Levels
		4 - Completed Online Levels
		5 - Completed Demons
		6 - Stars
		7 - Map Packs
		8 - Secret Coins
		9 - Players Destroyed
		10 - Liked/Disliked Levels
		11 - Rated Levels
		*/
	};
}
#endif

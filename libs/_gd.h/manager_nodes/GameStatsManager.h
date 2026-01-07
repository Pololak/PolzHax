#ifndef __GAMESTATSMANAGER_H__
#define __GAMESTATSMANAGER_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class GameStatsManager : public GManager {
	public:

		static GameStatsManager* sharedState() {
			return reinterpret_cast<GameStatsManager * (__stdcall*)()>(
				base + 0x76ec0 //76770
				)();
		}

		int getStat(const char* index) {
			return reinterpret_cast<int(__thiscall*)(GameStatsManager*, const char*)>(
				base + 0x4f40 //4ED0
				)(this, index);
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

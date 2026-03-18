#pragma once
#include "pch.h"

namespace LeaderboardsLayer {
	inline bool(__thiscall* init)(gd::LeaderboardsLayer*, gd::LeaderboardState);
	bool __fastcall initH(gd::LeaderboardsLayer*, void*, gd::LeaderboardState);

	class Callback : public gd::LeaderboardsLayer {
	public:
		void onRefresh(CCObject*);
	};

	void mem_init();
}
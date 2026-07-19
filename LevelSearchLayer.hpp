#pragma once
#include "pch.h"

namespace LevelSearchLayer {
	inline bool(__thiscall* init)(gd::LevelSearchLayer*);
	bool __fastcall initH(gd::LevelSearchLayer*);

	inline void(__thiscall* clearFilters)(gd::LevelSearchLayer*);
	void __fastcall clearFiltersH(gd::LevelSearchLayer*);

	class Callback : public gd::LevelSearchLayer {
	public:
		void onSearchPlayer(CCObject*);
		void onTestDemonlist(CCObject*);
		void onProcessHttpRequestCompleted(extension::CCHttpClient*, extension::CCHttpResponse*);
	};

	void mem_init();
}
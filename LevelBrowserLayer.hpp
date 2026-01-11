#pragma once
#include "pch.h"

namespace LevelBrowserLayer {
	inline bool(__thiscall* init)(gd::LevelBrowserLayer*, gd::GJSearchObject*);
	bool __fastcall initH(gd::LevelBrowserLayer*, void*, gd::GJSearchObject*);

	class Callback : public gd::LevelBrowserLayer {
	public:
		void onImportLevel(CCObject*);
		void onRefresh(CCObject*);

		void onFirstPage(CCObject*);
	};

	void mem_init();
}
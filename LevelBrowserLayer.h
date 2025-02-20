#pragma once
#include "pch.h"

namespace LevelBrowserLayer {
	inline bool(__thiscall* init)(gd::LevelBrowserLayer*, gd::GJSearchObject*);
	bool __fastcall initH(gd::LevelBrowserLayer* self, void*, gd::GJSearchObject* searchObject);

	inline void(__thiscall* onNextPage)(gd::LevelBrowserLayer*, CCObject*);
	void __fastcall onNextPageH(gd::LevelBrowserLayer* self, void*, CCObject* sender);

	inline void(__thiscall* onPrevPage)(gd::LevelBrowserLayer*, CCObject*);
	void __fastcall onPrevPageH(gd::LevelBrowserLayer* self, void*, CCObject* sender);

	class Callback : public gd::LevelBrowserLayer {
	public:
		void onRefresh(CCObject*);
		void onLastPage(CCObject*);
		void onFirstPage(CCObject*);
		void onImportLevel(CCObject*);
		void onSearch(CCObject*);
		void onClearSearch(CCObject*);
	};

	void mem_init();
}
#pragma once
#include "pch.h"

namespace LevelBrowserLayer {
	inline bool(__thiscall* init)(gd::LevelBrowserLayer*, gd::GJSearchObject*);
	bool __fastcall initH(gd::LevelBrowserLayer*, void*, gd::GJSearchObject*);

	inline void(__thiscall* loadPage)(gd::LevelBrowserLayer*, gd::GJSearchObject*);
	void __fastcall loadPageH(gd::LevelBrowserLayer*, void*, gd::GJSearchObject*);

	void updatePageButton(gd::LevelBrowserLayer*);

	void setCustomSearchQuery(std::string);

	class Callback : public gd::LevelBrowserLayer {
	public:
		void onImportLevel(CCObject*);
		void onRefresh(CCObject*);

		void onFirstPage(CCObject*);
		void onLastPage(CCObject*);
		void onGoToPage(CCObject*);

		void onSearch(CCObject*);
		void onCancelSearch(CCObject*);
	};

	void mem_init();
}
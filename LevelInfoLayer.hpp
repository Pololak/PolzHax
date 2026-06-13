#pragma once
#include "pch.h"

namespace LevelInfoLayer {
	inline bool(__thiscall* init)(gd::LevelInfoLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::LevelInfoLayer*, void*, gd::GJGameLevel*);

	inline void(__thiscall* onClone)(gd::LevelInfoLayer*, CCObject*);
	void __fastcall onCloneH(gd::LevelInfoLayer*, void*, CCObject*);

	inline void(__thiscall* FLAlert_Clicked)(gd::LevelInfoLayer*, gd::FLAlertLayer*, bool);
	void __fastcall FLAlert_ClickedH(gd::LevelInfoLayer*, void*, gd::FLAlertLayer*, bool);

	inline void(__thiscall* onLevelInfo)(gd::LevelInfoLayer*, CCObject*);
	void __fastcall onLevelInfoH(gd::LevelInfoLayer*, void*, CCObject*);

	inline void(__thiscall* levelUpdateFinished)(gd::LevelInfoLayer*, gd::GJGameLevel*, gd::UpdateResponse);
	void __fastcall levelUpdateFinishedH(gd::LevelInfoLayer*, void*, gd::GJGameLevel*, gd::UpdateResponse);

	inline void(__thiscall* destructor)(gd::LevelInfoLayer*);
	void __fastcall destructorH(gd::LevelInfoLayer*);

	inline void(__thiscall* songWidget)();
	void __fastcall songWidgetH();

	class Callback : public gd::LevelInfoLayer {
	public:
		void onExportLevel(CCObject*);
		void onGarage(CCObject*);
		void onMoveToTop(CCObject*);
		void onFavorite(CCObject*);
		void onPlayReplay(CCObject*);
	};

	void mem_init();
}
#pragma once
#include "pch.h"

namespace EditLevelLayer {
	inline bool(__thiscall* init)(gd::EditLevelLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::EditLevelLayer*, void*, gd::GJGameLevel*);

	inline void(__thiscall* onEdit)(gd::EditLevelLayer*, CCObject*);
	void __fastcall onEditH(gd::EditLevelLayer*, void*, CCObject*);

	inline void(__thiscall* FLAlert_Clicked)(gd::EditLevelLayer*, gd::FLAlertLayer*, bool);
	void __fastcall FLAlert_ClickedH(gd::EditLevelLayer*, void*, gd::FLAlertLayer*, bool);

	inline void(__thiscall* onClone)(gd::EditLevelLayer*);
	void __fastcall onCloneH(gd::EditLevelLayer*);

	inline void(__thiscall* levelActionsMenu)();
	void __fastcall levelActionsMenuH();

	inline void(__thiscall* destructor)(gd::EditLevelLayer*);
	void __fastcall destructorH(gd::EditLevelLayer*);

	void updateLevelOrderLabel(gd::EditLevelLayer*);

	class Callback : public gd::EditLevelLayer {
	public:
		void onMoveToTop(CCObject*);
		void onExportLevel(CCObject*);
		void onLevelOrderUp(CCObject*);
		void onLevelOrderDown(CCObject*);
	};

	void mem_init();
}
#pragma once
#include "pch.h"

namespace EditorPauseLayer {
	inline bool(__thiscall* init)(gd::EditorPauseLayer*, gd::LevelEditorLayer*);
	bool __fastcall initH(gd::EditorPauseLayer*, void*, gd::LevelEditorLayer*);

	inline void(__thiscall* customSetup)(gd::EditorPauseLayer*);
	void __fastcall customSetupH(gd::EditorPauseLayer*);

	inline void(__thiscall* FLAlert_Clicked)(gd::EditorPauseLayer*, gd::FLAlertLayer*, bool);
	void __fastcall FLAlert_ClickedH(gd::EditorPauseLayer*, void*, gd::FLAlertLayer*, bool);

	inline void(__thiscall* keyDown)(gd::EditorPauseLayer*, enumKeyCodes);
	void __fastcall keyDownH(gd::EditorPauseLayer*, void*, enumKeyCodes);

	inline void(__thiscall* saveLevel)(gd::EditorPauseLayer*);
	void __fastcall saveLevelH(gd::EditorPauseLayer*);

	inline void(__thiscall* destructor)(gd::EditorPauseLayer*);
	void __fastcall destructorH(gd::EditorPauseLayer*);

	class Callback : public gd::EditorPauseLayer {
	public:
		void onSave(CCObject*);

		void onSelectAll(CCObject*);
		void onSelectAllLeft(CCObject*);
		void onSelectAllRight(CCObject*);
		void onPasteString(CCObject*);
		void onAlignY(CCObject*);
		void onAlignX(CCObject*);

		void onSmallEditorStep(CCObject*);
		void onSelectFilter(CCObject*);
		void onShowObjectInfo(CCObject*);
		void onPreviewMode(CCObject*);
		void onShowGround(CCObject*);

		void onEditorOptions(CCObject*);

		void onResetStartPos(CCObject*);
	};

	void mem_init();
}
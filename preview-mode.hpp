#pragma once
#include "pch.h"

namespace preview_mode {
	void init();

	class Callback {
	public:
		void hideUI_btn(CCObject*);
	};
}

namespace EditorUI {
	inline bool(__thiscall* init)(gd::EditorUI* self, CCLayer* editor);
	bool __fastcall init_H(gd::EditorUI* self, void*, CCLayer* editor);

	inline bool(__thiscall* dtor)(gd::EditorUI* self);
	bool __fastcall dtor_H(gd::EditorUI* self);

	inline bool(__thiscall* scrollWheel)(gd::EditorUI* _self, float dy, float dx);
	void __fastcall scrollWheel_H(gd::EditorUI* _self, void* edx, float dy, float dx);

	inline bool(__thiscall* createMoveMenu)(gd::EditorUI* self);
	void __fastcall createMoveMenu_H(gd::EditorUI* self);

	inline void(__thiscall* onPause)(gd::EditorUI*, CCObject*);
	void __fastcall onPause_H(gd::EditorUI* self, void*, CCObject* sender);

	void mem_init();
}

namespace EditorPauseLayer {
	inline bool(__thiscall* customSetup)(gd::CCBlockLayer* self);
	void __fastcall customSetup_H(gd::CCBlockLayer* self);

	inline void(__thiscall* onSaveAndPlay)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onSaveAndPlay_H(gd::EditorPauseLayer* self, void*, CCObject* sender);

	inline void(__thiscall* onSaveAndExit)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onSaveAndExit_H(gd::EditorPauseLayer* self, void*, CCObject* sender);

	inline void(__thiscall* onExitNoSave)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onExitNoSave_H(gd::EditorPauseLayer* self, void*, CCObject* sender);

	void mem_init();
}

namespace Scheduler {
	inline bool(__thiscall* update)(CCScheduler* self, float dt);
	void __fastcall update_H(CCScheduler* self, void* edx, float dt);

	void mem_init();
}
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

	inline bool(__thiscall* init)(gd::EditorUI* self, gd::LevelEditorLayer* editor);
	bool __fastcall init_H(gd::EditorUI* self, void*, gd::LevelEditorLayer* editor);

	inline bool(__thiscall* dtor)(gd::EditorUI* self);
	bool __fastcall dtor_H(gd::EditorUI* self);

	inline bool(__thiscall* scrollWheel)(gd::EditorUI* _self, float dy, float dx);
	void __fastcall scrollWheel_H(gd::EditorUI* _self, void* edx, float dy, float dx);

	//inline bool(__thiscall* keyDown)(gd::EditorUI*, cocos2d::enumKeyCodes);
	//void __fastcall keyDown_H(gd::EditorUI* self, void* edx, cocos2d::enumKeyCodes key);

	//inline bool(__thiscall* keyUp)(gd::EditorUI*, cocos2d::enumKeyCodes);
	//void __fastcall keyUp_H(gd::EditorUI* self, void* edx, cocos2d::enumKeyCodes key);

	class Callback {
	public:
		void onGoToBaseLayer(CCObject*);
		void onGoToNextFreeLayer(CCObject*);
		void onGoToGroup(CCObject*);
		void rotate45CW(CCObject*);
		void rotate45CCW(CCObject*);
		void rotate265CW(CCObject*);
		void rotate265CCW(CCObject*);
	};

	void mem_init();
}

namespace EditorPauseLayer {
	inline bool(__thiscall* customSetup)(gd::EditorPauseLayer* self);
	void __fastcall customSetup_H(gd::EditorPauseLayer* self);

	inline void(__thiscall* onSaveAndPlay)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onSaveAndPlay_H(gd::EditorPauseLayer* self, void*, CCObject* sender);

	inline void(__thiscall* onSaveAndExit)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onSaveAndExit_H(gd::EditorPauseLayer* self, void*, CCObject* sender);

	inline void(__thiscall* onExitNoSave)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onExitNoSave_H(gd::EditorPauseLayer* self, void*, CCObject* sender);

	inline void(__thiscall* onExitEditor)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onExitEditor_H(gd::EditorPauseLayer* self, void*, CCObject* sender);

	inline bool(__thiscall* keyDown)(gd::EditorPauseLayer*, cocos2d::enumKeyCodes);
	void __fastcall keyDown_H(gd::EditorPauseLayer* self, void* edx, cocos2d::enumKeyCodes key);

	class Callback {
	public:
		void SmallEditorStepToggler(CCObject*);
		void PreviewModeToggler(CCObject*);
		void VanillaSelectAllButton(CCObject*);
		void extraFeaturesToggler(CCObject*);
		void extraInfoPopup(CCObject*);
	};

	void mem_init();
}

namespace Scheduler {
	inline bool(__thiscall* update)(CCScheduler* self, float dt);
	void __fastcall update_H(CCScheduler* self, void* edx, float dt);

	void mem_init();
}


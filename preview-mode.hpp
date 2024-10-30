#pragma once
#include "pch.h"

namespace preview_mode {
	void init();

	void showZoomText(gd::EditorUI* ui);

	class Callback {
	public:
		void hideUI_btn(CCObject*);
	};
}

namespace LevelEditorLayer {
	inline void(__thiscall* onPlaytest)(gd::LevelEditorLayer*);
	void __fastcall onPlaytestH(gd::LevelEditorLayer* self);

	void mem_init();
}

namespace EditorUI {

	inline bool(__thiscall* init)(gd::EditorUI* self, gd::LevelEditorLayer* editor);
	bool __fastcall init_H(gd::EditorUI* self, void*, gd::LevelEditorLayer* editor);

	inline bool(__thiscall* dtor)(gd::EditorUI* self);
	bool __fastcall dtor_H(gd::EditorUI* self);

	inline bool(__thiscall* scrollWheel)(gd::EditorUI* _self, float dy, float dx);
	void __fastcall scrollWheel_H(gd::EditorUI* _self, void* edx, float dy, float dx);

	inline void(__thiscall* updateGridNodeSize)(gd::EditorUI*);
	void __fastcall updateGridNodeSizeH(gd::EditorUI* self);

	inline void(__thiscall* createMoveMenu)(gd::EditorUI*);
	void __fastcall createMoveMenuH(gd::EditorUI* self);

	inline void(__thiscall* selectObject)(gd::EditorUI*, gd::GameObject*);
	void __fastcall selectObjectH(gd::EditorUI* self, void* edx, gd::GameObject* object);

	inline void(__thiscall* selectObjects)(gd::EditorUI*, CCArray*);
	void __fastcall selectObjectsH(gd::EditorUI* self, void* edx, CCArray* objects);

	inline bool(__thiscall* keyDown)(gd::EditorUI*, cocos2d::enumKeyCodes);
	void __fastcall keyDown_H(gd::EditorUI* self, void* edx, cocos2d::enumKeyCodes key);

	//inline bool(__thiscall* keyDown)(gd::EditorUI*, cocos2d::enumKeyCodes);
	//void __fastcall keyDown_H(gd::EditorUI* self, void* edx, cocos2d::enumKeyCodes key);

	//inline bool(__thiscall* keyUp)(gd::EditorUI*, cocos2d::enumKeyCodes);
	//void __fastcall keyUp_H(gd::EditorUI* self, void* edx, cocos2d::enumKeyCodes key);

	inline CCPoint*(__thiscall* moveForCommand)(gd::EditorUI*, CCPoint*, gd::EditCommand);
	CCPoint* __fastcall moveForCommandH(gd::EditorUI* self, void* edx, CCPoint* pos, gd::EditCommand com);

	inline void(__thiscall* transformObject)(gd::EditorUI*, gd::GameObject*, gd::EditCommand, bool);
	void __fastcall transformObjectH(gd::EditorUI* self, void* edx, gd::GameObject* obj, gd::EditCommand com, bool idk);

	class Callback {
	public:
		void onGoToBaseLayer(CCObject*);
		void onGoToNextFreeLayer(CCObject*);
		void onGoToGroup(CCObject*);
		void rotate45CW(CCObject*);
		void rotate45CCW(CCObject*);
		void rotate265CW(CCObject*);
		void rotate265CCW(CCObject*);

		void moveObjectHalfLeft(CCObject*);
		void moveObjectHalfRight(CCObject*);
		void moveObjectHalfUp(CCObject*);
		void moveObjectHalfDown(CCObject*);

		void moveObjectQuarterLeft(CCObject*);
		void moveObjectQuarterRight(CCObject*);
		void moveObjectQuarterUp(CCObject*);
		void moveObjectQuarterDown(CCObject*);

		void moveObjectEightLeft(CCObject*);
		void moveObjectEightRight(CCObject*);
		void moveObjectEightUp(CCObject*);
		void moveObjectEightDown(CCObject*);

		void moveObjectSmallerLeft(CCObject*);
		void moveObjectSmallerRight(CCObject*);
		void moveObjectSmallerUp(CCObject*);
		void moveObjectSmallerDown(CCObject*);

		void moveObjectQUnitLeft(CCObject*);
		void moveObjectQUnitRight(CCObject*);
		void moveObjectQUnitUp(CCObject*);
		void moveObjectQUnitDown(CCObject*);
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
		void selectFilterToggle(CCObject*);

		void onSaveLevel(CCObject*);
	};

	void mem_init();
}

namespace BoomScrollLayer {
	inline void(__thiscall* updateDots)(gd::BoomScrollLayer*, float);
	void __fastcall updateDotsH(gd::BoomScrollLayer* self, void* edx, float width);



	void mem_init();
}

namespace Scheduler {
	inline bool(__thiscall* update)(CCScheduler* self, float dt);
	void __fastcall update_H(CCScheduler* self, void* edx, float dt);

	void mem_init();
}


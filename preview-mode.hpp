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

	inline void(__thiscall* removeObject)(gd::LevelEditorLayer*, gd::GameObject*, bool);
	void __fastcall removeObjectH(gd::LevelEditorLayer* self, void*, gd::GameObject* obj, bool idk);

	inline void(__thiscall* update)(gd::LevelEditorLayer*, float);
	void __fastcall updateH(gd::LevelEditorLayer* self, void*, float dt);

	inline void(__thiscall* draw)(gd::LevelEditorLayer*);
	void __fastcall drawH(gd::LevelEditorLayer*);

	inline void(__thiscall* flipGravity)(gd::LevelEditorLayer*, gd::PlayerObject*, bool, bool);
	void __fastcall flipGravityH(gd::LevelEditorLayer*, void*, gd::PlayerObject*, bool, bool);

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

	inline void(__thiscall* keyDown)(gd::EditorUI*, enumKeyCodes);
	void __fastcall keyDownH(gd::EditorUI*, void*, enumKeyCodes);

	inline void(__thiscall* keyUp)(gd::EditorUI*, enumKeyCodes);
	void __fastcall keyUpH(gd::EditorUI*, void*, enumKeyCodes);

	inline CCPoint*(__thiscall* moveForCommand)(gd::EditorUI*, CCPoint*, gd::EditCommand);
	CCPoint* __fastcall moveForCommandH(gd::EditorUI* self, void* edx, CCPoint* pos, gd::EditCommand com);

	inline void(__thiscall* transformObject)(gd::EditorUI*, gd::GameObject*, gd::EditCommand, bool);
	void __fastcall transformObjectH(gd::EditorUI* self, void* edx, gd::GameObject* obj, gd::EditCommand com, bool idk);

	class Callback : public gd::EditorUI {
	public:
		void onGoToBaseLayer(CCObject*);
		void onGoToNextFreeLayer(CCObject*);
		void onGoToGroup(CCObject*);

		void onCustomMoveObject(CCObject*);
		void onCustomRotateObject(CCObject*);
	};

	inline void(__thiscall* onCopy)(gd::EditorUI*, CCObject*);
	void __fastcall onCopyH(gd::EditorUI* self, void*, CCObject* sender);

	inline void(__thiscall* sliderChanged)(gd::EditorUI*, CCObject*);
	void __fastcall sliderChangedH(gd::EditorUI* self, void*, CCObject* sender);

	inline void(__thiscall* setupDeleteMenu)(gd::EditorUI*);
	void __fastcall setupDeleteMenuH(gd::EditorUI* self);

	inline void(__thiscall* updateButtons)(gd::EditorUI*);
	void __fastcall updateButtonsH(gd::EditorUI* self);

	inline void(__thiscall* onGroupDown)(gd::EditorUI*, CCObject*);
	void __fastcall onGroupDownH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* onGroupUp)(gd::EditorUI*, CCObject*);
	void __fastcall onGroupUpH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* editObject)(gd::EditorUI*, CCObject*);
	void __fastcall editObjectH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* onDuplicate)(gd::EditorUI*, CCObject*);
	void __fastcall onDuplicateH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* draw)(gd::EditorUI*);
	void __fastcall drawH(gd::EditorUI* self);

	inline CCPoint*(__thiscall* offsetForKey)(gd::EditorUI*, int);
	CCPoint* __fastcall offsetForKeyH(gd::EditorUI*, void*, int);

	inline void(__thiscall* moveObject)(gd::EditorUI*, gd::GameObject*, CCPoint);
	void __fastcall moveObjectH(gd::EditorUI*, void*, gd::GameObject*, CCPoint);

	inline void(__thiscall* onCreateObject)(gd::EditorUI*, int);
	void __fastcall onCreateObjectH(gd::EditorUI*, void*, int);

	inline void(__thiscall* transformObjectCall)(gd::EditorUI*, gd::EditCommand);
	void __fastcall transformObjectCallH(gd::EditorUI*, void*, gd::EditCommand);

	inline void(__thiscall* angleChanged)(gd::EditorUI*, float);
	void __fastcall angleChangedH(gd::EditorUI*, void*, float);

	inline void(__thiscall* onPaste)(gd::EditorUI*, CCObject*);
	void __fastcall onPasteH(gd::EditorUI*, void*, CCObject*);

	void updateObjectInfo(gd::EditorUI*);
	void updateObjectHitbox(gd::EditorUI*);

	void mem_init();
}

namespace EditorPauseLayer {
	inline bool(__thiscall* customSetup)(gd::EditorPauseLayer* self);
	void __fastcall customSetup_H(gd::EditorPauseLayer* self);

	inline void(__thiscall* onSaveAndPlay)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onSaveAndPlayH(gd::EditorPauseLayer* self, void*, CCObject* sender);

	inline void(__thiscall* onSaveAndExit)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onSaveAndExit_H(gd::EditorPauseLayer* self, void*, CCObject* sender);

	inline void(__thiscall* onExitNoSave)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onExitNoSave_H(gd::EditorPauseLayer* self, void*, CCObject* sender);

	inline void(__thiscall* onExitEditor)(gd::EditorPauseLayer*, CCObject*);
	void __fastcall onExitEditorH(gd::EditorPauseLayer* self, void*, CCObject* sender);

	inline bool(__thiscall* keyDown)(gd::EditorPauseLayer*, cocos2d::enumKeyCodes);
	void __fastcall keyDown_H(gd::EditorPauseLayer* self, void* edx, cocos2d::enumKeyCodes key);

	inline void(__thiscall* saveLevel)(gd::EditorPauseLayer*);
	void __fastcall saveLevelH(gd::EditorPauseLayer* self);

	class Callback : public gd::EditorPauseLayer {
	public:
		void SmallEditorStepToggler(CCObject*);
		void PreviewModeToggler(CCObject*);
		void VanillaSelectAllButton(CCObject*);
		void selectFilterToggle(CCObject*);

		void onSaveLevel(CCObject*);
		void onCreateTriggers(CCObject*);
	};

	void mem_init();
}

namespace BoomScrollLayer {
	inline void(__thiscall* updateDots)(gd::BoomScrollLayer*, float);
	void __fastcall updateDotsH(gd::BoomScrollLayer* self, void* edx, float width);



	void mem_init();
}

namespace SetGroupIDLayer {
	inline bool(__thiscall* init)(gd::SetGroupIDLayer*, gd::GameObject*, CCArray*);
	bool __fastcall initH(gd::SetGroupIDLayer* self, void* edx, gd::GameObject* obj, CCArray* arr);

	inline void(__thiscall* updateGroupID)(gd::SetGroupIDLayer*);
	void __fastcall updateGroupIDH(gd::SetGroupIDLayer*);

	inline void(__thiscall* dtor)(gd::SetGroupIDLayer*);
	void __fastcall dtorH(gd::SetGroupIDLayer*);

	class Callback : public gd::SetGroupIDLayer {
	public:
		void onCurrentGroup(CCObject*);
	};

	void mem_init();
}

namespace ColorSelectPopup {
	inline bool(__thiscall* init)(gd::ColorSelectPopup*, gd::GameObject*, int, int, int);
	bool __fastcall initH(gd::ColorSelectPopup* self, void*, gd::GameObject* obj, int color_id, int idk, int idk2);

	inline bool(__thiscall* dtor)(gd::ColorSelectPopup*);
	void __fastcall dtorH(gd::ColorSelectPopup* self);

	inline void(__thiscall* closeColorSelect)(gd::ColorSelectPopup*, CCObject*);
	void __fastcall closeColorSelectH(gd::ColorSelectPopup* self, void*, CCObject* sender);

	inline void(__thiscall* sliderChanged)(gd::ColorSelectPopup*, CCObject*);
	void __fastcall sliderChangedH(gd::ColorSelectPopup* self, void*, CCObject* sender);

	inline void(__thiscall* colorValueChanged)(gd::ColorSelectPopup*, ccColor3B);
	void __fastcall colorValueChangedH(gd::ColorSelectPopup* self, void*, ccColor3B color);

	class Callback : public gd::ColorSelectPopup {
	public:
		void onTypeUp(CCObject*);
		void onTypeDown(CCObject*);
	};

	void mem_init();
}

namespace LevelSettingsLayer {
	inline bool(__thiscall* init)(gd::LevelSettingsLayer*, gd::LevelSettingsObject*);
	bool __fastcall initH(gd::LevelSettingsLayer* self, void*, gd::LevelSettingsObject* settingsObject);

	inline void(__thiscall* dtor)(gd::LevelSettingsLayer*);
	void __fastcall dtorH(gd::LevelSettingsLayer*);
}

namespace DrawGridLayer {
	inline void(__thiscall* addToSpeedObjects)(gd::DrawGridLayer*, gd::GameObject*);
	void __fastcall addToSpeedObjectsH(gd::DrawGridLayer* self, void*, gd::GameObject* gameObject);

	inline void(__thiscall* draw)(gd::DrawGridLayer*);
	void __fastcall drawH(gd::DrawGridLayer*);
}

namespace Scheduler {
	inline bool(__thiscall* update)(CCScheduler* self, float dt);
	void __fastcall update_H(CCScheduler* self, void* edx, float dt);

	void mem_init();
}


#include "pch.h"

namespace EditorUI {
	inline bool(__thiscall* init)(gd::EditorUI*, gd::LevelEditorLayer*);
	bool __fastcall initH(gd::EditorUI*, void*, gd::LevelEditorLayer*);

	inline void(__thiscall* destructor)(gd::EditorUI*);
	void __fastcall destructorH(gd::EditorUI*);

	inline void(__thiscall* selectObject)(gd::EditorUI*, gd::GameObject*);
	void __fastcall selectObjectH(gd::EditorUI*, void*, gd::GameObject*);

	inline void(__thiscall* selectObjects)(gd::EditorUI*, CCArray*);
	void __fastcall selectObjectsH(gd::EditorUI*, void*, CCArray*);

	inline void(__thiscall* moveObject)(gd::EditorUI*, gd::GameObject*, CCPoint);
	void __fastcall moveObjectH(gd::EditorUI*, void*, gd::GameObject*, CCPoint);

	inline void(__thiscall* transformObject)(gd::EditorUI*, gd::GameObject*, gd::EditCommand, bool);
	void __fastcall transformObjectH(gd::EditorUI*, void*, gd::GameObject*, gd::EditCommand, bool);

	inline void(__thiscall* angleChanged)(gd::EditorUI*, float);
	void __fastcall angleChangedH(gd::EditorUI*, void*, float);

	inline void(__thiscall* updateButtons)(gd::EditorUI*);
	void __fastcall updateButtonsH(gd::EditorUI*, void*);

	inline void(__thiscall* clickOnPosition)(gd::EditorUI*, CCPoint);
	void __fastcall clickOnPositionH(gd::EditorUI*, void*, CCPoint);

	inline void(__thiscall* toggleSpecialEditButtons)(gd::EditorUI*);
	void __fastcall toggleSpecialEditButtonsH(gd::EditorUI*);

	inline CCPoint*(__thiscall* offsetForKey)(gd::EditorUI*, int);
	CCPoint* __fastcall offsetForKeyH(gd::EditorUI*, void*, int);

	inline CCPoint*(__thiscall* moveForCommand)(gd::EditorUI*, CCPoint*, gd::EditCommand);
	CCPoint* __fastcall moveForCommandH(gd::EditorUI*, void*, CCPoint*, gd::EditCommand);

	inline void(__thiscall* onCopy)(gd::EditorUI*, CCObject*);
	void __fastcall onCopyH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* onGroupDown)(gd::EditorUI*, CCObject*);
	void __fastcall onGroupDownH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* onGroupUp)(gd::EditorUI*, CCObject*);
	void __fastcall onGroupUpH(gd::EditorUI*, void*, CCObject*);

	inline bool(__thiscall* ccTouchBegan)(gd::EditorUI*, CCTouch*, CCEvent*);
	bool __fastcall ccTouchBeganH(gd::EditorUI*, void*, CCTouch*, CCEvent*);

	inline void(__thiscall* ccTouchEnded)(gd::EditorUI*, CCTouch*, CCEvent*);
	void __fastcall ccTouchEndedH(gd::EditorUI*, void*, CCTouch*, CCEvent*);

	inline void(__thiscall* onPlaytest)(gd::EditorUI*, CCObject*);
	void __fastcall onPlaytestH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* updateZoom)(gd::EditorUI*, float);
	void __fastcall updateZoomH(gd::EditorUI*, void*, float);

	inline void(__thiscall* scrollWheel)(gd::EditorUI*, float, float);
	void __fastcall scrollWheelH(gd::EditorUI*, void*, float, float);

	inline void(__thiscall* onCreateButton)(gd::EditorUI*, CCObject*);
	void __fastcall onCreateButtonH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* setupDeleteMenu)(gd::EditorUI*);
	void __fastcall setupDeleteMenuH(gd::EditorUI*);

	inline void(__thiscall* keyDown)(gd::EditorUI*, enumKeyCodes);
	void __fastcall keyDownH(gd::EditorUI*, void*, enumKeyCodes);

	inline void(__thiscall* keyUp)(gd::EditorUI*, enumKeyCodes);
	void __fastcall keyUpH(gd::EditorUI*, void*, enumKeyCodes);

	inline void(__thiscall* onDuplicate)(gd::EditorUI*, CCObject*);
	void __fastcall onDuplicateH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* updateGridNodeSize)(gd::EditorUI*);
	void __fastcall updateGridNodeSizeH(gd::EditorUI*);

	inline bool(__thiscall* editButtonUsable)(gd::EditorUI*);
	bool __fastcall editButtonUsableH(gd::EditorUI*);

	inline void(__thiscall* editObject)(gd::EditorUI*, CCObject*);
	void __fastcall editObjectH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* setupCreateMenu)(gd::EditorUI*);
	void __fastcall setupCreateMenuH(gd::EditorUI*);

	std::string colorToShortString(int);

	gd::EditorUI* get();

	void updateObjectInfoLabel(gd::EditorUI*);
	void updateGuideTogglePosition(gd::EditorUI*);

	bool isColorTriggersSelected(gd::EditorUI*);

	void setupStartPos(gd::EditorUI*, gd::StartPosObject*);

	class Callback : public gd::EditorUI {
	public:
		void onAllGroup(CCObject*);
		void onNextFreeGroup(CCObject*);
		void onColorFilter(CCObject*);
		void onGoToGroup(CCObject*);
		void onGridSize(CCObject*);
		void onCircleTool(CCObject*);
		void onStartPos(CCObject*);
	};

	void mem_init();
}
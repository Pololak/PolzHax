#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include <gd.h>

namespace gd {
	class UndoObject;
	class LevelEditorLayer;
	class BoomScrollLayer;
	class Slider;
	class CCMenuItemSpriteExtra;
	class GameObject;
	class CCMenuItemToggler;
	class CreateMenuItem;

	class GJRotationControl : public cocos2d::CCLayer {
	public:
		cocos2d::CCPoint m_cursorDifference; // 0x118
		cocos2d::CCPoint m_controlPosition; // 0x11c
		cocos2d::CCSprite* m_controlSprite; // 0x120
		float m_startingRotation; // 0x124
		float m_currentRotation; // 0x128
		int m_touchID; // 0x12c
		GJRotationControlDelegate* m_delegate; // 0x130

		void finishTouch() {
			reinterpret_cast<void(__fastcall*)(GJRotationControl*)>(base + 0x4ff70)(this);
		}
	};

	class EditButtonBar : public cocos2d::CCNode {
	public:
		BoomScrollLayer* m_scrollLayer;
		cocos2d::CCArray* m_pagesArray;

		static EditButtonBar* create(cocos2d::CCArray* objects, cocos2d::CCPoint position, int page, bool isObjects) {
			auto ret = reinterpret_cast<EditButtonBar * (__fastcall*)(cocos2d::CCArray*, int, bool, cocos2d::CCPoint)>(base + 0x3aa80)(objects, page, isObjects, position);

			__asm add esp, 0xc

			return ret;
		}

		void goToPage(int page) {
			
		}
	};

	class EditorUI : public cocos2d::CCLayer, FLAlertLayerProtocol, ColorSelectDelegate, GJRotationControlDelegate, MusicDownloadDelegate {
	public:
		float m_gridSize; // 0x128
		int m_playerTouchID; // 0x12c
		int m_player2TouchID; // 0x130
		bool m_playtestMusic; // 0x134
		UndoObject* m_storedUndoObject; // 0x138
		bool m_swipeModifier;
		bool m_inSwipeAction;
		int m_rotationTouchID; // 0x140
		int m_touchID; // 0x144
		GJRotationControl* m_rotationControl; // 0x148
		cocos2d::CCPoint m_scalePos;
		bool m_touchDown;
		cocos2d::CCDictionary* m_editButtonDict; // 0x158
		EditButtonBar* m_selectedBuildTab; // 0x15c
		EditButtonBar* m_editButtonBar; // 0x160
		Slider* m_positionSlider; // 0x164
		PAD(0x4)
		float m_limitY; // 0x16c
		PAD(0x4)
		bool m_swipeEnabled; // 0x174
		PAD(3)
		bool m_freeMoveEnabled; // 0x178
		PAD(19)
		cocos2d::CCArray* m_selectedObjects; // 0x18c
		cocos2d::CCMenu* m_deleteMenu; // 0x190
		CCMenuItemSpriteExtra* m_deleteModeBtn; // 0x194
		CCMenuItemSpriteExtra* m_buildModeBtn; // 0x198
		CCMenuItemSpriteExtra* m_editModeBtn; // 0x19c
		CCMenuItemSpriteExtra* m_swipeBtn; // 0x1a0
		CCMenuItemSpriteExtra* m_freeMoveBtn; // 0x1a4
		CCMenuItemSpriteExtra* m_deselectBtn; // 0x1a8
		CCMenuItemSpriteExtra* m_snapBtn; // 0x1ac
		CCMenuItemSpriteExtra* m_rotateBtn; // 0x1b0
		CCMenuItemSpriteExtra* m_playbackBtn; // 0x1b4
		CCMenuItemSpriteExtra* m_playtestBtn; // 0x1b8
		CCMenuItemSpriteExtra* m_playtestStopBtn; // 0x1bc
		CCMenuItemSpriteExtra* m_undoBtn; // 0x1c0
		CCMenuItemSpriteExtra* m_redoBtn; // 0x1c4
		CCMenuItemSpriteExtra* m_editObjectBtn; // 0x1c8
		CCMenuItemSpriteExtra* m_editGroupBtn; // 0x1cc
		CCMenuItemSpriteExtra* m_copyPasteBtn; // 0x1d0
		CCMenuItemSpriteExtra* m_copyBtn; // 0x1d4
		CCMenuItemSpriteExtra* m_pasteBtn; // 0x1d8
		CCMenuItemToggler* m_guideToggle; // 0x1dc
		cocos2d::CCArray* m_createButtonBars; // 0x1e0
		cocos2d::CCMenu* m_tabsMenu; // 0x1e4
		cocos2d::CCArray* m_tabsArray; // 0x1e8
		cocos2d::CCSprite* m_unusedSpr; // 0x1ec
		cocos2d::CCSprite* m_deleteFilterSprite; // 0x1f0
		CCMenuItemSpriteExtra* m_deleteBtn; // 0x1f4
		CCMenuItemSpriteExtra* m_deleteFilterBtn; // 0x1f8
		CCMenuItemSpriteExtra* m_deleteFilterNone; // 0x1fc
		CCMenuItemSpriteExtra* m_deleteFilterStatic; // 0x200
		CCMenuItemSpriteExtra* m_deleteFilterDetails; // 0x204
		CCMenuItemSpriteExtra* m_deleteFilterCustom; // 0x208
		cocos2d::CCLabelBMFont* m_currentGroupLabel; // 0x20c
		CCMenuItemSpriteExtra* m_groupNextBtn; // 0x210
		CCMenuItemSpriteExtra* m_groupPrevBtn; // 0x214
		PAD(0x8)
		int m_selectedCreateObjectID; // 0x220
		cocos2d::CCArray* m_createButtonArray; // 0x224
		int m_editMode; // 0x228
		LevelEditorLayer* m_editorLayer; // 0x22c
		cocos2d::CCPoint m_swipeStart; // 0x230
		cocos2d::CCPoint m_swipeEnd; // 0x238
		cocos2d::CCPoint m_swipePosition; // 0x240
		PAD(16)
		GameObject* m_selectedObject; // 0x258
		GameObject* m_movingObject; // 0x25c // free move object
		PAD(4)
		std::string m_clipboard; // 0x264
		PAD(4)
		int m_selectedTab; // 0x280
		int m_timesSelected; // 0x284

		cocos2d::CCArray* getSelectedObjects() {
			return reinterpret_cast<cocos2d::CCArray*(__fastcall*)(EditorUI*)>(base + 0x48200)(this);
		}

		void onDeleteSelected(CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, CCObject*)>(base + 0x42bc0)(this, sender);
		}

		CCMenuItemSpriteExtra* getSpriteButton(cocos2d::CCNode* sprite, cocos2d::SEL_MenuHandler callback, cocos2d::CCMenu* menu, float scale, int buttonID, cocos2d::CCPoint point) {
			return reinterpret_cast<CCMenuItemSpriteExtra*(__thiscall*)(EditorUI*, cocos2d::CCNode*, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float, int, cocos2d::CCPoint)>(base + 0x41790)(this, sprite, callback, menu, scale, buttonID, point);
		}

		CCMenuItemSpriteExtra* getSpriteButton(const char* sprite, cocos2d::SEL_MenuHandler callback, cocos2d::CCMenu* menu, float scale) {
			return getSpriteButton(cocos2d::CCSprite::createWithSpriteFrameName(sprite), callback, menu, scale, 1, { 0.f, 0.f });
		}

		CreateMenuItem* getCreateBtn(int id, int bg, bool p0) {
			return reinterpret_cast<CreateMenuItem*(__thiscall*)(EditorUI*, int, int, bool)>(base + 0x47200)(this, id, bg, p0);
		}

		CreateMenuItem* getCreateBtn(int id, int bg) {
			return EditorUI::getCreateBtn(id, bg, false);
		}

		void rotateObjects(cocos2d::CCArray* objects, float angle, cocos2d::CCPoint center) {
			__asm movss xmm2, angle;
			reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCArray*, cocos2d::CCPoint)>(base + 0x4c280)(this, objects, center);
		}

		void moveObjectCall(cocos2d::CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCObject*)>(base + 0x4b2a0)(this, sender);
		}

		void moveObjectCall(EditCommand command) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, EditCommand)>(base + 0x4b2c0)(this, command);
		}

		void transformObjectCall(cocos2d::CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCObject*)>(base + 0x4b580)(this, sender);
		}

		void transformObjectCall(EditCommand command) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, EditCommand)>(base + 0x4b5a0)(this, command);
		}

		void selectObjects(cocos2d::CCArray* objects) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCArray*)>(base + 0x47fa0)(this, objects);
		}

		void updateButtons() {
			reinterpret_cast<void(__thiscall*)(EditorUI*)>(base + 0x41450)(this);
		}

		void deactivateRotationControl() {
			m_rotationTouchID = -1;
			if (m_rotationControl->isVisible()) {
				m_rotationControl->setVisible(false);
				m_rotationControl->finishTouch();
			}
		}

		cocos2d::CCArray* pasteObjects(std::string objString) {
			return reinterpret_cast<cocos2d::CCArray*(__thiscall*)(EditorUI*, std::string)>(base + 0x492a0)(this, objString);
		}

		void updateZoom(float amt) {
			reinterpret_cast<void(__vectorcall*)(float, float, EditorUI*)>(base + 0x48c30)(0.f, amt, this);
		}

		void constrainGameLayerPosition(float x, float y) {
			__asm {
				movss xmm1, x
				movss xmm2, y
			}

			reinterpret_cast<void(__fastcall*)(EditorUI*)>(base + 0x4c8c0)(this);
		}

		void constrainGameLayerPosition() {
			reinterpret_cast<void(__fastcall*)(EditorUI*)>(base + 0x41e60)(this);
		}

		void onDuplicate(cocos2d::CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCObject*)>(base + 0x48e70)(this, sender);
		}

		void moveObject(GameObject* obj, cocos2d::CCPoint position) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, GameObject*, cocos2d::CCPoint)>(base + 0x4b410)(this, obj, position);
		}

		void updateGridNodeSize() {
			reinterpret_cast<void(__fastcall*)(EditorUI*)>(base + 0x41ae0)(this);
		}

		void createUndoSelectObject(bool p0) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, bool)>(base + 0x48240)(this, p0);
		}

		void onPlaytest(cocos2d::CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCObject*)>(base + 0x489c0)(this, sender);
		}

		void editObject(cocos2d::CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCObject*)>(base + 0x4ae20)(this, sender);
		}

		bool editButtonUsable() {
			return reinterpret_cast<bool(__fastcall*)(EditorUI*)>(base + 0x49680)(this);
		}

		void onSelectBuildTab(cocos2d::CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCObject*)>(base + 0x494c0)(this, sender);
		}

		void updateCreateMenu(bool p0) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, bool)>(base + 0x47440)(this, p0);
		}

		void selectBuildTab(int p0) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, int)>(base + 0x494e0)(this, p0);
		}
	};
}

#endif

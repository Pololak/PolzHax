#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include <gd.h>
#include "cocos2d.h"
#include "cocos-ext.h"

namespace gd {

	class CustomSongWidget;
	class CCMenuItemSpriteExtra;
	class CCMenuItemToggler;
	class Slider;
	class GameObject;
	class LevelSettingsObject;

	enum EditCommand {
		kEditCommandSmallLeft = 1,
		kEditCommandSmallRight = 2,
		kEditCommandSmallUp = 3,
		kEditCommandSmallDown = 4,

		kEditCommandLeft = 5,
		kEditCommandRight = 6,
		kEditCommandUp = 7,
		kEditCommandDown = 8,

		kEditCommandBigLeft = 9,
		kEditCommandBigRight = 10,
		kEditCommandBigUp = 11,
		kEditCommandBigDown = 12,

		kEditCommandTinyLeft = 13,
		kEditCommandTinyRight = 14,
		kEditCommandTinyUp = 15,
		kEditCommandTinyDown = 16,

		kEditCommandFlipX = 17,
		kEditCommandFlipY = 18,
		kEditCommandRotateCW = 19,
		kEditCommandRotateCCW = 20,
		kEditCommandRotateCW45 = 21,
		kEditCommandRotateCCW45 = 22,
		kEditCommandRotateFree = 23,
		kEditCommandRotateSnap = 24,

		kEditCommandScale = 25,
	};

	class ExtendedLayer : public cocos2d::CCLayer {};

	class ButtonPage : public cocos2d::CCLayer {
	public:
		static ButtonPage* create(cocos2d::CCArray* array) {
			return reinterpret_cast<ButtonPage * (__thiscall*)(cocos2d::CCArray*)>(base + 0x3b060)(array);
		}
	};

	class BoomScrollLayer : public cocos2d::CCLayer {
	public:
		cocos2d::CCArray* m_dotsArray; // 0x118
		int m_animatingToPage; // 0x11c
		float m_touchX; // 0x120
		float m_animateSpeed; // 0x124
		PAD(4)
		DynamicScrollDelegate* m_dynamicScrollDelegate; // 0x12c
		cocos2d::CCArray* m_dynamicScrollPages; // 0x130
		bool m_dynamic; // 0x134
		int m_touchHasMoved; // 0x138
		PAD(4)
		cocos2d::CCTouch* m_currentTouch; // 0x140
		cocos2d::CCArray* m_pages; // 0x144
		double m_touchTimer; // 0x148
		PAD(8)
		ExtendedLayer* m_mainLayer; // 0x158
		cocos2d::CCRect m_scrollArea; // 0x15c
		float m_minTouchSpeed; // 0x16c
		float m_touchSpeedFast; // 0x170
		float m_touchSpeedMid; // 0x174
		BoomScrollLayerDelegate* m_delegate; // 0x178
		bool m_movingToPage; // 0x17c
		float m_minimumTouchLengthToSlide; // 0x180
		float m_minimumTouchLengthToChangePage; // 0x184
		float m_marginOffset; // 0x18c
		bool m_stealTouches; // 0x190
		bool m_showPagesIndicator; // 0x191
		cocos2d::CCPoint m_pagesIndicatorPosition; // 0x194
		cocos2d::ccColor4B m_pagesIndicatorSelectedColor; // 0x19c
		cocos2d::ccColor4B m_pagesIndicatorNormalColor; // 0x1a0
		int m_currentScreen; // 0x1a4
		float m_pagesWidthOffset; // 0x1a8
		void* m_unusedPages; // 0x1ac
		
		void instantMoveToPage(int page) {
			reinterpret_cast<void(__thiscall*)(BoomScrollLayer*, int)>(
				base + 0x8430
				)(this, page);
		}

		void moveToPage(int page) {
			reinterpret_cast<void(__thiscall*)(BoomScrollLayer*, int)>(
				base + 0x8500
				)(this, page);
		}
	};

	class EditButtonBar : public cocos2d::CCNode {
	public:
		BoomScrollLayer* m_scrollLayer; // 0x1e0
		cocos2d::CCArray* m_pagesArray; // 0x1e4
	};

	class GJRotationControl : public cocos2d::CCLayer {};
	class GJScaleControl : public cocos2d::CCLayer {};

	class SetGroupIDLayer : public FLAlertLayer {};

	class EditorUI : public cocos2d::CCLayer {
	public:
		PAD(0x10)
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
		cocos2d::CCPoint m_lastClickPos; // 0x230
		PAD(32)
		GameObject* m_selectedObject; // 0x258
		GameObject* m_movingObject; // 0x25c // free move object
		PAD(4)
		std::string m_clipboard; // 0x264
		PAD(4)
		int m_selectedTab; // 0x280
		int m_timesSelected; // 0x284

		auto pasteObjects(const std::string& str) {
			return reinterpret_cast<cocos2d::CCArray * (__thiscall*)(EditorUI*, gd::string)>(base + 0x492a0)(this, str);
		}

		void moveForCommand(cocos2d::CCPoint* pos, EditCommand* command) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCPoint*, EditCommand*)>(base + 0x4b040)(this, pos, command);
		}

		void rotateObjects(cocos2d::CCArray* objects, float angle, cocos2d::CCPoint center) {
			__asm movss xmm2, angle;
			reinterpret_cast<void(__thiscall*)(
				EditorUI*, cocos2d::CCArray*, cocos2d::CCPoint
				)>(base + 0x4c280)(this, objects, center);
		}

		//CCMenuItemSpriteExtra* getSpriteButton(
		//	const char* sprite,
		//	cocos2d::SEL_MenuHandler callback,
		//	cocos2d::CCMenu* menu,
		//	float scale,
		//	int idk,
		//	cocos2d::CCPoint* idk2,
		//	cocos2d::CCPoint* idk3
		//) {
		//	return reinterpret_cast<CCMenuItemSpriteExtra * (__thiscall*)(
		//		EditorUI*,
		//		const char*,
		//		cocos2d::SEL_MenuHandler,
		//		cocos2d::CCMenu*,
		//		float,
		//		int,
		//		cocos2d::CCPoint*,
		//		cocos2d::CCPoint*
		//		)>(
		//			base + 0x41790
		//			)(
		//				this, sprite, callback, menu, scale, idk, idk2, idk3
		//				);
		//}

		CCMenuItemSpriteExtra* getSpriteButton(const char* sprite, cocos2d::SEL_MenuHandler callback, cocos2d::CCMenu* menu, float scale, int buttonID, cocos2d::CCPoint point) {
			return reinterpret_cast<CCMenuItemSpriteExtra * (__thiscall*)(gd::EditorUI*, const char*, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float, int, cocos2d::CCPoint)>(base + 0x41790)(this, sprite, callback, menu, scale, buttonID, point);
		}

		void moveObjectCall(cocos2d::CCObject* pSender) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCObject*)>(
				base + 0x4b2a0
				)(this, pSender);
		}

		void moveObjectCall(EditCommand command) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, EditCommand)>(
				base + 0x4b2c0
				)(this, command);
		}

		void transformObjectCall(EditCommand command) {
			reinterpret_cast<void(__thiscall*)(EditorUI*, EditCommand)>(base + 0x4b5a0)(this, command);
		}

		auto& clipboard() {
			return from<gd::string>(this, 0x264);
		}

		auto selectedObjectToolboxID() {
			return from<int>(this, 0x220);
		}

		void updateButtons() {
			reinterpret_cast<void(__thiscall*)(EditorUI*)>(base + 0x41450)(this);
		}

		bool isPlayback() {
			return from<bool>(this, 0x134);
		}

		void updateZoom(float amt) {
			reinterpret_cast<void(__vectorcall*)(float, float, EditorUI*)>(base + 0x48c30)(0.f, amt, this);
		}

		LevelEditorLayer* getLevelEditorLayer() {
			return from<LevelEditorLayer*>(this, 0x22C);
		}

		void selectObjects(cocos2d::CCArray* arr) {
			return reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCArray*)>(base + 0x47fa0)(this, arr); //0xf1f20 - GHS 1.92 // - Taswert: wtf is this??? Why did I put it here??? // - Polz: maybe for select all?
		}

		EditButtonBar* editButtonBar() {
			return from<EditButtonBar*>(this, 0x160);
		}

		cocos2d::CCArray* getSomeObjects() {
			return from<cocos2d::CCArray*>(this, 0x1E8);
		}

		CCMenuItemSpriteExtra* getRedoBtn() {
			return from<CCMenuItemSpriteExtra*>(this, 0x1c4);
		}

		CCMenuItemSpriteExtra* getDeselectBtn() {
			return from<CCMenuItemSpriteExtra*>(this, 0x1a8);
		}

		cocos2d::CCArray* getSelectedObjectsOfCCArray() {
			auto output = cocos2d::CCArray::create();
			gd::GameObject* single = from<gd::GameObject*>(this, 0x258);
			if (single)
			{
				output->addObject(reinterpret_cast<cocos2d::CCObject*>(single));
				return output;
			}
			return from<cocos2d::CCArray*>(this, 0x18c);
		}

		cocos2d::CCArray* getAllObjects() {
			return from<cocos2d::CCArray*>(this, 0x224); //no
		}

		std::vector<GameObject*> getSelectedObjects() {
			const auto single = from<GameObject*>(this, 0x258);
			if (single) return { single };
			const auto selectedArr = from<cocos2d::CCArray*>(this, 0x18c);
			if (!selectedArr) return {};
			std::vector<GameObject*> output;
			for (size_t i = 0; i < selectedArr->count(); ++i)
				output.push_back(reinterpret_cast<GameObject*>(selectedArr->objectAtIndex(i)));
			return output;
		}



		gd::GameObject* getSingleSelectedObj() {
			return from<gd::GameObject*>(this, 0x258);
		}

		gd::EditorUI* selectedObjectID() {
			return from<gd::EditorUI*>(this, 0x220);
		}

		void onDeleteSelected(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(EditorUI*, CCObject*)>(base + 0x42bc0)(this, sender);
		}

		void onSettings(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(EditorUI*, CCObject*)>(base + 0x41190)(this, sender);
		}

		void onDeselectAll(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(EditorUI*, CCObject*)>(base + 0x48340)(this, sender);
		}

		void getCreateBtn() {
			return reinterpret_cast<void(__fastcall*)()>(base + 0x47200)();
		}

		int selectedMode() {
			return from<int>(this, 0x228);
		}

		float getGridSize() {
			return from<float>(this, 0x128);
		}

		static constexpr const int Mode_Create = 2;
		static constexpr const int Mode_Delete = 1;
		static constexpr const int Mode_Edit = 3;

		void onPause(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(EditorUI*, CCObject*)>(base + 0x411f0)(this, sender);
		}

		void editObject(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(EditorUI*, CCObject*)>(base + 0x4ae20)(this, sender);
		}
		void onDuplicate(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(EditorUI*, CCObject*)>(base + 0x48e70)(this, sender);
		}
		void updateGridNodeSize() {
			reinterpret_cast<void(__fastcall*)(EditorUI*)>(base + 0x41ae0)(this);
		}
	};

	class EditorPauseLayer : public gd::CCBlockLayer, FLAlertLayerProtocol {
	public:
		PAD(0x4)
		CCMenuItemSpriteExtra* m_audioOnBtn;
		CCMenuItemSpriteExtra* m_audioOffBtn;
		LevelEditorLayer* m_editorLayer;

		void onResume(cocos2d::CCObject* pSender) {
			reinterpret_cast<void(__thiscall*)(EditorPauseLayer*, cocos2d::CCObject*)>(
				base + 0x3eea0
				)(this, pSender);
		}

		void saveLevel() {
			reinterpret_cast<void(__thiscall*)(EditorPauseLayer*)>(
				base + 0x3eec0
				)(this);
		}

		CCMenuItemSpriteExtra* bpmButton() {
			return from<CCMenuItemSpriteExtra*>(this, 0x1a4);
		}

		LevelEditorLayer* getEditorLayer() {
			return from<LevelEditorLayer*>(this, 0xac);
		}
	};

	class LevelSettingsLayer : public gd::FLAlertLayer {
	public:
		PAD(0x18)
		cocos2d::CCSprite* m_bgBtnSpr; // 0x1d4
		cocos2d::CCSprite* m_gBtnSpr; // 0x1d8
		cocos2d::CCSprite* m_lBtnSpr; // 0x1dc
		cocos2d::CCSprite* m_objBtnSpr; // 0x1e0
		cocos2d::CCSprite* m_dlBtnSpr; // 0x1e4
		cocos2d::CCSprite* m_col1BtnSpr; // 0x1e8
		cocos2d::CCSprite* m_col2BtnSpr; // 0x1ec
		cocos2d::CCSprite* m_col3BtnSpr; // 0x1f0
		cocos2d::CCSprite* m_col4BtnSpr; // 0x1f4
		cocos2d::CCSprite* m_bgIcon; // 0x1f8
		cocos2d::CCSprite* m_gIcon; // 0x1fc
		LevelSettingsObject* m_settingsObject; // 0x200
		cocos2d::CCLabelBMFont* m_selectedSong; // 0x204
		cocos2d::CCArray* m_unkArray208; // 0x208
		cocos2d::CCArray* m_unkArray20c; // 0x20c
		LevelSettingsDelegate* m_delegate; // 0x210
		LevelEditorLayer* m_editorLayer; // 0x214
		cocos2d::CCArray* m_unkArray218; // 0x218
		cocos2d::CCArray* m_unkArray21c; // 0x21c
		CCMenuItemSpriteExtra* m_normalBtn; // 0x220
		CCMenuItemSpriteExtra* m_customBtn; // 0x224
		CCMenuItemSpriteExtra* m_selectCustomSongBtn; // 0x228
		CCMenuItemSpriteExtra* m_changeSongBtn; // 0x22c
		CustomSongWidget* m_customSongWidget; // 0x230

		void onGravityFlipped(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(LevelSettingsLayer*, CCObject*)>(base + 0x99b10)(this, sender);
		}
		static CCMenuItemToggler* createToggleButton(cocos2d::SEL_MenuHandler callback, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint point) {
			auto pRet = reinterpret_cast<CCMenuItemToggler * (__fastcall*)(cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint)>(base + 0x99830)(callback, toggled, menu, point);
			return pRet;
		}
	};
}

#endif

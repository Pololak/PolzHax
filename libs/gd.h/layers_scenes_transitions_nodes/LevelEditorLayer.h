#ifndef __LEVELEDITORLAYER_H__
#define __LEVELEDITORLAYER_H__

#include <gd.h>

namespace gd {

	class GJBaseGameLayer;
	class EditorUI;
	class GameObject;
	class GJGameLevel;
	class DrawGridLayer;
	class PlayerObject;
	class LevelSettingsObject;
	class StartPosObject;

	enum class UndoCommand {
		Delete = 1,
		New = 2,
		Paste = 3,
		DeleteMulti = 4,
		Transform = 5,
		Select = 6,
	};

	class UndoObject : public cocos2d::CCObject {
	public:
		static UndoObject* createWithArray(cocos2d::CCArray* objects, UndoCommand command) {
			return reinterpret_cast<UndoObject * (__fastcall*)(cocos2d::CCArray*, UndoCommand)>(base + 0x92970)(objects, command);
		}

		static UndoObject* createWithTransformObjects(cocos2d::CCArray* objects, UndoCommand nCommand) {
			return reinterpret_cast<UndoObject * (__fastcall*)(cocos2d::CCArray*, UndoCommand)>(base + 0x92770)(objects, nCommand);
		}
	};

	class LevelEditorLayer : public cocos2d::CCLayer {
	public:
		PAD(12)
		int m_firstVisibleSection; // 0x124
		int m_lastVisibleSection; // 0x128
		int m_groupIDFilter; // 0x12c
		StartPosObject* m_startPosObject; // 0x130
		float m_gameLayerScale; // 0x134
		OBB2D* m_obb2d; // 0x138
		OBB2D* m_obb2d_2; // 0x13c
		cocos2d::CCSprite* m_deathSprite; // 0x140
		PAD(0x10)
		GameObject* m_lastVehicle; // 0x154
		GameObject* m_dualPortal; // 0x158
		EditorUI* m_uiLayer; // 0x15c
		cocos2d::CCSprite* m_backgroundSprite; // 0x160
		cocos2d::CCSpriteBatchNode* m_objectBatchNode; // 0x164
		cocos2d::CCSpriteBatchNode* m_playerBatchNode; // 0x168
		cocos2d::CCArray* m_levelSections; // 0x16c
		cocos2d::CCArray* m_undoList; // 0x170
		cocos2d::CCArray* m_redoList; // 0x174
		cocos2d::CCPoint m_cameraPos; // 0x178
		int m_objectCount; // 0x180
		DrawGridLayer* m_gridLayer; // 0x184
		cocos2d::CCLayer* m_gameLayer; // 0x188
		GJGameLevel* m_level; // 0x18c
		LevelSettingsObject* m_levelSettings; // 0x190
		int m_levelDistance; // 0x194
		int m_playerState; // 0x198
		PlayerObject* m_player; // 0x19c
		PlayerObject* m_player2; // 0x1a0
		bool m_dualMode; // 0x1a4
		float m_minPortalY; // 0x1a8
		float m_maxPortalY; // 0x1ac
		float m_clkTimer; // 0x1b0
		std::string m_oldLevelString; // 0x1b4

		static LevelEditorLayer* create(GJGameLevel* level) {
			return reinterpret_cast<LevelEditorLayer * (__fastcall*)(GJGameLevel*)>(gd::base + 0x8c220)(level);
		}

		EditorUI* getEditorUI() {
			return from<EditorUI*>(this, 0x15C);
		}
		auto backgroundSprite() {
			return from<cocos2d::CCSprite*>(this, 0x160);
		}
		auto gameLayer() {
			return from<cocos2d::CCLayer*>(this, 0x188);
		}
		auto getLevelSettings() {
			return from<LevelSettingsObject*>(this, 0x190);
		}
		DrawGridLayer* getDrawGrid() {
			return from<DrawGridLayer*>(this, 0x184);
		}
		auto getPlaytestState() {
			return from<int>(this, 0x198);
		}
		auto getPlayer1() {
			return from<PlayerObject*>(this, 0x19c);
		}
		auto getLevelSections() {
			return from<cocos2d::CCArray*>(this, 0x16c);
		}
		void addToSection(GameObject* object) {
			reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, GameObject*)>(base + 0x8d220)(this, object);
		}
		auto getObjectBatchNode() {
			return from<cocos2d::CCSpriteBatchNode*>(this, 0x164);
		}
		GJGameLevel* getLevel() {
			return from<GJGameLevel*>(this, 0x18c);
		}

		PlayerObject* player1() {
			return from<PlayerObject*>(this, 0x19c);
		}

		PlayerObject* player2() {
			return from<PlayerObject*>(this, 0x1a0);
		}

		void removeObject(GameObject* obj, bool idk) {
			reinterpret_cast<void(__thiscall*)(
				LevelEditorLayer*, GameObject*, bool
				)>(
					base + 0x161cb0
					)(
						this, obj, idk
						);
		}

		int getNextFreeGroupID(cocos2d::CCArray* objs) {
			return reinterpret_cast<int(__thiscall*)(
				LevelEditorLayer*, cocos2d::CCArray*
				)>(
					base + 0x164ae0
					)(
						this, objs
						);
		}

		GameObject* addObjectFromString(std::string const& str) {
			auto ret = reinterpret_cast<GameObject * (__thiscall*)(
				LevelEditorLayer*, std::string
				)>(
					base + 0x160c80
					)(
						this, str
						);

			// no stack fix required... apparently

			return ret;
		}

		cocos2d::CCArray* getAllObjects() {
			return from<cocos2d::CCArray*>(this, 0x16c);
		}
		void removeAllObjects() {
			return reinterpret_cast<void(__thiscall*)(LevelEditorLayer*)>(base + 0x8d770)(this);
		}
		auto getLayerGroup() {
			return from<int>(this, 0x12C);
		}

		auto getFirstVisibleSection() {
			return from<int>(this, 0x124);
		}
		auto getLastVisibleSection() {
			return from<int>(this, 0x128);
		}

		//void addToUndoList(UndoObject* undoObj, bool add) {
		//    reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, UndoObject*, bool)>(base + 0x8ea60)(this, undoObj, add);
		//}

		void addToUndoList(UndoObject* undoObj, bool idk) {
			reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, UndoObject*, bool)>(base + 0x8ea60)(this, undoObj, idk);
		}

		void pushButton(int idk, bool idk2) {
			reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, int, bool)>(base + 0x90800)(this, idk, idk2);
		}

		void releaseButton(int idk, bool idk2) {
			reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, int, bool)>(base + 0x908f0)(this, idk, idk2);
		}

		/*auto gameLayer() {
			return from<CCLayer*>(this, 0x188);
		}

		auto getDrawGrid() {
			return from<gd::DrawGridLayer*>(this, 0x184);
		}
		auto getPlaytestState() {
			return from<int>(this, 0x198);
		}
		auto getPlayer1() {
			return from<gd::PlayerObject*>(this, 0x19c);
		}
		auto getLevelSettings() {
			return from<gd::LevelSettingsObject*>(this, 0x190);
		}
		auto backgroundSprite() {
			return from<cocos2d::CCSprite*>(this, 0x160);
		}*/
		//0x8d770

		GameObject* createObject(int id, cocos2d::CCPoint const& position) {
			return reinterpret_cast<GameObject * (__thiscall*)(LevelEditorLayer*, int, cocos2d::CCPoint)>(base + 0x8d150)(this, id, position);
		}

		int sectionForPos(float x)
		{
			int section = x / 100;
			if (section < 0)
				section = 0;
			return section;
		}
	};

}

#endif

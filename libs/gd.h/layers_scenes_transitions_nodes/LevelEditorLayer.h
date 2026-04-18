#ifndef __LEVELEDITORLAYER_H__
#define __LEVELEDITORLAYER_H__

#include <gd.h>

namespace gd {
	class StartPosObject;
	class OBB2D;
	class GameObject;
	class EditorUI;
	class DrawGridLayer;
	class GJGameLevel;
	class LevelSettingsObject;
	class PlayerObject;
	class UndoObject;

	class LevelEditorLayer : public cocos2d::CCLayer, LevelSettingsDelegate, GameplayDelegate {
	public:
		bool m_debugDraw; // 0x120
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

		cocos2d::CCArray* getAllObjects() {
			return reinterpret_cast<cocos2d::CCArray*(__fastcall*)(LevelEditorLayer*)>(base + 0x8e0a0)(this);
		}

		float getLastObjectX() { // custom func
			cocos2d::CCArray* objects = this->getAllObjects();
			float fVar5;
			float fVar6 = 0.f; // result

			for (int i = 0; i < objects->count(); i++) {
				auto object = reinterpret_cast<cocos2d::CCNode*>(objects->objectAtIndex(i));

				fVar5 = object->getPositionX();
				if ((fVar5 != fVar6) && (fVar5 > fVar6)) {
					fVar6 = object->getPositionX();
				}
			}

			return fVar6;
		}

		void pushButton(int p0, bool p1) {
			reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, int, bool)>(base + 0x90800)(this, p0, p1);
		}

		void releaseButton(int p0, bool p1) {
			reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, int, bool)>(base + 0x908f0)(this, p0, p1);
		}

		void setStartPosObject(StartPosObject* startPos) {
			if (this->m_startPosObject != startPos) {
				if (startPos) {
					startPos->retain();
				}
				if (this->m_startPosObject) {
					this->m_startPosObject->release();
				}
				this->m_startPosObject = startPos;
			}
		}

		void addToUndoList(UndoObject* p0, bool p1) {
			reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, UndoObject*, bool)>(base + 0x8ea60)(this, p0, p1);
		}

		void setupLevelStart(LevelSettingsObject* p0) {
			reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, LevelSettingsObject*)>(base + 0x912e0)(this, p0);
		}

		void playMusic() {
			reinterpret_cast<void(__fastcall*)(LevelEditorLayer*)>(base + 0x90d70)(this);
		}
	};
}

#endif

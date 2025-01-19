#pragma once
#include "pch.h"
#include "CheckPoint.h"
#include <vector>
#include <Windows.h>

namespace PlayLayer {
	inline bool(__thiscall* init)(gd::PlayLayer* self, gd::GJGameLevel* level);
	bool __fastcall init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level);

	inline bool(__thiscall* update)(gd::PlayLayer* self, float dt);
	void __fastcall update_H(gd::PlayLayer* self, void*, float dt);

	inline bool(__thiscall* resume)(CCLayer* self);
	bool __fastcall resume_H(CCLayer* self);

	inline bool(__thiscall* onQuit)(gd::PlayLayer* self);
	void __fastcall onQuit_H(gd::PlayLayer* self, void* edx);

	inline bool(__thiscall* resetLevel)(gd::PlayLayer* self);
	void __fastcall resetLevel_H(gd::PlayLayer* self);

	inline void(__thiscall* spawnPlayer2)(gd::PlayLayer* self);
	void __fastcall spawnPlayer2_H(gd::PlayLayer* self);

	inline bool(__thiscall* destroyPlayer)(gd::PlayLayer* self, gd::PlayerObject* player);
	void __fastcall destroyPlayer_H(gd::PlayLayer* self, void* edx, gd::PlayerObject* player);

	inline bool(__thiscall* pushButton)(gd::PlayLayer* self, int idk1, bool idk2);
	void __fastcall pushButton_H(gd::PlayLayer* self, void* edx, int idk1, bool idk2);

	inline void(__thiscall* releaseButton)(gd::PlayLayer*, int, bool);
	void __fastcall releaseButton_H(gd::PlayLayer* self, void* edx, int idk1, bool idk2);

	inline void(__thiscall* applyEnterEffect)(gd::PlayLayer*, gd::GameObject*);
	void __fastcall applyEnterEffect_H(gd::PlayLayer* self, void* edx, gd::GameObject* obj);

	inline void(__thiscall* togglePracticeMode)(gd::PlayLayer*, bool);
	void __fastcall togglePracticeModeH(gd::PlayLayer* self, void* edx, bool practice);

	inline void(__thiscall* createCheckpoint)(gd::PlayLayer*);
	void __fastcall createCheckpointH(gd::PlayLayer* self);

	inline void(__thiscall* removeLastCheckpoint)(gd::PlayLayer*);
	void __fastcall removeLastCheckpointH(gd::PlayLayer* self);

	inline void(__thiscall* levelComplete)(gd::PlayLayer*);
	void __fastcall levelCompleteH(gd::PlayLayer* self);

	inline void(__thiscall* removePlayer2)(gd::PlayLayer* self);
	void __fastcall removePlayer2H(gd::PlayLayer* self);

	inline void(__thiscall* markCheckpoint)(gd::PlayLayer*);
	void __fastcall markCheckpointH(gd::PlayLayer* self);

	void onNextStartPos();
	void onPrevStartPos();

	class Callback {
	public:
		void onNextPos(CCObject*);
		void onPrevPos(CCObject*);
	};

	void mem_init();

	//extern bool inPractice;
}

namespace PauseLayer {
	inline void(__thiscall* customSetup)(gd::PauseLayer* self);
	void __fastcall customSetupH(gd::PauseLayer* self);

	inline bool(__thiscall* onEdit)(CCLayer* self);
	void __fastcall onEdit_H(CCLayer* self);

	inline bool(__thiscall* onRestart)(CCObject* sender);
	void __fastcall onRestart_H(CCObject* sender);

	inline bool(__thiscall* onQuit)(CCObject* btn);
	void __fastcall onQuit_H(CCObject* btn);

	inline void(__thiscall* togglerMenu)();
	void __fastcall togglerMenuH(); // This thing is funny af

	inline bool(__thiscall* onProgressBar)(gd::PauseLayer*, CCObject*);
	void __fastcall onProgressBarH(gd::PauseLayer* self, void*, CCObject* obj);

	class Callback {
	public:
		void PercentageToggler(CCObject*);
		void onCopyLevel(CCObject*);
	};

	void mem_init();
}

namespace EndLevelLayer {
	inline void(__thiscall* customSetup)(gd::EndLevelLayer*);
	void __fastcall customSetup_H(gd::EndLevelLayer* self);

	class Callback {
	public:
		void onReset(CCObject*);
		void onHideLayer(CCObject*);
		void onShowLayer(CCObject*);
	};

	void mem_init();
}

namespace PlayerObject {
	inline void(__thiscall* updatePlayerFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerFrame_H(gd::PlayerObject* self, void* edx, int frameID);

	inline void(__thiscall* updatePlayerRollFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerRollFrame_H(gd::PlayerObject* self, void* edx, int frameID);

	inline void(__thiscall* runBallRotation2)(gd::PlayerObject*);
	void __fastcall runBallRotation2H(gd::PlayerObject* self);

	inline void(__thiscall* ringJump)(gd::PlayerObject*, gd::GameObject*);
	void __fastcall ringJumpH(gd::PlayerObject* self, void*, gd::GameObject* object);

	void mem_init();
}

namespace GameObject {
	inline void(__thiscall* setOpacity)(gd::GameObject*, GLubyte);
	void __fastcall setOpacityH(gd::GameObject* self, void* edx, GLubyte opacity);

	inline void(__thiscall* shouldBlendColor)(gd::GameObject*);
	void __fastcall shouldBlendColor_H(gd::GameObject* self, void* edx);

	inline int*(__thiscall* getEditorColor)(gd::GameObject*, cocos2d::ccColor4B);
	void __fastcall getEditorColorH(gd::GameObject* self, void* edx, cocos2d::ccColor4B color);

	inline void(__thiscall* activateObject)(gd::GameObject*);
	void __fastcall activateObjectH(gd::GameObject* self);

	void mem_init();
}

namespace CCCircleWave {
	inline void(__thiscall* draw)(gd::CCCircleWave*);
	void __fastcall drawH(gd::CCCircleWave* self);

	void mem_init();
}

namespace RingObject {
	inline void(__thiscall* spawnCircle)(CCObject*);
	void __fastcall spawnCircleH(CCObject* effect);
}
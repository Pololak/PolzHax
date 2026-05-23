#pragma once
#include "pch.h"

namespace PlayerObject {
	inline bool(__thiscall* init)(gd::PlayerObject*, int, gd::IconType, CCLayer*);
	bool __fastcall initH(gd::PlayerObject*, void*, int, gd::IconType, CCLayer*);

	inline void(__thiscall* updatePlayerFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* updatePlayerShipFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerShipFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* updatePlayerRollFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerRollFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* updatePlayerBirdFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerBirdFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* updatePlayerDartFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerDartFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* updateGlowColor)(gd::PlayerObject*);
	void __fastcall updateGlowColorH(gd::PlayerObject*);

	inline void(__thiscall* updatePlayerGlow)(gd::PlayerObject*);
	void __fastcall updatePlayerGlowH(gd::PlayerObject*);

	inline void(__thiscall* toggleFlyMode)(gd::PlayerObject*, bool);
	void __fastcall toggleFlyModeH(gd::PlayerObject*, void*, bool);

	inline void(__thiscall* toggleRollMode)(gd::PlayerObject*, bool);
	void __fastcall toggleRollModeH(gd::PlayerObject*, void*, bool);

	inline void(__thiscall* toggleBirdMode)(gd::PlayerObject*, bool);
	void __fastcall toggleBirdModeH(gd::PlayerObject*, void*, bool);

	inline void(__thiscall* toggleDartMode)(gd::PlayerObject*, bool);
	void __fastcall toggleDartModeH(gd::PlayerObject*, void*, bool);

	inline void(__thiscall* togglePlayerScale)(gd::PlayerObject*, bool);
	void __fastcall togglePlayerScaleH(gd::PlayerObject*, void*, bool);

	inline void(__thiscall* runBallRotation2)(gd::PlayerObject*);
	void __fastcall runBallRotation2H(gd::PlayerObject*);

	inline void(__thiscall* collidedWithObject)(gd::PlayerObject*, gd::GameObject*, CCRect);
	void __fastcall collidedWithObjectH(gd::PlayerObject*, void*, gd::GameObject*, CCRect);

	inline void(__thiscall* loadFromCheckpoint)(gd::PlayerObject*, gd::PlayerCheckpoint*);
	void __fastcall loadFromCheckpointH(gd::PlayerObject*, void*, gd::PlayerCheckpoint*);

	inline void(__thiscall* activateStreak)(gd::PlayerObject*);
	void __fastcall activateStreakH(gd::PlayerObject*);

	inline void(__thiscall* ringJump)(gd::PlayerObject*);
	void __fastcall ringJumpH(gd::PlayerObject*);

	inline void(__thiscall* updateJump)(gd::PlayerObject*, float);
	void __fastcall updateJumpH(gd::PlayerObject*, void*);

	inline void updateSwing(gd::PlayerObject* self, const float delta);

	void newPlayerExtraFrame(gd::PlayerObject*, const char*);
	void newVehicleExtraFrame(gd::PlayerObject*, const char*);

	void setCubeIcon(int);
	void setShipIcon(int);
	void setRollIcon(int);
	void setBirdIcon(int);
	void setDartIcon(int);

	void mem_init();
}
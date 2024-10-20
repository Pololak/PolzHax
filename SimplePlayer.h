#pragma once
#include "pch.h"

namespace SimplePlayer {
	inline bool(__thiscall* init)(gd::SimplePlayer*, int);
	bool __fastcall init_H(gd::SimplePlayer* self, void* edx, int frameID);

	void newExtraFrame(char const* extra);

	inline bool(__thiscall* updatePlayerFrame)(gd::SimplePlayer*, int, gd::IconType);
	void __fastcall updatePlayerFrame_H(gd::SimplePlayer* self, void* edx, int frameID, gd::IconType iconType);

	void mem_init();
}
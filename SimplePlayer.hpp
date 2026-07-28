#pragma once
#include "pch.h"

namespace SimplePlayer {
	inline bool(__thiscall* init)(gd::SimplePlayer*, int);
	bool __fastcall initH(gd::SimplePlayer*, void*, int);

	inline void(__thiscall* updatePlayerFrame)(gd::SimplePlayer*, int, gd::IconType);
	void __fastcall updatePlayerFrameH(gd::SimplePlayer*, void*, int, gd::IconType);

	void updateExtraSprite(gd::SimplePlayer*, std::string);

	void mem_init();
}
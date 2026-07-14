#pragma once
#include "pch.h"

namespace LoadingLayer {
	inline bool(__thiscall* init)(gd::LoadingLayer*, bool);
	bool __fastcall initH(gd::LoadingLayer*, void*, bool);

	inline const char*(__thiscall* getLoadingString)(gd::LoadingLayer*);
	const char* __fastcall getLoadingStringH(gd::LoadingLayer*);

	void mem_init();
}
#pragma once
#include "pch.h"

namespace LoadingLayer {
	inline const char*(__thiscall* getLoadingString)(gd::LoadingLayer*);
	const char* __fastcall getLoadingStringH(gd::LoadingLayer*);

	void mem_init();
}
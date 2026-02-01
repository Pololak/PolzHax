#pragma once
#include "pch.h"

namespace GJRotationControl {
	inline bool(__thiscall* init)(gd::GJRotationControl*);
	bool __fastcall initH(gd::GJRotationControl*);

	void mem_init();
}
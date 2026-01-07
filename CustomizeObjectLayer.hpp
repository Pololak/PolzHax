#pragma once
#include "pch.h"

namespace CustomizeObjectLayer {
	inline bool(__thiscall* init)(gd::CustomizeObjectLayer*, gd::GameObject*, CCArray*);
	bool __fastcall initH(gd::CustomizeObjectLayer*, void*, gd::GameObject*, CCArray*);

	inline void(__thiscall* destructor)(gd::CustomizeObjectLayer*);
	void __fastcall destructorH(gd::CustomizeObjectLayer*);

	void mem_init();
}
#pragma once
#include "pch.h"

namespace CustomizeObjectLayer {
	inline bool(__thiscall* init)(gd::CustomizeObjectLayer*, gd::GameObject*, CCArray*);
	bool __fastcall initH(gd::CustomizeObjectLayer*, void*, gd::GameObject*, CCArray*);

	inline void(__thiscall* onSelectColor)(gd::CustomizeObjectLayer*, CCObject*);
	void __fastcall onSelectColorH(gd::CustomizeObjectLayer*, void*, CCObject*);

	void mem_init();
}
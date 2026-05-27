#pragma once
#include "pch.h"

namespace DrawGridLayer {
	inline void(__thiscall* draw)(gd::DrawGridLayer*);
	void __fastcall drawH(gd::DrawGridLayer*);

	inline void(__thiscall* destructor)(gd::DrawGridLayer*);
	void __fastcall destructorH(gd::DrawGridLayer*);

	inline void(__thiscall* loadTimeMarkers)(gd::DrawGridLayer*, gd::string);
	void __fastcall loadTimeMarkersH(gd::DrawGridLayer*, void*, gd::string);

	void mem_init();
}
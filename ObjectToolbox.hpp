#pragma once
#include "pch.h"

namespace ObjectToolbox {
	inline void(__stdcall* gridNodeSizeForKey)(int);
	void __stdcall gridNodeSizeForKeyH(int);

	void mem_init();
}
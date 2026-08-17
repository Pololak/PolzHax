#include "ObjectToolbox.hpp"
#include "Setting.hpp"

void __stdcall ObjectToolbox::gridNodeSizeForKeyH(int id) {
	float gridSize = setting().m_customEditorGridSize;

	if (gridSize < 1.f || roundf(gridSize) == 30.f) {
		return ObjectToolbox::gridNodeSizeForKey(id);
	}

	__asm {
		movss xmm0, gridSize
		add esp, 0x4
	}
}

void ObjectToolbox::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xcfc90), ObjectToolbox::gridNodeSizeForKeyH, reinterpret_cast<void**>(&ObjectToolbox::gridNodeSizeForKey));
}
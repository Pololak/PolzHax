#include "ObjectToolbox.hpp"
#include "Setting.hpp"

float __fastcall ObjectToolbox::gridNodeSizeForKeyH(gd::ObjectToolbox* self, void*, int id) {
	auto size = setting().m_customEditorGridSize;
	if (size < 1 || roundf(size) == 30) {
		return ObjectToolbox::gridNodeSizeForKey(self, id);
	}
	return size;
}

void ObjectToolbox::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xcfc90), ObjectToolbox::gridNodeSizeForKeyH, reinterpret_cast<void**>(&ObjectToolbox::gridNodeSizeForKey));
}
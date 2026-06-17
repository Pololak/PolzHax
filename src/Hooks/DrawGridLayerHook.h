#pragma once
#include "../include.h"

namespace DrawGridLayerHook {
	inline void(*draw)(DrawGridLayer*);
	void drawH(DrawGridLayer*);

	inline void(*destructor)(DrawGridLayer*);
	void destructorH(DrawGridLayer*);

	inline void(*loadTimeMarkers)(DrawGridLayer*, std::string);
	void loadTimeMarkersH(DrawGridLayer*, std::string);

	void mem_init();
}
#ifndef __STARTPOSOBJECT_H__
#define __STARTPOSOBJECT_H__

#include <gd.h>

namespace gd {
	class LevelSettingsObject;

	class StartPosObject : public GameObject {
	public:
		LevelSettingsObject* m_settings; // 0x328
	};
}

#endif // !__STARTPOSOBJECT_H__

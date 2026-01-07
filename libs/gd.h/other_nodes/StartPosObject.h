#ifndef __STARTPOSOBJECT_H__
#define __STARTPOSOBJECT_H__

#include <gd.h>

namespace gd {
	class LevelSettingsObject;

	class StartPosObject : public GameObject {
	public:
		LevelSettingsObject* m_settings;
	};
}

#endif // !__STARTPOSOBJECT_H__

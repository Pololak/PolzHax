#ifndef __SETTINGSCOLOROBJECT_H__
#define __SETTINGSCOLOROBJECT_H__

#include <gd.h>

namespace gd {
	//class LevelSettingsObject;

	class SettingsColorObject : public cocos2d::CCNode {
	public:
		cocos2d::ccColor3B m_color;
		bool m_blending;
		int m_custom;
	};

	static_assert(sizeof(SettingsColorObject) == 0xF0, "size wrong");
}


#endif // !__SETTINGSCOLOROBJECT_H__

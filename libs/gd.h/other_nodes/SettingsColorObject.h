#ifndef __SETTINGSCOLOROBJECT_H__
#define __SETTINGSCOLOROBJECT_H__

#include <gd.h>

namespace gd {
	class SettingsColorObject : public cocos2d::CCNode {
	public:
		cocos2d::ccColor3B m_color;
		bool m_blending;
		int m_playerColor; // m_custom
	};
}

#endif // !__SETTINGSCOLOROBJECT_H__

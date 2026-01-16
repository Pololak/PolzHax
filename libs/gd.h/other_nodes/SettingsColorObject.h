#ifndef __SETTINGSCOLOROBJECT_H__
#define __SETTINGSCOLOROBJECT_H__

#include <gd.h>

namespace gd {
	class SettingsColorObject : public cocos2d::CCNode {
	public:
		cocos2d::ccColor3B m_color; // 0xe8
		bool m_blending; // 0xec
		int m_playerColor; // 0xf0
	};
}

#endif // !__SETTINGSCOLOROBJECT_H__

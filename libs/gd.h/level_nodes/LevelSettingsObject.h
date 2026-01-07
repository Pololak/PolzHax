#ifndef __LEVELSETTINGSOBJECT_H__
#define __LEVELSETTINGSOBJECT_H__

#include <gd.h>

namespace gd {
	class SettingsColorObject;
	class GJGameLevel;

	class LevelSettingsObject : public cocos2d::CCNode {
	public:
		SettingsColorObject* m_backgroundColor; // 0xe8
		SettingsColorObject* m_groundColor; // 0xec
		SettingsColorObject* m_lineColor; // 0xf0
		SettingsColorObject* m_objectColor; // 0xf4
		SettingsColorObject* m_3DLineColor; // 0xf8
		SettingsColorObject* m_customColor01; // 0xfc
		SettingsColorObject* m_customColor02; // 0x100
		SettingsColorObject* m_customColor03; // 0x104
		SettingsColorObject* m_customColor04; // 0x108
		int m_startMode; // 0x10c
		int m_startSpeed; // 0x110
		bool m_startMini; // 0x114
		bool m_startDual; // 0x115
		bool m_twoPlayerMode; // 0x116
		float m_songOffset; // 0x118
		bool m_fadeIn; // 0x11c
		bool m_fadeOut; // 0x11d
		int m_backgroundIndex; // 0x120
		int m_groundIndex; // 0x124
		bool m_startsWithStartPos; // 0x128
		bool m_isFlipped; // 0x129
		GJGameLevel* m_level; // 0x12c
		std::string m_guidelineString; // 0x130
		bool m_songChanged; // 0x148
	};
}

#endif // !__LEVELSETTINGSOBJECT_H__

#ifndef __LEADERBOARDSLAYER_H__
#define __LEADERBOARDSLAYER_H__

#include <gd.h>

namespace gd {
	class GJListLayer;
	class CCMenuItemToggler;
	class CCMenuItemSpriteExtra;
	class LoadingCircle;
	class TextArea;

	enum class LeaderboardState {
		Default = 0,
		Top100 = 1,
		Global = 2,
		Creators = 3,
		TopWeek = 4
	};

	class LeaderboardsLayer : public cocos2d::CCLayer, LeaderboardManagerDelegate {
	public:
		GJListLayer* m_list; // 0x11c
		PAD(0x4)
		LeaderboardState m_leaderboardState; // 0x124
		CCMenuItemToggler* m_top100Tab; // 0x128
		CCMenuItemToggler* m_globalTab; // 0x12c
		CCMenuItemToggler* m_creatorsTab; // 0x130
		CCMenuItemToggler* m_topWeekTab; // 0x134
		CCMenuItemSpriteExtra* m_infoButton; // 0x138
		LoadingCircle* m_loadingCircle; // 0x13c
		TextArea* m_noInternet; // 0x140
	};
}

#endif // !__LEADERBOARDSLAYER_H__

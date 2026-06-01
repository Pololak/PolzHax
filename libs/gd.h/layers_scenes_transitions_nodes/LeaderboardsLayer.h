#ifndef __LEADERBOARDSLAYER_H__
#define __LEADERBOARDSLAYER_H__

#include <gd.h>

namespace gd {
	class GJListLayer;
	class CCMenuItemToggler;
	class CCMenuItemSpriteExtra;
	class LoadingCircle;
	class TextArea;
	enum class LeaderboardState;

	class LeaderboardsLayer : public cocos2d::CCLayer, LeaderboardManagerDelegate {
	public:
		GJListLayer* m_list; // 0x11c
		cocos2d::CCArray* m_userScores; // 0x120
		LeaderboardState m_leaderboardState; // 0x124
		CCMenuItemToggler* m_top100Tab; // 0x128
		CCMenuItemToggler* m_globalTab; // 0x12c
		CCMenuItemToggler* m_creatorsTab; // 0x130
		CCMenuItemToggler* m_topWeekTab; // 0x134
		CCMenuItemSpriteExtra* m_infoButton; // 0x138
		LoadingCircle* m_loadingCircle; // 0x13c
		TextArea* m_noInternet; // 0x140

		void selectLeaderboard(LeaderboardState state) {
			reinterpret_cast<void(__thiscall*)(LeaderboardsLayer*, LeaderboardState)>(base + 0x88b70)(this, state);
		}
	};
}

#endif // !__LEADERBOARDSLAYER_H__

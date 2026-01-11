#ifndef __ACHIEVEMENTNOTIFIER_H__
#define __ACHIEVEMENTNOTIFIER_H__

#include <gd.h>

namespace gd {

	class AchievementBar;

	class AchievementNotifier : public cocos2d::CCNode {
	public:
		cocos2d::CCScene* m_nextScene; // 0xe8
		cocos2d::CCArray* m_achievementBarArray; // 0xec
		AchievementBar* m_activeAchievementBar; // 0xf0

		static AchievementNotifier* sharedState() {
			return reinterpret_cast<AchievementNotifier*(__stdcall*)()>(base + 0x5bc0)();
		}

		void showNextAchievement() {
			reinterpret_cast<void(__thiscall*)(AchievementNotifier*)>(base + 0x5c90)(this);
		}

		void notifyAchievement(char const* title, char const* desc, char const* icon) {
			auto achievement = AchievementBar::create(title, desc, icon);
			this->m_achievementBarArray->addObject(achievement);
			if (this->m_activeAchievementBar == nullptr) {
				this->showNextAchievement();
			}
		}
	};
}

#endif
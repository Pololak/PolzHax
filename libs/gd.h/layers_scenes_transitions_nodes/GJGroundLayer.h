#ifndef __GJGROUNDLAYER_H__
#define __GJGROUNDLAYER_H__

#include <gd.h>

namespace gd {
	class GJGroundLayer : public cocos2d::CCLayer {
	public:
		cocos2d::CCSprite* m_groundSprite; // 0x118
		float m_groundWidth; // 0x11c
		cocos2d::CCSprite* m_line; // 0x120
		bool m_isActive; // 0x124

		void showGround() {
			this->m_isActive = true;
		}

		void fadeOutGround() {
			this->m_isActive = false;
		}

		void deactivateGround() {
			this->stopAllActions();
			this->m_isActive = false;
		}

		void fadeInFinished() {
			this->m_isActive = true;
		}

		void fadeInGround(float delay) {
			reinterpret_cast<void(__thiscall*)(GJGroundLayer*, float)>(base + 0x81620)(this, delay);
		}

		void hideShadows() {
			if (auto leftShadow = this->getChildByTag(0)) leftShadow->setVisible(false);
			if (auto rightShadow = this->getChildByTag(1)) rightShadow->setVisible(false);
		}
	};
}

#endif // !__GJGROUNDLAYER_H__

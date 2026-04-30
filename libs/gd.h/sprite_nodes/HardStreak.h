#ifndef __HARDSTREAK_H__
#define __HARDSTREAK_H__

#include <gd.h>

namespace gd {
	class HardStreak : public cocos2d::CCDrawNode {
	public:
		PAD(16)
		cocos2d::CCArray* m_pointArray; // 0x118
		cocos2d::CCPoint m_currentPoint; // 0x11c
		float m_waveSize; // 0x120
		float m_pulseSize; // 0x124
		bool m_disableDual; // 0x128

		void updateStroke(float dt) {
			reinterpret_cast<void(__fastcall*)(HardStreak*, float)>(base + 0x833e0)(this, dt);
		}
	};
}

#endif // !__HARDSTREAK_H__

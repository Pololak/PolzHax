#ifndef __HARDSTREAK_H__
#define __HARDSTREAK_H__

#include <gd.h>

namespace gd {
	class HardStreak : public cocos2d::CCDrawNode {
		public:
			PAD(16)
			cocos2d::CCArray* m_pointArray; // 0x118
			cocos2d::CCPoint m_currentPoint; // 0x11c
			float m_waveSize; // 0x124
			float m_pulseSize; // 0x128
			bool m_disableDual; // 0x12c

			cocos2d::CCPoint currentPoint() {
				return from<cocos2d::CCPoint>(this, 0x11c);
			}

			auto getStrokeScale() {
				return from<float>(this, 0x124);
			} // m_waveSize

			auto& pulseSize() {
				return from<float>(this, 0x128);
			}
			void addPoint(cocos2d::CCPoint p0) {
				return reinterpret_cast<void(__thiscall*)(HardStreak*, cocos2d::CCPoint)>(base + 0x83a60)(this, p0);
			}
	}; // size = 0x134
}

#endif
#ifndef __HARDSTREAK_H__
#define __HARDSTREAK_H__

#include "../include/gd.h"

class HardStreak : public cocos2d::CCDrawNode {
public:
	PAD(16)
	cocos2d::CCArray* m_pointArray;
	cocos2d::CCPoint m_currentPoint;
	float m_waveSize;
	float m_pulseSize;
	bool m_disableDual;

	void updateStroke(float dt);
};

#endif

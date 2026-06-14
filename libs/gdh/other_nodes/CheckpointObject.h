#ifndef __CHECKPOINTOBJECT_H__
#define __CHECKPOINTOBJECT_H__

#include "../include/gd.h"

class GameObject;
class ColorAction;

class PlayerCheckpoint : public cocos2d::CCNode {
public:
	cocos2d::CCPoint m_playerPos;	// 0xe8
	float m_playerYVel;				// 0xf0
	bool m_flipGravity;				// 0xf4
	bool m_flyMode;					// 0xf5
	bool m_rollMode;				// 0xf6
	bool m_birdMode;				// 0xf7
	bool m_dartMode;				// 0xf8
	bool m_canJump;					// 0xf9
	int m_ghostType;				// 0xfc
	bool m_isScaled;				// 0x100
	float m_timeMod;				// 0x104
};

class CheckpointObject : public cocos2d::CCNode {
public:
	GameObject* m_object;				// 0xe8
	PlayerCheckpoint* m_playerCheck01;	// 0xec
	PlayerCheckpoint* m_playerCheck02;	// 0xf0
	bool m_dualMode;					// 0xf4
	bool m_isFlipped;					// 0xf5
	cocos2d::CCPoint m_cameraPos;		// 0xf8
	ColorAction* m_bgColorAction;		// 0x100
	ColorAction* m_lineColorAction;		// 0x104
	ColorAction* m_objColorAction;		// 0x108
	ColorAction* m_gColorAction;		// 0x10c
	ColorAction* m_dLineColorAction;	// 0x110
	ColorAction* m_customColor01Action; // 0x114
	ColorAction* m_customColor02Action; // 0x118
	ColorAction* m_customColor03Action; // 0x11c
	ColorAction* m_customColor04Action; // 0x120
	EnterEffect m_activeEnterEffect;	// 0x124
	GameObject* m_portalObject;			// 0x128
	double m_timeStamp;					// 0x12c
};

#endif // !__CHECKPOINTOBJECT_H__

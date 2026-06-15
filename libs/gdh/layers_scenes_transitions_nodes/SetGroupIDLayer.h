#ifndef __SETGROUPIDLAYER_H__
#define __SETGROUPIDLAYER_H__

#include "../include/gd.h"

class GameObject;

class SetGroupIDLayer : public FLAlertLayer {
public:
	GameObject* m_targetObject; // 0x1bc
	cocos2d::CCArray* m_targetObjects; // 0x1c0
	cocos2d::CCLabelBMFont* m_groupLabel; // 0x1c4
	int m_groupIndex; // 0x1c8

	void updateGroupID();
	void updateLabel();
};

#endif
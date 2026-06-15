#ifndef __CCINDEXPATH_H__
#define __CCINDEXPATH_H__

#include "../include/gd.h"

class CCIndexPath : public cocos2d::CCObject {
protected:
	int m_nUnknown1;
	int m_nUnknown2;
public:
	static CCIndexPath* create(unsigned int idk1, int idk2);
};

#endif
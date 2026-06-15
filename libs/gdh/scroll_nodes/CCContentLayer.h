#ifndef __CCCONTENTLAYER_H__
#define __CCCONTENTLAYER_H__

#include "../include/gd.h"

class CCContentLayer : public cocos2d::CCLayerColor {
public:
	static CCContentLayer* create(const cocos2d::ccColor4B& color, float width, float height);
};

#endif
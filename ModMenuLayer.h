#pragma once
#include "pch.h"

class ModMenuLayer : public CCLayer {
protected:
	virtual bool init();
	virtual void keyBackClicked();
	void backButtonCallback(CCObject*);
public:
	static ModMenuLayer* create();
	void switchCustomLayerButton(CCObject*);
};

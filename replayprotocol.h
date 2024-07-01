#pragma once
#include "pch.h"

class ReplayProtocol : public CCLayer {
protected:
public:
	virtual void keyBackClicked();
	void backButton();
	bool init();
	static ReplayProtocol* create();
	void replayCallback(CCObject*);
};

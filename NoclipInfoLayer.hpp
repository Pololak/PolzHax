#pragma once
#include "pch.h"

class NoclipInfoLayer : public gd::FLAlertLayer {
protected:
	CCLabelBMFont* m_pageLabel;
	int m_currentPage;
	CCArray* m_runLabels;
	CCNode* m_pageNode;

	virtual bool init();
	virtual void keyBackClicked();
	void onClose(CCObject*);
	void updatePageLabel();
	void loadPage();
	void onNext(CCObject*);
	void onPrev(CCObject*);

public:
	static NoclipInfoLayer* create() {
		NoclipInfoLayer* ret = new NoclipInfoLayer();
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};
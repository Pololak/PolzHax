#ifndef __BOOMSCROLLLAYER_H__
#define __BOOMSCROLLLAYER_H__

#include "../include/gd.h"

class BoomScrollLayer : public cocos2d::CCLayer {
public:
	cocos2d::CCArray* m_dotsArray;
	int m_animatingToPage;
	float m_touchX;
	float m_animateSpeed;
	bool m_bUnknown1;
	void* m_dynamicScrollDelegate;
	cocos2d::CCArray* m_dynamicScrollPages;
	bool m_dynamic;
	int m_touchHasMoved;
	bool m_bUnknown3;
	cocos2d::CCTouch* m_currentTouch;
	cocos2d::CCArray* m_pages;
	double m_touchTimer;
	cocos2d::CCPoint m_pUnknown4;
	cocos2d::CCLayer* m_mainLayer;
	cocos2d::CCRect m_scrollArea;
	float m_minTouchSpeed;
	float m_touchSpeedFast;
	float m_touchSpeedMid;
	void* m_delegate;
	bool m_movingToPage;
	float m_minimumTouchLengthToSlide;
	float m_minimumTouchLengthToChangePage;
	float m_marginOffset;
	bool m_stealTouches;
	bool m_showPagesIndicator;
	cocos2d::CCPoint m_pagesIndicatorPosition;
	cocos2d::ccColor4B m_pagesIndicatorSelectedColor;
	cocos2d::ccColor4B m_pagesIndicatorNormalColor;
	int m_currentScreen;
	float m_pagesWidthOffset;
	void* m_unusedPages;
};

#endif
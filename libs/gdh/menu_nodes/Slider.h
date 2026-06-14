#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "../include/gd.h"

class Slider;

class SliderThumb : public cocos2d::CCMenuItemImage {
public:
    float getValue() {
        return (this->getPosition().x + this->getScale() * 200.0 * 0.5) / (this->getScale() * 200.0);
    }
};

class SliderTouchLogic : public cocos2d::CCMenu {
public:
    float m_unknownUnused;
    float m_length;
    SliderThumb* m_thumb;
    Slider* m_slider;
    bool m_activateThumb;
    cocos2d::CCPoint m_position;
};

class Slider : public cocos2d::CCLayer {
public:
	SliderTouchLogic* m_touchLogic; // 0x118
	cocos2d::CCSprite* m_sliderBar; // 0x11c
	float m_width; // 0x120
	float m_height; // 0x124

	static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler callback, char const* bar, char const* groove, char const* thumb, char const* thumbSelected, float scale);

	static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler callback, float scale) {
		return Slider::create(target, callback, "sliderBar.png", "slidergroove.png", "sliderthumb.png", "sliderthumbsel.png", scale);
	}

	static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler callback) {
		return Slider::create(target, callback, 1.0f);
	}

	void setValue(float value);

	void updateBar();
};

#endif
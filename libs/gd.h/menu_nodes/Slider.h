#ifndef __SLIDER_H__
#define __SLIDER_H__

#include <gd.h>

namespace gd {
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
		SliderTouchLogic* m_touchLogic;
		cocos2d::CCSprite* m_groove;
		float m_width;
		float m_height;
	};
}

#endif // !__SLIDER_H__

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
		SliderTouchLogic* m_touchLogic; // 0x118
		cocos2d::CCSprite* m_groove; // 0x11c
		float m_width; // 0x120
		float m_height; // 0x124

		static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler callback, char const* bar, char const* groove, char const* thumb, char const* thumbSelected, float scale) {
			auto ret = reinterpret_cast<Slider * (__fastcall*)(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float)>(base + 0x1cff0)(target, callback, bar, groove, thumb, thumbSelected, scale);

			__asm add esp, 0x14

			return ret;
		}

		static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler callback, float scale) {
			return Slider::create(target, callback, "sliderBar.png", "slidergroove.png", "sliderthumb.png", "sliderthumbsel.png", scale);
		}

		static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler callback) {
			return Slider::create(target, callback, 1.0f);
		}

		void setValue(float value) {
			__asm movss xmm1, value

			reinterpret_cast<void(__fastcall*)(Slider*)>(base + 0x1d270)(this);
		}
	};
}

#endif // !__SLIDER_H__

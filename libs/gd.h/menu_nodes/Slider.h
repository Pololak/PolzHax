#ifndef SLIDER_H
#define SLIDER_H

#include <gd.h>

namespace gd {

    class Slider;

    class SliderThumb : public cocos2d::CCMenuItemImage {
        public:
            float m_fLength;
            bool m_bVertical;

        public:
            void setValue(float val) {
                __asm movss xmm1, val

                reinterpret_cast<void(__fastcall*)(
                    SliderThumb*
                )>( base + 0x2e1b0 )( this );
            }

            float getValue() {
                return (this->getPosition().x + this->getScale() * 200.0f * 0.5f) / (this->getScale() * 200.0f);
            }
    };

    class SliderTouchLogic : public cocos2d::CCMenu {
        public:
            PAD(0x4)
            float m_fLength;
            SliderThumb* m_pThumb;
            Slider* m_pSlider;
            bool m_bUnknown;
            PAD(0x8)
            bool m_bVertical;

        public:
            SliderThumb* getThumb() {
                return from<SliderThumb*>(this, 0x130);
            }
    };

    class Slider : public cocos2d::CCLayer {
        public:
            SliderTouchLogic* m_touchLogic;
            cocos2d::CCSprite* m_groove;
            float m_width;
            float m_height;

        public:
            SliderTouchLogic* getTouchLogic() {
                return from<SliderTouchLogic*>(this, 0x118);
            }
            SliderThumb* getThumb() {
                return from<SliderThumb*>((this->getTouchLogic()), 0x13C);
            }
            float getValue() {
                return from<float>((this->getThumb()), 0x2C);
            }
            void setValue(float newVal) {
                from<float>((this->getThumb()), 0x2C) = newVal;
            }

            auto getBarSprite() {
                return from<cocos2d::CCSprite*>(this, 0x11c);
            }

            auto getGrooveSprite() {
                return from<cocos2d::CCSprite*>(getBarSprite(), 0xAC);
            }

            void setBarVisibility(bool v) {
                this->getBarSprite()->setVisible(v);
            }

			void updateBar() {
				reinterpret_cast<void(__fastcall*)(Slider*)>(base + 0x1d310)(this);
            }

            static Slider* create(
                cocos2d::CCNode* target,
                cocos2d::SEL_MenuHandler click,
                const char* grooveSpr,
                const char* thumbSpr,
                const char* thumbSprSel,
                float scale
            ) {
                auto ret = reinterpret_cast<Slider*(__fastcall*)(
                    cocos2d::CCNode*, cocos2d::SEL_MenuHandler, const char*,
                    const char*, const char*, const char*, float
                )>(
                    base + 0x2e6e0
                )(
                    target, click, nullptr, grooveSpr, thumbSpr, thumbSprSel, scale
                );

                __asm add esp, 0x14

                return ret;
            }

            static Slider* create(
                cocos2d::CCNode* target,
                cocos2d::SEL_MenuHandler click,
                float scale
            ) {
                return create(target, click, "slidergroove.png", "sliderthumb.png", "sliderthumbsel.png", scale);
            }

        // todo
    };

}

#endif

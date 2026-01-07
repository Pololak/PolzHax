#ifndef SLIDER_H
#define SLIDER_H

#include <gd.h>

namespace gd {

    class Slider;

    class SliderThumb : public cocos2d::CCMenuItemImage {
        protected:
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
                return (this->getPosition().x + this->getScale() * 200.0 * 0.5) / (this->getScale() * 200.0);
            }
    };

    class SliderTouchLogic : public cocos2d::CCMenu {
        protected:
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
        protected:
            SliderTouchLogic* m_pTouchLogic;
            cocos2d::CCSprite* m_pSliderBar;
            cocos2d::CCSprite* m_pGroove;
            float m_fUnknown;
            float m_fHeight;

        public:
            SliderTouchLogic* getTouchLogic() {
                return from<SliderTouchLogic*>(this, 0x118);
            }

            void setValue(float val) {
                this->m_pTouchLogic->getThumb()->setValue(val);
            }

            float getValue() {
                return this->getTouchLogic()->getThumb()->getValue();
            }

            void setBarVisibility(bool v) {
                this->m_pSliderBar->setVisible(v);
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

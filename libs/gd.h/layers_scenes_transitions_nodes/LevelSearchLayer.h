#ifndef __LEVELSEARCHLAYER_H__
#define __LEVELSEARCHLAYER_H__

#include <gd.h>

class SearchButton;

namespace gd {
    class LevelSearchLayer : public cocos2d::CCLayer {
        public:
            static LevelSearchLayer* create() {
                return reinterpret_cast<LevelSearchLayer*(__fastcall*)()>(
                    base + 0x17d9c0
                )();
            }
    };
    
    class SearchButton : public cocos2d::CCSprite {
    public:
        static auto create(const char* background, const char* text, const char* icon) {
            auto ret = reinterpret_cast<SearchButton * (__fastcall*)(const char*, const char*, const char*)>
                (base + 0xa3180)(background, text, icon);
            __asm add esp, 0x8;
            return ret;
        }
    };
}

#endif

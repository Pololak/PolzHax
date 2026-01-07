#ifndef __OBB2D_H__
#define __OBB2D_H__

#include <gd.h>

namespace gd {
    
    class OBB2D : public cocos2d::CCNode {
    public:

        virtual cocos2d::CCRect* getBoundingRect(cocos2d::CCRect* rect) {
            return reinterpret_cast<cocos2d::CCRect * (__thiscall*)(OBB2D*, cocos2d::CCRect*)>(base + 0x722a0)(this, rect);
        }

        cocos2d::CCPoint getPoint1() {
            auto f1 = from<float>(this, 0xE8);
            auto f2 = from<float>(this, 0xEC);
            cocos2d::CCPoint point = { f1, f2 };
            return point;
        }

        cocos2d::CCPoint getPoint2() {
            auto f1 = from<float>(this, 0xF0);
            auto f2 = from<float>(this, 0xF4);
            cocos2d::CCPoint point = { f1, f2 };
            return point;
        }

        cocos2d::CCPoint getPoint3() {
            auto f1 = from<float>(this, 0xF8);
            auto f2 = from<float>(this, 0xFC);
            cocos2d::CCPoint point = { f1, f2 };
            return point;
        }

        cocos2d::CCPoint getPoint4() {
            auto f1 = from<float>(this, 0x100);
            auto f2 = from<float>(this, 0x104);
            cocos2d::CCPoint point = { f1, f2 };
            return point;
        }

        cocos2d::CCPoint getPoint5() {
            auto f1 = from<float>(this, 0x108);
            auto f2 = from<float>(this, 0x10C);
            cocos2d::CCPoint point = { f1, f2 };
            return point;
        }

        cocos2d::CCPoint getPoint6() {
            auto f1 = from<float>(this, 0x110);
            auto f2 = from<float>(this, 0x114);
            cocos2d::CCPoint point = { f1, f2 };
            return point;
        }

        cocos2d::CCPoint getPoint7() {
            auto f1 = from<float>(this, 0x118);
            auto f2 = from<float>(this, 0x11C);
            cocos2d::CCPoint point = { f1, f2 };
            return point;
        }

        cocos2d::CCPoint getPoint8() {
            auto f1 = from<float>(this, 0x120);
            auto f2 = from<float>(this, 0x124);
            cocos2d::CCPoint point = { f1, f2 };
            return point;
        }

        cocos2d::CCPoint getCenterPoint() {
            auto f1 = from<float>(this, 0x158);
            auto f2 = from<float>(this, 0x15C);
            cocos2d::CCPoint point = { f1, f2 };
            return point;
        }

        float getMinX() {
            return from<float>(this, 0xE8);
        }
        float getMinY() {
            return from<float>(this, 0xEC);
        }
        float getMaxX() {
            return from<float>(this, 0xF0);
        }
        float getMaxY() {
            return from<float>(this, 0xFC);
        }
    };

}

#endif

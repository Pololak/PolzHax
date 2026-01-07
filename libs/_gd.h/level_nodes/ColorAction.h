#ifndef __COLORACTION_H__
#define __COLORACTION_H__

#include <gd.h>

namespace gd {

    class ColorAction : public cocos2d::CCNode {
    public:
        cocos2d::ccColor3B m_color;
        bool m_blending;
        int m_custom;
    };

    static_assert(sizeof(ColorAction) == 0xf0, "size wrong");
}

#endif
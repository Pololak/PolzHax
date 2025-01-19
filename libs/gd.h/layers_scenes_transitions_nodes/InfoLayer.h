#ifndef __INFOLAYER_H__
#define __INFOLAYER_H__

#include <gd.h>

namespace gd {
    class GJGameLevel;
    class GJUserScore;

    class InfoLayer : public FLAlertLayer, FLAlertLayerProtocol {
    public:
        static InfoLayer* create(gd::GJGameLevel* level, gd::GJUserScore* score) {
            return reinterpret_cast<InfoLayer*(__fastcall*)(gd::GJGameLevel*, gd::GJUserScore*)>(
                gd::base + 0x14F4F0
            )(level, score);
        }

        void loadPage(int page) {
            reinterpret_cast<void(__thiscall*)(InfoLayer*, int)>(base + 0x85e30)(this, page);
        }
    };

}

#endif

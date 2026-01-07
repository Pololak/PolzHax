#ifndef __MOREOPTIONSLAYER_H__
#define __MOREOPTIONSLAYER_H__

#include <gd.h>
class FLAlertLayer;

namespace gd {
    class MoreOptionsLayer : public FLAlertLayer {
    public:
        void addToggle(const char* name, const char* key, const char* info) {
            reinterpret_cast<void* (__thiscall*)(MoreOptionsLayer*, const char*, const char*, const char*)>(base + 0xd1d30)(this, name, key, info);
        }
    };
}

#endif
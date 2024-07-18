#ifndef __LOCALLEVELMANAGER_H__
#define __LOCALLEVELMANAGER_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {
    class LocalLevelManager : GManager {
    public:

        static auto sharedState() {
            return reinterpret_cast<LocalLevelManager * (__stdcall*)()>(base + 0xac180)();
        }
        //static GameManager* sharedState() {
        //    return reinterpret_cast<GameManager * (__stdcall*)()>(
        //        base + 0x66170 //0xC4A50 original gd
        //        )();
        //}

        int updateLevelOrder() {
            return reinterpret_cast<bool(__thiscall*)(LocalLevelManager*)>(
                base + 0xacce0
                )(this);
        }
        auto getLocalLevels() {
            return from<cocos2d::CCArray*>(this, 0x108);
        }



        /*inline static LocalLevelManager* get() {
            return LocalLevelManager::sharedState();
        }
        bool init() = mac 0x2384e0;
        virtual void encodeDataTo(DS_Dictionary* data) = 0x18e040;
        virtual void dataLoaded(DS_Dictionary* data) = 0x18e070;

        cocos2d::CCDictionary* getAllLevelsInDict() = 0x18d7c0;

        PAD = 0x1C;*/

        cocos2d::CCDictionary* m_loadData;
        cocos2d::CCDictionary* m_levelData;
        cocos2d::CCArray* m_localLevels;
    };
}

#endif
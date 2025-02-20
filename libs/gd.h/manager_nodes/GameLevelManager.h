#ifndef __GAMELEVELMANAGER_H__
#define __GAMELEVELMANAGER_H__

#include <gd.h>

namespace gd {

class GJGameLevel;

class GameLevelManager : public cocos2d::CCNode {
    public:
        cocos2d::CCDictionary* m_mainLevels;
        cocos2d::CCDictionary* m_valueDict;
        cocos2d::CCDictionary* m_savedLevelsDict;
        cocos2d::CCDictionary* m_timerDict;
        cocos2d::CCDictionary* m_userValues;
        cocos2d::CCDictionary* m_storedLevels;
        cocos2d::CCDictionary* m_pageInfo;
        cocos2d::CCDictionary* m_unknownDict;
        cocos2d::CCDictionary* m_savedPacks;
        cocos2d::CCDictionary* m_activeDownloads;
        cocos2d::CCDictionary* m_activeUploads;

        static auto sharedState() {
            return reinterpret_cast<GameLevelManager * (__stdcall*)()>(base + 0x55850)();
        }

        GJGameLevel* createNewLevel() {
            return reinterpret_cast<GJGameLevel * (__thiscall*)(GameLevelManager*)>(base + 0x56590)(this);
        }
        //inline static GameLevelManager* sharedState() {
        //    return reinterpret_cast<GameLevelManager*(__stdcall*)()>( gd::base + 0x55850)(); //1.91 55190
        //}

        //inline static gd::GJGameLevel* createNewLevel() {
        //    return reinterpret_cast<gd::GJGameLevel*(__stdcall*)()>( gd::base + 0x56590)(); // 1.92
        //}

        cocos2d::CCArray* getSavedLevels() {
            return reinterpret_cast<cocos2d::CCArray*(__thiscall*)(GameLevelManager*)>(gd::base + 0x57640)(this); //1.91 56f80
        }

        virtual void levelUpdate(GJGameLevel* level) {
            return reinterpret_cast<void(__fastcall*)(GJGameLevel*)>(gd::base + 0x5b530)(level); //1.91 5ae70
        }

        GJGameLevel* getMainLevel(int id, bool unk) {
            return reinterpret_cast<GJGameLevel * (__thiscall*)(
                GameLevelManager*, int, bool
                )>(base + 0x561f0)(this, id, unk);
        }

        void resetTimerForKey(char const* key) {
            return reinterpret_cast<void(__thiscall*)(GameLevelManager*, char const*)>(gd::base + 0x5440)(this, key);
        }

        auto getTimeLeft(char const* key) {
            return reinterpret_cast<int(__thiscall*)(GameLevelManager*, char const*)>(gd::base + 0x585d0)(this, key);
        }

        const char* getPostCommentKey(int level) {
            return cocos2d::CCString::createWithFormat("comment_%i", level)->getCString();
        }

        void makeTimeStamp(char const* key) {
            return reinterpret_cast<void(__thiscall*)(GameLevelManager*, char const*)>(gd::base + 0x58350)(this, key);
        }

        void updateUserScore() {
            return reinterpret_cast<void(__thiscall*)(GameLevelManager*)>(base + 0x5c950)(this);
        }

        int accountIDforID(int userID) {
            return reinterpret_cast<int(__thiscall*)(GameLevelManager*, int)>(base + 0x56e10)(this, userID);
        }

        std::string getLengthStr(bool ti, bool sh, bool me, bool lo) {
            return reinterpret_cast<std::string(__thiscall*)(GameLevelManager*, bool, bool, bool, bool)>(base + 0x599b0)(this, ti, sh, me, lo);
        }
};

}

#endif

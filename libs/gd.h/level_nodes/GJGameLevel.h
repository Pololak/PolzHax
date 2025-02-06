#pragma once
#ifndef GJGAMELEVEL_H
#define GJGAMELEVEL_H

#include <gd.h>
#include <utility>

namespace gd {

    /*
enum GJLevelType {
    kGJLevelTypeLocal = 1,
    kGJLevelTypeEditor = 2,
    kGJLevelTypeSaved = 3
};
*/

enum class GJLevelType {
    Local = 1,
    Editor = 2,
    Saved = 3
};

class GJGameLevel : public cocos2d::CCNode {
public:
    cocos2d::CCDictionary* m_lastBuildSave;
    int m_levelID; // 0xec
    gd::string m_levelName; // 0xf0
    gd::string m_levelDesc; // 0x108
    gd::string m_levelString; // 0x120
    gd::string m_userName; // 0x138
    gd::string m_recordString; // 0x150
    gd::string m_uploadDate; // 0x168
    gd::string m_updateDate; // 0x180
    int m_userID; // 0x198
    PAD(0x4)
    int m_audioTrack; // 0x1a0
    int m_songID; // 0x1a4
    PAD(0x14)
    int m_downloads; // 0x1bc
    PAD(0x5)
    bool m_isVerified; // 0x1c5
    bool m_isUploaded; // 0x1c6
    bool m_hasBeenModified; // 0x1c7
    int m_levelVersion; // 0x1c8
    int m_gameVersion; // 0x1cc
    int m_attempts; // 0x1d0
    int m_jumps; // 0x1d4
    int m_normalPercent; // 0x1d8
    int m_practicePercent; // 0x1dc
    int m_likes; // 0x1e0
    int m_dislikes; // 0x1e4
    int m_levelLength; // 0x1e8
    int m_featured; // 0x1ec
    bool m_demon; // 0x1f0
    int m_stars; // 0x1f4
    bool m_autoLevel; // 0x1f8
    int m_coins; // 0x1fc
    int m_password; // 0x200
    int m_originalLevel; // 0x204
    bool m_twoPlayerMode; // 0x208
    int m_failedPasswordAttempts; // 0x20c
    bool m_showedSongWarning; // 0x210
    int m_starRatings;
    int m_starRatingsSum;
    int m_maxStarRatings;
    int m_minStarRatings;
    int m_demonVotes;
    int m_rateStars;
    bool m_rateFeature;
    gd::string m_rateUser;
    bool m_dontSave;
    bool m_isHidden;
    //int m_requiredCoins;
    //bool m_isUnlocked;
    cocos2d::CCPoint m_lastCameraPos; // 0x254
    float m_lastEditorZoom; // 0x25c
    int m_lastBuildTab; // 0x260
    int m_lastBuildPage; // 0x264
    int m_lastBuildGroupID; // 0x268
    GJLevelType m_levelType; // 0x26c
    int m_ID; // 0x270
    gd::string m_tempName; // 0x274
    int m_capacity001; // 0x278
    int m_capacity002; // 0x27c
    int m_capacity003; // 0x280
    int m_capacity004; // 0x284

    static auto create() {
        return reinterpret_cast<GJGameLevel * (__stdcall*)()>(base + 0x621c0)(); //0x621C0 - GD 1.92
    }
    auto getPassword() {
        return from<int>(this, 0x200);
    }
    auto getLikes() {
        return from<int>(this, 0x1E0);
    }
    auto getDownloads() {
        return from<int>(this, 0x1BC);
    }
    auto getObjectCount() {
        return from<int>(this, 0x1AC);
    }
    auto getUserID() {
        return from<int>(this, 0x198);
    }
    auto& songID() {
        return from<int>(this, 0x1a4);
    }
    auto getLevelLength() {
        return from<int>(this, 0x1e8);
    }
    auto getLevelString() {
        return from<gd::string>(this, 0x120);
    }
    auto getLevelType() {
        return from<GJLevelType>(this, 0x26c);
    }

    int originalLevel() {
        return from<int>(this, 0x204);
    }

};
/*
class GJGameLevel : public cocos2d::CCNode {
    public:
        cocos2d::CCDictionary *lastBuildSave;

        int levelID_rand; // levelID + seed = levelID_rand
        int levelID_seed;
        int levelID;

        std::string levelName;
        std::string levelDesc;
        std::string levelString;
        std::string userName;
        std::string recordString;

        std::string uploadDate;
        std::string updateDate;

        int userID_rand;
        int userID_seed;
        int userID;

        int accountID_rand;
        int accountID_seed;
        int accountID;

        int difficulty;
        int audioTrack;
        int songID;

        int levelRev;

        bool unlisted;

        int objectCount_rand;
        int objectCount_seed;
        int objectCount;
        int averageDifficulty;
        int ratings;
        int ratingsSum;

        int downloads;

        bool isEditable;
        bool gauntletLevel;
        bool gauntletLevel2;

        int workingTime;
        int workingTime2;

        bool lowDetailMode;
        bool lowDetailModeToggled;

        int isVerified_rand;
        int isVerified_seed;
        bool isVerified;
        bool isUploaded;
        bool hasBeenModified;

        int levelVersion;
        int gameVersion;

        int attempts_rand;
        int attempts_seed;
        int attempts;

        int jumps_rand;
        int jumps_seed;
        int jumps;

        int clicks_rand;
        int clicks_seed;
        int clicks;

        int attemptTime_rand;
        int attemptTime_seed;
        int attemptTime;

        int chk;

        bool isChkValid;
        bool isCompletionLegitimate;

        int normalPercent; // yes, it is out of order
        int normalPercent_seed;
        int normalPercent_rand;

        int orbCompletion_rand;
        int orbCompletion_seed;
        int orbCompletion;

        int newNormalPercent2_rand;
        int newNormalPercent2_seed;
        int newNormalPercent2;

        int practicePercent;

        int likes;
        int dislikes;
        int levelLength;
        int featured;

        bool isEpic;
        bool levelFavorited;
        int levelFolder;

        int dailyID_rand;
        int dailyID_seed;
        int dailyID;

        int demon_rand;
        int demon_seed;
        int demon;
        int demonDifficulty;
        int stars_rand;
        int stars_seed;
        int stars;

        bool autoLevel;
        int coins;
        int coinsVerified_rand;
        int coinsVerified_seed;
        int coinsVerified;

        int password_rand;
        int password_seed;

        int originalLevel_rand;
        int originalLevel_seed;
        int originalLevel;

        bool twoPlayerMode;

        int failedPasswordAttempts;

        int firstCoinVerified_rand;
        int firstCoinVerified_seed;
        int firstCoinVerified;

        int secondCoinVerified_rand;
        int secondCoinVerified_seed;
        int secondCoinVerified;

        int thirdCoinVerified_rand;
        int thirdCoinVerified_seed;
        int thirdCoinVerified;

        int starsRequested;

        bool showedSongWarning;
        int starRatings;
        int starRatingsSum;
        int maxStarRatings;
        int minStarRatings;
        int demonVotes;
        int rateStars;
        int rateFeature;

        std::string rateUser;

        bool dontSave;
        bool levelDownloaded;

        int requiredCoins;
        bool isUnlocked;

        cocos2d::CCPoint lastCameraPos;

        float lastEditorZoom;
        int lastBuildTab;
        int lastBuildPage;
        int lastBuildGroupID;

        GJLevelType levelType;

        int M_ID;
        std::string tempName;
        std::string capacityString;

        bool highObjectsEnabled;
        std::string personalBests;

        
        auto& songID() {
            return from<int>(this, 0x1a4); 
        }
        static auto create() {
            return reinterpret_cast<GJGameLevel * (__stdcall*)()>(base + 0x61b30)(); //0x621C0 - GD 1.92
        }
        

        // this function is inlined on pc builds
        static GJGameLevel *create() {
            auto pRet = new GJGameLevel();

            if (pRet && pRet->init()) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                pRet = nullptr;
                return nullptr;
            }
        }

        std::string getAudioFileName() {
            std::string ret;

            reinterpret_cast<void(__thiscall*)(
                GJGameLevel*, std::string*
            )>(
                base + 0xbdc70 //wrong address
            )(
                this, &ret
            );

            return ret;
        }
};
*/

}

#endif

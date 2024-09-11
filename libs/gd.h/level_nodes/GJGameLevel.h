#pragma once
#ifndef GJGAMELEVEL_H
#define GJGAMELEVEL_H

#include <gd.h>

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

    cocos2d::CCDictionary* m_lastBuildSave;
    int m_levelID;
    gd::string m_levelName;
    gd::string m_levelDesc;
    gd::string m_levelString;
    gd::string m_userName;
    gd::string m_recordString;
    gd::string m_uploadDate;
    gd::string m_updateDate;
    gd::string m_audioFileName;
    int m_userID;
    int m_difficulty;
    int m_audioTrack;
    int m_songID;
    int m_levelRev;
    int m_objectCount;
    int m_order;
    int m_ratings;
    int m_ratingsSum;
    int m_downloads;
    int m_completes;
    bool m_isEditable;
    bool m_isVerified;
    bool m_isUploaded;
    bool m_hasBeenModified;
    int m_levelVersion;
    int m_gameVersion;
    int m_attempts;
    int m_jumps;
    int m_normalPercent;
    int m_practicePercent;
    int m_likes;
    int m_dislikes;
    int m_levelLength;
    int m_featured;
    bool m_demon;
    int m_stars;
    bool m_autoLevel;
    int m_coins;
    int m_password;
    int m_originalLevel;
    bool m_twoPlayerMode;
    int m_failedPasswordAttempts;
    bool m_showedSongWarning;
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
    int m_requiredCoins;
    bool m_isUnlocked;
    cocos2d::CCPoint m_lastCameraPos;
    float m_lastEditorZoom;
    int m_lastBuildTab;
    int m_lastBuildPage;
    int m_lastBuildGroupID;
    GJLevelType m_levelType;
    int m_ID;
    gd::string m_tempName;
    int m_capacity001;
    int m_capacity002;
    int m_capacity003;
    int m_capacity004;
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

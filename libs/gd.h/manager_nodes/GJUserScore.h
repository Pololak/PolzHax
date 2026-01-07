#ifndef __GJUSERSCORE_H__
#define __GJUSERSCORE_H__

#include <gd.h>

namespace gd {
    class GJUserScore : public cocos2d::CCNode {
        protected:
            std::string userName_;
            std::string userUDID_;
            int scoreType_; // legacy field, used in 1.9 to determine shown info
            int userID_;
            int accountID_;

            int stars_;
            int diamonds_;
            int demons_;
            int playerRank_;
            int creatorPoints_; // note - internally this is named levels
            int secretCoins_;
            int userCoins_;

            int iconID_; // for leaderboards, icon id to show
            int color1_;
            int color2_;
            int special_;
            IconType iconType_;

            int messageState_;
            int friendStatus_;
            int commentHistoryStatus_;

            std::string youtubeURL_;
            std::string twitterURL_;
            std::string twitchURL_;

            int playerCube_;
            int playerShip_;
            int playerBall_;
            int playerUfo_;
            int playerWave_;
            int playerRobot_;
            int playerSpider_;
            int playerStreak_;
            bool glowEnabled_ : 4;

            int playerExplosion_;

            int modBadge_;
            int globalRank_;

            int friendReqStatus_;
            int newMsgCount_;
            int friendReqCount_;
            bool isBlocked : 4;

            std::string lastScoreAge;

        public:
            // static GJUserScore* create();
            // static GJUserScore* create(cocos2d::CCDictionary*);

            // bool isCurrentUser();
            // void mergeWithScore(GJUserScore*);

            std::string userName() {
                return from<std::string>(this, 0xe8);
            }

            auto stars() {
                return from<int>(this, 0x124);
            }

            auto demons() {
                return from<int>(this, 0x128);
            }

            auto secretCoins() {
                return from<int>(this, 0x134);
            }

            auto creatorPoints() {
                return from<int>(this, 0x130);
            }

            auto globalRank() {
                return from<int>(this, 0x12c);
            }

            auto userID() {
                return from<int>(this, 0x11c);
            }

            auto icon() {
                return from<int>(this, 0x138);
            }

            IconType iconType() {
                return from<IconType>(this, 0x148);
            }

            auto color1() {
                return from<int>(this, 0x13c);
            }

            auto color2() {
                return from<int>(this, 0x140);
            }

            auto special() {
                return from<int>(this, 0x144);
            }

            auto accountID() {
                return from<int>(this, 0x100);
            }

            // Types:
            // Top 100 = 0,
            // Creator = 1,
            auto scoreType() {
                return from<int>(this, 0x118); // 0 - Top 100, 1 - Creator
            }
    };
}

#endif

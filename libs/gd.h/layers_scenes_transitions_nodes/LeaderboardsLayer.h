#ifndef __LEADERBOARDSLAYER_H__
#define __LEADERBOARDSLAYER_H__

#include <gd.h>

namespace gd {
    enum class LeaderboardState {
        Default = 0,
        Top100 = 1,
        Global = 2,
        Creator = 3,
        Week = 4,
    };

    class LeaderboardsLayer : public cocos2d::CCLayer {
    public:
        static LeaderboardsLayer* create(LeaderboardState state) { //lb state is actually enum member
            return reinterpret_cast<LeaderboardsLayer * (__fastcall*)(LeaderboardState)>(
                gd::base + 0x158710
                )(state);
        }

    };

}

#endif
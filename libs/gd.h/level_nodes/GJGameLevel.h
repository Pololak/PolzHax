#ifndef __GJGAMELEVEL_H__
#define __GJGAMELEVEL_H__

#include <gd.h>

namespace gd {
	class GJGameLevel : public cocos2d::CCNode {
	public:
        cocos2d::CCDictionary* m_lastBuildSave; // 0xe8
        int m_levelID; // 0xec
        std::string m_levelName; // 0xf0
        std::string m_levelDesc; // 0x108
        std::string m_levelString; // 0x120
        std::string m_userName; // 0x138
        std::string m_recordString; // 0x150
        std::string m_uploadDate; // 0x168
        std::string m_updateDate; // 0x180
        int m_userID; // 0x198
        int m_difficulty; // 0x19c
        int m_audioTrack; // 0x1a0
        int m_songID; // 0x1a4
        int m_levelRev; // 0x1a8
        int m_objectCount; // 0x1ac
        int m_order; // 0x1b0
        int m_ratings; // 0x1b4
        int m_ratingsSum; // 0x1b8
        int m_downloads; // 0x1bc
        int m_completes; // 0x1c0
        bool m_isEditable; // 0x1c4
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
        int m_starRatings; // 0x214
        int m_starRatingsSum; // 0x218
        int m_maxStarRatings; // 0x21c
        int m_minStarRatings; // 0x220
        int m_demonVotes; // 0x224
        int m_rateStars; // 0x228
        bool m_rateFeature; // 0x22c
        std::string m_rateUser; // 0x230
        bool m_dontSave; // 0x248
        bool m_isHidden; // 0x249
        int m_requiredCoins; // 0x24c
        bool m_isUnlocked; // 0x250
        cocos2d::CCPoint m_lastCameraPos; // 0x254
        float m_lastEditorZoom; // 0x25c
        int m_lastBuildTab; // 0x260
        int m_lastBuildPage; // 0x264
        int m_lastBuildGroupID; // 0x268
        GJLevelType m_levelType; // 0x26c
        int m_ID; // 0x270
        std::string m_tempName; // 0x274
        int m_capacity001; // 0x278
        int m_capacity002; // 0x27c
        int m_capacity003; // 0x280
        int m_capacity004; // 0x284
	};
}

#endif
#ifndef __LEVELINFOLAYER_H__
#define __LEVELINFOLAYER_H__

#include <gd.h>

namespace gd {

	class GJGameLevel;
	enum class GJLevelType;

	class LevelInfoLayer : public cocos2d::CCLayer {
	public:
		PAD(0x20)
		cocos2d::CCMenu* m_playBtnMenu; // 0x138
		GJGameLevel* m_level; // 0x13c
		CCMenuItemSpriteExtra* m_diffRateBtn; // 0x140
		CCMenuItemSpriteExtra* m_likeBtn; // 0x144
		CCMenuItemSpriteExtra* m_starRateBtn; // 0x148
		PAD(0x4)
		cocos2d::CCLabelBMFont* m_lengthLabel; // 0x150
		cocos2d::CCLabelBMFont* m_downloadsLabel; // 0x154
		cocos2d::CCLabelBMFont* m_likesLabel; // 0x158
		PAD(0x4)
		cocos2d::CCSprite* m_likesIcon; // 0x160
		GJLevelType m_levelType; // 0x164
		bool m_levelNeedsDownload; // 0x168
		LoadingCircle* m_loadingCircle; // 0x16c
		cocos2d::CCSprite* m_difficultyIcon; // 0x170
		cocos2d::CCSprite* m_starIcon; // 0x174
		cocos2d::CCLabelBMFont* m_starsLabel; // 0x178
		cocos2d::CCSprite* m_featuredCoin; // 0x17c

		bool shouldDownloadLevel() {
			return reinterpret_cast<bool(__thiscall*)(LevelInfoLayer*)>(base + 0x9cc40)(this);
		}

		static auto scene(GJGameLevel* level) {
			return reinterpret_cast<cocos2d::CCScene * (__fastcall*)(GJGameLevel*)>(base + 0x9bb30)(level);
		}
	};
}

#endif

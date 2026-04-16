#ifndef __LEVELSEARCHLAYER_H__
#define __LEVELSEARCHLAYER_H__

#include <gd.h>

namespace gd {
	class CCTextInputNode;

	class LevelSearchLayer : public cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol {
	public:
		CCTextInputNode* m_searchInput; // 0x120
		cocos2d::CCSprite* m_starSprite; // 0x124
		cocos2d::CCArray* m_difficultySprites; // 0x128
		cocos2d::CCArray* m_lengthLabels; // 0x12c
		cocos2d::CCDictionary* m_difficultyDict; // 0x130
	};
}

#endif // !__LEVELSEARCHLAYER_H__

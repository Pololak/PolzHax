#ifndef __GJSCORECELL_H__
#define __GJSCORECELL_H__

#include <gd.h>

namespace gd {

	class GJUserScore;

	class GJScoreCell : public cocos2d::CCLayer {
	public:
		GJUserScore* userScore() {
			return from<GJUserScore*>(this, 0x170);
		}
		
		auto layer() {
			return from<cocos2d::CCLayer*>(this, 0x16c);
		}

		auto menu() {
			return from<cocos2d::CCMenu*>(this, 0x118);
		}
	};
}

#endif
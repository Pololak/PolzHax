#ifndef __ENDLEVELLAYER_H__
#define __ENDLEVELLAYER_H__

#include <gd.h>

namespace gd {
	class GJDropDownLayer;

	class EndLevelLayer : public GJDropDownLayer {
	public:
		bool m_isExiting; // 0x1bc
		bool m_shouldSave; // 0x1bd
		cocos2d::CCMenu* m_actionsMenu; // 0x1c0
		cocos2d::CCArray* m_coinEffects; // 0x1c4
		cocos2d::CCPoint m_starPositionl; // 0x1c8

		static EndLevelLayer* create() {
			return reinterpret_cast<EndLevelLayer * (__stdcall*)()>(
				base + 0x502C0
				)();
		}
	};
}

#endif
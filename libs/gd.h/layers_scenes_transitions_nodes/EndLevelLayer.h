#ifndef __ENDLEVELLAYER_H__
#define __ENDLEVELLAYER_H__

#include <gd.h>

namespace gd {
	class EndLevelLayer : public GJDropDownLayer {
	public:
		bool m_isExiting;
		bool m_shouldSave;
		cocos2d::CCMenu* m_actionsMenu;
		cocos2d::CCArray* m_coinEffects;
		cocos2d::CCPoint m_starPosition;
	};
}

#endif // !__ENDLEVELLAYER_H__

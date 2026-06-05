#ifndef __SHARELEVELLAYER_H__
#define __SHARELEVELLAYER_H__

#include <gd.h>

namespace gd {
	class GJGameLevel;

	class ShareLevelLayer : public FLAlertLayer {
	public:
		GJGameLevel* m_level; // 0x1bc
	};
}

#endif // !__SHARELEVELLAYER_H__

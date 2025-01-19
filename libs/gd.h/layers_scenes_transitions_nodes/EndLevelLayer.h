#ifndef __ENDLEVELLAYER_H__
#define __ENDLEVELLAYER_H__

#include <gd.h>

namespace gd {
	class GJDropDownLayer;

	class EndLevelLayer : public GJDropDownLayer {
	public:
		static EndLevelLayer* create() {
			return reinterpret_cast<EndLevelLayer * (__stdcall*)()>(
				base + 0x502C0
				)();
		}
	};
}

#endif
#ifndef __OPTIONSLAYER_H__
#define __OPTIONSLAYER_H__

#include <gd.h>

namespace gd {
	class OptionsLayer : public GJDropDownLayer {
	public:
		static OptionsLayer* create() {
			return reinterpret_cast<OptionsLayer * (__fastcall*)()>(base + 0xd0360)();
		}
	};
}

#endif // !__OPTIONSLAYER_H__

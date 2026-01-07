#ifndef __TEXTAREA_H__
#define __TEXTAREA_H__

#include <gd.h>

namespace gd {
	#pragma runtime_checks("s", off)
	class TextArea : public cocos2d::CCSprite {
	protected:
		PAD(0x58);

	public:
		static TextArea* create(std::string str, char const* font, float scale, float width, cocos2d::CCPoint anchor, float lineHeight, bool disableColor) {
			auto pRet = reinterpret_cast<TextArea * (__fastcall*)(std::string, char const*, float, float, cocos2d::CCPoint, float, bool)>(base + 0x200a0)(str, font, scale, width, anchor, lineHeight, disableColor);
			return pRet;
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif
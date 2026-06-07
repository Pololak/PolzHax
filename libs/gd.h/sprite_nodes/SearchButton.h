#ifndef __SEARCHBUTTON_H__
#define __SEARCHBUTTON_H__

#include <gd.h>

namespace gd {
	class SearchButton : public cocos2d::CCSprite {
	public:
		cocos2d::CCLabelBMFont* m_label; // 0x1b8
		cocos2d::CCSprite* m_sprite; // 0x1bc

		static SearchButton* create(char const* texture, char const* label, char const* icon, float labelScale) {
			__asm movss xmm2, labelScale
			auto ret = reinterpret_cast<SearchButton * (__fastcall*)(char const*, char const*, char const*)>(base + 0xa3180)(texture, label, icon);

			__asm add esp, 0x4

			return ret;
		}
	};
}

#endif // !__SEARCHBUTTON_H__

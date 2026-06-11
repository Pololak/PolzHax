#ifndef __SEARCHBUTTON_H__
#define __SEARCHBUTTON_H__

#include <gd.h>

namespace gd {
	class SearchButton : public cocos2d::CCSprite {
	public:
		cocos2d::CCLabelBMFont* m_label; // 0x1b8
		cocos2d::CCSprite* m_sprite; // 0x1bc

		virtual bool init(char const* texture, char const* label, float labelScale, char const* icon) {
			__asm movss xmm2, labelScale
			return reinterpret_cast<bool(__thiscall*)(SearchButton*, char const*, char const*, char const*)>(base + 0xa3270)(this, texture, label, icon);
		}

		static SearchButton* create(char const* texture, char const* label, float labelScale, char const* icon) {
			__asm movss xmm2, labelScale
			auto ret = reinterpret_cast<SearchButton * (__fastcall*)(char const*, char const*, char const*)>(base + 0xa3180)(texture, label, icon);

			__asm add esp, 0x4

			return ret;
		}
	};
}

#endif // !__SEARCHBUTTON_H__

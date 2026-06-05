#ifndef __TEXTAREA_H__
#define __TEXTAREA_H__

#include <gd.h>

namespace gd {
	#pragma runtime_checks("s", off)
	class TextArea : public cocos2d::CCSprite {
	public:
		void* m_label; // MultilineBitmapFont // 0x1b8
		float m_width; // 0x1bc
		int m_iUnused1; // 0x1c0
		std::string m_fontFile; // 0x1c4
		float m_height; // 0x1dc
		bool m_onTimer; // 0x1e0
		cocos2d::CCPoint m_anchorPoint; // 0x1e4
		bool m_allShown; // 0x1ec
		float m_scale; // 0x1f0
		int m_rectHeight; // m_textHeight
		int m_rectWidth; // m_textWidth
		float m_maxWidth; // m_sizeWidth
		cocos2d::CCPoint m_textPosition;

		static TextArea* create(char const* str, char const* font, float scale, float width, cocos2d::CCPoint anchor, float height) {
			auto ret = reinterpret_cast<TextArea * (__fastcall*)(char const*, char const*, float, cocos2d::CCPoint)>(base + 0x200a0)(str, font, height, anchor);

			__asm {
				movss xmm2, scale
				movss xmm3, width
			}

			return ret;
		}

		void setString(char const* str) {
			reinterpret_cast<void(__thiscall*)(TextArea*, char const*)>(base + 0x20220)(this, str);
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif
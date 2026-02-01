#ifndef __BUTTONSPRITE_H__
#define __BUTTONSPRITE_H__

#include <gd.h>

namespace gd {
	#pragma runtime_checks("s", off)
	class ButtonSprite : public cocos2d::CCSprite {
	public:
		int m_mode;
		float m_width;
		float m_fUnknown1;
		float m_scale;
		float m_height;
		bool m_absolute;
		cocos2d::CCLabelBMFont* m_label;
		cocos2d::CCSprite* m_btnSprite;
		cocos2d::extension::CCScale9Sprite* m_bgSprite;
		cocos2d::CCPoint m_textOffset;
		cocos2d::CCPoint m_spritePosition;
		std::string m_caption;

		static auto create(const char* label, int buttonWidth, int widthLimit, float scale, bool limitWidth, const char* font, const char* sprite, float buttonHeight) {
			auto ret = reinterpret_cast<ButtonSprite*(__vectorcall*)(
				float, float, float, float, float, float, // xmm registers
				const char*, int, // ecx and edx
				int, bool, const char*, const char*, float // stack
				)>(base + 0x9800)(0.f, 0.f, 0.f, scale, 0.f, 0.f, label, buttonWidth, widthLimit, limitWidth, font, sprite, buttonHeight);

			__asm add esp, 0x14

			return ret;
		}

		void setColor(cocos2d::ccColor3B color) {
			if (m_label) {
				m_label->setColor(color);
			}

			if (m_btnSprite) {
				m_btnSprite->setColor(color);
			}

			m_bgSprite->setColor(color);
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif
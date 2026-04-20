#ifndef __BUTTONSPRITE_H__
#define __BUTTONSPRITE_H__

#include <gd.h>

namespace gd {
	#pragma runtime_checks("s", off)
	class ButtonSprite : public cocos2d::CCSprite {
	public:
		int m_mode; // 0x1b8
		float m_width; // 0x1bc
		float m_fUnknown1; // 0x1c0
		float m_scale; // 0x1c4
		float m_height; // 0x1c8
		bool m_absolute; // 0x1cc
		cocos2d::CCLabelBMFont* m_label; // 0x1d0
		cocos2d::CCSprite* m_btnSprite; // 0x1d4
		cocos2d::extension::CCScale9Sprite* m_bgSprite; // 0x1d8
		cocos2d::CCPoint m_textOffset; // 0x1dc
		cocos2d::CCPoint m_spritePosition; // 0x1e4
		std::string m_caption; // 0x1ec
		
		static ButtonSprite* create(const char* label, int buttonWidth, int widthLimit, float scale, bool limitWidth, const char* font, const char* sprite, float buttonHeight) {
			__asm movss xmm3, scale
			auto ret = reinterpret_cast<ButtonSprite * (__fastcall*)(const char*, int, int, bool, const char*, const char*, float)>(base + 0x9800)(label, buttonWidth, widthLimit, limitWidth, font, sprite, buttonHeight);

			__asm add esp, 0x14

			return ret;
		}

		static ButtonSprite* create(cocos2d::CCSprite* topSprite, int buttonWidth, bool absolute, float scale, int p0, const char* sprite, bool p1, float height) {
			__asm movss xmm3, height
			auto ret = reinterpret_cast<ButtonSprite * (__fastcall*)(cocos2d::CCSprite*, int, bool, float, int, const char*, bool)>(base + 0x9510)(topSprite, buttonWidth, absolute, scale, p0, sprite, p1);

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
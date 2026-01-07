#ifndef __BUTTONSPRITE_H__
#define __BUTTONSPRITE_H__

#include <gd.h>

namespace gd {
	#pragma runtime_checks("s", off)
	class ButtonSprite_Original : public cocos2d::CCSprite {
	public:
		/*PARAMS:
		* caption - the button's caption.
		* width - the width of the button. only used if absolute is TRUE.
		* absolute - whether or not to use provided width. if FALSE, game will auto-adjust.
		* font - font file for caption.
		* texture - texture file for button background.
		* height - height of button. put 0 for auto.
		* scale - scale of the caption.
		*/
		static ButtonSprite_Original* create(const char* caption, int width, bool absolute,
			const char* font, const char* texture, float height, float scale) {
			//scale is passed in lower 4 bytes of xmm3
			__asm movss xmm3, scale
			//arg 3 is always 0. dunno why it's not optimized out as a param
			auto pRet = reinterpret_cast<ButtonSprite_Original * (__fastcall*)(const char*,
				int, int, bool, const char*, const char*, float)>(
					base + 0x137D0
					)(caption, width, 0, absolute, font, texture, height);
			//clean stack before returning
			__asm add esp, 0x14
			return pRet;
		}
	};
	class ButtonSprite : public cocos2d::CCSprite {
	public:
		static auto create(const char* label, int buttonWidth, int widthLimit, float scale, bool limitWidth, const char* font, const char* sprite, float labelHeight) {
			auto ret = reinterpret_cast<ButtonSprite * (__vectorcall*)(
				float, float, float, float, float, float, // xmm registers
				const char*, int, // ecx and edx
				int, bool, const char*, const char*, float // stack
				)>(base + 0x9800)(0.f, 0.f, 0.f, scale, 0.f, 0.f, label, buttonWidth, widthLimit, limitWidth, font, sprite, labelHeight); //0x9800 - GD 1.92
			// TODO: fix vectorcall version to work with clang
			// __asm movss xmm3, scale
			// auto ret = reinterpret_cast<ButtonSprite*(__fastcall*)(
			// 	const char*, int, // ecx and edx
			// 	int, bool, const char*, const char*, float // stack
			// )>(base + 0x9800)(label, width, idk, absolute, font, sprite, height);
			__asm add esp, 20;
			return ret;
		}

		auto getLabel()
		{
			return from<cocos2d::CCLabelBMFont*>(this, 0x1d0);
			//return reinterpret_cast<cocos2d::CCLabelBMFont*>(reinterpret_cast<uintptr_t>(this) + 0x1d0);
		}
		auto getCCS9Sprite()
		{
			return from<cocos2d::extension::CCScale9Sprite*>(this, 0x1d8);
			//return reinterpret_cast<cocos2d::extension::CCScale9Sprite*>(reinterpret_cast<uintptr_t>(this) + 0x1d8);
		}
		void setColor(cocos2d::ccColor3B color)
		{
			if (this->getLabel() != nullptr) this->getLabel()->setColor(color);
			this->getCCS9Sprite()->setColor(color);
		}
	};

	#pragma runtime_checks("s", restore)
}

#endif
#ifndef __GAMETOOLBOX_H__
#define __GAMETOOLBOX_H__

#include <gd.h>

namespace gd {
	class CCMenuItemToggler;

	class GameToolbox {
	public:
		static CCMenuItemToggler* createToggleButton(cocos2d::SEL_MenuHandler callback, bool isActive, cocos2d::CCMenu* targetMenu, cocos2d::CCPoint position, cocos2d::CCNode* callbackObject, cocos2d::CCNode* targetNode, float toggleScale, float labelMaxScale, float labelMaxWidth, cocos2d::CCPoint labelOffset, char const* font, bool verticalLayout, std::string label) {
			auto ret = reinterpret_cast<CCMenuItemToggler * (__fastcall*)(cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCNode*, cocos2d::CCNode*, float, float, float, cocos2d::CCPoint, char const*, bool, std::string)>(base + 0x184c0)(callback, isActive, targetMenu, position, callbackObject, targetNode, toggleScale, labelMaxScale, labelMaxWidth, labelOffset, font, verticalLayout, label);

			__asm add esp, 0x48

			return ret;
		}

		static CCMenuItemToggler* createToggleButton(
			cocos2d::SEL_MenuHandler callback,
			bool isActive,
			cocos2d::CCMenu* targetMenu,
			cocos2d::CCNode* callbackObject,
			cocos2d::CCNode* targetNode,
			float toggleScale,
			float labelMaxScale,
			float labelMaxWidth,
			char const* font,
			bool verticalLayout,
			std::string label,
			cocos2d::CCPoint position,
			cocos2d::CCPoint labelOffset
		) {
			auto ret = reinterpret_cast<CCMenuItemToggler*(__fastcall*)(
				cocos2d::SEL_MenuHandler,
				bool,
				cocos2d::CCMenu*,
				cocos2d::CCNode*,
				cocos2d::CCNode*,
				float,
				float,
				float,
				char const*,
				bool,
				std::string,
				cocos2d::CCPoint,
				cocos2d::CCPoint
				)>(base + 0x184c0)(
					callback,
					isActive,
					targetMenu,
					callbackObject,
					targetNode,
					toggleScale,
					labelMaxScale,
					labelMaxWidth,
					font,
					verticalLayout,
					label,
					position,
					labelOffset);

			__asm add esp, 0x48

			return ret;
		}

		static cocos2d::ccColor3B getMixedColor(cocos2d::ccColor3B color1, cocos2d::ccColor3B color2, float ratio) {
			auto r = color1.r * ratio + color2.r * (1.f - ratio);
			auto g = color1.g * ratio + color2.g * (1.f - ratio);
			auto b = color1.b * ratio + color2.b * (1.f - ratio);
			return { std::clamp<uint8_t>(r, 0, 255), std::clamp<uint8_t>(g, 0, 255), std::clamp<uint8_t>(b, 0, 255) };
		}
	};
}

#endif // !__GAMETOOLBOX_H__

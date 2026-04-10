#ifndef __MORESEARCHLAYER_H__
#define __MORESEARCHLAYER_H__

#include <gd.h>

namespace gd {
	class CCMenuItemSpriteExtra;
	class CCTextInputNode;
	class CCMenuItemToggler;

	class MoreSearchLayer : public FLAlertLayer, TextInputDelegate {
	public:
		cocos2d::CCLabelBMFont* m_audioTrackName; // 0x1c0
		CCMenuItemSpriteExtra* m_songLeftBtn; // 0x1c4
		CCMenuItemSpriteExtra* m_songRightBtn; // 0x1c8
		CCMenuItemSpriteExtra* m_normalBtn; // 0x1cc
		CCMenuItemSpriteExtra* m_customBtn; // 0x1d0
		CCTextInputNode* m_enterSongID; // 0x1d4
		cocos2d::CCArray* m_songTabNodes; // 0x1d8
		cocos2d::CCArray* m_originalSongNodes; // 0x1dc
		cocos2d::CCArray* m_customSongNodes; // 0x1e0

		CCMenuItemToggler* createToggleButton(std::string label, cocos2d::SEL_MenuHandler callback, bool isActive, cocos2d::CCMenu* menu, cocos2d::CCPoint position/*, float labelScale*/) {
			//__asm movss xmm0, labelScale 
			auto ret = reinterpret_cast<CCMenuItemToggler*(__thiscall*)(MoreSearchLayer*, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, std::string, cocos2d::CCPoint)>(base + 0xa52a0)(this, callback, isActive, menu, label, position);



			return ret;
		}
	};
}

#endif // !__MORESEARCHLAYER_H__
#ifndef __CCTEXTINPUTNODE_H__
#define __CCTEXTINPUTNODE_H__

#include <gd.h>

namespace gd {
	class TextInputDelegate;

	#pragma runtime_checks("s", off)
	class CCTextInputNode : public cocos2d::CCLayer, public cocos2d::CCIMEDelegate, public cocos2d::CCTextFieldDelegate {
	protected:
		PAD(0x4);
		std::string m_sCaption;
		PAD(0x8);
		std::string m_sFilter;
		float m_fWidth;
		float m_fMaxLabelScale;
		float m_fPlaceholderScale;
		cocos2d::ccColor3B m_cPlaceholderColor;
		cocos2d::ccColor3B m_cNormalColor;
		cocos2d::CCLabelBMFont* m_pCursor;
		cocos2d::CCTextFieldTTF* m_textField;
		TextInputDelegate* m_delegate;
		int m_nMaxLabelLength;
		cocos2d::CCLabelBMFont* m_pPlaceholderLabel;
		bool m_bUnknown;
		bool m_bUnknown2;
		bool m_bForceOffset;

	public:
		//own vtable
		void onClickTrackNode(bool) {}

		//static CCTextInputNode* create(const char* caption, cocos2d::CCObject* target, const char* font) {
		//	auto pRet = reinterpret_cast<CCTextInputNode * (__thiscall*)(const char*, cocos2d::CCObject*, const char*)>(
		//		base + 0x13a90)(
		//			caption, target, font);
		//	__asm add esp, 0x8
		//	return pRet;
		//}

		static CCTextInputNode* create(const char* caption, cocos2d::CCObject* target, 
			const char* fntFile, float width, float height) {
			__asm {
				movss xmm0, width
				movss xmm1, height
			}
			auto pRet = reinterpret_cast<CCTextInputNode* (__thiscall*)
				(const char*, cocos2d::CCObject*, const char*)>(
				base + 0x13a90
			)(caption, target, fntFile);
			__asm add esp, 0x8
			return pRet;
		}

		void setLabelPlaceholderColor(cocos2d::ccColor3B color) {
			from<cocos2d::ccColor3B>(this, 0x164) = color;
			this->refreshLabel();
		}
		void setLabelPlaceholerScale(float scale) {
			from<float>(this, 0x160) = scale;
			this->refreshLabel();
		}
		void setMaxLabelScale(float scale) {
			from<float>(this, 0x15c) = scale;
			this->refreshLabel();
		}
		void setMaxLabelWidth(float width) {
			from<float>(this, 0x158) = width;
		}
		void setMaxLabelLength(int length) { 
			from<int>(this, 0x178) = length;
		}
		void setAllowedChars(std::string filter) {
			from<std::string>(this, 0x140) = filter;
		}
		void refreshLabel() {
			return reinterpret_cast<void(__thiscall*)(CCTextInputNode*)>(
				base + 0x14030
				)(this);
		}
		void setString(std::string text) {
			return reinterpret_cast<void(__thiscall*)(CCTextInputNode*, std::string)>(base + 0x13d70)(this, text);
		}
		cocos2d::CCTextFieldTTF* getTextField() {
			return from<cocos2d::CCTextFieldTTF*>(this, 0x170);
		}
		const char* getString() {
			return getTextField()->getString();
		}
		void setDelegate(TextInputDelegate* delegate) { 
			from<TextInputDelegate*>(this, 0x174) = delegate;
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif
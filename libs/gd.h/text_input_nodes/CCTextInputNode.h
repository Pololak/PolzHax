#ifndef __CCTEXTINPUTNODE_H__
#define __CCTEXTINPUTNODE_H__

#include <gd.h>

namespace gd {
	#pragma runtime_checks("s", off)
	class CCTextInputNode : public cocos2d::CCLayer, public cocos2d::CCIMEDelegate, public cocos2d::CCTextFieldDelegate {
	public:
		std::string m_caption; // 0x120
		PAD(0x4)
		bool m_selected; // 0x13c
		bool m_keyboardPresent; // 0x13d
		std::string m_allowedChars; // 0x140
		float m_maxLabelWidth; // 0x158
		float m_maxLabelScale; // 0x15c
		float m_placeholderScale; // 0x160
		cocos2d::ccColor3B m_placeholderColor; // 0x164
		cocos2d::ccColor3B m_textColor; // 0x168
		cocos2d::CCLabelBMFont* m_cursor; // 0x16c
		cocos2d::CCTextFieldTTF* m_textField; // 0x170
		TextInputDelegate* m_delegate; // 0x174
		int m_maxLabelLength; // 0x178
		cocos2d::CCLabelBMFont* m_textLabel; // 0x17c
		bool m_filterSwearWords; // 0x180
		bool m_usePasswordChar; // 0x181
		bool m_forceOffset; // 0x182

		static CCTextInputNode* create(float width, float height, char const* caption, cocos2d::CCObject* target, char const* fntFile) {
			__asm {
				movss xmm0, width
				movss xmm1, height
			}

			auto ret = reinterpret_cast<CCTextInputNode * (__thiscall*)(char const*, cocos2d::CCObject*, char const*)>(base + 0x13a90)(caption, target, fntFile);

			__asm add esp, 0x8

			return ret;
		}

		std::string getString() {
			return m_textField->getString();
		}

		const char* getCString() {
			return m_textField->getString();
		}

		void setAllowedChars(std::string filter) {
			m_allowedChars = filter;
		}

		void setLabelPlaceholderColor(cocos2d::ccColor3B color) {
			m_placeholderColor = color;
			this->refreshLabel();
		}

		void setLabelPlaceholderScale(float scale) {
			m_placeholderScale = scale;
			this->refreshLabel();
		}

		void setMaxLabelScale(float scale) {
			m_maxLabelScale = scale;
			this->refreshLabel();
		}

		void setMaxLabelWidth(float width) {
			m_maxLabelWidth = width;
			this->refreshLabel();
		}

		void setDelegate(TextInputDelegate* delegate) {
			m_delegate = delegate;
		}

		void setCharLimit(int limit) {
			m_maxLabelLength = limit;
		}

		void setString(std::string string) {
			reinterpret_cast<void(__thiscall*)(CCTextInputNode*, std::string)>(base + 0x13d70)(this, string);
		}

		void refreshLabel() {
			reinterpret_cast<void(__thiscall*)(CCTextInputNode*)>(base + 0x14030)(this);
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif
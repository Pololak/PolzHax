#ifndef __CCTEXTINPUTNODE_H__
#define __CCTEXTINPUTNODE_H__

#include "../include/gd.h"
#include <string>
#include <fstream>

class CCTextInputNode : public cocos2d::CCLayer, public cocos2d::CCIMEDelegate, public cocos2d::CCTextFieldDelegate {
public:
    std::string m_caption;
    PAD(0x4)
    bool m_selected;
    bool m_keyboardPresent;
    std::string m_allowedChars;
    float m_maxLabelWidth;
    float m_maxLabelScale;
    float m_placeholderScale;
    cocos2d::ccColor3B m_placeholderColor;
    cocos2d::ccColor3B m_textColor;
    cocos2d::CCLabelBMFont* m_cursor;
    cocos2d::CCTextFieldTTF* m_textField;
    TextInputDelegate* m_delegate;
    int m_maxLabelLength;
    cocos2d::CCLabelBMFont* m_textLabel;
    bool m_filterSwearWords;
    bool m_usePasswordChar;
    bool m_forceOffset;

    static CCTextInputNode* create(float width, float height, char const* placeholder, char const* thonburi, int fontSize, char const* fontPath);
    static CCTextInputNode* create(float width, float height, char const* placeholder, char const* fontPath) {
        return CCTextInputNode::create(width, height, placeholder, "Thonburi", 0x18, fontPath);
    }

    void setDelegate(TextInputDelegate* delegate) {
        m_delegate = delegate;
    }

    void setCharLimit(int limit) {
        m_maxLabelLength = limit;
    }

    std::string getString() {
        return m_textField->getString();
    }

    void setAllowedChars(std::string str) {
        m_allowedChars = str;
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

    void setString(std::string);
    void refreshLabel();
    void updateLabel(std::string);
};

#endif
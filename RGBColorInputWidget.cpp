#include "RGBColorInputWidget.hpp"
#include "hsv.hpp"

inline std::string color_to_hex(ccColor3B color) {
    static constexpr auto digits = "0123456789ABCDEF";
    std::string output;
    output += digits[color.r >> 4 & 0xF];
    output += digits[color.r & 0xF];
    output += digits[color.g >> 4 & 0xF];
    output += digits[color.g & 0xF];
    output += digits[color.b >> 4 & 0xF];
    output += digits[color.b & 0xF];
    return output;
}

bool RGBColorInputWidget::init(gd::ColorSelectPopup* parent) {
    if (!CCLayer::init()) return false;
    this->parent = parent;

    const ccColor3B placeholder_color = { 100, 100, 100 };

    constexpr float total_w = 115.f;
    constexpr float spacing = 4.f;
    constexpr float comp_width = (total_w - spacing * 2.f) / 3.f; // components (R G B) width
    constexpr float comp_height = 22.5f;
    constexpr float hex_height = 30.f;
    constexpr float hex_y = -hex_height - spacing / 2.f;
    constexpr float r_xpos = -comp_width - spacing;
    constexpr float b_xpos = -r_xpos;
    constexpr float bg_scale = 1.6f;
    constexpr GLubyte opacity = 100;

    red_input = gd::CCTextInputNode::create("R", this, "bigFont.fnt", 30.f, 20.f);
    red_input->setAllowedChars("0123456789");
    red_input->setMaxLabelLength(3);
    red_input->setMaxLabelScale(0.6f);
    red_input->setLabelPlaceholderColor(placeholder_color);
    red_input->setLabelPlaceholerScale(0.5f);
    red_input->setPositionX(r_xpos);
    red_input->setDelegate(this);

    green_input = gd::CCTextInputNode::create("G", this, "bigFont.fnt", 30.f, 20.f);
    green_input->setAllowedChars("0123456789");
    green_input->setMaxLabelLength(3);
    green_input->setMaxLabelScale(0.6f);
    green_input->setLabelPlaceholderColor(placeholder_color);
    green_input->setLabelPlaceholerScale(0.5f);
    green_input->setPositionX(0.f);
    green_input->setDelegate(this);

    blue_input = gd::CCTextInputNode::create("B", this, "bigFont.fnt", 30.f, 20.f);
    blue_input->setAllowedChars("0123456789");
    blue_input->setMaxLabelLength(3);
    blue_input->setMaxLabelScale(0.6f);
    blue_input->setLabelPlaceholderColor(placeholder_color);
    blue_input->setLabelPlaceholerScale(0.5f);
    blue_input->setPositionX(b_xpos);
    blue_input->setDelegate(this);

    hex_input = gd::CCTextInputNode::create("hex", this, "bigFont.fnt", 100.f, 20.f);
    hex_input->setAllowedChars("0123456789ABCDEFabcdef");
    hex_input->setMaxLabelLength(6);
    hex_input->setMaxLabelScale(0.7f);
    hex_input->setLabelPlaceholderColor(placeholder_color);
    hex_input->setLabelPlaceholerScale(0.5f);
    hex_input->setPositionY(hex_y);
    hex_input->setDelegate(this);

    //h_input = gd::CCTextInputNode::create("H", this, "bigFont.fnt", 30.f, 20.f);
    //h_input->setAllowedChars("0123456789");
    //h_input->setMaxLabelLength(3);
    //h_input->setMaxLabelScale(0.6f);
    //h_input->setLabelPlaceholderColor(placeholder_color);
    //h_input->setLabelPlaceholerScale(0.5f);
    //h_input->setPositionX(b_xpos);
    //h_input->setDelegate(this);

    //s_input = gd::CCTextInputNode::create("S", this, "bigFont.fnt", 30.f, 20.f);
    //s_input->setAllowedChars("0123456789");
    //s_input->setMaxLabelLength(3);
    //s_input->setMaxLabelScale(0.6f);
    //s_input->setLabelPlaceholderColor(placeholder_color);
    //s_input->setLabelPlaceholerScale(0.5f);
    //s_input->setPositionX(b_xpos);
    //s_input->setDelegate(this);

    //v_input = gd::CCTextInputNode::create("V", this, "bigFont.fnt", 30.f, 20.f);
    //v_input->setAllowedChars("0123456789");
    //v_input->setMaxLabelLength(3);
    //v_input->setMaxLabelScale(0.6f);
    //v_input->setLabelPlaceholderColor(placeholder_color);
    //v_input->setLabelPlaceholerScale(0.5f);
    //v_input->setPositionX(b_xpos);
    //v_input->setDelegate(this);

    addChild(red_input);
    addChild(green_input);
    addChild(blue_input);
    addChild(hex_input);
    //addChild(h_input);
    //addChild(s_input);
    //addChild(v_input);

    update_labels(true, true, true);

    auto bg = extension::CCScale9Sprite::create("square02_small.png");
    bg->setContentSize({ total_w * bg_scale, hex_height * bg_scale });
    bg->setScale(1.f / bg_scale);
    bg->setOpacity(opacity);
    bg->setZOrder(-1);
    bg->setPositionY(hex_y);
    addChild(bg);

    bg = extension::CCScale9Sprite::create("square02_small.png");
    bg->setContentSize({ comp_width * bg_scale, comp_height * bg_scale });
    bg->setScale(1.f / bg_scale);
    bg->setOpacity(opacity);
    bg->setZOrder(-1);
    bg->setPositionX(r_xpos);
    addChild(bg);

    bg = extension::CCScale9Sprite::create("square02_small.png");
    bg->setContentSize({ comp_width * bg_scale, comp_height * bg_scale });
    bg->setScale(1.f / bg_scale);
    bg->setOpacity(opacity);
    bg->setZOrder(-1);
    addChild(bg);

    bg = extension::CCScale9Sprite::create("square02_small.png");
    bg->setContentSize({ comp_width * bg_scale, comp_height * bg_scale });
    bg->setScale(1.f / bg_scale);
    bg->setOpacity(opacity);
    bg->setZOrder(-1);
    bg->setPositionX(b_xpos);
    addChild(bg);

    return true;
}

void RGBColorInputWidget::textChanged(gd::CCTextInputNode* input) {
    if (ignore)
        return;

    if (input == hex_input) {

        if (!input->getString() || !strlen(input->getString()))
            return;

        std::string value(input->getString());
        ccColor3B color;

        if (value.empty())
            return;
        if (value.size() > 6)
            return;

        int num_value;
        try { num_value = std::stoi(value, 0, 16); }
        catch (...) {
            return gd::FLAlertLayer::create(
                nullptr,
                "bruh",
                "Stop trying to break my mods by using character bypass",
                "OK", nullptr, 240.f, false, 140.f
            )->show();
        }

        switch (value.size()) {
        case 6: {
            auto r = static_cast<uint8_t>((num_value & 0xFF0000) >> 16);
            auto g = static_cast<uint8_t>((num_value & 0x00FF00) >> 8);
            auto b = static_cast<uint8_t>((num_value & 0x0000FF));

            color = { r, g, b };
        } break;

        case 3: {
            auto r = static_cast<uint8_t>(((num_value & 0xF00) >> 8) * 17);
            auto g = static_cast<uint8_t>(((num_value & 0x0F0) >> 4) * 17);
            auto b = static_cast<uint8_t>(((num_value & 0x00F)) * 17);

            color = { r, g, b };
        } break;

        case 2: {
            auto number = static_cast<uint8_t>(num_value);

            color = { number, number, number };
        } break;

        default: return;
        }

        ignore = true;
        parent->m_colorPicker->setColorValue(color);
        ignore = false;

        update_labels(false, true, true);

    }
    else if (input == red_input || input == green_input || input == blue_input) {
        std::string value(input->getString());
        try {
            auto _num = value.empty() ? 0 : std::stoi(value);
            if (_num > 255) {
                _num = 255;
                input->setString("255");
            }
            GLubyte num = static_cast<GLubyte>(_num);
            auto color = parent->m_colorPicker->getColorValue();
            if (input == red_input)
                color.r = num;
            else if (input == green_input)
                color.g = num;
            else if (input == blue_input)
                color.b = num;
            ignore = true;
            parent->m_colorPicker->setColorValue(color);
            ignore = false;
            update_labels(true, false, true);
        }
        catch (...) {}
    }
    //else if (input == h_input || input == s_input || input == v_input) {
    //    std::string value(input->getString());
    //    try {
    //        auto _num = value.empty() ? 0 : std::stoi(value);
    //        if (_num > 255) {
    //            _num = 255;
    //            input->setString("255");
    //        }
    //        GLubyte num = static_cast<GLubyte>(_num);
    //        auto color = parent->m_colorPicker->getColorValue();
    //        auto hsv_value = color_utils::rgb_to_hsv({ color.r / 255., color.g / 255., color.b / 255. });
    //        double h_value, s_value, v_value;
    //        h_value = hsv_value.h;
    //        s_value = hsv_value.s;
    //        v_value = hsv_value.v;
    //        if (h_input) {
    //            h_value = (num * 360.) / 255;
    //        }
    //        else if (s_input) {
    //            s_value = (num * 1.) / 255;
    //        }
    //        else if (v_input) {
    //            v_value = (num * 1.) / 255;
    //        }
    //        auto finalValue = color_utils::hsv_to_rgb({ h_value, s_value, v_value });
    //        ignore = true;
    //        std::cout << h_value << std::endl;
    //        std::cout << s_value << std::endl;
    //        std::cout << v_value << std::endl;
    //        //parent->m_colorPicker->setColorValue(color);
    //        std::cout << finalValue.r << std::endl;
    //        std::cout << finalValue.g << std::endl;
    //        std::cout << finalValue.b << std::endl;
    //        ignore = false;
    //        update_labels(true, true, false);
    //    }
    //    catch (...) {}
    //}
}

void RGBColorInputWidget::update_labels(bool hex, bool rgb, bool hsv) {
    if (ignore) return;
    ignore = true;
    auto color = parent->m_colorPicker->getColorValue();
    auto hsv_value = color_utils::rgb_to_hsv({ color.r / 255., color.g / 255., color.b / 255. });
    auto h_value = (hsv_value.h * 255.) / 360;
    auto s_value = (hsv_value.s * 255.) / 1;
    auto v_value = (hsv_value.v * 255.) / 1;
    if (hex) {
        hex_input->setString(color_to_hex(color).c_str());
    }
    if (rgb) {
        red_input->setString(std::to_string(color.r).c_str());
        green_input->setString(std::to_string(color.g).c_str());
        blue_input->setString(std::to_string(color.b).c_str());
    }
    //if (hsv) {
    //    h_input->setString(CCString::createWithFormat("%.0f%", h_value)->getCString());
    //    s_input->setString(CCString::createWithFormat("%.0f%", s_value)->getCString());
    //    v_input->setString(CCString::createWithFormat("%.0f%", v_value)->getCString());
    //}
    ignore = false;
}

RGBColorInputWidget* RGBColorInputWidget::create(gd::ColorSelectPopup* parent) {
    auto pRet = new RGBColorInputWidget();

    if (pRet && pRet->init(parent)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}
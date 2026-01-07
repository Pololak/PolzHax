#include "EditorLayerInput.hpp"
#include "EditorUI.hpp"

bool EditorLayerInput::init(gd::EditorUI* ui) {
    m_editorUI = ui;
    if (!CCLayer::init()) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto inputBg = extension::CCScale9Sprite::create("square02_small.png");
    inputBg->setOpacity(100);
    inputBg->setContentSize({ 40.f, 30.f });
    inputBg->setScale(.8f);
    this->addChild(inputBg, -1);

    m_input = gd::CCTextInputNode::create(40.f, 30.f, "G", this, "bigFont.fnt");
    m_input->setDelegate(this);
    m_input->setLabelPlaceholderColor(ccc3(150, 150, 150));
    m_input->setLabelPlaceholderScale(.5f);
    m_input->setCharLimit(3);
    m_input->setAllowedChars("AaLl0123456789");
    m_input->setMaxLabelScale(.5f);
    if (ui->m_editorLayer->m_groupIDFilter < 0) {
        m_input->setString("All");
    }
    else {
        m_input->setString(std::to_string(ui->m_editorLayer->m_groupIDFilter).c_str());
    }
    this->addChild(m_input);

    return true;
}

bool compareIgnoreCase(const std::string& str1, const std::string& str2) {
    if (str1.length() != str2.length()) {
        return false;
    }

    return std::equal(str1.begin(), str1.end(), str2.begin(),
        [](char a, char b) {
            return std::tolower(a) == std::tolower(b);
        });
}

void EditorLayerInput::textChanged(gd::CCTextInputNode* input) {
    if (m_editorUI == nullptr) return;

    if (compareIgnoreCase(input->getString(), "All")) {
        m_editorUI->m_editorLayer->m_groupIDFilter = -1;
    }
    else {
        m_editorUI->m_editorLayer->m_groupIDFilter = std::atoi(input->getString().c_str());
    }

    auto onAllGroup = static_cast<gd::CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(m_editorUI->m_deselectBtn->getParent())->getChildByTag(2702));
    if (onAllGroup) {
        onAllGroup->setVisible(!(m_editorUI->m_editorLayer->m_groupIDFilter == -1));
        onAllGroup->setEnabled(!(m_editorUI->m_editorLayer->m_groupIDFilter == -1));
    }

    EditorUI::updateGuideTogglePosition(m_editorUI);
}

void EditorLayerInput::updateInputNode() {
    if (m_editorUI == nullptr) return;

    if (m_editorUI->m_editorLayer->m_groupIDFilter < 0) {
        m_input->setString("All");
    }
    else {
        m_input->setString(std::to_string(m_editorUI->m_editorLayer->m_groupIDFilter).c_str());
    }
}

EditorLayerInput* EditorLayerInput::create(gd::EditorUI* ui) {
    EditorLayerInput* ret = new EditorLayerInput();
    if (ret && ret->init(ui)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
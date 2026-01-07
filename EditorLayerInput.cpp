#include "EditorLayerInput.h"

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

    m_input = gd::CCTextInputNode::create("G", this, "bigFont.fnt", 40.f, 30.f);
    m_input->setDelegate(this);
    m_input->setLabelPlaceholderColor(ccc3(150, 150, 150));
    m_input->setLabelPlaceholderScale(.5f);
    m_input->setMaxLabelLength(3);
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
        m_editorUI->m_editorLayer->m_groupIDFilter = std::atoi(input->getString());
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

bool ObjectGroupInput::init(gd::SetGroupIDLayer* parent) {
    m_parent = parent;

    m_layerInput = gd::CCTextInputNode::create("G", this, "bigFont.fnt", 60.f, 35.f);
    m_layerInput->setLabelPlaceholderColor({ 120, 120, 120 });
    m_layerInput->setAllowedChars("0123456789");
    m_layerInput->setDelegate(this);
    m_layerInput->setString(from<CCLabelBMFont*>(parent, 0x1c4)->getString());

    auto bg = extension::CCScale9Sprite::create("square02_small.png");
    bg->setContentSize({ 60.f, 35.f });
    bg->setZOrder(-1);
    bg->setOpacity(75);

    addChild(m_layerInput);
    addChild(bg);
}
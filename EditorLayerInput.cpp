#include "EditorLayerInput.h"

bool EditorLayerInput::init(gd::EditorUI* parent) {
	m_parent = parent;

	m_layerInput = gd::CCTextInputNode::create("All", this, "bigFont.fnt", 40.f, 30.f);
	m_layerInput->setLabelPlaceholderColor({ 120, 120, 120 });
	m_layerInput->setAllowedChars("012345689");
	m_layerInput->setAnchorPoint({ 0.f, 0.f });
	m_layerInput->setScale(.6f);
	m_layerInput->setDelegate(this);
	m_layerInput->setString(parent->m_currentGroupLabel->getString());

	auto bg = extension::CCScale9Sprite::create("square02_small.png");
	bg->setContentSize({ 50.f, 30.f });
	bg->setZOrder(-1);
	bg->setOpacity(75);
	bg->setScale(.7f);

	addChild(m_layerInput);
	addChild(bg);
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
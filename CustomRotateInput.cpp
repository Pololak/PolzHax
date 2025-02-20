#include "CustomRotateInput.h"

bool CustomRotateInput::init(gd::GJRotationControl* rotationControl) {
	m_parent = rotationControl;

	m_rotateInput = gd::CCTextInputNode::create("Rot", this, "bigFont.fnt", 50.f, 30.f);
	m_rotateInput->setLabelPlaceholderColor({ 120, 120, 120 });
	m_rotateInput->setAllowedChars("0123456789.");
	m_rotateInput->setDelegate(this);


}
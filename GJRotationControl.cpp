#include "GJRotationControl.hpp"

class CustomRotateInput : public CCNode, gd::TextInputDelegate {
protected:
	gd::GJRotationControl* m_parent;

	virtual bool init(gd::GJRotationControl* parent) {
		m_parent = parent;
		if (!CCNode::init()) return false;

		auto inputBg = extension::CCScale9Sprite::create("square02_small.png");
		inputBg->setOpacity(75);
		inputBg->setContentSize({ 45.f, 30.f });
		this->addChild(inputBg);

		m_input = gd::CCTextInputNode::create(40.f, 30.f, "Rot", this, "bigFont.fnt");
		m_input->setLabelPlaceholderColor(ccGRAY);
		m_input->setAllowedChars("0123456789");
		m_input->setDelegate(this);
		this->addChild(m_input);

		return true;
	}

	virtual void textChanged(gd::CCTextInputNode* input) {
		auto value = std::atof(input->getString().c_str());
		m_parent->m_delegate->angleChangeBegin();
		m_parent->m_delegate->angleChanged(value);
		m_parent->m_delegate->angleChangeEnded();
	}

public:
	gd::CCTextInputNode* m_input;

	static CustomRotateInput* create(gd::GJRotationControl* parent) {
		CustomRotateInput* ret = new CustomRotateInput();
		if (ret && ret->init(parent)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

bool __fastcall GJRotationControl::initH(gd::GJRotationControl* self) {
	if (!GJRotationControl::init(self)) return false;

	auto customRotateInput = CustomRotateInput::create(self);
	customRotateInput->setPosition(90.f, 45.f);
	self->addChild(customRotateInput);

	return true;
}

void GJRotationControl::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4fce0), GJRotationControl::initH, reinterpret_cast<void**>(&GJRotationControl::init));
}
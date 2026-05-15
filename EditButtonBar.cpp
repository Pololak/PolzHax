#include "EditButtonBar.hpp"
#include "EditorUI.hpp"
#include "Setting.hpp"
#include "utils.hpp"

bool __fastcall EditButtonBar::initH(gd::EditButtonBar* self, void*, CCArray* objs, int page, bool isObjects, CCPoint pos) {
	auto editorUI = EditorUI::get();
	
	if (editorUI) {
		if (isObjects) {
			if (setting().onUnusedObjects) {
				switch (page) {
				case 3:
					objs->addObject(editorUI->getCreateBtn(371, 4));
					objs->addObject(editorUI->getCreateBtn(372, 4));
					objs->addObject(editorUI->getCreateBtn(373, 4));
					objs->addObject(editorUI->getCreateBtn(374, 4));
					break;
				case 4:
					objs->addObject(editorUI->getCreateBtn(421, 4));
					objs->addObject(editorUI->getCreateBtn(422, 4));
					break;
				case 6:
					objs->addObject(editorUI->getCreateBtn(142, 4));
					break;
				case 7:
					objs->addObject(editorUI->getCreateBtn(419, 4));
					objs->addObject(editorUI->getCreateBtn(420, 4));
					break;
				case 8:
					objs->addObject(editorUI->getCreateBtn(466, 4));
					objs->addObject(editorUI->getCreateBtn(461, 4));
					objs->addObject(editorUI->getCreateBtn(462, 4));
					objs->addObject(editorUI->getCreateBtn(463, 4));
					objs->addObject(editorUI->getCreateBtn(464, 4));
					objs->addObject(editorUI->getCreateBtn(465, 4));
					objs->addObject(editorUI->getCreateBtn(406, 4));
					objs->addObject(editorUI->getCreateBtn(407, 4));
					objs->addObject(editorUI->getCreateBtn(408, 4));
					objs->addObject(editorUI->getCreateBtn(725, 4));
					break;
				case 11:
					objs->addObject(editorUI->getCreateBtn(55, 4));
					break;
				}
			}
		}
		else {
			// 1/2

			auto halfLabel = CCLabelBMFont::create("1/2", "bigFont.fnt");
			halfLabel->setScale(0.5f);
			halfLabel->setPosition({ 20.f, 10.f });

			auto halfUp = editorUI->getSpriteButton("edit_upBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(halfUp->getChildren()->objectAtIndex(0))->addChild(halfLabel, 5);
			halfUp->setTag(103);
			objs->addObject(halfUp);

			auto halfDown = editorUI->getSpriteButton("edit_downBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(halfDown->getChildren()->objectAtIndex(0))->addChild(halfLabel, 5);
			halfDown->setTag(104);
			objs->addObject(halfDown);

			auto halfLeft = editorUI->getSpriteButton("edit_leftBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(halfLeft->getChildren()->objectAtIndex(0))->addChild(halfLabel, 5);
			halfLeft->setTag(101);
			objs->addObject(halfLeft);

			auto halfRight = editorUI->getSpriteButton("edit_rightBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(halfRight->getChildren()->objectAtIndex(0))->addChild(halfLabel, 5);
			halfRight->setTag(102);
			objs->addObject(halfRight);

			// 1/4

			auto quarterLabel = CCLabelBMFont::create("1/4", "bigFont.fnt");
			quarterLabel->setScale(0.5f);
			quarterLabel->setPosition({ 20.f, 10.f });

			auto quarterUp = editorUI->getSpriteButton("edit_upBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(quarterUp->getChildren()->objectAtIndex(0))->addChild(quarterLabel, 5);
			quarterUp->setTag(107);
			objs->addObject(quarterUp);

			auto quarterDown = editorUI->getSpriteButton("edit_downBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(quarterDown->getChildren()->objectAtIndex(0))->addChild(quarterLabel, 5);
			quarterDown->setTag(108);
			objs->addObject(quarterDown);

			auto quarterLeft = editorUI->getSpriteButton("edit_leftBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(quarterLeft->getChildren()->objectAtIndex(0))->addChild(quarterLabel, 5);
			quarterLeft->setTag(105);
			objs->addObject(quarterLeft);

			auto quarterRight = editorUI->getSpriteButton("edit_rightBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(quarterRight->getChildren()->objectAtIndex(0))->addChild(quarterLabel, 5);
			quarterRight->setTag(106);
			objs->addObject(quarterRight);

			// 1/8

			auto eightLabel = CCLabelBMFont::create("1/8", "bigFont.fnt");
			eightLabel->setScale(0.5f);
			eightLabel->setPosition({ 20.f, 10.f });

			auto eightUp = editorUI->getSpriteButton("edit_upBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(eightUp->getChildren()->objectAtIndex(0))->addChild(eightLabel, 5);
			eightUp->setTag(111);
			objs->addObject(eightUp);

			auto eightDown = editorUI->getSpriteButton("edit_downBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(eightDown->getChildren()->objectAtIndex(0))->addChild(eightLabel, 5);
			eightDown->setTag(112);
			objs->addObject(eightDown);

			auto eightLeft = editorUI->getSpriteButton("edit_leftBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(eightLeft->getChildren()->objectAtIndex(0))->addChild(eightLabel, 5);
			eightLeft->setTag(109);
			objs->addObject(eightLeft);

			auto eightRight = editorUI->getSpriteButton("edit_rightBtn2_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(eightRight->getChildren()->objectAtIndex(0))->addChild(eightLabel, 5);
			eightRight->setTag(110);
			objs->addObject(eightRight);

			// 0.5

			auto smallerLabel = CCLabelBMFont::create("0.5", "bigFont.fnt");
			smallerLabel->setScale(0.5f);
			smallerLabel->setPosition({ 20.f, 10.f });

			auto smallerUp = editorUI->getSpriteButton("edit_upBtn_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(smallerUp->getChildren()->objectAtIndex(0))->addChild(smallerLabel, 5);
			smallerUp->setTag(115);
			objs->addObject(smallerUp);

			auto smallerDown = editorUI->getSpriteButton("edit_downBtn_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(smallerDown->getChildren()->objectAtIndex(0))->addChild(smallerLabel, 5);
			smallerDown->setTag(116);
			objs->addObject(smallerDown);

			auto smallerLeft = editorUI->getSpriteButton("edit_leftBtn_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(smallerLeft->getChildren()->objectAtIndex(0))->addChild(smallerLabel, 5);
			smallerLeft->setTag(113);
			objs->addObject(smallerLeft);

			auto smallerRight = editorUI->getSpriteButton("edit_rightBtn_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(smallerRight->getChildren()->objectAtIndex(0))->addChild(smallerLabel, 5);
			smallerRight->setTag(114);
			objs->addObject(smallerRight);

			// 0.1

			auto unitLabel = CCLabelBMFont::create("0.1", "bigFont.fnt");
			unitLabel->setScale(0.5f);
			unitLabel->setPosition({ 20.f, 10.f });

			auto unitUp = editorUI->getSpriteButton("edit_upBtn_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(unitUp->getChildren()->objectAtIndex(0))->addChild(unitLabel, 5);
			unitUp->setTag(119);
			objs->addObject(unitUp);

			auto unitDown = editorUI->getSpriteButton("edit_downBtn_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(unitDown->getChildren()->objectAtIndex(0))->addChild(unitLabel, 5);
			unitDown->setTag(120);
			objs->addObject(unitDown);

			auto unitLeft = editorUI->getSpriteButton("edit_leftBtn_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(unitLeft->getChildren()->objectAtIndex(0))->addChild(unitLabel, 5);
			unitLeft->setTag(117);
			objs->addObject(unitLeft);

			auto unitRight = editorUI->getSpriteButton("edit_rightBtn_001.png", menu_selector(gd::EditorUI::moveObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(unitRight->getChildren()->objectAtIndex(0))->addChild(unitLabel, 5);
			unitRight->setTag(118);
			objs->addObject(unitRight);

			// 45

			auto rotate45Label = CCLabelBMFont::create("45", "bigFont.fnt");
			rotate45Label->setScale(.45f);
			rotate45Label->setPosition(20.f, 22.f);

			auto rotate45CW = editorUI->getSpriteButton("edit_cwBtn_001.png", menu_selector(gd::EditorUI::transformObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(rotate45CW->getChildren()->objectAtIndex(0))->addChild(rotate45Label, 5);
			rotate45CW->setTag(121);
			objs->addObject(rotate45CW);

			auto rotate45CCW = editorUI->getSpriteButton("edit_ccwBtn_001.png", menu_selector(gd::EditorUI::transformObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(rotate45CCW->getChildren()->objectAtIndex(0))->addChild(rotate45Label, 5);
			rotate45CCW->setTag(122);
			objs->addObject(rotate45CCW);

			// 26

			auto rotate26Label = CCLabelBMFont::create("26", "bigFont.fnt");
			rotate26Label->setScale(.45f);
			rotate26Label->setPosition(20.f, 22.f);

			auto rotate26CW = editorUI->getSpriteButton("edit_cwBtn_001.png", menu_selector(gd::EditorUI::transformObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(rotate26CW->getChildren()->objectAtIndex(0))->addChild(rotate26Label, 5);
			rotate26CW->setTag(123);
			objs->addObject(rotate26CW);

			auto rotate26CCW = editorUI->getSpriteButton("edit_ccwBtn_001.png", menu_selector(gd::EditorUI::transformObjectCall), nullptr, .9f);
			static_cast<gd::ButtonSprite*>(rotate26CCW->getChildren()->objectAtIndex(0))->addChild(rotate26Label, 5);
			rotate26CCW->setTag(124);
			objs->addObject(rotate26CCW);
		}
	}

	if (!EditButtonBar::init(self, objs, page, isObjects, pos)) return false;

	//if (isObjects) {
	//	if (page == 11) {
	//		for (auto btn : CCArrayExt<gd::CreateMenuItem*>(objs)) {
	//			if (btn) {
	//				auto buttonSprite = static_cast<gd::ButtonSprite*>(btn->getChildren()->objectAtIndex(0));
	//				if (buttonSprite) {
	//					auto object = reinterpret_cast<gd::GameObject*>(buttonSprite->getChildren()->objectAtIndex(0));
	//					if (object) {
	//						std::cout << object->m_objectID << ", ";
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	return true;
}

void EditButtonBar::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3ab60), EditButtonBar::initH, reinterpret_cast<void**>(&EditButtonBar::init));
}
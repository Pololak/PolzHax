#define NOMINMAX
#include "EditorUI.hpp"
#include "Setting.hpp"
#include "GlobalClipboard.hpp"
#include "utils.hpp"
#include "EditorLayerInput.hpp"
#include "LevelEditorLayer.hpp"
#include "ColorFilterPopup.hpp"
#include <unordered_set>
#include <array>
#include <numbers>
#include "RotateSaws.hpp"
#include "NewCustomizeObjectLayer.hpp"
#include "CircleToolPopup.hpp"

gd::EditorUI* m_editorUI;

gd::EditorUI* EditorUI::get() {
	return m_editorUI;
}

CCLabelBMFont* m_gridSizeLabel = nullptr;

std::unordered_set<int> m_gameObjects = {
	1, 83, 2, 3, 4, 5, 502, 6, 7, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 80, 81, 82, 90, 91, 92, 93, 94, 95, 96, 116, 117, 118, 119, 120, 121, 122, 160, 161, 162, 163, 164, 165, 166, 167, 737, 168, 169, 193, 207, 208, 209, 210, 211, 212, 213, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 476, 477, 478, 479, 480, 481, 482, 485, 486, 487, 488, 489, 490, 491, 641, 642, 643, 644, 645, 646, 647, 648, 739, 649, 650, 658, 722, 659, 734, 40, 369, 370, 195, 196, 215, 220, 219, 146, 147, 206, 204, 673, 674, 62, 66, 65, 68, 63, 64, 170, 171, 172, 192, 173, 194, 174, 175, 176, 197, 143, 653, 724, 654, 736, 657, 656, 723, 655, 735, 467, 468, 469, 470, 471, 472, 473, 474, 475, 661, 662, 663, 664, 665, 666, 289, 291, 299, 301, 305, 307, 309, 311, 315, 317, 321, 323, 324, 325, 358, 294, 295, 296, 297, 326, 327, 328, 329, 331, 333, 337, 339, 343, 345, 349, 351, 353, 355, 483, 484, 492, 493, 651, 652, 709, 710, 726, 727, 711, 712, 728, 729, 681, 682, 683, 684, 685, 686, 687, 688, 689, 690, 691, 692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 702, 703, 704, 705, 706, 707, 708, 713, 714, 730, 731, 715, 716, 732, 733, 371, 372, 373, 374, 8, 39, 103, 392, 216, 217, 218, 458, 144, 205, 145, 459, 177, 178, 179, 191, 198, 199, 393, 9, 61, 243, 244, 135, 363, 364, 365, 366, 367, 368, 446, 447, 667, 720, 421, 422, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 35, 140, 67, 36, 141, 84, 10, 11, 12, 13, 47, 111, 660, 45, 46, 99, 101, 286, 287, 200, 201, 202, 203, 142, 18, 19, 20, 21, 48, 49, 113, 114, 115, 157, 158, 159, 227, 228, 242, 448, 449, 450, 451, 452, 503, 504, 505, 419, 420, 41, 110, 106, 107, 498, 499, 129, 130, 131, 128, 123, 124, 125, 126, 127, 134, 190, 151, 152, 153, 225, 226, 500, 501, 229, 230, 237, 238, 239, 240, 241, 231, 232, 283, 284, 285, 279, 280, 233, 281, 282, 234, 235, 277, 278, 245, 246, 409, 410, 411, 412, 413, 453, 454, 455, 456, 457, 668, 669, 670, 671, 672, 738, 719, 721, 466, 461, 462, 463, 464, 465, 406, 407, 408, 725, 15, 16, 17, 50, 51, 52, 53, 54, 60, 148, 149, 405, 132, 460, 494, 133, 136, 150, 236, 497, 495, 496, 88, 89, 98, 397, 398, 399, 675, 676, 677, 186, 187, 188, 740, 741, 742, 678, 679, 680, 183, 184, 185, 85, 86, 87, 97, 137, 138, 139, 154, 155, 156, 180, 181, 182, 222, 223, 224, 375, 376, 377, 378, 394, 395, 396, 29, 30, 104, 105, 744, 221, 717, 718, 743, 31, 33, 32, 22, 24, 23, 25, 26, 27, 28, 56, 57, 58, 59, 55
};

std::string EditorUI::colorToShortString(int id) {
	switch (id) {
	case 0: return "D"; break;
	case 1: return "P1"; break;
	case 2: return "P2"; break;
	case 3: return "C1"; break;
	case 4: return "C2"; break;
	case 5: return "LBG"; break;
	case 6: return "C3"; break;
	case 7: return "C4"; break;
	case 8: return "3DL"; break;
	case 9: return "W"; break;
	default: return "Unk"; break;
	}
}

std::string colorToString(int id) {
	switch (id) {
	case 0: return "Default"; break;
	case 1: return "P-Col1"; break;
	case 2: return "P-Col2"; break;
	case 3: return "Col1"; break;
	case 4: return "Col2"; break;
	case 5: return "LBG"; break;
	case 6: return "Col3"; break;
	case 7: return "Col4"; break;
	case 8: return "3DL"; break;
	case 9: return "White"; break;
	default: return "Unknown"; break;
	}
}

std::string typeToString(gd::GameObjectType type) {
	switch (type) {
	case gd::GameObjectType::Solid: return "Solid"; break;
	case gd::GameObjectType::Hazard: return "Hazard"; break;
	case gd::GameObjectType::InverseGravityPortal: return "Inverse Gravity Portal"; break;
	case gd::GameObjectType::NormalGravityPortal: return "Normal Gravity Portal"; break;
	case gd::GameObjectType::ShipPortal: return "Ship Portal"; break;
	case gd::GameObjectType::CubePortal: return "Cube Portal"; break;
	case gd::GameObjectType::Decoration: return "Decoration"; break;
	case gd::GameObjectType::PulsingDecoration: return "Pulse Object"; break;
	case gd::GameObjectType::YellowJumpPad: return "Yellow Jump Pad"; break;
	case gd::GameObjectType::PinkJumpPad: return "Pink Jump Pad"; break;
	case gd::GameObjectType::GravityPad: return "Gravity Pad"; break;
	case gd::GameObjectType::YellowJumpRing: return "Yellow Jump Ring"; break;
	case gd::GameObjectType::PinkJumpRing: return "Pink Jump Ring"; break;
	case gd::GameObjectType::GravityRing: return "Gravity Ring"; break;
	case gd::GameObjectType::InverseMirrorPortal: return "Inverse Mirror Portal"; break;
	case gd::GameObjectType::NormalMirrorPortal: return "Normal Mirror Portal"; break;
	case gd::GameObjectType::BallPortal: return "Ball Portal"; break;
	case gd::GameObjectType::RegularSizePortal: return "Regular Size Portal"; break;
	case gd::GameObjectType::MiniSizePortal: return "Mini Size Portal"; break;
	case gd::GameObjectType::UfoPortal: return "UFO Portal"; break;
	case gd::GameObjectType::Modifier: return "Modifier"; break;
	case gd::GameObjectType::Breakable: return "Breakable"; break;
	case gd::GameObjectType::SecretCoin: return "Secret Coin"; break;
	case gd::GameObjectType::DualPortal: return "Dual Portal"; break;
	case gd::GameObjectType::SoloPortal: return "Solo Portal"; break;
	case gd::GameObjectType::Slope: return "Slope"; break;
	case gd::GameObjectType::WavePortal: return "Wave Portal"; break;
	default: return "Unknown"; break;
	}
}

void EditorUI::updateObjectInfoLabel(gd::EditorUI* self) {
	if (!setting().onShowObjectInfo) return;

	auto objectInfoLabel = static_cast<CCLabelBMFont*>(self->getChildByTag(2701));
	if (objectInfoLabel) {
		if (self->m_selectedObject || self->m_selectedObjects->count() == 1) {
			std::stringstream ss;

			gd::GameObject* object;
			if (self->m_selectedObjects->count() == 1) {
				object = reinterpret_cast<gd::GameObject*>(self->m_selectedObjects->objectAtIndex(0));
			}
			else {
				object = self->m_selectedObject;
			}

			ss << "C: " << colorToString(static_cast<int>(object->getColorMode())) << " (" << static_cast<int>(object->getColorMode()) << ")" << "\n";
			ss << "G: " << object->m_editorGroup << "\n";
			ss << "Rot: " << object->getRotation() << "\n";
			ss << "X: " << object->getPositionX() << "\n";
			ss << "Y: " << object->getPositionY() << "\n";
			ss << "ID: " << object->m_objectID << "\n";
			ss << "Type: " << typeToString(object->m_objectType) << "\n";
			ss << "Time: " << self->m_editorLayer->m_gridLayer->timeForXPos(object->getPositionX()) << "\n";
			ss << "Addr: 0x" << std::hex << reinterpret_cast<uintptr_t>(object) << std::dec << "\n";

			objectInfoLabel->setString(ss.str().c_str());
		}
		else if (self->m_selectedObjects->count() > 1) {
			std::stringstream ss;

			ss << "Objects: " << self->m_selectedObjects->count() << "\n";

			objectInfoLabel->setString(ss.str().c_str());
		}
		else {
			objectInfoLabel->setString("");
		}
	}
}

void EditorUI::updateGuideTogglePosition(gd::EditorUI* self) {
	auto onAllGroup = static_cast<gd::CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(self->m_deselectBtn->getParent())->getChildByTag(2702));
	if (onAllGroup) {
		self->m_guideToggle->setPositionX(onAllGroup->isVisible() ? (self->m_groupPrevBtn->getPositionX() - 46.f) : (self->m_groupPrevBtn->getPositionX() - 26.f));
	}
}

void EditorUI::Callback::onAllGroup(CCObject* sender) {
	this->m_currentGroupLabel->setString("All");
	this->m_editorLayer->m_groupIDFilter = -1;
	
	static_cast<gd::CCMenuItemSpriteExtra*>(sender)->setEnabled(false);
	static_cast<gd::CCMenuItemSpriteExtra*>(sender)->setVisible(false);

	updateGuideTogglePosition(this);

	auto editorLayerInput = static_cast<EditorLayerInput*>(this->getChildByTag(2703));
	if (editorLayerInput) {
		editorLayerInput->updateInputNode();
	}
}

void EditorUI::Callback::onNextFreeGroup(CCObject* sender) {
	auto objs = this->m_editorLayer->getAllObjects();

	std::set<int> layers;

	for (auto obj : CCArrayExt<gd::GameObject*>(objs)) {
		layers.insert(obj->m_editorGroup);
	}

	int last = -1;
	for (auto const& layer : layers) {
		if (last + 1 != layer) break;
		last = layer;
	}

	this->m_currentGroupLabel->setString(CCString::createWithFormat("%d", last + 1)->getCString());
	this->m_editorLayer->m_groupIDFilter = last + 1;

	auto onAllGroup = static_cast<gd::CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(this->m_deselectBtn->getParent())->getChildByTag(2702));
	if (onAllGroup) {
		onAllGroup->setVisible(true);
		onAllGroup->setEnabled(true);
	}

	updateGuideTogglePosition(this);

	auto editorLayerInput = static_cast<EditorLayerInput*>(this->getChildByTag(2703));
	if (editorLayerInput) {
		editorLayerInput->updateInputNode();
	}
}

void EditorUI::Callback::onGoToGroup(CCObject*) {
	auto objs = this->getSelectedObjects();

	for (auto obj : CCArrayExt<gd::GameObject*>(objs)) {
		if (obj) {
			int objectGroup = obj->m_editorGroup;
			this->m_editorLayer->m_groupIDFilter = objectGroup;
			this->m_currentGroupLabel->setString(CCString::createWithFormat("%d", objectGroup)->getCString());

			auto onAllGroup = static_cast<gd::CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(this->m_deselectBtn->getParent())->getChildByTag(2702));
			if (onAllGroup) {
				onAllGroup->setVisible(false);
				onAllGroup->setEnabled(false);
			}

			updateGuideTogglePosition(this);

			auto editorLayerInput = static_cast<EditorLayerInput*>(this->getChildByTag(2703));
			if (editorLayerInput) {
				editorLayerInput->updateInputNode();
			}
		}
	}
}

static std::array SNAP_GRID_SIZES {
	1.f, 2.f, 3.75f, 7.5f, 15.f, 30.f, 60.f, 90.f, 120.f
};

void updateGridSizeLabel() {
	if (m_gridSizeLabel) {
		std::stringstream gridSizeStr;
		gridSizeStr << setting().m_customEditorGridSize;
		m_gridSizeLabel->setString(gridSizeStr.str().c_str());
	}
}

void decrementGridSize(gd::EditorUI* self) {
	auto next = std::lower_bound(SNAP_GRID_SIZES.begin(), SNAP_GRID_SIZES.end(), setting().m_customEditorGridSize);
	if (next != SNAP_GRID_SIZES.begin()) next--;
	setting().m_customEditorGridSize = *next;
	self->updateGridNodeSize();
	updateGridSizeLabel();
}

void incrementGridSize(gd::EditorUI* self) {
	auto next = std::upper_bound(SNAP_GRID_SIZES.begin(), SNAP_GRID_SIZES.end(), setting().m_customEditorGridSize);
	if (next == SNAP_GRID_SIZES.end()) next--;
	setting().m_customEditorGridSize = *next;
	self->updateGridNodeSize();
	updateGridSizeLabel();
}

void EditorUI::Callback::onGridSize(CCObject* sender) {
	auto btn = static_cast<gd::CCMenuItemSpriteExtra*>(sender);
	if (btn->getTag() == -1) {
		decrementGridSize(this);
	}
	else {
		incrementGridSize(this);
	}
}

bool __fastcall EditorUI::initH(gd::EditorUI* self, void*, gd::LevelEditorLayer* editorLayer) {
	m_editorUI = self;
	if (!EditorUI::init(self, editorLayer)) return false;
	self->setVisible(!setting().onHideUI);
	loadClipboard(self);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	CCMenu* leftMenu = static_cast<CCMenu*>(self->m_undoBtn->getParent());
	CCMenu* rightMenu = static_cast<CCMenu*>(self->m_deselectBtn->getParent());

	self->m_positionSlider->setAnchorPoint({ 0.f, 0.f });
	self->m_positionSlider->setScale(.8f);

	auto objectInfoLabel = CCLabelBMFont::create("", "chatFont.fnt");
	objectInfoLabel->setAnchorPoint({ 0.f, 1.f });
	objectInfoLabel->setScale(.6f);
	objectInfoLabel->setPosition(director->getScreenLeft() + 50.f, director->getScreenTop() - 50.f);
	objectInfoLabel->setVisible(setting().onShowObjectInfo);
	self->addChild(objectInfoLabel, 0, 2701);

	auto onTrashSpr = CCSprite::create("GJ_trashBtn_001.png");
	onTrashSpr->setScale(.925f);
	auto onTrash = gd::CCMenuItemSpriteExtra::create(onTrashSpr, self, menu_selector(gd::EditorUI::onDeleteSelected));
	onTrash->setPosition(self->m_redoBtn->getPositionX() + 50.f, self->m_redoBtn->getPositionY() - 1.f);
	onTrash->setOpacity(175);
	onTrash->setColor(ccGRAY);
	onTrash->setEnabled(false);
	leftMenu->addChild(onTrash, 0, 201);

	self->m_groupPrevBtn->setPositionX(self->m_groupPrevBtn->getPositionX() - 10.f);
	self->m_groupNextBtn->setPositionX(self->m_groupNextBtn->getPositionX() - 10.f);
	self->m_currentGroupLabel->setPositionX(self->m_currentGroupLabel->getPositionX() - 10.f);

	auto onAllGroupSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	onAllGroupSpr->setScale(.5f);
	onAllGroupSpr->setOpacity(175);
	auto onAllGroup = gd::CCMenuItemSpriteExtra::create(onAllGroupSpr, self, menu_selector(EditorUI::Callback::onAllGroup));
	onAllGroup->setPosition(-90.f, -172.f);
	rightMenu->addChild(onAllGroup, 0, 2702);
	onAllGroup->setVisible(!(self->m_editorLayer->m_groupIDFilter == -1));
	onAllGroup->setEnabled(!(self->m_editorLayer->m_groupIDFilter == -1));

	auto onNextFreeGroupSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	onNextFreeGroupSpr->setFlipX(true);
	onNextFreeGroupSpr->setScale(.5f);
	onNextFreeGroupSpr->setOpacity(175);
	auto onNextFreeGroup = gd::CCMenuItemSpriteExtra::create(onNextFreeGroupSpr, self, menu_selector(EditorUI::Callback::onNextFreeGroup));
	onNextFreeGroup->setPosition(10.f, -172.f);
	rightMenu->addChild(onNextFreeGroup);

	self->m_currentGroupLabel->setOpacity(0);
	auto editorLayerInput = EditorLayerInput::create(self);
	editorLayerInput->setPosition(self->m_currentGroupLabel->getPosition());
	self->addChild(editorLayerInput, 0, 2703);

	updateGuideTogglePosition(self);

	auto onGoToGroupSpr = CCSprite::create("GJ_goToGroupBtn_001.png");
	onGoToGroupSpr->setScale(.85f);
	onGoToGroupSpr->setPositionY(onGoToGroupSpr->getPositionY() - 1.f);
	auto onGoToGroup = gd::CCMenuItemSpriteExtra::create(onGoToGroupSpr, self, menu_selector(EditorUI::Callback::onGoToGroup));
	onGoToGroup->setPosition(self->m_editGroupBtn->getPositionX() - 44.f, self->m_editGroupBtn->getPositionY());
	onGoToGroup->setOpacity(175);
	onGoToGroup->setColor(ccGRAY);
	onGoToGroup->setEnabled(false);
	onGoToGroup->setVisible(false);
	rightMenu->addChild(onGoToGroup, 0, 2704);

	// fake buttons lol
	auto buttonPageMenu = static_cast<CCMenu*>(static_cast<gd::ButtonPage*>(self->m_editButtonBar->m_pagesArray->objectAtIndex(1))->getChildren()->objectAtIndex(0));
	
	if (buttonPageMenu) {
		auto freeRotateBtn = static_cast<gd::CCMenuItemSpriteExtra*>(self->m_editButtonDict->objectForKey("21"));
		auto snapRotateBtn = static_cast<gd::CCMenuItemSpriteExtra*>(self->m_editButtonDict->objectForKey("22"));

		auto fakeFreeRotate = self->getSpriteButton("edit_freeRotateBtn_001.png", nullptr, nullptr, .9f);
		auto fakeSnapRotate = self->getSpriteButton("edit_rotateSnapBtn_001.png", nullptr, nullptr, .9f);
		
		fakeFreeRotate->setEnabled(false);
		static_cast<gd::ButtonSprite*>(fakeFreeRotate->getChildren()->objectAtIndex(0))->setColor(ccGRAY);
		fakeSnapRotate->setEnabled(false);
		static_cast<gd::ButtonSprite*>(fakeSnapRotate->getChildren()->objectAtIndex(0))->setColor(ccGRAY);

		fakeFreeRotate->setPosition(freeRotateBtn->getPosition());
		fakeSnapRotate->setPosition(snapRotateBtn->getPosition());

		buttonPageMenu->addChild(fakeFreeRotate, -1);
		buttonPageMenu->addChild(fakeSnapRotate, -1);
	}
	//

	auto gridSizeMenu = CCMenu::create();
	auto gsMenuPos = ccp(director->getScreenRight() - 134.f, director->getScreenTop() - 20.f);
	if ((winSize.width / winSize.height) <= 1.6f) {
		gsMenuPos = ccp(winSize.width / 2.f, director->getScreenTop() - 50.f);
	}
	gridSizeMenu->setPosition(gsMenuPos);
	self->addChild(gridSizeMenu);

	auto onDecrementGridSpr = CCSprite::createWithSpriteFrameName("GJ_zoomInBtn_001.png");
	onDecrementGridSpr->setScale(.4f);
	auto onDecrementGrid = gd::CCMenuItemSpriteExtra::create(onDecrementGridSpr, self, menu_selector(EditorUI::Callback::onGridSize));
	onDecrementGrid->setPositionX(-25.f);
	gridSizeMenu->addChild(onDecrementGrid, 0, -1);

	auto onIncrementGridSpr = CCSprite::createWithSpriteFrameName("GJ_zoomOutBtn_001.png");
	onIncrementGridSpr->setScale(.4f);
	auto onIncrementGrid = gd::CCMenuItemSpriteExtra::create(onIncrementGridSpr, self, menu_selector(EditorUI::Callback::onGridSize));
	onIncrementGrid->setPositionX(25.f);
	gridSizeMenu->addChild(onIncrementGrid, 0, 1);

	auto gridSizeBg = extension::CCScale9Sprite::create("square02_small.png");
	gridSizeBg->setOpacity(100);
	gridSizeBg->setContentSize({ 75.f, 28.f });
	gridSizeMenu->addChild(gridSizeBg, -1);

	m_gridSizeLabel = CCLabelBMFont::create("", "bigFont.fnt");
	updateGridSizeLabel();
	m_gridSizeLabel->setScale(.35f);
	gridSizeMenu->addChild(m_gridSizeLabel);

	if (setting().onDeveloperMode) {
		auto selectedObjectInToolboxIdLabel = CCLabelBMFont::create("ID: ", "chatFont.fnt");
		selectedObjectInToolboxIdLabel->setAnchorPoint({ 0.f, .5f });
		selectedObjectInToolboxIdLabel->setScale(.65f);
		selectedObjectInToolboxIdLabel->setString(CCString::createWithFormat("ID: %i", self->m_selectedCreateObjectID)->getCString());
		selectedObjectInToolboxIdLabel->setPosition(director->getScreenLeft() + 90.f, director->getScreenBottom() + 80.f);
		self->addChild(selectedObjectInToolboxIdLabel, 10, 756);
	}

	return true;
}

void __fastcall EditorUI::selectObjectH(gd::EditorUI* self, void*, gd::GameObject* object) {
	int selectedCustomMode = gd::GameManager::sharedState()->getIntGameVariable("0005");
	if (selectedCustomMode != 3) gd::GameManager::sharedState()->setIntGameVariable("0006", 0);
	int selectFilterObject = gd::GameManager::sharedState()->getIntGameVariable("0006");

	updateObjectInfoLabel(self);

	if ((selectFilterObject != 0) && setting().onSelectFilter) {
		if (object->m_objectID == selectFilterObject) return EditorUI::selectObject(self, object);
	}
	else if ((setting().colorFilter != 0) && setting().onSelectFilter) {
		if (object->getColorMode() == static_cast<gd::GJCustomColorMode>(setting().colorFilter)) return EditorUI::selectObject(self, object);
	}
	else {
		EditorUI::selectObject(self, object);
	}
}

void __fastcall EditorUI::selectObjectsH(gd::EditorUI* self, void*, CCArray* objects) {
	int selectedCustomMode = gd::GameManager::sharedState()->getIntGameVariable("0005");
	if (selectedCustomMode != 3) gd::GameManager::sharedState()->setIntGameVariable("0006", 0);
	int selectFilterObject = gd::GameManager::sharedState()->getIntGameVariable("0006");

	updateObjectInfoLabel(self);

	if ((selectFilterObject != 0) && setting().onSelectFilter) {
		auto filteredObjects = CCArray::create();
		for (int i = 0; i < objects->count(); i++) {
			if (reinterpret_cast<gd::GameObject*>(objects->objectAtIndex(i))->m_objectID == selectFilterObject) {
				filteredObjects->addObject(objects->objectAtIndex(i));
			}
		}
		return EditorUI::selectObjects(self, filteredObjects);
	}
	else if ((setting().colorFilter != 0) && setting().onSelectFilter) {
		auto filteredObjects = CCArray::create();
		for (int i = 0; i < objects->count(); i++) {
			if (reinterpret_cast<gd::GameObject*>(objects->objectAtIndex(i))->getColorMode() == static_cast<gd::GJCustomColorMode>(setting().colorFilter)) {
				filteredObjects->addObject(objects->objectAtIndex(i));
			}
		}
		return EditorUI::selectObjects(self, filteredObjects);
	}
	else {
		return EditorUI::selectObjects(self, objects);
	}

	EditorUI::selectObjects(self, objects);
}

void __fastcall EditorUI::moveObjectH(gd::EditorUI* self, void*, gd::GameObject* object, CCPoint pos) {
	if (object == nullptr) return;

	EditorUI::moveObject(self, object, pos);

	updateObjectInfoLabel(self);

	LevelEditorLayer::moveTrigger(object);
}

void __fastcall EditorUI::angleChangedH(gd::EditorUI* _self, void*, float angle) {
	auto self = reinterpret_cast<gd::EditorUI*>(reinterpret_cast<uintptr_t>(_self) - 0x120);
	EditorUI::angleChanged(_self, angle);

	updateObjectInfoLabel(self);
}

void __fastcall EditorUI::updateButtonsH(gd::EditorUI* self, void*) {
	EditorUI::updateButtons(self);

	CCMenu* leftMenu = static_cast<CCMenu*>(self->m_undoBtn->getParent());
	CCMenu* rightMenu = static_cast<CCMenu*>(self->m_deselectBtn->getParent());

	auto onTrash = static_cast<gd::CCMenuItemSpriteExtra*>(leftMenu->getChildByTag(201));
	if (onTrash) {
		if (self->getSelectedObjects()->count()) {
			onTrash->setOpacity(255);
			onTrash->setColor(ccWHITE);
			onTrash->setEnabled(true);
		}
		else {
			onTrash->setOpacity(175);
			onTrash->setColor(ccGRAY);
			onTrash->setEnabled(false);
		}
	}

	auto onGoToGroup = static_cast<gd::CCMenuItemSpriteExtra*>(rightMenu->getChildByTag(2704));
	if (onGoToGroup) {
		if (self->getSelectedObjects()->count()) {
			onGoToGroup->setOpacity(255);
			onGoToGroup->setColor(ccWHITE);
			onGoToGroup->setEnabled(true);
			onGoToGroup->setVisible(true);
		}
		else {
			onGoToGroup->setOpacity(175);
			onGoToGroup->setColor(ccGRAY);
			onGoToGroup->setEnabled(false);
			onGoToGroup->setVisible(false);
		}
	}

	updateObjectInfoLabel(self);
}

void __fastcall EditorUI::clickOnPositionH(gd::EditorUI* self, void*, cocos2d::CCPoint pos) {
	EditorUI::clickOnPosition(self, pos);

	updateObjectInfoLabel(self);
}

void __fastcall EditorUI::toggleSpecialEditButtonsH(gd::EditorUI* self) {
	EditorUI::toggleSpecialEditButtons(self);

	auto freeRotateBtn = static_cast<gd::CCMenuItemSpriteExtra*>(self->m_editButtonDict->objectForKey("21"));
	if (freeRotateBtn == nullptr) return;
}

CCPoint* __fastcall EditorUI::offsetForKeyH(gd::EditorUI* self, void*, int id) {
	CCPoint* ret = EditorUI::offsetForKey(self, id);

	switch (id) {
	case 185:
		*ret = ccp(-4.5f, 4.5f); return ret; break;
	case 397:
		*ret = ccp(0, -8.f); return ret; break;
	case 398:
		*ret = ccp(0, -11.f); return ret; break;
	case 399:
		*ret = ccp(0, -12.5f); return ret; break;
	case 410:
		*ret = ccp(0, -2.5f); return ret; break;
	case 411:
		*ret = ccp(0, -2.5f); return ret; break;
	case 412:
		*ret = ccp(0, -9.f); return ret; break;
	case 413:
		*ret = ccp(0, -9.f); return ret; break;
	default:
		return EditorUI::offsetForKey(self, id); break;
	}
}

CCPoint* __fastcall EditorUI::moveForCommandH(gd::EditorUI* self, void*, CCPoint* pos, gd::EditCommand command) {
	float gridSize = self->m_gridSize;

	switch (command) {
	case static_cast<gd::EditCommand>(101): *pos = ccp(-1.f / 2.f, 0.f) * gridSize; return pos; // half left
	case static_cast<gd::EditCommand>(102): *pos = ccp(1.f / 2.f, 0.f) * gridSize; return pos; // half right
	case static_cast<gd::EditCommand>(103): *pos = ccp(0.f, 1.f / 2.f) * gridSize; return pos; // half up
	case static_cast<gd::EditCommand>(104): *pos = ccp(0.f, -1.f / 2.f) * gridSize; return pos; // half down

	case static_cast<gd::EditCommand>(105): *pos = ccp(-1.f / 4.f, 0.f) * gridSize; return pos; // quarter left
	case static_cast<gd::EditCommand>(106): *pos = ccp(1.f / 4.f, 0.f) * gridSize; return pos; // quarter right
	case static_cast<gd::EditCommand>(107): *pos = ccp(0.f, 1.f / 4.f) * gridSize; return pos; // quarter up
	case static_cast<gd::EditCommand>(108): *pos = ccp(0.f, -1.f / 4.f) * gridSize; return pos; // quarter down

	case static_cast<gd::EditCommand>(109): *pos = ccp(-1.f / 8.f, 0.f) * gridSize; return pos; // eight left
	case static_cast<gd::EditCommand>(110): *pos = ccp(1.f / 8.f, 0.f) * gridSize; return pos; // eight right
	case static_cast<gd::EditCommand>(111): *pos = ccp(0.f, 1.f / 8.f) * gridSize; return pos; // eight up
	case static_cast<gd::EditCommand>(112): *pos = ccp(0.f, -1.f / 8.f) * gridSize; return pos; // eight down

	case static_cast<gd::EditCommand>(113): *pos = ccp(-.5f, 0.f); return pos; // smaller left
	case static_cast<gd::EditCommand>(114): *pos = ccp(.5f, 0.f); return pos; // smaller right
	case static_cast<gd::EditCommand>(115): *pos = ccp(0.f, .5f); return pos; // smaller up
	case static_cast<gd::EditCommand>(116): *pos = ccp(0.f, -.5f); return pos; // smaller down

	case static_cast<gd::EditCommand>(117): *pos = ccp(-.1f, 0.f); return pos; // unit left
	case static_cast<gd::EditCommand>(118): *pos = ccp(.1f, 0.f); return pos; // unit right
	case static_cast<gd::EditCommand>(119): *pos = ccp(0.f, .1f); return pos; // unit up
	case static_cast<gd::EditCommand>(120): *pos = ccp(0.f, -.1f); return pos; // unit down

	default: return EditorUI::moveForCommand(self, pos, command);
	}
}

void __fastcall EditorUI::transformObjectH(gd::EditorUI* self, void*, gd::GameObject* obj, gd::EditCommand command, bool p0) {
	CCArray* selectedObjects = self->getSelectedObjects();
	int selectedObjectsCount = selectedObjects->count();

	if (obj->canRotateFree()) {
		switch (command) {
		case static_cast<gd::EditCommand>(121):
			self->rotateObjects(selectedObjects, (45.f / selectedObjectsCount), ccp(0.f, 0.f)); break;
		case static_cast<gd::EditCommand>(122):
			self->rotateObjects(selectedObjects, -(45.f / selectedObjectsCount), ccp(0.f, 0.f)); break;
		case static_cast<gd::EditCommand>(123):
			self->rotateObjects(selectedObjects, (26.f / selectedObjectsCount), ccp(0.f, 0.f)); break;
		case static_cast<gd::EditCommand>(124):
			self->rotateObjects(selectedObjects, -(26.f / selectedObjectsCount), ccp(0.f, 0.f)); break;
		}
	}

	EditorUI::transformObject(self, obj, command, p0);

	updateObjectInfoLabel(self);
}

void __fastcall EditorUI::onCopyH(gd::EditorUI* self, void*, CCObject* sender) {
	if (setting().onPreviewRotations) RotateSaws::stopRotations(self->m_editorLayer);

	EditorUI::onCopy(self, sender);

	if (setting().onPreviewRotations) RotateSaws::beginRotations(self->m_editorLayer);

	if (setting().onCopyString) {
		clipboard::write(self->m_clipboard);
	}
}

void __fastcall EditorUI::onGroupDownH(gd::EditorUI* self, void*, CCObject* sender) {
	EditorUI::onGroupDown(self, sender);
	auto onAllGroup = static_cast<gd::CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(self->m_deselectBtn->getParent())->getChildByTag(2702));
	if (onAllGroup) {
		onAllGroup->setVisible(!(self->m_editorLayer->m_groupIDFilter == -1));
		onAllGroup->setEnabled(!(self->m_editorLayer->m_groupIDFilter == -1));
	}

	updateGuideTogglePosition(self);

	auto editorLayerInput = static_cast<EditorLayerInput*>(self->getChildByTag(2703));
	if (editorLayerInput) {
		editorLayerInput->updateInputNode();
	}
}

void __fastcall EditorUI::onGroupUpH(gd::EditorUI* self, void*, CCObject* sender) {
	EditorUI::onGroupUp(self, sender);
	auto onAllGroup = static_cast<gd::CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(self->m_deselectBtn->getParent())->getChildByTag(2702));
	if (onAllGroup) {
		onAllGroup->setVisible(true);
		onAllGroup->setEnabled(true);
	}

	updateGuideTogglePosition(self);

	auto editorLayerInput = static_cast<EditorLayerInput*>(self->getChildByTag(2703));
	if (editorLayerInput) {
		editorLayerInput->updateInputNode();
	}
}

bool touchIntersectsInput(gd::CCTextInputNode* input, CCTouch* touch) {
	if (!input) return false;

	auto inputSize = CCSize({ input->getScaleX() * input->getContentSize().width, input->getScaleY() * input->getContentSize().height });

	auto rect = CCRect{
		input->getPositionX() - inputSize.width / 2,
		input->getPositionY() - inputSize.height / 2,
		inputSize.width,
		inputSize.height
	};

	if (!rect.containsPoint(input->getParent()->convertTouchToNodeSpace(touch))) {
		input->m_textField->detachWithIME();
		return false;
	}
	else
		return true;
}

bool m_isHoldingInEditor;

bool __fastcall EditorUI::ccTouchBeganH(gd::EditorUI* _self, void*, CCTouch* touch, CCEvent* event) {
	auto self = reinterpret_cast<gd::EditorUI*>(reinterpret_cast<uintptr_t>(_self) - 0xe8);
	m_isHoldingInEditor = true;

	auto editorLayerInput = static_cast<EditorLayerInput*>(self->getChildByTag(2703));
	if (editorLayerInput) {
		if (touchIntersectsInput(editorLayerInput->m_input, touch)) return false;
	}

	return EditorUI::ccTouchBegan(_self, touch, event);
}

void __fastcall EditorUI::ccTouchEndedH(gd::EditorUI* self, void*, CCTouch* touch, CCEvent* event) {
	m_isHoldingInEditor = false;
	return EditorUI::ccTouchEnded(self, touch, event);
}

void __fastcall EditorUI::onPlaytestH(gd::EditorUI* self, void*, CCObject* obj) {
	if (!m_isHoldingInEditor) EditorUI::onPlaytest(self, obj);
}

void __fastcall EditorUI::scrollWheelH(gd::EditorUI* _self, void*, float dy, float dx) { // From BEv6
	auto self = reinterpret_cast<gd::EditorUI*>(reinterpret_cast<uintptr_t>(_self) - 0xf8);

	auto kb = CCDirector::sharedDirector()->m_pKeyboardDispatcher;
	auto objectLayer = self->m_editorLayer->m_gameLayer;
	auto prevScale = objectLayer->getScale();
	auto swipeStart = objectLayer->convertToNodeSpace(self->m_swipeStart) * prevScale;

	if (kb->getControlKeyPressed()) {
		auto zoom = objectLayer->getScale();

		zoom = static_cast<float>(std::pow(std::numbers::e, std::log(std::max(zoom, .001f)) - dy * .01f));

		zoom = std::clamp(zoom, .1f, 1000000.f);

		auto mousePos = getMousePos();
		auto prevPos = objectLayer->convertToNodeSpace(mousePos);
		self->updateZoom(zoom);
		auto newPos = objectLayer->convertToWorldSpace(prevPos);
		objectLayer->setPosition(objectLayer->getPosition() + mousePos - newPos);
	}
	else if (kb->getShiftKeyPressed()) {
		self->m_editorLayer->m_gameLayer->setPositionX(self->m_editorLayer->m_gameLayer->getPositionX() - dy * 1.f);
	}
	else {
		EditorUI::scrollWheel(_self, dy, dx);
	}

	auto newSwipeStart = objectLayer->convertToNodeSpace(self->m_swipeStart) * prevScale;
	auto rel = swipeStart - newSwipeStart;
	rel = rel * (objectLayer->getScale()) / prevScale;
	self->m_swipeStart = self->m_swipeStart + rel;
}

void __fastcall EditorUI::onCreateButtonH(gd::EditorUI* self, void*, CCObject* sender) {
	EditorUI::onCreateButton(self, sender);

	auto selectedObjectInToolboxIdLabel = static_cast<CCLabelBMFont*>(self->getChildByTag(756));
	if (selectedObjectInToolboxIdLabel) {
		selectedObjectInToolboxIdLabel->setString(CCString::createWithFormat("ID: %i", self->m_selectedCreateObjectID)->getCString());
	}
}

void EditorUI::Callback::onColorFilter(CCObject* sender) {
	ColorFilterPopup::create(this)->show();
}

void __fastcall EditorUI::setupDeleteMenuH(gd::EditorUI* self) {
	EditorUI::setupDeleteMenu(self);

	const int colors[] = {
		0, 1, 2, 5, 3, 4, 6, 7, 8, 9
	};

	auto onColorFilterSpr = gd::ButtonSprite::create("", 24, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 40.f);
	onColorFilterSpr->setScale(.9f);
	onColorFilterSpr->m_label->setString(EditorUI::colorToShortString(colors[setting().colorFilter]).c_str());
	onColorFilterSpr->m_label->limitLabelWidth(32.f, .75f, .1f);
	auto onColorFilter = gd::CCMenuItemSpriteExtra::create(onColorFilterSpr, self, menu_selector(EditorUI::Callback::onColorFilter));
	onColorFilter->setPosition({ 33.f, -18.f });

	self->m_deleteMenu->addChild(onColorFilter, 0, 23);
}

void __fastcall EditorUI::keyDownH(gd::EditorUI* _self, void*, enumKeyCodes key) {
	auto self = reinterpret_cast<gd::EditorUI*>(reinterpret_cast<uintptr_t>(_self) - 0xf4);
	auto kb = CCDirector::sharedDirector()->getKeyboardDispatcher();
	bool ctrlAltPressed = kb->getControlKeyPressed() && kb->getAltKeyPressed();
	bool shiftPressed = kb->getShiftKeyPressed();

	if ((key == KEY_Up) || (key == setting().m_p1click)) {
		if (self->m_editorLayer->m_playerState == 1) {
			self->m_editorLayer->pushButton(1, true);
		}
	}
	else if (key == setting().m_p2click) {
		if (self->m_editorLayer->m_playerState == 1) {
			self->m_editorLayer->pushButton(1, false);
		}
	}
	else if (key == KEY_W && ctrlAltPressed) {
		self->moveObjectCall(static_cast<gd::EditCommand>(103));
	}	
	else if (key == KEY_A && ctrlAltPressed) {
		self->moveObjectCall(static_cast<gd::EditCommand>(101));
	}
	else if (key == KEY_S && ctrlAltPressed) {
		self->moveObjectCall(static_cast<gd::EditCommand>(104));
	}
	else if (key == KEY_D && ctrlAltPressed) {
		self->moveObjectCall(static_cast<gd::EditCommand>(102));
	}
	else if (key == KEY_Q && shiftPressed) {
		self->transformObjectCall(static_cast<gd::EditCommand>(122));
	}
	else if (key == KEY_E && shiftPressed) {
		self->transformObjectCall(static_cast<gd::EditCommand>(121));
	}
	else if (key == KEY_F3) {
		setting().onPreviewMode = !setting().onPreviewMode;
		if (setting().onPreviewMode) {
			LevelEditorLayer::updatePreviewMode();
		}
		else {
			LevelEditorLayer::resetColors();
		}
	}
	else {
		EditorUI::keyDown(_self, key);
	}
}

void __fastcall EditorUI::keyUpH(gd::EditorUI* _self, void*, enumKeyCodes key) {
	auto self = reinterpret_cast<gd::EditorUI*>(reinterpret_cast<uintptr_t>(_self) - 0xf4);

	if ((key == KEY_Up) || (key == setting().m_p1click)) {
		if (self->m_editorLayer->m_playerState == 1) {
			self->m_editorLayer->releaseButton(1, true);
		}
	}
	else if (key == setting().m_p2click) {
		if (self->m_editorLayer->m_playerState == 1) {
			self->m_editorLayer->releaseButton(1, false);
		}
	}
	else {
		EditorUI::keyUp(_self, key);
	}
}

void __fastcall EditorUI::onDuplicateH(gd::EditorUI* self, void*, CCObject* sender) {
	// Wacky bug fix
	int currentEditorLayer = self->m_editorLayer->m_groupIDFilter;

	self->m_editorLayer->m_groupIDFilter = -1;

	if (setting().onPreviewRotations) RotateSaws::stopRotations(self->m_editorLayer);

	EditorUI::onDuplicate(self, sender);

	if (setting().onPreviewRotations) RotateSaws::beginRotations(self->m_editorLayer);

	self->m_editorLayer->m_groupIDFilter = currentEditorLayer;
}

void __fastcall EditorUI::updateGridNodeSizeH(gd::EditorUI* self) {
	auto size = setting().m_customEditorGridSize;
	if (size < 1 || roundf(size) == 30) {
		return EditorUI::updateGridNodeSize(self);
	}

	int actualMode = self->m_selectedTab;
	self->m_selectedTab = 2;
	EditorUI::updateGridNodeSize(self);
	self->m_selectedTab = actualMode;
}

static std::unordered_set<int> colorTriggerIds = { 29, 30, 104, 105, 221, 717, 718, 743, 744 };

bool EditorUI::isColorTriggersSelected(gd::EditorUI* editorUI) {
	for (int i = 0; i < editorUI->m_selectedObjects->count(); i++) {
		auto object = reinterpret_cast<gd::GameObject*>(editorUI->m_selectedObjects->objectAtIndex(i));
		if (colorTriggerIds.contains(object->m_objectID)) {
			return true;
		}
	}

	return false;
}

bool __fastcall EditorUI::editButtonUsableH(gd::EditorUI* self) {
	return EditorUI::editButtonUsable(self);
}

void __fastcall EditorUI::editObjectH(gd::EditorUI* self, void*, CCObject* sender) {
	if (setting().onNewColorSelectMenu) {
		if (self->editButtonUsable()) {
			if ((self->m_selectedObject == nullptr) || (self->m_selectedObject->m_objectType != gd::GameObjectType::SecretCoin)) {
				if ((self->m_selectedObject == nullptr) || (self->m_selectedObject->m_objectID != 31)) {
					if ((self->m_selectedObject == nullptr) || self->m_selectedObject->canChangeCustomColor()) {
						if ((self->m_selectedObjects->count() != 0) || self->m_selectedObject != nullptr && self->m_selectedObject->canChangeCustomColor()) {
							NewCustomizeObjectLayer::create(self->m_selectedObject, self->m_selectedObjects)->show();
							return;
						}
					}
				}
			}
		}
	}
	
	EditorUI::editObject(self, sender);
}

//void __fastcall EditorUI::setupCreateMenuH(gd::EditorUI* self) {
//	EditorUI::setupCreateMenu(self);
//
//	auto director = CCDirector::sharedDirector();
//	auto winSize = director->getWinSize();
//
//	auto objects = CCArray::create();
//	objects->addObject(self->getCreateBtn(1, 4));
//	objects->addObject(self->getCreateBtn(29, 4));
//	objects->addObject(self->getCreateBtn(34, 4));
//	objects->addObject(self->getCreateBtn(33, 4));
//
//	auto testObjects = gd::EditButtonBar::create(objects, ccp(winSize.width / 2.f - 5.f, director->getScreenBottom() + 86.f), 12, true);
//	self->addChild(testObjects, 10);
//	self->m_createButtonBars->addObject(testObjects);
//
//	auto tabOff = CCSprite::createWithSpriteFrameName("GJ_tabOff_001.png");
//	tabOff->setScale(1.1f);
//	tabOff->setOpacity(150);
//	auto tabOn = CCSprite::createWithSpriteFrameName("GJ_tabOn_001.png");
//	tabOn->setScale(1.1f);
//	tabOn->setOpacity(150);
//
//	auto testObjectsTab = gd::CCMenuItemToggler::create(tabOff, tabOn, self, menu_selector(gd::EditorUI::onSelectBuildTab));
//	testObjectsTab->setSizeMult(1.2f);
//
//	self->m_tabsMenu->addChild(testObjectsTab, 0, 12);
//	//self->m_tabsMenu->alignItemsHorizontallyWithPadding(0.f);
//	self->m_tabsArray->addObject(testObjectsTab);
//
//	self->selectBuildTab(self->m_editorLayer->m_level->m_lastBuildTab);
//	//self->m_selectedBuildTab->goToPage(self->m_editorLayer->m_level->m_lastBuildPage);
//	self->updateCreateMenu(false);
//}

void EditorUI::Callback::onCircleTool(CCObject*) {
	if (this->getSelectedObjects()->count()) {
		CircleToolPopup::create()->show();
	}
	else {
		gd::FLAlertLayer::create("Circle Tool", "You must first select the objects.", "OK")->show();
	}
}

void __fastcall EditorUI::destructorH(gd::EditorUI* self) {
	saveClipboard(self);
	EditorUI::destructor(self);
	m_gridSizeLabel = nullptr;
	m_editorUI = nullptr;
}

void EditorUI::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3fdc0), EditorUI::initH, reinterpret_cast<void**>(&EditorUI::init));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x47f10), EditorUI::selectObjectH, reinterpret_cast<void**>(&EditorUI::selectObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x47fa0), EditorUI::selectObjectsH, reinterpret_cast<void**>(&EditorUI::selectObjects));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4b410), EditorUI::moveObjectH, reinterpret_cast<void**>(&EditorUI::moveObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4cfb0), EditorUI::angleChangedH, reinterpret_cast<void**>(&EditorUI::angleChanged));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41450), EditorUI::updateButtonsH, reinterpret_cast<void**>(&EditorUI::updateButtons));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41640), EditorUI::clickOnPositionH, reinterpret_cast<void**>(&EditorUI::clickOnPosition));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x499e0), EditorUI::toggleSpecialEditButtonsH, reinterpret_cast<void**>(&EditorUI::toggleSpecialEditButtons));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4efe0), EditorUI::offsetForKeyH, reinterpret_cast<void**>(&EditorUI::offsetForKey));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4b040), EditorUI::moveForCommandH, reinterpret_cast<void**>(&EditorUI::moveForCommand));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4b7e0), EditorUI::transformObjectH, reinterpret_cast<void**>(&EditorUI::transformObject));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x490c0), EditorUI::onCopyH, reinterpret_cast<void**>(&EditorUI::onCopy));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4afc0), EditorUI::onGroupDownH, reinterpret_cast<void**>(&EditorUI::onGroupDown));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4af50), EditorUI::onGroupUpH, reinterpret_cast<void**>(&EditorUI::onGroupUp));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4d5e0), EditorUI::ccTouchBeganH, reinterpret_cast<void**>(&EditorUI::ccTouchBegan));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4de40), EditorUI::ccTouchEndedH, reinterpret_cast<void**>(&EditorUI::ccTouchEnded));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x489c0), EditorUI::onPlaytestH, reinterpret_cast<void**>(&EditorUI::onPlaytest));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4ee90), EditorUI::scrollWheelH, reinterpret_cast<void**>(&EditorUI::scrollWheel));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x47400), EditorUI::onCreateButtonH, reinterpret_cast<void**>(&EditorUI::onCreateButton));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x42080), EditorUI::setupDeleteMenuH, reinterpret_cast<void**>(&EditorUI::setupDeleteMenu));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x48e70), EditorUI::onDuplicateH, reinterpret_cast<void**>(&EditorUI::onDuplicate));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41ae0), EditorUI::updateGridNodeSizeH, reinterpret_cast<void**>(&EditorUI::updateGridNodeSize));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x49680), EditorUI::editButtonUsableH, reinterpret_cast<void**>(&EditorUI::editButtonUsable));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4ae20), EditorUI::editObjectH, reinterpret_cast<void**>(&EditorUI::editObject));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x43590), EditorUI::setupCreateMenuH, reinterpret_cast<void**>(&EditorUI::setupCreateMenu));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4e550), EditorUI::keyDownH, reinterpret_cast<void**>(&EditorUI::keyDown));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x4ee40), EditorUI::keyUpH, reinterpret_cast<void**>(&EditorUI::keyUp));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3fb90), EditorUI::destructorH, reinterpret_cast<void**>(&EditorUI::destructor));
}
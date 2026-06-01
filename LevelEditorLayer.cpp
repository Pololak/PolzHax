#define NOMINMAX
#include "LevelEditorLayer.hpp"
#include "EditorUI.hpp"
#include "EditorObjectLayering.hpp"
#include "Setting.hpp"
#include "hsv.hpp"
#include "utils.hpp"
#include "Hitboxes.hpp"
#include "RotateSaws.hpp"

#include <unordered_set>
#include <unordered_map>

gd::LevelEditorLayer* m_editorLayer;
bool isEditorPaused = false;
bool wasPreviewModeEnabled = false;
gd::StartPosObject* m_playtestStartPos;
gd::GJGroundLayer* m_groundLayer;
GDColor m_color01;
GDColor m_color02;
GDColor m_color03;
GDColor m_color04;
GDColor m_color3DL;

gd::LevelEditorLayer* LevelEditorLayer::get() {
	return m_editorLayer;
}

bool LevelEditorLayer::getIsEditorPaused() {
	return isEditorPaused;
}

void LevelEditorLayer::setIsEditorPaused(bool val) {
	isEditorPaused = val;
}

gd::StartPosObject* LevelEditorLayer::getPlaytestStartPos() {
	return m_playtestStartPos;
}

void LevelEditorLayer::setPlaytestStartPos(gd::StartPosObject* val) {
	m_playtestStartPos = val;
}

GDColor LevelEditorLayer::getColor01() {
	return m_color01;
}

GDColor LevelEditorLayer::getColor02() {
	return m_color02;
}

GDColor LevelEditorLayer::getColor03() {
	return m_color03;
}

GDColor LevelEditorLayer::getColor04() {
	return m_color04;
}

GDColor LevelEditorLayer::getColor3DL() {
	return m_color3DL;
}

void LevelEditorLayer::updateShowHitboxes() {
	auto self = LevelEditorLayer::get();

	if (self == nullptr) return;

	auto playerDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(124));
	auto objectDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(125));
	if (!setting().m_clearHitboxes) playerDrawNode->clear();
	objectDrawNode->clear();

	if (setting().onHitboxes) {
		if (setting().onPlayerHitboxes) {
			if (self->m_player) Hitboxes::drawPlayerHitbox(self->m_player, playerDrawNode);
			if (self->m_player2) Hitboxes::drawPlayerHitbox(self->m_player2, playerDrawNode);
		}

		for (int i = self->m_firstVisibleSection + 1; i <= self->m_lastVisibleSection - 1; i++) {
			if (i < 0) continue;
			if (i >= self->m_levelSections->count()) break;

			auto objectAtIndex = self->m_levelSections->objectAtIndex(i);
			auto objArr = reinterpret_cast<CCArray*>(objectAtIndex);

			for (int j = 0; j < objArr->count(); j++) {
				auto obj = reinterpret_cast<gd::GameObject*>(objArr->objectAtIndex(j));
				if (setting().onSolidHitboxes)
					Hitboxes::drawSolidsObjectHitbox(obj, objectDrawNode);
				if (setting().onHazardHitboxes)
					Hitboxes::drawHazardsObjectHitbox(obj, objectDrawNode);
				if (setting().onSpecialHitboxes)
					Hitboxes::drawSpecialsObjectHitbox(obj, objectDrawNode);
			}
		}
	}
}

struct CompareTriggers {
	bool operator()(gd::GameObject* a, gd::GameObject* b) const {
		return a->getPositionX() < b->getPositionX();
	}

	bool operator()(gd::GameObject* a, float b) const {
		return a->getPositionX() < b;
	}

	bool operator()(float a, gd::GameObject* b) const {
		return a < b->getPositionX();
	}
};

template <class T>
T lerp(float amt, T a, T b) {
	return static_cast<T>(static_cast<float>(a) * (1.f - amt) + static_cast<float>(b) * amt);
}

GDColor mix_color(float value, GDColor a, GDColor b) {
	return GDColor{
		lerp<uint8_t>(value, a.r, b.r),
		lerp<uint8_t>(value, a.g, b.g),
		lerp<uint8_t>(value, a.b, b.b),
		b.blending
	};
}

enum class ColorTriggers {
	BG = 29,
	G = 30,
	Line = 104,
	Obj = 105,
	DLine = 744,
	Col1 = 221,
	Col2 = 717,
	Col3 = 718,
	Col4 = 743,
};

static std::unordered_set<int> colorTriggerIds = { 29, 30, 104, 105, 221, 717, 718, 743, 744 };

bool isColorTrigger(gd::GameObject* object) {
	return colorTriggerIds.find(object->m_objectID) != colorTriggerIds.end();
}

std::unordered_map<ColorTriggers, std::vector<gd::GameObject*>> m_colorTriggers;
CCSpriteBatchNode* m_blendingBatchNode;
std::unordered_map<gd::GJCustomColorMode, bool> m_currentColor;
float m_lastPos;

void LevelEditorLayer::setLastPos(float val) {
	m_lastPos = val;
}

void LevelEditorLayer::insertTrigger(gd::GameObject* object) {
	auto& triggers = (m_colorTriggers)[ColorTriggers(object->m_objectID)];
	for (size_t i = 0; i < triggers.size(); ++i) {
		if (CompareTriggers()(object, triggers[i])) {
			triggers.insert(triggers.begin() + i, object);
			return;
		}
	}
	triggers.push_back(object);
}

void LevelEditorLayer::removeTrigger(gd::GameObject* object) {
	auto& triggers = (m_colorTriggers)[ColorTriggers(object->m_objectID)];
	for (size_t i = 0; i < triggers.size(); ++i) {
		if (triggers[i] == object) {
			triggers.erase(triggers.begin() + i);
			break;
		}
	}
}

void LevelEditorLayer::moveTrigger(gd::GameObject* object) {
	if (isColorTrigger(object)) {
		removeTrigger(object);
		insertTrigger(object);
	}
}

float timeBetweenPos(float a, float b) {
	if (m_editorLayer) {
		auto l = m_editorLayer->m_gridLayer;
		return std::abs(l->timeForXPos(a) - l->timeForXPos(b));
	}
	else {
		return 0.f;
	}
}

GDColor calculateColor(std::vector<gd::GameObject*> triggers, const float pos, const GDColor starting_color) {
	if (triggers.empty()) return starting_color;
	auto bound = std::lower_bound(triggers.begin(), triggers.end(), pos, CompareTriggers()) - triggers.begin();
	if (bound == 0) {
		return starting_color;
	}
	else {
		auto trigger = triggers[bound - 1];
		if (trigger->m_triggerDuration < 0) trigger->m_triggerDuration = 0;
		GDColor color_to = trigger;
		auto dist = timeBetweenPos(trigger->getPosition().x, pos) / (trigger->m_triggerDuration);
		auto color_from = starting_color;
		if (bound > 1) {
			auto trigger = triggers[bound - 2];
			auto dist = timeBetweenPos(trigger->getPosition().x, pos) / trigger->m_triggerDuration;
			color_from = trigger;
			if (dist < 1.f)
				color_from = mix_color(dist, bound > 2 ? triggers[bound - 3] : starting_color, color_from);
		}
		if (trigger->m_touchTriggered) return color_from;
		if (trigger->m_copyPlayerColor1) return mix_color(std::min(dist, 1.f), color_from, gd::GameManager::sharedState()->colorForIdx(gd::GameManager::sharedState()->m_playerColor));
		else if (trigger->m_copyPlayerColor2) return mix_color(std::min(dist, 1.f), color_from, gd::GameManager::sharedState()->colorForIdx(gd::GameManager::sharedState()->m_playerColor2));
		else return mix_color(std::min(dist, 1.f), color_from, color_to);
	}
}

// accurate lbg calculation, thanks to sSolsta and zmx
GDColor calculateLbg(const GDColor& bg_color, const GDColor& p1_color) {
	auto hsv = color_utils::rgb_to_hsv({
		bg_color.r / 255., bg_color.g / 255., bg_color.b / 255. });
	hsv.s = std::max(hsv.s - 0.2, 0.0);
	hsv.v = std::min(hsv.v + 0.2, 1.0);

	const auto rgb = color_utils::hsv_to_rgb(hsv);

	const auto lbg_color = GDColor{
		static_cast<uint8_t>(rgb.r * 255.),
		static_cast<uint8_t>(rgb.g * 255.),
		static_cast<uint8_t>(rgb.b * 255.),
		true
	};

	const auto amt = (static_cast<float>(bg_color.r) + static_cast<float>(bg_color.g) + static_cast<float>(bg_color.b)) / 150.f;

	if (amt < 1.f) {
		return mix_color(amt, p1_color, lbg_color);
	}
	else {
		return lbg_color;
	}
}

void updateObjectColor(gd::GameObject* object, const GDColor& color) {
	if (m_editorLayer) {
		if (object->m_hasColor) object->setChildColor(color);
		else object->setObjectColor(color);

		CCSprite* node = object;
		if (object->m_hasColor)
			node = object->m_colorSprite;

		auto batch = m_blendingBatchNode;

		if (color.blending) {
			if (node->getParent() != batch) {
				node->removeFromParent();
				batch->addChild(node);
			}
		}
		else {
			if (node->getParent() == batch) {
				node->removeFromParent();
				m_editorLayer->m_objectBatchNode->addChild(node);
			}
		}

		if (setting().onPreviewRotations && RotateSaws::objectIsSaw(object)) {
			object->stopActionByTag(9957);
			if (object->m_myAction) object->runAction(object->m_myAction);
		}
	}
}

bool LevelEditorLayer::isColorBlending(gd::GJCustomColorMode mode) {
	return (m_currentColor)[mode];
}

float getPreviewPos() {
	if (m_editorLayer) {
		if (m_editorLayer->m_playerState != 0) {
			return m_editorLayer->m_player->getPositionX();
		}
		else if (m_editorLayer->m_uiLayer->m_playtestMusic) {
			return m_editorLayer->m_gridLayer->m_lastMusicXPosition;
		}
		else {
			return m_editorLayer->m_gameLayer->convertToNodeSpace(CCDirector::sharedDirector()->getWinSize() / 2.f).x;
		}
	}
	else return 0.f;
}

void LevelEditorLayer::updatePreviewMode() {
	if (m_editorLayer) {
		GDColor bgColor, gColor, lColor, objColor, dlColor, color1, color2, color3, color4;
		GDColor white(ccc3(255, 255, 255));

		auto gm = gd::GameManager::sharedState();

		GDColor p1Color(gm->colorForIdx(gm->m_playerColor), true);
		GDColor p2Color(gm->colorForIdx(gm->m_playerColor2), true);

		const auto pos = getPreviewPos();

		auto settings = m_editorLayer->m_levelSettings;
		for (auto& [type, triggers] : m_colorTriggers) {
			GDColor* color = nullptr;
			GDColor startingColor;

			switch (type) {
			case ColorTriggers::BG:
				if (settings->m_backgroundColor->m_playerColor == 1) {
					startingColor = p1Color;
				}
				else if (settings->m_backgroundColor->m_playerColor == 2) {
					startingColor = p2Color;
				}
				else {
					startingColor = settings->m_backgroundColor;
				}
				color = &bgColor;
				break;
			case ColorTriggers::G:
				if (settings->m_groundColor->m_playerColor == 1) {
					startingColor = p1Color;
				}
				else if (settings->m_groundColor->m_playerColor == 2) {
					startingColor = p2Color;
				}
				else {
					startingColor = settings->m_groundColor;
				}
				color = &gColor;
				break;
			case ColorTriggers::Line:
				if (settings->m_lineColor->m_playerColor == 1) {
					startingColor = p1Color;
				}
				else if (settings->m_lineColor->m_playerColor == 2) {
					startingColor = p2Color;
				}
				else {
					startingColor = settings->m_lineColor;
				}
				color = &lColor;
				break;
			case ColorTriggers::Obj:
				if (settings->m_objectColor->m_playerColor == 1) {
					startingColor = p1Color;
				}
				else if (settings->m_objectColor->m_playerColor == 2) {
					startingColor = p2Color;
				}
				else {
					startingColor = settings->m_objectColor;
				}
				color = &objColor;
				break;
			case ColorTriggers::DLine:
				if (settings->m_3DLineColor->m_playerColor == 1) {
					startingColor = p1Color;
				}
				else if (settings->m_3DLineColor->m_playerColor == 2) {
					startingColor = p2Color;
				}
				else {
					startingColor = settings->m_3DLineColor;
				}
				color = &dlColor;
				break;
			case ColorTriggers::Col1:
				if (settings->m_customColor01->m_playerColor == 1) {
					startingColor = p1Color;
				}
				else if (settings->m_customColor01->m_playerColor == 2) {
					startingColor = p2Color;
				}
				else {
					startingColor = settings->m_customColor01;
				}
				color = &color1;
				break;
			case ColorTriggers::Col2:
				if (settings->m_customColor02->m_playerColor == 1) {
					startingColor = p1Color;
				}
				else if (settings->m_customColor02->m_playerColor == 2) {
					startingColor = p2Color;
				}
				else {
					startingColor = settings->m_customColor02;
				}
				color = &color2;
				break;
			case ColorTriggers::Col3:
				if (settings->m_customColor03->m_playerColor == 1) {
					startingColor = p1Color;
				}
				else if (settings->m_customColor03->m_playerColor == 2) {
					startingColor = p2Color;
				}
				else {
					startingColor = settings->m_customColor03;
				}
				color = &color3;
				break;
			case ColorTriggers::Col4:
				if (settings->m_customColor04->m_playerColor == 1) {
					startingColor = p1Color;
				}
				else if (settings->m_customColor04->m_playerColor == 2) {
					startingColor = p2Color;
				}
				else {
					startingColor = settings->m_customColor04;
				}
				color = &color4;
				break;
			default: continue;
			}
			*color = calculateColor(triggers, pos, startingColor);
		}

		(m_currentColor)[gd::GJCustomColorMode::Col1] = color1.blending;
		(m_currentColor)[gd::GJCustomColorMode::Col2] = color2.blending;
		(m_currentColor)[gd::GJCustomColorMode::Col3] = color3.blending;
		(m_currentColor)[gd::GJCustomColorMode::Col4] = color4.blending;
		(m_currentColor)[gd::GJCustomColorMode::Col3DL] = dlColor.blending;

		m_editorLayer->m_backgroundSprite->setColor(bgColor);

		if (m_groundLayer) {
			m_groundLayer->m_groundSprite->setColor(gColor);
			m_groundLayer->m_line->setColor(lColor);
		}

		m_color01 = color1;
		m_color02 = color2;
		m_color03 = color3;
		m_color04 = color4;
		m_color3DL = dlColor;

		auto lbgColor = calculateLbg(bgColor, p1Color);

		for (auto section : CCArrayExt<CCArray*>(m_editorLayer->m_levelSections)) {
			if (section) {
				for (auto object : CCArrayExt<gd::GameObject*>(section)) {
					if (!object || !object->getParent() || object->m_isSelected) continue;

					if (object->m_isTintObject)
						object->setObjectColor(objColor);

					auto mode = object->getColorMode();
					switch (mode)
					{
					case gd::GJCustomColorMode::Col3DL:
						updateObjectColor(object, dlColor);
						break;
					case gd::GJCustomColorMode::Col1:
						updateObjectColor(object, color1);
						break;
					case gd::GJCustomColorMode::Col2:
						updateObjectColor(object, color2);
						break;
					case gd::GJCustomColorMode::Col3:
						updateObjectColor(object, color3);
						break;
					case gd::GJCustomColorMode::Col4:
						updateObjectColor(object, color4);
						break;
					case gd::GJCustomColorMode::LightBG:
						updateObjectColor(object, lbgColor);
						break;
					case gd::GJCustomColorMode::PCol1:
						updateObjectColor(object, p1Color);
						break;
					case gd::GJCustomColorMode::PCol2:
						updateObjectColor(object, p2Color);
						break;
					case static_cast<gd::GJCustomColorMode>(9):
						updateObjectColor(object, white);
						break;
					default:;
					}
				}
			}
		}
	}
}

void LevelEditorLayer::resetColors() {
	if (m_editorLayer) {
		m_editorLayer->m_backgroundSprite->setColor(ccGRAY);

		if (m_groundLayer) {
			m_groundLayer->m_groundSprite->setColor(ccGRAY);
			m_groundLayer->m_line->setColor(ccWHITE);
		}

		for (auto section : CCArrayExt<CCArray*>(m_editorLayer->m_levelSections)) {
			if (section) {
				for (auto object : CCArrayExt<gd::GameObject*>(section)) {
					if (!object || object->m_isSelected) continue;

					if (object->m_isTintObject)
						object->setObjectColor(ccWHITE);

					auto mode = object->m_customColorMode;
					const std::unordered_map<gd::GJCustomColorMode, GDColor> defaultColors = {
						{gd::GJCustomColorMode::Default, GDColor(255, 255, 255, false)},
						{gd::GJCustomColorMode::Col3DL, GDColor(255, 255, 0, false)},
						{gd::GJCustomColorMode::Col1, GDColor(255, 150, 255, false)},
						{gd::GJCustomColorMode::Col2, GDColor(255, 255, 150, false)},
						{gd::GJCustomColorMode::Col3, GDColor(150, 255, 255, false)},
						{gd::GJCustomColorMode::Col4, GDColor(150, 255, 150, false)},
						{gd::GJCustomColorMode::LightBG, GDColor(75, 175, 255, false)},
						{gd::GJCustomColorMode::PCol1, GDColor(175, 150, 255, false)},
						{gd::GJCustomColorMode::PCol2, GDColor(255, 150, 150, false)},
						{static_cast<gd::GJCustomColorMode>(9), GDColor(255, 255, 255, false)}
					};
					updateObjectColor(object, defaultColors.at(mode));

					if (object->m_invisibleMode)
						object->setObjectColor(ccORANGE);

					if (object->getColorMode() == gd::GJCustomColorMode::Col3DL)
						object->setObjectColor(ccYELLOW);
				}
			}
		}
	}
}

void LevelEditorLayer::updateOrientedHitboxes(gd::LevelEditorLayer* self) {
	if (setting().onHitboxBugFix) {
		for (auto section : CCArrayExt<CCArray*>(self->m_levelSections)) {
			if (section) {
				for (auto object : CCArrayExt<gd::GameObject*>(section)) {
					if (object && object->canRotateFree()) {
						switch (object->m_objectType) {
						case gd::GameObjectType::Hazard:
						case gd::GameObjectType::InverseGravityPortal:
						case gd::GameObjectType::NormalGravityPortal:
						case gd::GameObjectType::ShipPortal:
						case gd::GameObjectType::CubePortal:
						case gd::GameObjectType::YellowJumpPad:
						case gd::GameObjectType::PinkJumpPad:
						case gd::GameObjectType::GravityPad:
						case gd::GameObjectType::YellowJumpRing:
						case gd::GameObjectType::PinkJumpRing:
						case gd::GameObjectType::GravityRing:
						case gd::GameObjectType::BallPortal:
						case gd::GameObjectType::RegularSizePortal:
						case gd::GameObjectType::MiniSizePortal:
						case gd::GameObjectType::UfoPortal:
						case gd::GameObjectType::Modifier:
						case gd::GameObjectType::DualPortal:
						case gd::GameObjectType::SoloPortal:
						case gd::GameObjectType::WavePortal:
							object->calculateOrientedBox();
						}
					}
				}
			}
		}
	}
}

void LevelEditorLayer::updateGroundWidth() {
	if (m_editorLayer) {
		if (m_groundLayer) {
			auto director = CCDirector::sharedDirector();
			auto winSize = director->getWinSize();

			m_groundLayer->setPositionX(m_editorLayer->m_gameLayer->convertToNodeSpace({ winSize.width / 2.f, 0.f }).x);
			m_groundLayer->m_line->setPositionX(m_groundLayer->convertToNodeSpace({ winSize.width / 2.f, 0.f }).x);

			float groundWidth = (128.f / m_editorLayer->m_gameLayer->getScale()) * (winSize.width / 128.f) * 1.5f;
			float groundOffset = m_editorLayer->m_gameLayer->convertToNodeSpace(winSize / 2.f).x;
			m_groundLayer->m_groundSprite->setTextureRect({ groundOffset, 0.f, groundWidth, 128.f });
		}
	}
}

void LevelEditorLayer::removeGroundLayer() {
	if (m_editorLayer) {
		if (m_groundLayer != nullptr) {
			m_groundLayer->removeFromParentAndCleanup(true);
			m_groundLayer = nullptr;
		}
	}
}

void LevelEditorLayer::createGroundLayer() {
	if (m_editorLayer) {
		if (m_groundLayer != nullptr) {
			removeGroundLayer();
		}
		m_groundLayer = gd::GJGroundLayer::create(m_editorLayer->m_levelSettings->m_groundIndex);
		m_groundLayer->hideShadows();
		m_groundLayer->m_groundSprite->setAnchorPoint({ .5f, 1.f });
		m_editorLayer->m_gameLayer->addChild(m_groundLayer, 10);

		if (setting().onPreviewMode) {
			updatePreviewMode();
		}
		else {
			resetColors();
		}
		updateGroundWidth();
	}
}

bool __fastcall LevelEditorLayer::initH(gd::LevelEditorLayer* self, void*, gd::GJGameLevel* level) {
	m_editorLayer = self;
	if (!LevelEditorLayer::init(self, level)) return false;

	auto& triggers = m_colorTriggers;
	triggers[ColorTriggers::BG];
	triggers[ColorTriggers::G];
	triggers[ColorTriggers::Line];
	triggers[ColorTriggers::Obj];
	triggers[ColorTriggers::DLine];
	triggers[ColorTriggers::Col1];
	triggers[ColorTriggers::Col2];
	triggers[ColorTriggers::Col3];
	triggers[ColorTriggers::Col4];

	auto gameSheetTexture = CCTextureCache::sharedTextureCache()->addImage("GJ_GameSheet.png", false);
	m_blendingBatchNode = CCSpriteBatchNode::createWithTexture(gameSheetTexture);
	m_blendingBatchNode->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
	self->m_gameLayer->addChild(m_blendingBatchNode, 0);

	auto playerDrawNode = CCDrawNode::create();
	self->m_gameLayer->addChild(playerDrawNode, 1000, 124);
	auto objectDrawNode = CCDrawNode::create();
	self->m_gameLayer->addChild(objectDrawNode, 1000, 125);

	if (setting().onSolidPlayerGlow) {
		self->m_player->m_playerFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
		self->m_player->m_vehicleFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
		self->m_player2->m_playerFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
		self->m_player2->m_vehicleFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
	}

	auto clicksDrawNode = CCDrawNode::create();
	self->m_gameLayer->addChild(clicksDrawNode, 1000, 126);

	if (setting().onShowGround) {
		createGroundLayer();
	}

	return true;
}

void __fastcall LevelEditorLayer::addSpecialH(gd::LevelEditorLayer* self, void*, gd::GameObject* object) {
	LevelEditorLayer::addSpecial(self, object);
	if (setting().onPreviewRotations && RotateSaws::objectIsSaw(object)) RotateSaws::beginRotateSaw(object);
	if (isColorTrigger(object)) insertTrigger(object);
}

void __fastcall LevelEditorLayer::removeSpecialH(gd::LevelEditorLayer* self, void*, gd::GameObject* object) {
	if (m_playtestStartPos == object) {
		m_playtestStartPos = nullptr;
	}

	LevelEditorLayer::removeSpecial(self, object);

	if (isColorTrigger(object)) removeTrigger(object);
}

void __fastcall LevelEditorLayer::removeObjectH(gd::LevelEditorLayer* self, void*, gd::GameObject* object, bool p0) {
	LevelEditorLayer::removeObject(self, object, p0);
	if (setting().onPreviewRotations && RotateSaws::objectIsSaw(object)) RotateSaws::stopRotateSaw(object);
}

void __fastcall LevelEditorLayer::updateVisibilityH(gd::LevelEditorLayer* self, void*, float dt) {
	LevelEditorLayer::updateVisibility(self, dt);

	LevelEditorLayer::updateShowHitboxes();

	if (!isEditorPaused && setting().onPreviewMode/* && (self->m_playerState != 1)*/) {
		LevelEditorLayer::updatePreviewMode();
	}

	if (setting().onShowGround) {
		LevelEditorLayer::updateGroundWidth();
	}
}

void __fastcall LevelEditorLayer::updateH(gd::LevelEditorLayer* self, void*, float dt) {
	LevelEditorLayer::update(self, dt);

	LevelEditorLayer::updateShowHitboxes();

	// breaks preview rotations
	//if (!isEditorPaused && setting().onPreviewMode && (self->m_playerState == 1)) {
	//	LevelEditorLayer::updatePreviewMode();
	//}

	if (setting().onShowGround) {
		LevelEditorLayer::updateGroundWidth();
	}
}

gd::GameObject* __fastcall LevelEditorLayer::addObjectFromStringH(gd::LevelEditorLayer* self, void*, std::string object) {
	auto obj = LevelEditorLayer::addObjectFromString(self, object);

	if (obj && setting().onExperimentalLayering) updateObjectLayering(obj);

	return obj;
}

gd::GameObject* __fastcall LevelEditorLayer::createObjectH(gd::LevelEditorLayer* self, void*, int id, CCPoint position) {
	auto obj = LevelEditorLayer::createObject(self, id, position);

	if (obj && setting().onExperimentalLayering) updateObjectLayering(obj);

	return obj;
}

void __fastcall LevelEditorLayer::flipGravityH(gd::LevelEditorLayer* _self, void*, gd::PlayerObject* acted, bool isFlipped, bool showEffect) { // taken from Zmx https://github.com/qimiko/gdps-public/blob/238b71e9f3cd8fdf855556ce4cc7c498f22cf3c0/src/hooks/LevelEditorLayer.cpp#L21
	auto self = reinterpret_cast<gd::LevelEditorLayer*>(reinterpret_cast<uintptr_t>(_self) - 0x11c);

	if (acted->m_gravityFlipped == isFlipped) return;

	acted->flipGravity(isFlipped, showEffect);

	if (!self->m_dualMode) return;

	if (self->m_levelSettings->m_twoPlayerMode) return; // Zmx probably forgot something

	auto player = self->m_player;
	auto secondPlayer = self->m_player2;

	if (!(player->m_flyMode == secondPlayer->m_flyMode)) return;
	if (!(player->m_rollMode == secondPlayer->m_rollMode)) return;
	if (!(player->m_birdMode == secondPlayer->m_birdMode)) return;

	auto actedId = acted->m_ID;
	auto playerId = player->m_ID;

	auto otherPlayer = player;
	if (actedId == playerId) otherPlayer = secondPlayer;

	otherPlayer->flipGravity(!isFlipped, showEffect);
}

void runCustomPlaytest(gd::LevelEditorLayer* self, gd::StartPosObject* startPos) {
	self->setStartPosObject(startPos);

	self->m_player->setPosition(startPos->getPosition());
	self->m_player2->setPosition(startPos->getPosition());

	self->setupLevelStart(startPos->m_settings);

	if (startPos->m_settings->m_startMode == 0) {
		self->m_player->toggleFlyMode(false);
		self->m_player2->toggleFlyMode(false);
		self->m_player->toggleRollMode(false);
		self->m_player2->toggleRollMode(false);
		self->m_player->toggleBirdMode(false);
		self->m_player2->toggleBirdMode(false);
		self->m_player->toggleDartMode(false);
		self->m_player2->toggleDartMode(false);
	}

	self->m_player->resumeSchedulerAndActions();
	self->m_player2->resumeSchedulerAndActions();

	self->m_playerState = 1;

	self->scheduleUpdate();

	self->playMusic();
}

void __fastcall LevelEditorLayer::onPlaytestH(gd::LevelEditorLayer* self) {
	gd::StartPosObject* selectedPlaytestStartPos = self->m_startPosObject;
	LevelEditorLayer::onPlaytest(self);

	bool fromSelectedStartPos = false;

	if (selectedPlaytestStartPos) {
		runCustomPlaytest(self, selectedPlaytestStartPos);
		fromSelectedStartPos = true;
	}
	
	if (m_playtestStartPos) {
		if (!fromSelectedStartPos) {
			runCustomPlaytest(self, m_playtestStartPos);
		}
	}

	auto clicksDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(126));
	if (clicksDrawNode) {
		clicksDrawNode->clear();
	}

	LevelEditorLayer::updateOrientedHitboxes(self);

	if (setting().onPreviewRotations) RotateSaws::beginRotations(self);
}

void __fastcall LevelEditorLayer::onResumePlaytestH(gd::LevelEditorLayer* self) {
	LevelEditorLayer::onResumePlaytest(self);

	LevelEditorLayer::updateOrientedHitboxes(self);

	if (setting().onPreviewRotations) RotateSaws::resumeRotations(self);
}

void __fastcall LevelEditorLayer::onPausePlaytestH(gd::LevelEditorLayer* self) {
	LevelEditorLayer::onPausePlaytest(self);

	if (setting().onPreviewRotations) RotateSaws::pauseRotations(self);
}

void __fastcall LevelEditorLayer::onStopPlaytestH(gd::LevelEditorLayer* self) {
	LevelEditorLayer::onStopPlaytest(self);

	if (setting().onPreviewRotations) {
		RotateSaws::pauseRotations(self);
		RotateSaws::resumeRotations(self);
	}
}

void __fastcall LevelEditorLayer::pushButtonH(gd::LevelEditorLayer* self, void*, int p0, bool p1) {
	LevelEditorLayer::pushButton(self, p0, p1);

	auto clicksDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(126));
	if (clicksDrawNode && setting().onShowClicks) {
		clicksDrawNode->drawDot(self->m_player->getPosition(), 3.f, ccc4f(1.f, .5f, 0.f, 1.f));
	}
}

void __fastcall LevelEditorLayer::releaseButtonH(gd::LevelEditorLayer* self, void*, int p0, bool p1) {
	LevelEditorLayer::releaseButton(self, p0, p1);

	auto clicksDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(126));
	if (clicksDrawNode && setting().onShowClicks) {
		clicksDrawNode->drawDot(self->m_player->getPosition(), 3.f, ccc4f(0.f, 1.f, 1.f, 1.f));
	}
}

void __fastcall LevelEditorLayer::drawH(gd::LevelEditorLayer* self) {
	LevelEditorLayer::draw(self);

	glLineWidth(2);
	ccDrawColor4B(255, 0, 255, 255);
	ccDrawLine(ccp(100, 100), ccp(500, 100));

	//for (int i = self->m_firstVisibleSection + 1; i <= self->m_lastVisibleSection - 1; i++) {
	//	if (i < 0) continue;
	//	if (i >= self->m_levelSections->count()) break;

	//	auto objectAtIndex = self->m_levelSections->objectAtIndex(i);
	//	auto objArr = reinterpret_cast<CCArray*>(objectAtIndex);

	//	for (int j = 0; j < objArr->count(); j++) {
	//		auto obj = reinterpret_cast<gd::GameObject*>(objArr->objectAtIndex(j));
	//		if (obj) {
	//			ccDrawRect(ccp(obj->getTextureRect().getMinX(), obj->getTextureRect().getMaxY()), ccp(obj->getTextureRect().getMaxX(), obj->getTextureRect().getMinY()));
	//		}
	//	}
	//}
}

void __fastcall LevelEditorLayer::playMusicH(gd::LevelEditorLayer* self) {
	LevelEditorLayer::playMusic(self);

	std::cout << "playMusic()" << std::endl;
}

void __fastcall LevelEditorLayer::destructorH(gd::LevelEditorLayer* self) {
	LevelEditorLayer::destructor(self);
	m_colorTriggers.clear();
	m_currentColor.clear();
	m_lastPos = 0.f;
	m_blendingBatchNode = nullptr;
	m_playtestStartPos = nullptr;
	m_groundLayer = nullptr;
	m_editorLayer = nullptr;
}

void LevelEditorLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8c2c0), LevelEditorLayer::initH, reinterpret_cast<void**>(&LevelEditorLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x905b0), LevelEditorLayer::flipGravityH, reinterpret_cast<void**>(&LevelEditorLayer::flipGravity));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8d090), LevelEditorLayer::addObjectFromStringH, reinterpret_cast<void**>(&LevelEditorLayer::addObjectFromString));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8d150), LevelEditorLayer::createObjectH, reinterpret_cast<void**>(&LevelEditorLayer::createObject));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8ed10), LevelEditorLayer::addSpecialH, reinterpret_cast<void**>(&LevelEditorLayer::addSpecial));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8ee30), LevelEditorLayer::removeSpecialH, reinterpret_cast<void**>(&LevelEditorLayer::removeSpecial));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8e180), LevelEditorLayer::removeObjectH, reinterpret_cast<void**>(&LevelEditorLayer::removeObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8ef20), LevelEditorLayer::updateVisibilityH, reinterpret_cast<void**>(&LevelEditorLayer::updateVisibility));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x91620), LevelEditorLayer::updateH, reinterpret_cast<void**>(&LevelEditorLayer::update));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x909f0), LevelEditorLayer::onPlaytestH, reinterpret_cast<void**>(&LevelEditorLayer::onPlaytest));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x90fc0), LevelEditorLayer::onResumePlaytestH, reinterpret_cast<void**>(&LevelEditorLayer::onResumePlaytest));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x90f10), LevelEditorLayer::onPausePlaytestH, reinterpret_cast<void**>(&LevelEditorLayer::onPausePlaytest));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x91170), LevelEditorLayer::onStopPlaytestH, reinterpret_cast<void**>(&LevelEditorLayer::onStopPlaytest));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x90800), LevelEditorLayer::pushButtonH, reinterpret_cast<void**>(&LevelEditorLayer::pushButton));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x908f0), LevelEditorLayer::releaseButtonH, reinterpret_cast<void**>(&LevelEditorLayer::releaseButton));

	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x90d70), LevelEditorLayer::playMusicH, reinterpret_cast<void**>(&LevelEditorLayer::playMusic));

	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x926b0), LevelEditorLayer::drawH, reinterpret_cast<void**>(&LevelEditorLayer::draw));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8c080), LevelEditorLayer::destructorH, reinterpret_cast<void**>(&LevelEditorLayer::destructor));
}
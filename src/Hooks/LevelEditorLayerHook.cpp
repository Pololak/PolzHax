#include "LevelEditorLayerHook.h"
#include "EditorUIHook.h"
#include "../GameVariables.h"
#include "../hsv.h"
#include "../utils.h"
#include "../Setting.h"
#include "../EditorObjectLayering.h"
#include "../RotateSaws.h"

#include <unordered_set>
#include <unordered_map>

LevelEditorLayer* m_editorLayer;

LevelEditorLayer* LevelEditorLayerHook::get() {
    return m_editorLayer;
}

bool isEditorPaused = false;
bool wasPreviewModeEnabled = false;
StartPosObject* m_playtestStartPos;
GJGroundLayer* m_groundLayer;
GDColor m_color01;
GDColor m_color02;
GDColor m_color03;
GDColor m_color04;
GDColor m_color3DL;

bool LevelEditorLayerHook::getIsEditorPaused() {
	return isEditorPaused;
}

void LevelEditorLayerHook::setIsEditorPaused(bool val) {
	isEditorPaused = val;
}

StartPosObject* LevelEditorLayerHook::getPlaytestStartPos() {
	return m_playtestStartPos;
}

void LevelEditorLayerHook::setPlaytestStartPos(StartPosObject* val) {
	m_playtestStartPos = val;
}

GDColor LevelEditorLayerHook::getColor01() {
	return m_color01;
}

GDColor LevelEditorLayerHook::getColor02() {
	return m_color02;
}

GDColor LevelEditorLayerHook::getColor03() {
	return m_color03;
}

GDColor LevelEditorLayerHook::getColor04() {
	return m_color04;
}

GDColor LevelEditorLayerHook::getColor3DL() {
	return m_color3DL;
}

struct CompareTriggers {
	bool operator()(GameObject* a, GameObject* b) const {
		return a->getPositionX() < b->getPositionX();
	}

	bool operator()(GameObject* a, float b) const {
		return a->getPositionX() < b;
	}

	bool operator()(float a, GameObject* b) const {
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

bool isColorTrigger(GameObject* object) {
	return colorTriggerIds.find(object->m_objectID) != colorTriggerIds.end();
}

template <> struct std::hash<ColorTriggers> {
	size_t operator()(ColorTriggers x) const {
		return std::hash<int>()(static_cast<int>(x));
	}
};
template <> struct std::hash<GJCustomColorMode> {
	size_t operator()(GJCustomColorMode x) const {
		return std::hash<int>()(static_cast<int>(x));
	}
};

std::unordered_map<ColorTriggers, std::vector<GameObject*>> m_colorTriggers;
CCSpriteBatchNode* m_blendingBatchNode;
std::unordered_map<GJCustomColorMode, bool> m_currentColor;
float m_lastPos;

void LevelEditorLayerHook::setLastPos(float val) {
    m_lastPos = val;
}

void LevelEditorLayerHook::insertTrigger(GameObject* object) {
	auto& triggers = (m_colorTriggers)[ColorTriggers(object->m_objectID)];
	for (size_t i = 0; i < triggers.size(); ++i) {
		if (CompareTriggers()(object, triggers[i])) {
			triggers.insert(triggers.begin() + i, object);
			return;
		}
	}
	triggers.push_back(object);
}

void LevelEditorLayerHook::removeTrigger(GameObject* object) {
	auto& triggers = (m_colorTriggers)[ColorTriggers(object->m_objectID)];
	for (size_t i = 0; i < triggers.size(); ++i) {
		if (triggers[i] == object) {
			triggers.erase(triggers.begin() + i);
			break;
		}
	}
}

void LevelEditorLayerHook::moveTrigger(GameObject* object) {
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

GDColor calculateColor(std::vector<GameObject*> triggers, const float pos, const GDColor starting_color) {
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
		if (trigger->m_copyPlayerColor1) return mix_color(std::min(dist, 1.f), color_from, GameManager::sharedState()->colorForIdx(GameManager::sharedState()->m_playerColor));
		else if (trigger->m_copyPlayerColor2) return mix_color(std::min(dist, 1.f), color_from, GameManager::sharedState()->colorForIdx(GameManager::sharedState()->m_playerColor2));
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

void updateObjectColor(GameObject* object, const GDColor& color) {
    if (object == nullptr) return;

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

bool LevelEditorLayerHook::isColorBlending(GJCustomColorMode mode) {
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

void LevelEditorLayerHook::updatePreviewMode() {
	if (m_editorLayer) {
		GDColor bgColor, gColor, lColor, objColor, dlColor, color1, color2, color3, color4;
		GDColor white(ccc3(255, 255, 255));

		auto gm = GameManager::sharedState();

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

		(m_currentColor)[GJCustomColorMode::Col1] = color1.blending;
		(m_currentColor)[GJCustomColorMode::Col2] = color2.blending;
		(m_currentColor)[GJCustomColorMode::Col3] = color3.blending;
		(m_currentColor)[GJCustomColorMode::Col4] = color4.blending;
		(m_currentColor)[GJCustomColorMode::Col3DL] = dlColor.blending;

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

        CCARRAY_FOREACH_B_TYPE(m_editorLayer->m_levelSections, section, CCArray) {
            if (section) {
                CCARRAY_FOREACH_B_TYPE(section, object, GameObject) {
                    if (!object || !object->getParent() || object->m_isSelected) continue;

					if (object->m_isTintObject)
						object->setObjectColor(objColor);

					auto mode = object->getColorMode();
					switch (mode)
					{
					case GJCustomColorMode::Col3DL:
						updateObjectColor(object, dlColor);
						break;
					case GJCustomColorMode::Col1:
						updateObjectColor(object, color1);
						break;
					case GJCustomColorMode::Col2:
						updateObjectColor(object, color2);
						break;
					case GJCustomColorMode::Col3:
						updateObjectColor(object, color3);
						break;
					case GJCustomColorMode::Col4:
						updateObjectColor(object, color4);
						break;
					case GJCustomColorMode::LightBG:
						updateObjectColor(object, lbgColor);
						break;
					case GJCustomColorMode::PCol1:
						updateObjectColor(object, p1Color);
						break;
					case GJCustomColorMode::PCol2:
						updateObjectColor(object, p2Color);
						break;
					case static_cast<GJCustomColorMode>(9):
						updateObjectColor(object, white);
						break;
					default:;
					}
                }
            }
        }
	}
}

void LevelEditorLayerHook::resetColors() {
	if (m_editorLayer) {
		m_editorLayer->m_backgroundSprite->setColor(ccGRAY);

		if (m_groundLayer) {
			m_groundLayer->m_groundSprite->setColor(ccGRAY);
			m_groundLayer->m_line->setColor(ccWHITE);
		}

		CCARRAY_FOREACH_B_TYPE(m_editorLayer->m_levelSections, section, CCArray) {
            if (section) {
                CCARRAY_FOREACH_B_TYPE(section, object, GameObject) {
					if (!object || object->m_isSelected) continue;

					if (object->m_isTintObject)
						object->setObjectColor(ccWHITE);

					auto mode = object->m_customColorMode;
					const std::unordered_map<GJCustomColorMode, GDColor> defaultColors = {
						{GJCustomColorMode::Default, GDColor(255, 255, 255, false)},
						{GJCustomColorMode::Col3DL, GDColor(255, 255, 0, false)},
						{GJCustomColorMode::Col1, GDColor(255, 150, 255, false)},
						{GJCustomColorMode::Col2, GDColor(255, 255, 150, false)},
						{GJCustomColorMode::Col3, GDColor(150, 255, 255, false)},
						{GJCustomColorMode::Col4, GDColor(150, 255, 150, false)},
						{GJCustomColorMode::LightBG, GDColor(75, 175, 255, false)},
						{GJCustomColorMode::PCol1, GDColor(175, 150, 255, false)},
						{GJCustomColorMode::PCol2, GDColor(255, 150, 150, false)},
						{static_cast<GJCustomColorMode>(9), GDColor(255, 255, 255, false)}
					};
					updateObjectColor(object, defaultColors.at(mode));

					if (object->m_invisibleMode)
						object->setObjectColor(ccORANGE);

					if (object->getColorMode() == GJCustomColorMode::Col3DL)
						object->setObjectColor(ccYELLOW);
				}
			}
		}
	}
}

void LevelEditorLayerHook::updateOrientedHitboxes(LevelEditorLayer* self) {
	if (setting().onHitboxBugFix) {
		CCARRAY_FOREACH_B_TYPE(self->m_levelSections, section, CCArray) {
			if (section) {
				CCARRAY_FOREACH_B_TYPE(section, object, GameObject) {
					if (object && object->canRotateFree()) {
						switch (object->m_objectType) {
						case GameObjectType::Hazard:
						case GameObjectType::InverseGravityPortal:
						case GameObjectType::NormalGravityPortal:
						case GameObjectType::ShipPortal:
						case GameObjectType::CubePortal:
						case GameObjectType::YellowJumpPad:
						case GameObjectType::PinkJumpPad:
						case GameObjectType::GravityPad:
						case GameObjectType::YellowJumpRing:
						case GameObjectType::PinkJumpRing:
						case GameObjectType::GravityRing:
						case GameObjectType::BallPortal:
						case GameObjectType::RegularSizePortal:
						case GameObjectType::MiniSizePortal:
						case GameObjectType::UfoPortal:
						case GameObjectType::Modifier:
						case GameObjectType::DualPortal:
						case GameObjectType::SoloPortal:
						case GameObjectType::WavePortal:
							object->calculateOrientedBox();
						}
					}
				}
			}
		}
	}
}

void LevelEditorLayerHook::updateGroundWidth() {
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

void LevelEditorLayerHook::removeGroundLayer() {
	if (m_editorLayer) {
		if (m_groundLayer != nullptr) {
			m_groundLayer->removeFromParentAndCleanup(true);
			m_groundLayer = nullptr;
		}
	}
}

void LevelEditorLayerHook::createGroundLayer() {
	if (m_editorLayer) {
		if (m_groundLayer != nullptr) {
			LevelEditorLayerHook::removeGroundLayer();
		}
		m_groundLayer = GJGroundLayer::create(m_editorLayer->m_levelSettings->m_groundIndex);
		m_groundLayer->hideShadows();
		m_groundLayer->m_groundSprite->setAnchorPoint({ .5f, 1.f });
		m_editorLayer->m_gameLayer->addChild(m_groundLayer, 10);

		if (GameManager::sharedState()->getGameVariable(PREVIEW_MODE)) {
			LevelEditorLayerHook::updatePreviewMode();
		}
		else {
			LevelEditorLayerHook::resetColors();
		}
		LevelEditorLayerHook::updateGroundWidth();
	}
}

void LevelEditorLayerHook::Callback::onHideUI(CCObject*) {
	this->m_uiLayer->setVisible(!this->m_uiLayer->isVisible());
}

bool LevelEditorLayerHook::initH(LevelEditorLayer* self, GJGameLevel* level) {
    m_editorLayer = self;
    if (!LevelEditorLayerHook::init(self, level)) return false;

	auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

	auto hideUIMenu = CCMenu::create();
	hideUIMenu->setPosition(director->getScreenLeft(), director->getScreenTop());
	self->addChild(hideUIMenu, 100);

	auto hideUIOff = CCSprite::create("BE_eye-off-btn.png");
	hideUIOff->setOpacity(100);
	auto hideUIOn = CCSprite::create("BE_eye-on-btn.png");

	auto onHideUI = CCMenuItemToggler::create(hideUIOn, hideUIOff, self, menu_selector(LevelEditorLayerHook::Callback::onHideUI));
	onHideUI->setScale(.5f);
	onHideUI->setPosition(165.f, -21.f);
	hideUIMenu->addChild(onHideUI);

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

	// if (setting().onSolidPlayerGlow) {
	// 	self->m_player->m_playerFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
	// 	self->m_player->m_vehicleFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
	// 	self->m_player2->m_playerFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
	// 	self->m_player2->m_vehicleFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
	// }

	auto clicksDrawNode = CCDrawNode::create();
	self->m_gameLayer->addChild(clicksDrawNode, 1000, 126);

	if (GameManager::sharedState()->getGameVariable(SHOW_GROUND)) {
		LevelEditorLayerHook::createGroundLayer();
	}

    return true;
}

void LevelEditorLayerHook::addSpecialH(LevelEditorLayer* self, GameObject* object) {
    LevelEditorLayerHook::addSpecial(self, object);
	if (setting().onPreviewRotations && RotateSaws::objectIsSaw(object)) RotateSaws::beginRotateSaw(object);
    if (isColorTrigger(object)) insertTrigger(object);
}

void LevelEditorLayerHook::removeSpecialH(LevelEditorLayer* self, GameObject* object) {
	if (m_playtestStartPos == object) {
		m_playtestStartPos = nullptr;
	}

    LevelEditorLayerHook::removeSpecial(self, object);
	if (setting().onPreviewRotations && RotateSaws::objectIsSaw(object)) RotateSaws::stopRotateSaw(object);
    if (isColorTrigger(object)) removeTrigger(object);
}

void LevelEditorLayerHook::updateVisibilityH(LevelEditorLayer* self, float dt) {
    LevelEditorLayerHook::updateVisibility(self, dt);

    if (!isEditorPaused && GameManager::sharedState()->getGameVariable(PREVIEW_MODE)) {
        LevelEditorLayerHook::updatePreviewMode();
    }

	if (GameManager::sharedState()->getGameVariable(SHOW_GROUND)) {
		LevelEditorLayerHook::updateGroundWidth();
	}
}

void LevelEditorLayerHook::updateH(LevelEditorLayer* self, float dt) {
	LevelEditorLayerHook::update(self, dt);

	if (GameManager::sharedState()->getGameVariable(SHOW_GROUND)) {
		LevelEditorLayerHook::updateGroundWidth();
	}
}

GameObject* LevelEditorLayerHook::addObjectFromStringH(LevelEditorLayer* self, std::string object) {
	auto obj = LevelEditorLayerHook::addObjectFromString(self, object);

	if (obj && setting().onExperimentalLayering) updateObjectLayering(obj);

	return obj;
}

GameObject* LevelEditorLayerHook::createObjectH(LevelEditorLayer* self, int id, CCPoint position) {
	auto obj = LevelEditorLayerHook::createObject(self, id, position);

	if (obj && setting().onExperimentalLayering) updateObjectLayering(obj);

	return obj;
}

void LevelEditorLayerHook::flipGravityH(LevelEditorLayer* self, PlayerObject* acted, bool isFlipped, bool showEffect) { // taken from Zmx https://github.com/qimiko/gdps-public/blob/238b71e9f3cd8fdf855556ce4cc7c498f22cf3c0/src/hooks/LevelEditorLayer.cpp#L21
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

void runCustomPlaytest(LevelEditorLayer* self, StartPosObject* startPos) {
	LevelSettingsObject* settingsObject = nullptr;
	if (startPos) {
		settingsObject = startPos->m_settings;
	}
	else {
		settingsObject = self->m_levelSettings;
	}

	if (startPos) {
		self->setStartPosObject(startPos);
	}
	else {
		self->setStartPosObject(nullptr);
	}

	if (startPos) {
		self->m_player->setPosition(startPos->getPosition());
		self->m_player2->setPosition(startPos->getPosition());
	}
	else {
		self->m_player->setPosition(ccp(0.f, 0.f));
		self->m_player2->setPosition(ccp(0.f, 0.f));
	}

	self->setupLevelStart(settingsObject);

	if (settingsObject->m_startMode == 0) {
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

void LevelEditorLayerHook::onPlaytestH(LevelEditorLayer* self) {
	StartPosObject* selectedPlaytestStartPos = self->m_startPosObject;
	LevelEditorLayerHook::onPlaytest(self);

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

	LevelEditorLayerHook::updateOrientedHitboxes(self);

	if (setting().onPreviewRotations) RotateSaws::beginRotations(self);
}

void LevelEditorLayerHook::onResumePlaytestH(LevelEditorLayer* self) {
	LevelEditorLayerHook::onResumePlaytest(self);

	LevelEditorLayerHook::updateOrientedHitboxes(self);

	if (setting().onPreviewRotations) RotateSaws::resumeRotations(self);
}

void LevelEditorLayerHook::onPausePlaytestH(LevelEditorLayer* self) {
	LevelEditorLayerHook::onPausePlaytest(self);

	if (setting().onPreviewRotations) RotateSaws::pauseRotations(self);
}

void LevelEditorLayerHook::onStopPlaytestH(LevelEditorLayer* self) {
	LevelEditorLayerHook::onStopPlaytest(self);

	if (setting().onPreviewRotations) {
		RotateSaws::pauseRotations(self);
		RotateSaws::resumeRotations(self);
	}
}

void LevelEditorLayerHook::pushButtonH(LevelEditorLayer* self, int p0, bool p1) {
	LevelEditorLayerHook::pushButton(self, p0, p1);

	auto clicksDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(126));
	if (clicksDrawNode && setting().onShowClicks) {
		clicksDrawNode->drawDot(self->m_player->getPosition(), 3.f, ccc4f(1.f, .5f, 0.f, 1.f));

		if (self->m_player2 && self->m_dualMode) {
			clicksDrawNode->drawDot(self->m_player2->getPosition(), 3.f, ccc4f(1.f, .5f, 1.f, 1.f));
		}
	}
}

void LevelEditorLayerHook::releaseButtonH(LevelEditorLayer* self, int p0, bool p1) {
	LevelEditorLayerHook::releaseButton(self, p0, p1);

	auto clicksDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(126));
	if (clicksDrawNode && setting().onShowClicks) {
		clicksDrawNode->drawDot(self->m_player->getPosition(), 3.f, ccc4f(0.f, 1.f, 1.f, 1.f));

		if (self->m_player2 && self->m_dualMode) {
			clicksDrawNode->drawDot(self->m_player2->getPosition(), 3.f, ccc4f(.5f, 1.f, .5f, 1.f));
		}
	}
}

void LevelEditorLayerHook::destructorH(LevelEditorLayer* self) {
    LevelEditorLayerHook::destructor(self);
    m_colorTriggers.clear();
    m_currentColor.clear();
    m_lastPos = 0.f;
    m_blendingBatchNode = nullptr;
    m_playtestStartPos = nullptr;
    m_groundLayer = nullptr;
    m_editorLayer = nullptr;
}

void LevelEditorLayerHook::mem_init() {
    HOOK("_ZN16LevelEditorLayer4initEP11GJGameLevel", LevelEditorLayerHook::initH, LevelEditorLayerHook::init);
    HOOK("_ZN16LevelEditorLayer11flipGravityEP12PlayerObjectbb", LevelEditorLayerHook::flipGravityH, LevelEditorLayerHook::flipGravity);

    HOOK("_ZN16LevelEditorLayer19addObjectFromStringESs", LevelEditorLayerHook::addObjectFromStringH, LevelEditorLayerHook::addObjectFromString);
    HOOK("_ZN16LevelEditorLayer12createObjectEiN7cocos2d7CCPointE", LevelEditorLayerHook::createObjectH, LevelEditorLayerHook::createObject);

    HOOK("_ZN16LevelEditorLayer10addSpecialEP10GameObject", LevelEditorLayerHook::addSpecialH, LevelEditorLayerHook::addSpecial);
    HOOK("_ZN16LevelEditorLayer13removeSpecialEP10GameObject", LevelEditorLayerHook::removeSpecialH, LevelEditorLayerHook::removeSpecial);
    HOOK("_ZN16LevelEditorLayer16updateVisibilityEf", LevelEditorLayerHook::updateVisibilityH, LevelEditorLayerHook::updateVisibility);
    HOOK("_ZN16LevelEditorLayer6updateEf", LevelEditorLayerHook::updateH, LevelEditorLayerHook::update);

	HOOK("_ZN16LevelEditorLayer10onPlaytestEv", LevelEditorLayerHook::onPlaytestH, LevelEditorLayerHook::onPlaytest);
	HOOK("_ZN16LevelEditorLayer16onResumePlaytestEv", LevelEditorLayerHook::onResumePlaytestH, LevelEditorLayerHook::onResumePlaytest);
	HOOK("_ZN16LevelEditorLayer15onPausePlaytestEv", LevelEditorLayerHook::onPausePlaytestH, LevelEditorLayerHook::onPausePlaytest);
	HOOK("_ZN16LevelEditorLayer14onStopPlaytestEv", LevelEditorLayerHook::onStopPlaytestH, LevelEditorLayerHook::onStopPlaytest);

	HOOK("_ZN16LevelEditorLayer10pushButtonEib", LevelEditorLayerHook::pushButtonH, LevelEditorLayerHook::pushButton);
	HOOK("_ZN16LevelEditorLayer13releaseButtonEib", LevelEditorLayerHook::releaseButtonH, LevelEditorLayerHook::releaseButton);

    HOOK("_ZN16LevelEditorLayerD0Ev", LevelEditorLayerHook::destructorH, LevelEditorLayerHook::destructor);
}
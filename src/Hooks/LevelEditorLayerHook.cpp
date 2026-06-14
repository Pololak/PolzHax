#include "LevelEditorLayerHook.h"
#include "EditorUIHook.h"
#include "../hsv.h"
#include "../utils.h"

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

bool LevelEditorLayerHook::initH(LevelEditorLayer* self, GJGameLevel* level) {
    m_editorLayer = self;
    if (!LevelEditorLayerHook::init(self, level)) return false;

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

    return true;
}

void LevelEditorLayerHook::addSpecialH(LevelEditorLayer* self, GameObject* object) {
    LevelEditorLayerHook::addSpecial(self, object);
    if (isColorTrigger(object)) insertTrigger(object);
}

void LevelEditorLayerHook::removeSpecialH(LevelEditorLayer* self, GameObject* object) {
    LevelEditorLayerHook::removeSpecial(self, object);
    if (isColorTrigger(object)) removeTrigger(object);
}

void LevelEditorLayerHook::updateVisibilityH(LevelEditorLayer* self, float dt) {
    LevelEditorLayerHook::updateVisibility(self, dt);

    if (!isEditorPaused) {
        LevelEditorLayerHook::updatePreviewMode();
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
    HOOK("_ZN16LevelEditorLayer10addSpecialEP10GameObject", LevelEditorLayerHook::addSpecialH, LevelEditorLayerHook::addSpecial);
    HOOK("_ZN16LevelEditorLayer13removeSpecialEP10GameObject", LevelEditorLayerHook::removeSpecialH, LevelEditorLayerHook::removeSpecial);
    HOOK("_ZN16LevelEditorLayer16updateVisibilityEf", LevelEditorLayerHook::updateVisibilityH, LevelEditorLayerHook::updateVisibility);
    HOOK("_ZN16LevelEditorLayerD0Ev", LevelEditorLayerHook::destructorH, LevelEditorLayerHook::destructor);
}
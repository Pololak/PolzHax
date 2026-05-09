#include "LevelEditorLayer.hpp"
#include "Setting.hpp"
#include "Hitboxes.hpp"

LevelEditorLayer* m_editorLayer;

LevelEditorLayer* PolzLevelEditorLayer::get() {
    return m_editorLayer;
}

void PolzLevelEditorLayer::updateShowHitboxes() {
    auto playerDrawNode = static_cast<CCDrawNode*>(this->m_gameLayer->getChildByTag(124));
    auto objectDrawNode = static_cast<CCDrawNode*>(this->m_gameLayer->getChildByTag(125));
    if (!setting().m_clearHitboxes) playerDrawNode->clear();
    objectDrawNode->clear();

    if (setting().onHitboxes) {
        if (setting().onPlayerHitboxes) {
            if (this->m_player) Hitboxes::drawPlayerHitbox(this->m_player, playerDrawNode);
            if (this->m_player2) Hitboxes::drawPlayerHitbox(this->m_player2, playerDrawNode);
        }

        for (int i = this->m_firstVisibleSection + 1; i <= this->m_lastVisibleSection - 1; i++) {
            if (i < 0) continue;
            if (i >= this->m_levelSections->count()) break;

            auto objectAtIndex = this->m_levelSections->objectAtIndex(i);
            auto objArr = static_cast<CCArray*>(objectAtIndex);

            for (int j = 0; j < objArr->count(); j++) {
                auto obj = typeinfo_cast<GameObject*>(objArr->objectAtIndex(j));
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

void PolzLevelEditorLayer::updateOrientedHitboxes() {
    if (setting().onHitboxBugFix) {
        for (auto section : CCArrayExt<CCArray*>(this->m_levelSections)) {
            if (section) {
                for (auto object : CCArrayExt<GameObject*>(section)) {
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

bool PolzLevelEditorLayer::init(GJGameLevel* level) {
    m_editorLayer = this;
    if (!LevelEditorLayer::init(level)) return false;

    auto playerDrawNode = CCDrawNode::create();
    this->m_gameLayer->addChild(playerDrawNode, 1000, 124);
    auto objectDrawNode = CCDrawNode::create();
    this->m_gameLayer->addChild(objectDrawNode, 1000, 125);

    if (setting().onSolidPlayerGlow) {
        this->m_player->m_playerFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
        this->m_player->m_vehicleFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
        this->m_player2->m_playerFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
        this->m_player2->m_vehicleFrameGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
    }

    return true;
}

void PolzLevelEditorLayer::updateVisibility(float dt) {
    LevelEditorLayer::updateVisibility(dt);
    this->updateShowHitboxes();
}

void PolzLevelEditorLayer::update(float dt) {
    LevelEditorLayer::update(dt);
    this->updateShowHitboxes();
}

void PolzLevelEditorLayer::onPlaytest() {
    LevelEditorLayer::onPlaytest();
    this->updateOrientedHitboxes();
}

void PolzLevelEditorLayer::onResumePlaytest() {
    LevelEditorLayer::onResumePlaytest();
    this->updateOrientedHitboxes();
}

void LevelEditorLayer_destructor(LevelEditorLayer* self) {
    reinterpret_cast<void(__thiscall*)(LevelEditorLayer*)>(geode::base::get() + 0x8c080)(self);
    m_editorLayer = nullptr;
}

$on_mod(Loaded) {
    Mod::get()->hook(reinterpret_cast<void*>(geode::base::get() + 0x8c080), &LevelEditorLayer_destructor, "LevelEditorLayer::~LevelEditorLayer", tulip::hook::TulipConvention::Thiscall);
}
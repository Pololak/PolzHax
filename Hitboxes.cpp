#include "Hitboxes.hpp"
#include "Setting.hpp"

void Hitboxes::drawRect(CCDrawNode* drawer, CCRect const& rect, ccColor4F col) {
    constexpr size_t N = 4;
    CCPoint vert[N];

    vert[0] = CCPointMake(rect.getMinX(), rect.getMinY());
    vert[1] = CCPointMake(rect.getMinX(), rect.getMaxY());
    vert[2] = CCPointMake(rect.getMaxX(), rect.getMaxY());
    vert[3] = CCPointMake(rect.getMaxX(), rect.getMinY());

    auto colorA = ccc4FFromccc4B(ccc4(col.r, col.g, col.b, 0));

    drawer->drawPolygon(vert, N, colorA, .5f, col);
}

void Hitboxes::drawTriangleObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {
    constexpr size_t N = 3;
    CCPoint points[N];
    int i = 0;
    ob->updateOrientedBox();
    if (ob->m_objectOBB2D) {
        auto obb = ob->m_objectOBB2D;
        if (ob->m_isFlippedX || !ob->m_isFlippedY) points[i++] = obb->m_obVertexBottomLeft;
        if (!ob->m_isFlippedX || !ob->m_isFlippedY) points[i++] = obb->m_obVertexBottomRight;
        if (!ob->m_isFlippedX || ob->m_isFlippedY) points[i++] = obb->m_obVertexTopRight;
        if (ob->m_isFlippedX || ob->m_isFlippedY) points[i++] = obb->m_obVertexTopLeft;
    }
    else {
        auto rect = ob->getObjectRect();
        if (ob->m_isFlippedX || !ob->m_isFlippedY) points[i++] = CCPointMake(rect.getMinX(), rect.getMinY());
        if (!ob->m_isFlippedX || !ob->m_isFlippedY) points[i++] = CCPointMake(rect.getMinX(), rect.getMaxY());
        if (!ob->m_isFlippedX || ob->m_isFlippedY) points[i++] = CCPointMake(rect.getMaxX(), rect.getMaxY());
        if (ob->m_isFlippedX || ob->m_isFlippedY) points[i++] = CCPointMake(rect.getMaxX(), rect.getMinY());
    }

    drawer->drawPolygon(points, 3, { 0, 0, 0, 0 }, 0.5, col);
}

void Hitboxes::drawRectObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {
    ob->updateOrientedBox();

    CCPoint vert[4];

    if (ob->m_objectOBB2D) {
        vert[0] = ob->m_objectOBB2D->m_obVertexBottomLeft;
        vert[1] = ob->m_objectOBB2D->m_obVertexBottomRight;
        vert[2] = ob->m_objectOBB2D->m_obVertexTopRight;
        vert[3] = ob->m_objectOBB2D->m_obVertexTopLeft;
    }

    drawer->drawPolygon(vert, 4, { 0, 0, 0, 0 }, .5f, col);
}

void Hitboxes::drawCircleObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {
    ob->updateOrientedBox();
    if (!ob->m_objectOBB2D) return;
    auto rad = ob->m_objectRadius;

    constexpr size_t N = 64;
    constexpr float PI = 3.14159265;

    CCPoint vert[N];
    for (size_t i = 0; i < N; ++i) {
        vert[i] = ob->m_objectOBB2D->m_center + CCPointMake(std::cos(2 * PI * i / N), std::sin(2 * PI * i / N)) * rad;
    }
    drawer->drawPolygon(vert, N, { 0, 0, 0, 0 }, 0.5, col);
}

void Hitboxes::drawPlayerHitbox(gd::PlayerObject* player, CCDrawNode* drawNode)
{
    CCPoint pointRectangle[4];
    CCRect rectRectangle;
    CCRect rectRectangleSmall;
    pointRectangle[0] = player->getOrientedBox()->m_obVertexBottomLeft;
    pointRectangle[1] = player->getOrientedBox()->m_obVertexBottomRight;
    pointRectangle[2] = player->getOrientedBox()->m_obVertexTopRight;
    pointRectangle[3] = player->getOrientedBox()->m_obVertexTopLeft;

    auto p1x = pointRectangle[0].x - pointRectangle[1].x;
    auto p1y = pointRectangle[0].y - pointRectangle[1].y;
    auto distance1 = sqrt(p1x * p1x + p1y * p1y);

    auto p2x = pointRectangle[1].x - pointRectangle[2].x;
    auto p2y = pointRectangle[1].y - pointRectangle[2].y;
    auto distance2 = sqrt(p2x * p2x + p2y * p2y);

    auto distanceS1 = distance1 / 4;
    auto distanceS2 = distance2 / 4;

    rectRectangle.setRect(player->getPositionX() - distance1 / 2, player->getPositionY() - distance2 / 2, distance1, distance2);
    rectRectangleSmall.setRect(player->getPositionX() - distanceS1 / 2, player->getPositionY() - distanceS2 / 2, distanceS1, distanceS2);

    Hitboxes::drawRect(drawNode, rectRectangleSmall, { setting().solidR / 255.f, setting().solidG / 255.f, setting().solidB / 255.f, setting().hitboxesOpacity / 255.f });
    drawNode->drawPolygon(pointRectangle, 4, { 0, 0, 0, 0 }, 0.5, { (setting().hazardR / 255.f) / 2.f, (setting().hazardG / 255.f) / 2.f, (setting().hazardB / 255.f) / 2.f, setting().hitboxesOpacity / 255.f });
    Hitboxes::drawRect(drawNode, rectRectangle, { setting().hazardR / 255.f, setting().hazardG / 255.f, setting().hazardB / 255.f, setting().hitboxesOpacity / 255.f });
}

void Hitboxes::drawSolidsObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode) {
    if (obj->m_isDestroyed) return;
    switch (obj->m_objectType)
    {
    case gd::GameObjectType::Solid:
    case gd::GameObjectType::Breakable:
        Hitboxes::drawRectObj(drawNode, obj, { setting().solidR / 255.f, setting().solidG / 255.f, setting().solidB / 255.f, setting().hitboxesOpacity / 255.f }); break;
    case gd::GameObjectType::Slope:
        Hitboxes::drawTriangleObj(drawNode, obj, { setting().solidR / 255.f, setting().solidG / 255.f, setting().solidB / 255.f, setting().hitboxesOpacity / 255.f }); break;
    }
}

void Hitboxes::drawHazardsObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode) {
    if (obj->m_objectRadius > 0)
        Hitboxes::drawCircleObj(drawNode, obj, { setting().hazardR / 255.f, setting().hazardG / 255.f, setting().hazardB / 255.f, setting().hitboxesOpacity / 255.f });
    else if (obj->m_objectType == gd::GameObjectType::Hazard)
        Hitboxes::drawRectObj(drawNode, obj, { setting().hazardR / 255.f, setting().hazardG / 255.f, setting().hazardB / 255.f, setting().hitboxesOpacity / 255.f });
}

void Hitboxes::drawSpecialsObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode) {
    switch (obj->m_objectType) {
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
    case gd::GameObjectType::InverseMirrorPortal:
    case gd::GameObjectType::NormalMirrorPortal:
    case gd::GameObjectType::BallPortal:
    case gd::GameObjectType::RegularSizePortal:
    case gd::GameObjectType::MiniSizePortal:
    case gd::GameObjectType::UfoPortal:
    case gd::GameObjectType::SecretCoin:
    case gd::GameObjectType::DualPortal:
    case gd::GameObjectType::SoloPortal:
    case gd::GameObjectType::WavePortal:
        Hitboxes::drawRectObj(drawNode, obj, { setting().specialR / 255.f, setting().specialG / 255.f, setting().specialB / 255.f, setting().hitboxesOpacity / 255.f });
    }
    switch (obj->m_objectID) {
    case 200:
    case 201:
    case 202:
    case 203:
        Hitboxes::drawRectObj(drawNode, obj, { setting().specialR / 255.f, setting().specialG / 255.f, setting().specialB / 255.f, setting().hitboxesOpacity / 255.f });
    }
}
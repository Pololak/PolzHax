#include "Hitboxes.h"
#include "ObjectsIds.h"
#include "state.h"

void Hitboxes::drawRect(CCDrawNode* drawer, CCRect const& rect, ccColor4F col) {
    constexpr size_t N = 4;
    CCPoint vert[N];

    vert[0] = CCPointMake(rect.getMinX(), rect.getMinY());
    vert[1] = CCPointMake(rect.getMinX(), rect.getMaxY());
    vert[2] = CCPointMake(rect.getMaxX(), rect.getMaxY());
    vert[3] = CCPointMake(rect.getMaxX(), rect.getMinY());

    auto colorA = ccc4FFromccc4B(ccc4(col.r, col.g, col.b, 0));

    drawer->drawPolygon(vert, N, colorA, 0.5, col);
}

void Hitboxes::drawTriangleObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {

    ob->updateOrientedBox();
    if (!ob->getOrientedBox()) return;

    CCPoint triangle[3];
    auto isFlippedX = ob->getIsFlippedX();
    auto isFlippedY = ob->getIsFlippedY();
    auto point1 = ob->getOrientedBox()->getPoint1();
    auto point2 = ob->getOrientedBox()->getPoint2();
    auto point3 = ob->getOrientedBox()->getPoint3();
    auto point4 = ob->getOrientedBox()->getPoint4();

    if (!isFlippedX && !isFlippedY) {
        triangle[0] = point1;
        triangle[1] = point2;
        triangle[2] = point3;
    }
    else if (isFlippedX && !isFlippedY) {
        triangle[0] = point2;
        triangle[1] = point3;
        triangle[2] = point4;
    }
    else if (!isFlippedX && isFlippedY) {
        triangle[0] = point1;
        triangle[1] = point2;
        triangle[2] = point4;
    }
    else if (isFlippedX && isFlippedY) {
        triangle[0] = point1;
        triangle[1] = point3;
        triangle[2] = point4;
    }

    drawer->drawPolygon(triangle, 3, { 0, 0, 0, 0 }, 0.5, col);
}

void Hitboxes::drawRectObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {
    ob->updateOrientedBox();
    if (!ob->getOrientedBox()) return;

    CCPoint vert[4];
    vert[0] = ob->getOrientedBox()->getPoint1();
    vert[1] = ob->getOrientedBox()->getPoint2();
    vert[2] = ob->getOrientedBox()->getPoint3();
    vert[3] = ob->getOrientedBox()->getPoint4();

    drawer->drawPolygon(vert, 4, { 0, 0, 0, 0 }, 0.5, col);
}

void Hitboxes::drawCircleObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {
    ob->updateOrientedBox();
    if (!ob->getOrientedBox()) return;
    auto rad = ob->getObjectRadius();

    constexpr size_t N = 64;
    constexpr float PI = 3.14159265;

    CCPoint vert[N];
    for (size_t i = 0; i < N; ++i) {
        vert[i] = ob->getOrientedBox()->getCenterPoint() + CCPointMake(std::cos(2 * PI * i / N), std::sin(2 * PI * i / N)) * rad;
    }
    drawer->drawPolygon(vert, N, { 0, 0, 0, 0 }, 0.5, col);
}

void Hitboxes::drawPlayerHitbox(gd::PlayerObject* player, CCDrawNode* drawNode)
{
    CCPoint pointRectangle[4];
    CCRect rectRectangle;
    CCRect rectRectangleSmall;
    pointRectangle[0] = player->getOrientedBox()->getPoint1();
    pointRectangle[1] = player->getOrientedBox()->getPoint2();
    pointRectangle[2] = player->getOrientedBox()->getPoint3();
    pointRectangle[3] = player->getOrientedBox()->getPoint4();

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

    Hitboxes::drawRect(drawNode, rectRectangleSmall, { 0, 0, 1, 1 });
    drawNode->drawPolygon(pointRectangle, 4, { 0, 0, 0, 0 }, 0.5, { 0.5, 0, 0, 1 });
    Hitboxes::drawRect(drawNode, rectRectangle, { 1, 0, 0, 1 });
}

//void Hitboxes::drawObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode) {
//    if (hazard.contains(obj->getObjectID()))
//        Hitboxes::drawRectObj(drawNode, obj, { 1, 0, 0, 1 });
//    else if (orbsnportals.contains(obj->getObjectID()))
//        Hitboxes::drawRectObj(drawNode, obj, { 0, 1, 0, 1 });
//    else if (solids.contains(obj->getObjectID()))
//        Hitboxes::drawRectObj(drawNode, obj, { 0, 0, 1, 1 });
//    else if (ramps.contains(obj->getObjectID()))
//        Hitboxes::drawTriangleObj(drawNode, obj, { 0, 0, 1, 1 });
//    else if (saws.contains(obj->getObjectID()))
//        Hitboxes::drawCircleObj(drawNode, obj, { 1, 0, 0, 1 });
//}

void Hitboxes::drawSolidsObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode) {
    if (solids.contains(obj->getObjectID()))
        Hitboxes::drawRectObj(drawNode, obj, { (setting().solidsColor[0]), (setting().solidsColor[1]), (setting().solidsColor[2]), (setting().hitboxOpacity * 255.f) });
    else if (ramps.contains(obj->getObjectID()))
        Hitboxes::drawTriangleObj(drawNode, obj, { (setting().solidsColor[0]), (setting().solidsColor[1]), (setting().solidsColor[2]), (setting().hitboxOpacity * 255.f) });
}

void Hitboxes::drawHazardsObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode) {
    if (hazard.contains(obj->getObjectID()))
        Hitboxes::drawRectObj(drawNode, obj, { (setting().hazardsColor[0]), (setting().hazardsColor[1]), (setting().hazardsColor[2]), (setting().hitboxOpacity * 255.f) });
    else if (saws.contains(obj->getObjectID()))
        Hitboxes::drawCircleObj(drawNode, obj, { (setting().hazardsColor[0]), (setting().hazardsColor[1]), (setting().hazardsColor[2]), (setting().hitboxOpacity * 255.f) });
}

void Hitboxes::drawSpecialsObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode) {
    if (orbsnportals.contains(obj->getObjectID()))
        Hitboxes::drawRectObj(drawNode, obj, { (setting().specialsColor[0]), (setting().specialsColor[1]), (setting().specialsColor[2]), (setting().hitboxOpacity * 255.f) });
}
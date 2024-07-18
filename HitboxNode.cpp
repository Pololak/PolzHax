#include "pch.h"
#include "HitboxNode.h"

//HitboxNode* HitboxNode::create() {
//	 		auto ret = new HitboxNode;
//	 		if (ret) {
//	 			ret->init();
//	 			return ret;
//	 		}
//	 		return nullptr;
//	 	}

	 	void HitboxNode::drawCircleObj(gd::GameObject* ob, _ccColor4B col) {
	 		ob->updateOrientedBox();
	 		if (!ob->getOrientedBox()) return;
	 		auto rad = ob->getObjectRadius();
		
	 		auto color1 = ccc4FFromccc4B(col);
	 		auto colorA = ccc4FFromccc4B(ccc4(0, 0, 0, 0));
		
	 		constexpr size_t N = 64;
	 		constexpr float PI = 3.14159265;
	 		CCPoint vert[N];
	 		for (size_t i = 0; i < N; ++i) {
	 			vert[i] = ob->getPosition() + CCPointMake(std::cos(2 * PI * i / N), std::sin(2 * PI * i / N)) * rad;
	 		}
	 		this->drawPolygon(vert, N, colorA, 0.5, color1);
	 	}

	 	void HitboxNode::drawRectObj(gd::GameObject* ob, ccColor4F col) {
	 		ob->updateOrientedBox();
	 		if (!ob->getOrientedBox()) return;

	 		//constexpr size_t N = 4;
	 		CCPoint vert[4];
	 		vert[0] = ob->getOrientedBox()->getPoint1();
	 		vert[1] = ob->getOrientedBox()->getPoint2();
	 		vert[2] = ob->getOrientedBox()->getPoint3();
	 		vert[3] = ob->getOrientedBox()->getPoint4();

	 		auto colorA = ccc4FFromccc4B(ccc4(0, 0, 0, 0));

	 		this->drawPolygon(vert, 4, colorA, 0.5, col);		
	 	}
	 	void HitboxNode::drawOBB2D(gd::OBB2D* obb, ccColor4F col) {
	 		constexpr size_t N = 4;
	 		CCPoint vert[N];
	 		vert[0] = obb->getPoint1();
	 		vert[1] = obb->getPoint2();
	 		vert[2] = obb->getPoint3();
	 		vert[3] = obb->getPoint4();

	 		auto colorA = ccc4FFromccc4B(ccc4(0, 0, 0, 0));

	 		drawPolygon(vert, N, colorA, 0.5, col);
	 	}
	 	void HitboxNode::drawRect(CCRect const& rect, ccColor4F col) {
	 		constexpr size_t N = 4;
	 		CCPoint vert[N];

	 		vert[0] = CCPointMake(rect.getMinX(), rect.getMinY());
	 		vert[1] = CCPointMake(rect.getMinX(), rect.getMaxY());
	 		vert[2] = CCPointMake(rect.getMaxX(), rect.getMaxY());
	 		vert[3] = CCPointMake(rect.getMaxX(), rect.getMinY());

	 		auto colorA = ccc4FFromccc4B(ccc4(0, 0, 0, 0));

	 		drawPolygon(vert, N, colorA, 0.5, col);
	 	}
		void HitboxNode::drawTriangleObj(gd::GameObject* ob, ccColor4F col) {

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

			drawPolygon(triangle, 3, { 0, 0, 0, 0 }, 0.5, col);




			//ob->updateOrientedBox();
			//if (!ob->getOrientedBox()) return;

			//constexpr size_t N = 3;
			//CCPoint vert[N];
			//int i = 0;
			//if (ob->isFlipX() or !ob->isFlipY())
			//	vert[i++] = ob->getOrientedBox()->getPoint1();
			//if (!ob->isFlipX() or !ob->isFlipY())
			//	vert[i++] = ob->getOrientedBox()->getPoint2();
			//if (!ob->isFlipX() or ob->isFlipY())
			//	vert[i++] = ob->getOrientedBox()->getPoint3();
			//if (ob->isFlipX() or ob->isFlipY())
			//	vert[i++] = ob->getOrientedBox()->getPoint4();

			//auto colorA = ccc4FFromccc4B(ccc4(0, 0, 0, 0));

			//drawPolygon(vert, N, colorA, 0.5, col);
		}
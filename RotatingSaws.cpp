#include "RotatingSaws.hpp"

#include <map>
#include <array>

std::map<gd::GameObject*, float> g_startRotations;
std::array<int, 100> g_sawIDs = {
	88, 89, 98,
	397, 398, 399,
	675, 676, 677,
	186, 187, 188,
	740, 741, 742,
	678, 679, 680,
	183, 184, 185,
	85, 86, 87, 97,
	137, 138, 139,
	154, 155, 156,
	180, 181, 182,
	222, 223, 224,
	375, 376, 377, 378,
	394, 395, 396,
};

bool RotateSaws::objectIsSaw(gd::GameObject* obj) {
	for (auto id : g_sawIDs)
		if (id == obj->getObjectID())
			return true;

	return false;
}

void RotateSaws::beginRotateSaw(gd::GameObject* obj) {
	CCAction* r;
	//switch (obj->getObjectID())
	//{
	//case 0x55: r = obj->createRotateAction(360.f); break;
	//case 0x56: r = obj->createRotateAction(360.f); break;
	//case 0x57: r = obj->createRotateAction(360.f); break;
	//case 0x61: r = obj->createRotateAction(360.f); break;

	//case 0x89: r = obj->createRotateAction(360.f); break;
	//case 0x8a: r = obj->createRotateAction(360.f); break;
	//case 0x8b: r = obj->createRotateAction(360.f); break;

	//case 0x9a: r = obj->createRotateAction(360.f); break;
	//case 0x9b: r = obj->createRotateAction(360.f); break;
	//case 0x9c: r = obj->createRotateAction(360.f); break;

	//case 0xb4: r = obj->createRotateAction(360.f); break;
	//case 0xb5: r = obj->createRotateAction(360.f); break;
	//case 0xb6: r = obj->createRotateAction(360.f); break;

	//case 0xde: r = obj->createRotateAction(360.f); break;
	//case 0xdf: r = obj->createRotateAction(360.f); break;
	//case 0xe0: r = obj->createRotateAction(360.f); break;

	//case 0x177: r = obj->createRotateAction(360.f); break;
	//case 0x178: r = obj->createRotateAction(360.f); break;
	//case 0x179: r = obj->createRotateAction(360.f); break;
	//case 0x17a: r = obj->createRotateAction(360.f); break;

	//case 0x18a: r = obj->createRotateAction(360.f); break;
	//case 0x18b: r = obj->createRotateAction(360.f); break;
	//case 0x18c: r = obj->createRotateAction(360.f); break;
	
	//default:
		r = obj->createRotateAction(360.f);
		//break;
	//}
	r->setTag(9957);
	g_startRotations[obj] = obj->getRotation();
	obj->runAction(r);
}

void RotateSaws::beginRotations(gd::LevelEditorLayer* self) {
	auto objs = CCArray::create();
	for (int i = 0; i < (self->getLevelSections()->count()); i++)
	{
		objs->addObjectsFromArray(reinterpret_cast<CCArray*>(self->getLevelSections()->objectAtIndex(i)));
	}

	auto objs2 = CCArray::create();
	for (int i = 0; i < (objs->count()); i++)
	{
		auto obj = reinterpret_cast<gd::GameObject*>(objs->objectAtIndex(i));
		if (objectIsSaw(obj))
			beginRotateSaw(obj);
	}
}

void RotateSaws::stopRotateSaw(gd::GameObject* obj) {
	obj->stopActionByTag(9957);
	if (g_startRotations.count(obj)) {
		obj->setRotation(g_startRotations[obj]);
		g_startRotations.erase(obj);
	}
}

void RotateSaws::stopRotations(gd::LevelEditorLayer* self) {
	auto objs = CCArray::create();
	for (int i = 0; i < (self->getLevelSections()->count()); i++)
	{
		objs->addObjectsFromArray(reinterpret_cast<CCArray*>(self->getLevelSections()->objectAtIndex(i)));
	}

	auto objs2 = CCArray::create();
	for (int i = 0; i < (objs->count()); i++)
	{
		auto obj = reinterpret_cast<gd::GameObject*>(objs->objectAtIndex(i));
		if (objectIsSaw(obj))
			stopRotateSaw(obj);
	}
}

void RotateSaws::pauseRotations(gd::LevelEditorLayer* self) {
	auto objs = CCArray::create();
	for (int i = 0; i < (self->getLevelSections()->count()); i++)
	{
		objs->addObjectsFromArray(reinterpret_cast<CCArray*>(self->getLevelSections()->objectAtIndex(i)));
	}

	auto objs2 = CCArray::create();
	for (int i = 0; i < (objs->count()); i++)
	{
		auto obj = reinterpret_cast<gd::GameObject*>(objs->objectAtIndex(i));
		if (objectIsSaw(obj))
			obj->stopActionByTag(9957);
	}
}
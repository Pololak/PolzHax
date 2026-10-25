#include "ShowTrajectory.hpp"

void ShowTrajectory::createPlayers() {
	auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

	m_player = gd::PlayerObject::create(1, 1, playLayer);
	m_player->retain();
	m_player->setPosition(ccp(0.f, 105.f));
	m_player->setVisible(true);
	playLayer->m_gameLayer->addChild(m_player);

	m_player2 = gd::PlayerObject::create(1, 1, playLayer);
	m_player2->retain();
	m_player2->setPosition(ccp(0.f, 105.f));
	m_player2->setVisible(true);
	playLayer->m_gameLayer->addChild(m_player2);
}

void ShowTrajectory::drawRectangleHitbox(cocos2d::CCDrawNode* node, cocos2d::CCRect const& rect, ccColor4B colBase, ccColor4B colBorder) {
	constexpr size_t N = 4;
	std::vector<cocos2d::CCPoint> points(N);

	points[0] = CCPointMake(rect.getMinX(), rect.getMinY());
	points[1] = CCPointMake(rect.getMinX(), rect.getMaxY());
	points[2] = CCPointMake(rect.getMaxX(), rect.getMaxY());
	points[3] = CCPointMake(rect.getMaxX(), rect.getMinY());

	node->drawPolygon(const_cast<cocos2d::CCPoint*>(points.data()), points.size(), ccc4FFromccc4B(colBase), 0.25, ccc4FFromccc4B(colBorder));
}

void ShowTrajectory::drawForPlayer(gd::PlayerObject* player) {
	if (!player)
		return;
}

void ShowTrajectory::resetCollisionLog(gd::PlayerObject* player) {
	player->m_collisionLogTop->removeAllObjects();
	player->m_collisionLogBottom->removeAllObjects();
}

void ShowTrajectory::iterateForPlayer(gd::PlayerObject* player, bool holding, bool isPlayerTwo) {
	player->setVisible(true);
	m_isSimulationDead = false;

	bool iterAction = false;

	size_t iterations = 300;
	float dt = m_frameDt;

	// performance mode
	//if (setting().onPerformanceMode) {
	//	iterations /= 2;
	//	dt *= 2.f;
	//}

	for (int i = 0; i < iterations; i++) {
		auto playerPrevPos = player->getPosition();
		this->resetCollisionLog(player);

		gd::GameManager::sharedState()->getPlayLayer()->checkCollisions(player, dt);

		if (m_isSimulationDead) {
			break;
		}

		if (!iterAction && (!isPlayerTwo && !m_p1Down || isPlayerTwo && !m_p2Down)) {
			iterAction = true;
			holding ? player->pushButton(1) : player->releaseButton(1);
		}

		player->update(dt);
		player->updateRotation(dt);
	}
}

void ShowTrajectory::simulationForPlayer(gd::PlayerObject* player, gd::PlayerObject* player2, gd::PlayerObject* playerBase) {
	if (!player || !playerBase) {
		return;
	}

	bool isPlayerTwo = playerBase == gd::GameManager::sharedState()->getPlayLayer()->m_player2;

	player->spawnFromPlayer(playerBase);
	//player->m_gravity = playerBase->m_gravity;

	if (isPlayerTwo && m_p2Down || !isPlayerTwo && m_p1Down) {
		player->pushButton(1);
	}

	if (playerBase->m_onGround) {
		player->m_onGround = true;
	}

	this->iterateForPlayer(player, true, isPlayerTwo);

	return;

	player2->spawnFromPlayer(playerBase);

	if (isPlayerTwo && m_p2Down || !isPlayerTwo && m_p1Down) {
		player->releaseButton(1);
	}

	if (playerBase->m_onGround) {
		player2->m_onGround = true;
	}

	this->iterateForPlayer(player2, false, isPlayerTwo);
}
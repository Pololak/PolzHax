#include "SonicWaveFirstWave.hpp"

void BotTest::sonicWave2856(gd::PlayLayer* playLayer) {
	auto player = playLayer->player1();
	auto player2 = playLayer->player2();
	auto x = player->getRealPlayerPosX();

	if (x >= 13939.4)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 14045.3)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 14125.7)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 14274.3)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 14372.4)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 14501.8)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 14680.9)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 14754.8)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 14892.8)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 15044.2)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 15062)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 15145.1)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 15251.8)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 15307.9)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 15324.3)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 15407.5)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 15473.5)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 15527.5)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 15644.8)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 15782)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 15938.4)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 16064.3)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 16136.1)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 16216.4)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 16326.6)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 16414.7)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 16499.3)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 16586.1)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 16667.8)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 16754.6)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 16842.7)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 16921.6)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 17007)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 17130.1)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 17224.6)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 17292.2)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 17337.7)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 17473.5)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 17579.4)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 17636.6)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 17675.4)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 17716.6)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 17755.7)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 17798.3)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 17875.8)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 17962.6)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 18135.3)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 18216.4)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 18325.1)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 18477.3)
		player->releaseButton(gd::PlayerButton::Jump);

	if (x >= 18516.3) {
		player->pushButton(gd::PlayerButton::Jump);
		player2->pushButton(gd::PlayerButton::Jump);
	}
	if (x >= 18564.6) {
		player->releaseButton(gd::PlayerButton::Jump);
		player2->releaseButton(gd::PlayerButton::Jump);
	}

	if (x >= 18659.9) {
		player->pushButton(gd::PlayerButton::Jump);
		player2->pushButton(gd::PlayerButton::Jump);
	}
	if (x >= 18732.4) {
		player->releaseButton(gd::PlayerButton::Jump);
		player2->releaseButton(gd::PlayerButton::Jump);
	}

	if (x >= 18881) {
		player->pushButton(gd::PlayerButton::Jump);
		player2->pushButton(gd::PlayerButton::Jump);
	}
	if (x >= 18967) {
		player->releaseButton(gd::PlayerButton::Jump);
		player2->releaseButton(gd::PlayerButton::Jump);
	}

	if (x >= 19089.3) {
		player->pushButton(gd::PlayerButton::Jump);
		player2->pushButton(gd::PlayerButton::Jump);
	}
	if (x >= 19195.2) {
		player->releaseButton(gd::PlayerButton::Jump);
		player2->releaseButton(gd::PlayerButton::Jump);
	}

	if (x >= 19258.5) {
		player->pushButton(gd::PlayerButton::Jump);
		player2->pushButton(gd::PlayerButton::Jump);
	}
	if (x >= 19322.5) {
		player->releaseButton(gd::PlayerButton::Jump);
		player2->releaseButton(gd::PlayerButton::Jump);
	}

	if (x >= 19385) {
		player->pushButton(gd::PlayerButton::Jump);
		player2->pushButton(gd::PlayerButton::Jump);
	}
	if (x >= 19429.8) {
		player->releaseButton(gd::PlayerButton::Jump);
		player2->releaseButton(gd::PlayerButton::Jump);
	}

	if (x >= 19540.7) {
		player->pushButton(gd::PlayerButton::Jump);
		player2->pushButton(gd::PlayerButton::Jump);
	}
	if (x >= 19623.9) {
		player->releaseButton(gd::PlayerButton::Jump);
		player2->releaseButton(gd::PlayerButton::Jump);
	}

	if (x >= 19785.3) {
		player->pushButton(gd::PlayerButton::Jump);
		player2->pushButton(gd::PlayerButton::Jump);
	}
	if (x >= 19872.7) {
		player->releaseButton(gd::PlayerButton::Jump);
		player2->releaseButton(gd::PlayerButton::Jump);
	}

	if (x >= 20024.9) {
		player->pushButton(gd::PlayerButton::Jump);
		player2->pushButton(gd::PlayerButton::Jump);
	}
	if (x >= 20244.5) {
		player->releaseButton(gd::PlayerButton::Jump);
		player2->releaseButton(gd::PlayerButton::Jump);
	}

	if (x >= 20268)
		player->pushButton(gd::PlayerButton::Jump);
	if (x >= 20336.3)
		player->releaseButton(gd::PlayerButton::Jump);
}
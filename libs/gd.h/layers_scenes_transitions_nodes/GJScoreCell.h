#ifndef __GJSCORECELL_H__
#define __GJSCORECELL_H__

#include <gd.h>

namespace gd {
	enum class GJScoreType {
		Unknown = 0,
		Creator = 1
	};

	class GJUserScore : public cocos2d::CCNode {
	public:
		std::string m_userName; // 0xe8
		std::string m_userUDID; // 0x100
		GJScoreType m_scoreType; // 0x118
		int m_userID; // 0x11c
		int m_accountID; // 0x120
		int m_stars; // 0x124
		int m_demons; // 0x128
		int m_playerRank; // 0x12c
		int m_creatorPoints; // 0x130
		int m_secretCoins; // 0x134
		int m_iconID; // 0x138
		int m_color1; // 0x13c
		int m_color2; // 0x140
		int m_special; // 0x144
		IconType m_iconType; // 0x148
	};

	class GJScoreCell : public TableViewCell {
	public:
		GJUserScore* m_userScore; // 0x170
	};
}

#endif // !__GJSCORECELL_H__

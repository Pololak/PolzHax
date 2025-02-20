#pragma once
#ifndef GJCOMMENT_H
#define GJCOMMENT_H

#include <gd.h>

namespace gd {
    class GJComment : public cocos2d::CCNode {
        public:
			std::string m_comment; // 0xe8
			std::string m_userName; // 0x100
			long m_commentID; // 0x118
			int m_userID; // 0x11c
			int m_likes; // 0x120
			int m_dislikes; // 0x124
			int m_levelID; // 0x128
			bool m_isSpam; // m_isHidden // 0x12c
			int m_accountID; // 0x130
    };

}

#endif

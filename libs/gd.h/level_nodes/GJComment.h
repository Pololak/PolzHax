#pragma once
#ifndef GJCOMMENT_H
#define GJCOMMENT_H

#include <gd.h>

namespace gd {
    class GJComment : public cocos2d::CCNode {
        public:
			std::string m_comment;
			std::string m_userName;
			long m_commentID;
			int m_userID;
			int m_likeCount; // m_likes
			int m_dislikes;
			int m_levelID;
			bool m_isSpam; // m_isHidden
			int m_accountID;
    };

}

#endif

#ifndef __COMMENTCELL_H__
#define __COMMENTCELL_H__

#include <gd.h>

namespace gd {

    class GJComment;

    class CommentCell : public cocos2d::CCLayer, public LikeItemDelegate, public FLAlertLayerProtocol {
    public:
        int m_idk;
    };

}

#endif

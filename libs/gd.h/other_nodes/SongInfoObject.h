#ifndef __SONGINFOOBJECT_H__
#define __SONGINFOOBJECT_H__

#include <gd.h>

namespace gd {
    enum class GJSongError {
        FailedToFetch = 1,
        NotAllowed = 2,
        DownloadCancelled = 3
    };

    class SongInfoObject : public cocos2d::CCNode {
    public:
    };
}

#endif // !__SONGINFOOBJECT_H__

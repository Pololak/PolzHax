#ifndef __GJSEARCHOBJECT_H__
#define __GJSEARCHOBJECT_H__

#include <gd.h>

namespace gd {
    enum class SearchType {
        Search = 0,
        Downloaded = 1,
        MostLiked = 2,
        Trending = 3,
        Recent = 4,
        UsersLevels = 5,
        Featured = 6,
        Magic = 7,
        Sends = 8,
        MapPack = 9,
        MapPackOnClick = 10,
        Sent = 27,
        MyLevels = 98,
        SavedLevels = 99
    };
}

#endif // !__GJSEARCHOBJECT_H__

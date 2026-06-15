#ifndef __GJSEARCHOBJECT_H__
#define __GJSEARCHOBJECT_H__

#include "../include/gd.h"

class GJSearchObject : public cocos2d::CCNode {
public:
    SearchType m_searchType; // 0xe8
    std::string m_searchQuery; // 0xec
    std::string m_difficulty; // 0x104
    std::string m_length; // 0x11c
    int m_page; // 0x134
    bool m_starFilter; // 0x138
    bool m_noStarFilter; // 0x139
    int m_total; // 0x13c
    bool m_uncompletedFilter; // 0x140
    bool m_featuredFilter; // 0x141
    bool m_originalFilter; // 0x142
    bool m_twoPlayerFilter; // 0x143
    int m_songID; // 0x144
    bool m_customSongFilter; // 0x148
    bool m_songFilter; // 0x149

    static GJSearchObject* create(SearchType searchType, std::string searchQuery);

    static GJSearchObject* create(SearchType type) {
        return GJSearchObject::create(type, "");
    }

    const char* getKey();
};

#endif // !__GJSEARCHOBJECT_H__

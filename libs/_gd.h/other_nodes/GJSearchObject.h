#ifndef __GJSEARCHOBJECT_H__
#define __GJSEARCHOBJECT_H__

#include <gd.h>

namespace gd {

/*
enum SearchType {
    kSearchTypeMyLevels        = 0x62,
    kSearchTypeSavedLevels     = 0x63,
    kSearchTypeFeatured        = 0x6,
    kSearchTypeHallOfFame      = 0x10,
    kSearchTypeMapPacks        = 0x9,
    kSearchTypeSearch          = 0x0,
    kSearchTypeMostDownloaded  = 0x1,
    kSearchTypeMostLiked       = 0x2,
    kSearchTypeTrending        = 0x3,
    kSearchTypeRecent          = 0x4,
    kSearchTypeMagic           = 0x7,
    kSearchTypeAwarded         = 0xB,
    kSearchTypeFollowed        = 0xC,
    kSearchTypeFriends         = 0xD,
    kSearchTypeFindUsers       = 0xE,
    kSearchTypeUsersLevels     = 0x5
};

class GJSearchObjectOrig : public cocos2d::CCNode {

    SearchType m_type;
    gd::string m_string;
    gd::string m_difficultyStr;
    gd::string m_lengthStr;
    int m_page;
    bool m_star;
    bool m_noStar;
    int m_total;
    bool m_uncompleted;
    bool m_featured;
    bool m_original;
    bool m_twoPlayer;
    int m_song;
    bool m_customSong;
    bool m_songFilter;

    
    protected:
        SearchType m_nScreenID;
        PAD(96)
        int m_nCurrentFolder;

        // havent checked if there are more members

    public:
        static GJSearchObject* create(SearchType nID) {
            return reinterpret_cast<GJSearchObject*(__fastcall*)(
                SearchType
            )>(
                gd::base + 0xc2b90 //WRONG ADDRESS
            )( nID );
        }

        SearchType getType() {
            return this->m_nScreenID;
        }
};
*/

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
    Awarded = 11,
    Followed = 12,
    Friends = 13,
    Users = 14,
    LikedGDW = 15,
    HallOfFame = 16,
    FeaturedGDW = 17,
    Similar = 18,
    MyLevels = 98,
    SavedLevels = 99,
    FavouriteLevels = 100
};

class GJSearchObject : public cocos2d::CCNode {
public:
    static auto create(SearchType type, const std::string& query, const std::string& difficultyStr,
        const std::string& lengthStr, int page, bool isStar, bool isCompleted,
        bool isFeatured, int song, bool isOriginal, bool isTwoP,
        bool isCustomSong, bool isSongFilter, bool isNoStar) {
        return reinterpret_cast<GJSearchObject * (__fastcall*)(SearchType, gd::string, gd::string,
            gd::string, int, bool, bool,
            bool, int, bool, bool,
            bool, bool, bool)>(base + 0x652a0)( //0x652a0 - GD 1.92
                type, query, difficultyStr, lengthStr, page, isStar,
                isCompleted, isFeatured, song, isOriginal, isTwoP,
                isCustomSong, isSongFilter, isNoStar
                );
    }
    static auto create(SearchType type) {
        return reinterpret_cast<GJSearchObject * (__fastcall*)(SearchType)>(base + 0x650d0)(type); //0x650d0 - GD 1.92
        // return create(type, "", "-", "-", 0, false, false, false, 0, false, false, false, false, false);
    }
    static auto create(int type) {
        return reinterpret_cast<GJSearchObject * (__fastcall*)(int)>(base + 0x650d0)(type); //0x650d0 - GD 1.92
        // return create(type, "", "-", "-", 0, false, false, false, 0, false, false, false, false, false);
    }

    SearchType m_type;
    gd::string m_string;
    gd::string m_difficultyStr;
    gd::string m_lengthStr;
    int m_page;
    bool m_star;
    bool m_noStar;
    int m_total;
    bool m_uncompleted;
    bool m_featured;
    bool m_original;
    bool m_twoPlayer;
    int m_song;
    bool m_customSong;
    bool m_songFilter;
};



}

#endif

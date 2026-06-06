#ifndef __SONGINFOOBJECT_H__
#define __SONGINFOOBJECT_H__

#include <gd.h>

namespace gd {
    class SongInfoObject : public cocos2d::CCNode {
    public:
		int m_songID; // 0xe8
		std::string m_songName;
		std::string m_artistName;
		std::string m_youtubeVideo; // m_songYT
		std::string m_youtubeChannel; // m_artistYT
		std::string m_songUrl;
		std::string m_artistID;
		float m_fileSize;
		bool m_isUnloaded;
		bool m_isVerified;
		bool m_isBlocked;
		int m_priority;
		LevelSettingsObject* m_levelSettings;
    };
}

#endif // !__SONGINFOOBJECT_H__

#ifndef __MUSICDOWNLOADMANAGER_H__
#define __MUSICDOWNLOADMANAGER_H__

#include <gd.h>

namespace gd {
	class SongInfoObject;

	class MusicDownloadManager : public cocos2d::CCNode, PlatformDownloadDelegate {
	public:
		cocos2d::CCDictionary* m_activeDownloads; // 0xf0
		cocos2d::CCArray* m_musicDownloadDelegates; // 0xf4
		cocos2d::CCDictionary* m_songObjects; // 0xf8
		int m_maxPriority; // 0xfc

		static MusicDownloadManager* sharedState() {
			return reinterpret_cast<MusicDownloadManager * (__stdcall*)()>(base + 0xb0e60)();
		}

		bool isSongDownloaded(int songID) {
			return reinterpret_cast<bool(__thiscall*)(MusicDownloadManager*, int)>(base + 0xb26c0)(this, songID);
		}

		SongInfoObject* getSongInfoObject(int songID) {
			return reinterpret_cast<SongInfoObject * (__thiscall*)(MusicDownloadManager*, int)>(base + 0xb25e0)(this, songID);
		}
	};
}

#endif // !__MUSICDOWNLOADMANAGER_H__

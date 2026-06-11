#ifndef __GJACCOUNTMANAGER_H__
#define __GJACCOUNTMANAGER_H__

#include <gd.h>

namespace gd {
	class GJAccountManager : public cocos2d::CCNode {
	public:
		cocos2d::CCDictionary* m_activeDownloads; // 0xe8
		std::string m_password; // 0xec
		std::string m_username; // 0x104
		int m_accountID; // 0x11c
		GJAccountRegisterDelegate* m_accountRegisterDelegate; // 0x120
		GJAccountLoginDelegate* m_accountLoginDelegate; // 0x124
		GJAccountDelegate* m_accountDelegate; // 0x128
		GJAccountBackupDelegate* m_backupDelegate; // 0x12c
		GJAccountSyncDelegate* m_syncDelegate; // 0x130

		static GJAccountManager* sharedState() {
			return reinterpret_cast<GJAccountManager * (__stdcall*)()>(base + 0x79a80)();
		}

		void loginAccount(std::string username, std::string password) {
			reinterpret_cast<void(__thiscall*)(GJAccountManager*, std::string, std::string)>(base + 0x7a5f0)(this, username, password);
		}
	};
}

#endif // !__GJACCOUNTMANAGER_H__

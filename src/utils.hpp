#pragma once
#include <Geode/Geode.hpp>
#include <ShlObj.h>
#include "patching.hpp"

using namespace geode::prelude;

template <class R, class T>
R& from(T base, intptr_t offset) {
	return *reinterpret_cast<R*>(reinterpret_cast<uintptr_t>(base) + offset);
}

inline std::pair<std::string, std::string> split_once(const std::string& str, char split) {
	const auto n = str.find(split);
	return { str.substr(0, n), str.substr(n + 1) };
}

template <class Stream, class... Args>
void format_to(Stream& stream, const std::string_view& str, Args&&... args) {
	if constexpr (sizeof...(Args) == 0) {
		stream << str;
	}
	else {
		const std::function<void(Stream&)> partials[sizeof...(Args)] = { [args](Stream& stream) { stream << args; }... };
		size_t counter = 0;
		for (size_t i = 0; i < str.size(); ++i) {
			const auto c = str[i];
			if (c == '{' && str[i + 1] == '}') (partials[counter++](stream), ++i);
			else stream << c;
		}
	}
}

template <class... Args>
std::string format(const std::string_view& str, Args&&... args) {
	std::stringstream stream;
	format_to(stream, str, args...);
	return stream.str();
}

#define public_cast(value, member) [](auto* v) { \
	class FriendClass__; \
	using T = std::remove_pointer<decltype(v)>::type; \
	class FriendeeClass__: public T { \
	protected: \
		friend FriendClass__; \
	}; \
	class FriendClass__ { \
	public: \
		auto& get(FriendeeClass__* v) { return v->member; } \
	} c; \
	return c.get(reinterpret_cast<FriendeeClass__*>(v)); \
}(value)

struct time {
	static inline time_t getTime() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()
		).count();
	}
};

inline void copyFileToClipboard(char szFileName[]) {
	UINT uDropEffect;
	HGLOBAL hGblEffect;
	LPDWORD lpdDropEffect;
	DROPFILES stDrop;

	HGLOBAL hGblFiles;
	LPSTR lpData;

	uDropEffect = RegisterClipboardFormat("Preferred DropEffect");
	hGblEffect = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(DWORD));
	lpdDropEffect = (LPDWORD)GlobalLock(hGblEffect);
	*lpdDropEffect = DROPEFFECT_COPY;//copy;  Clipart DU DU DROPEFFECT_MOVE
	GlobalUnlock(hGblEffect);

	stDrop.pFiles = sizeof(DROPFILES);
	stDrop.pt.x = 0;
	stDrop.pt.y = 0;
	stDrop.fNC = FALSE;
	stDrop.fWide = FALSE;

	hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, \
		sizeof(DROPFILES) + strlen(szFileName) + 2);
	lpData = (LPSTR)GlobalLock(hGblFiles);
	memcpy(lpData, &stDrop, sizeof(DROPFILES));
	strcpy(lpData + sizeof(DROPFILES), szFileName);
	GlobalUnlock(hGblFiles);

	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_HDROP, hGblFiles);
	SetClipboardData(uDropEffect, hGblEffect);
	CloseClipboard();
}

inline void safeModeON() {
	sequence_patch(geode::base::get() + 0xf0624, { 0xeb, 0x6c });
	sequence_patch(geode::base::get() + 0xe53b6, { 0xe9, 0x77, 0x01, 0x00, 0x00, 0x90 });
	sequence_patch(geode::base::get() + 0xe5419, { 0xe9, 0x14, 0x00, 0x00, 0x00, 0x90 });
}

inline void safeModeOFF() {
	sequence_patch(geode::base::get() + 0xf0624, { 0x75, 0x6c });
	sequence_patch(geode::base::get() + 0xe53b6, { 0x0f, 0x85, 0x76, 0x01, 0x00, 0x00 });
	sequence_patch(geode::base::get() + 0xe5419, { 0x0f, 0x85, 0x13, 0x01, 0x00, 0x00 });
}
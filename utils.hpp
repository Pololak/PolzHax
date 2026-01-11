#pragma once
#include "pch.h"
#include "support/base64.h"

#define CCARRAY_FOREACH_B_BASE(__array__, __obj__, __type__, __index__)                                                                    \
    if (__array__ && __array__->count())                                                                                                   \
        for (auto [__index__, __obj__] = std::tuple<unsigned int, __type__> {0u, nullptr};                                                 \
             (__index__ < __array__->count() && (__obj__ = reinterpret_cast<__type__>(__array__->objectAtIndex(__index__)))); __index__++)

#define CCARRAY_FOREACH_B_TYPE(__array__, __obj__, __type__) CCARRAY_FOREACH_B_BASE(__array__, __obj__, __type__*, ix)

inline auto getExePath() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(GetModuleHandleA(NULL), buffer, MAX_PATH);
	return std::filesystem::path(buffer).parent_path();
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

template <typename T, typename U>
T union_cast(U value) {
	static_assert(sizeof(T) == sizeof(U), "union_cast sizes must mach");
	union {
		T a;
		U b;
	} u;
	u.b = value;
	return u.a;
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

struct clipboard { // Geode
public:
	static bool write(std::string const& data) {
		if (!OpenClipboard(nullptr)) return false;
		if (!EmptyClipboard()) {
			CloseClipboard();
			return false;
		}

		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, data.size() + 1);

		if (!hg) {
			CloseClipboard();
			return false;
		}

		auto dest = GlobalLock(hg);

		if (!dest) {
			CloseClipboard();
			return false;
		}

		memcpy(dest, data.c_str(), data.size() + 1);

		GlobalUnlock(hg);

		SetClipboardData(CF_TEXT, hg);
		CloseClipboard();

		GlobalFree(hg);

		return true;
	}

	static std::string read() {
		if (!OpenClipboard(nullptr)) return "";

		HANDLE hData = GetClipboardData(CF_TEXT);
		if (hData == nullptr) {
			CloseClipboard();
			return "";
		}

		char* pszText = static_cast<char*>(GlobalLock(hData));
		if (pszText == nullptr) {
			CloseClipboard();
			return "";
		}

		std::string text(pszText);

		GlobalUnlock(hData);
		CloseClipboard();

		return text;
	}
};

namespace base64 {
	inline std::string encode(std::string_view str) {
		char* out;
		const auto size = cocos2d::base64Encode(str.data(), str.size(), &out, false);
		std::string outs(out);
		free(out);
		return outs;
	}

	inline std::string decode(std::string_view str) {
		char* out;
		const auto size = cocos2d::base64Decode(str.data(), str.size(), &out);
		std::string outs(out, size);
		free(out);
		return outs;
	}
}

inline void safeModeON() {
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f0624), "\xeb\x6c", 2, NULL);
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e53b6), "\xe9\x77\x01\x00\x00\x90", 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5419), "\xe9\x14\x00\x00\x00\x90", 6, NULL);
}

inline void safeModeOFF() {
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f0624), "\x75\x6c", 2, NULL);
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e53b6), "\x0f\x85\x76\x01\x00\x00", 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5419), "\x0f\x85\x13\x01\x00\x00", 6, NULL);
}
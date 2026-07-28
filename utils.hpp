#pragma once
#include "pch.h"
#include "support/base64.h"
#include "patching.hpp"
#include <ShlObj.h>
#include "hsv.hpp"

#define CCARRAY_FOREACH_B_BASE(__array__, __obj__, __type__, __index__)                                                                    \
    if (__array__ && __array__->count())                                                                                                   \
        for (auto [__index__, __obj__] = std::tuple<unsigned int, __type__> {0u, nullptr};                                                 \
             (__index__ < __array__->count() && (__obj__ = reinterpret_cast<__type__>(__array__->objectAtIndex(__index__)))); __index__++)

#define CCARRAY_FOREACH_B_TYPE(__array__, __obj__, __type__) CCARRAY_FOREACH_B_BASE(__array__, __obj__, __type__*, ix)

#define VARIABLE_NAME(var) (#var + 10)

inline auto getExePath() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(GetModuleHandleA(NULL), buffer, MAX_PATH);
	return std::filesystem::path(buffer).parent_path();
}

inline std::pair<std::string, std::string> split_once(const std::string& str, char split) {
	const auto n = str.find(split);
	return { str.substr(0, n), str.substr(n + 1) };
}

inline std::vector<std::string> split(const std::string& str, char delim) {
	std::vector<std::string> strings;
	size_t start;
	size_t end = 0;
	while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
		end = str.find(delim, start);
		strings.push_back(str.substr(start, end - start));
	}
	return strings;
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

struct time {
	static inline time_t getTime() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()
		).count();
	}
};

inline cocos2d::CCPoint getMousePos() {
	auto* director = CCDirector::sharedDirector();
	auto* gl = director->getOpenGLView();
	auto winSize = director->getWinSize();
	auto frameSize = gl->getFrameSize();
	auto mouse = gl->getMousePosition() / frameSize;
	return ccp(mouse.x, 1.f - mouse.y) * winSize;
}

template <class T>
class Ref final {
	static_assert(
		std::is_base_of_v<cocos2d::CCObject, T>,
		"Ref can only be used with a CCObject-inheriting class!"
		);

	T* m_obj = nullptr;

public:
	/**
	 * Construct a Ref of an object. The object will be retained and
	 * managed until Ref goes out of scope
	 * @param obj Object to construct the Ref from
	 */
	Ref(T* obj) : m_obj(obj) {
		CC_SAFE_RETAIN(obj);
	}

	Ref(Ref<T> const& other) : Ref(other.data()) {}

	Ref(Ref<T>&& other) noexcept : m_obj(other.m_obj) {
		other.m_obj = nullptr;
	}

	/**
	 * Construct an empty Ref (the managed object will be null)
	 */
	Ref() = default;

	~Ref() {
		CC_SAFE_RELEASE(m_obj);
	}

	/**
	 * Swap the managed object with another object. The managed object
	 * will be released, and the new object retained
	 * @param other The new object to swap to
	 */
	void swap(T* other) {
		CC_SAFE_RELEASE(m_obj);
		m_obj = other;
		CC_SAFE_RETAIN(other);
	}

	/**
	 * Return the managed object
	 * @returns The managed object
	 */
	T* data() const {
		return m_obj;
	}

	operator T* () const {
		return m_obj;
	}

	T* operator*() const {
		return m_obj;
	}

	T* operator->() const {
		return m_obj;
	}

	T* operator=(T* obj) {
		this->swap(obj);
		return obj;
	}

	Ref<T>& operator=(Ref<T> const& other) {
		this->swap(other.data());
		return *this;
	}

	Ref<T>& operator=(Ref<T>&& other) {
		m_obj = other.data();
		other.m_obj = nullptr;
		return *this;
	}

	bool operator==(T* other) const {
		return m_obj == other;
	}

	bool operator==(Ref<T> const& other) const {
		return m_obj == other.m_obj;
	}

	bool operator!=(T* other) const {
		return m_obj != other;
	}

	bool operator!=(Ref<T> const& other) const {
		return m_obj != other.m_obj;
	}

	// for containers
	bool operator<(Ref<T> const& other) const {
		return m_obj < other.m_obj;
	}
	bool operator<=(Ref<T> const& other) const {
		return m_obj <= other.m_obj;
	}
	bool operator>(Ref<T> const& other) const {
		return m_obj > other.m_obj;
	}
	bool operator>=(Ref<T> const& other) const {
		return m_obj >= other.m_obj;
	}
};

template <class T>
concept CocosObject = std::derived_from<T, cocos2d::CCObject>;

template <class T>
concept CocosObjectPtr = std::is_pointer_v<T> && std::is_convertible_v<T, cocos2d::CCObject const*>;

template <class K>
concept CocosDictionaryKey = std::same_as<K, int> || std::same_as<K, intptr_t> || std::same_as<K, gd::string> || std::same_as<K, std::string>;

template <class InpT, CocosObject T = std::remove_pointer_t<InpT>>
class CCArrayExt {
protected:
	Ref<cocos2d::CCArray> m_arr;

public:
	using value_type = T*;
	using iterator = T**;
	using const_iterator = const T**;

	CCArrayExt() : m_arr(cocos2d::CCArray::create()) {}

	CCArrayExt(cocos2d::CCArray* arr)
		: m_arr(arr) {
	}

	CCArrayExt(CCArrayExt const& a) : m_arr(a.m_arr) {}

	CCArrayExt(CCArrayExt&& a) : m_arr(a.m_arr) {
		a.m_arr = nullptr;
	}

	~CCArrayExt() {}

	T** begin() const {
		if (!m_arr) {
			return nullptr;
		}
		return reinterpret_cast<T**>(m_arr->data->arr);
	}

	T** end() const {
		if (!m_arr) {
			return nullptr;
		}
		return reinterpret_cast<T**>(m_arr->data->arr) + m_arr->count();
	}

	auto rbegin() const {
		return std::reverse_iterator(this->end());
	}

	auto rend() const {
		return std::reverse_iterator(this->begin());
	}

	size_t size() const {
		return m_arr ? m_arr->count() : 0;
	}

	T* operator[](size_t index) {
		return static_cast<T*>(m_arr->objectAtIndex(index));
	}

	void push_back(T* item) {
		m_arr->addObject(item);
	}

	T* pop_back() {
		T* ret = static_cast<T*>(m_arr->lastObject());
		m_arr->removeLastObject();
		return ret;
	}

	cocos2d::CCArray* inner() {
		return m_arr;
	}
};

/**
	 * A templated wrapper over `CCDictElement`, acting as a simple iterator over
	 * `CCDictionary`.
	 *
	 * @tparam Type Pointer to a type that inherits CCObject.
	 */
template <class K, class InpT, CocosObject T = std::remove_pointer_t<InpT>>
struct CCDictIterator {
public:
	CCDictIterator(cocos2d::CCDictElement* p) : m_ptr(p) {}

	cocos2d::CCDictElement* m_ptr;

	std::pair<K, T*> operator*() {
		if constexpr (std::is_same_v<K, std::string> || std::is_same_v<K, gd::string>) {
			return { m_ptr->getStrKey(), static_cast<T*>(m_ptr->getObject()) };
		}
		else {
			return { m_ptr->getIntKey(), static_cast<T*>(m_ptr->getObject()) };
		}
	}

	auto& operator++() {
		m_ptr = static_cast<decltype(m_ptr)>(m_ptr->hh.next);
		return *this;
	}

	friend bool operator==(CCDictIterator<K, InpT> const& a, CCDictIterator<K, InpT> const& b) {
		return a.m_ptr == b.m_ptr;
	};

	friend bool operator!=(CCDictIterator<K, InpT> const& a, CCDictIterator<K, InpT> const& b) {
		return a.m_ptr != b.m_ptr;
	};

	bool operator!=(int b) {
		return m_ptr != nullptr;
	}
};

/**
 * A simple struct that as an entry to a `CCDictionary`.
 *
 * @tparam Type Pointer to a type that inherits CCObject.
 */
template <class K, class InpT, CocosObject T = std::remove_pointer_t<InpT>>
struct CCDictEntry {
	K m_key;
	cocos2d::CCDictionary* m_dict;

	CCDictEntry(K key, cocos2d::CCDictionary* dict) : m_key(key), m_dict(dict) {}

	T* operator->() {
		return static_cast<T*>(m_dict->objectForKey(m_key));
	}

	operator T* () {
		return static_cast<T*>(m_dict->objectForKey(m_key));
	}

	CCDictEntry& operator=(T* f) {
		m_dict->setObject(f, m_key);
		return *this;
	}
};

/**
 * A templated wrapper over CCDictionary, providing easy iteration and indexing.
 * This will keep ownership of the given CCDictionary*.
 *
 * @tparam Key Type of the key. MUST only be int or gd::string or std::string.
 * @tparam ValuePtr Pointer to a type that inherits CCObject.
 *
 * @example
 * CCDictionaryExt<std::string, GJGameLevel*> levels = getSomeDict();
 * // Easy indexing, giving you the type you assigned
 * GJGameLevel* myLvl = levels["Cube Adventures"];
 *
 * // Easy iteration using C++ range-based for loops
 * for (auto [name, level] : levels) {
 *   log::info("{}: {}", name, level->m_levelID);
 * }
 */
template <CocosDictionaryKey Key, class ValueInpT, CocosObject Value = std::remove_pointer_t<ValueInpT>>
struct CCDictionaryExt {
protected:
	using ValuePtr = Value*;

	Ref<cocos2d::CCDictionary> m_dict;

public:
	CCDictionaryExt() : m_dict(cocos2d::CCDictionary::create()) {}

	CCDictionaryExt(cocos2d::CCDictionary* dict) : m_dict(dict) {}

	CCDictionaryExt(CCDictionaryExt const& d) : m_dict(d.m_dict) {}

	CCDictionaryExt(CCDictionaryExt&& d) : m_dict(d.m_dict) {
		d.m_dict = nullptr;
	}

	auto begin() {
		return CCDictIterator<Key, ValuePtr>(m_dict->m_pElements);
	}

	// do not use this
	auto end() {
		return nullptr;
	}

	size_t size() {
		return m_dict->count();
	}

	auto operator[](const Key& key) {
		auto ret = static_cast<ValuePtr>(m_dict->objectForKey(key));
		if (!ret) m_dict->setObject(cocos2d::CCNode::create(), key);

		return CCDictEntry<Key, ValuePtr>(key, m_dict);
	}

	bool contains(const Key& key) {
		return m_dict->objectForKey(key) != nullptr;
	}

	size_t count(const Key& key) {
		return this->contains(key) ? 1 : 0;
	}

	cocos2d::CCDictionary* inner() {
		return m_dict;
	}
};

inline auto ci_equal = [](char a, char b)
	{
		return std::tolower(static_cast<unsigned char>(a)) ==
			std::tolower(static_cast<unsigned char>(b));
	};

inline bool ci_contains(const std::string& haystack, const std::string& needle)
{
	return std::search(haystack.begin(), haystack.end(),
		needle.begin(), needle.end(),
		ci_equal) != haystack.end();
}

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

namespace cocos2d {
	typedef struct _hashElement {
		struct _ccArray* actions;
		CCObject* target;
		unsigned int                actionIndex;
		CCAction* currentAction;
		bool                        currentActionSalvaged;
		bool                        paused;
		UT_hash_handle                hh;
	} tHashElement;

	static std::vector<CCAction*> getAllActions(CCNode* target) {
		std::vector<CCAction*> out;

		auto am = target->m_pActionManager;

		tHashElement* pElement = nullptr;
		HASH_FIND_INT(am->m_pTargets, &target, pElement);

		if (pElement) {
			if (pElement->actions != nullptr) {
				unsigned int limit = pElement->actions->num;
				for (unsigned int i = 0; i < limit; ++i) {
					out.push_back((CCAction*)pElement->actions->arr[i]);
				}
			}
		}

		return out;
	}
}

inline cocos2d::ccColor3B getLightBGColor(ccColor3B bg, ccColor3B p1) {
	auto hsv = color_utils::rgb_to_hsv({ bg.r / 255., bg.g / 255., bg.b / 255. });
	hsv.s = (std::max)(hsv.s - 0.2, 0.0);
	hsv.v = (std::min)(hsv.v + 0.2, 1.0);

	const auto rgb = color_utils::hsv_to_rgb(hsv);

	const ccColor3B lbg = { rgb.r * 255., rgb.g * 255., rgb.b * 255. };

	const auto amt = (static_cast<float>(bg.r) + static_cast<float>(bg.g) + static_cast<float>(bg.b)) / 150.f;

	if (amt < 1.f) {
		return gd::GameToolbox::getMixedColor(lbg, p1, amt);
	}
	else {
		return lbg;
	}
}

inline std::vector<std::string> explode(std::string& string, char separator) {
	std::stringstream segmentstream(string);
	std::string segmented;
	std::vector<std::string> splitlist;

	while (std::getline(segmentstream, segmented, separator)) {
		splitlist.push_back(segmented);
	}

	return splitlist;
}

inline bool nodeIsVisible(CCNode* node) {
	if (!node->isVisible()) {
		return false;
	}
	if (auto parent = node->getParent()) {
		return nodeIsVisible(parent);
	}
	return true;
}

inline static gd::GameObject* getClosestObject(std::vector<gd::GameObject*>& vec, gd::StartPosObject* startPos) {
	gd::GameObject* closest = nullptr;

	std::ranges::sort(vec, [](gd::GameObject* a, gd::GameObject* b) {
		return a->getPositionX() < b->getPositionX();
		});

	for (auto obj : vec) {
		if (obj->getPositionX() - 10 > startPos->getPositionX())
			break;
		if (obj->getPositionX() - 10 < startPos->getPositionX())
			closest = obj;
	}

	return closest;
}

inline void safeModeON() {
	sequence_patch(gd::base + 0xf0624, { 0xeb, 0x6c });
	sequence_patch(gd::base + 0xe53b6, { 0xe9, 0x77, 0x01, 0x00, 0x00, 0x90 });
	sequence_patch(gd::base + 0xe5419, { 0xe9, 0x14, 0x00, 0x00, 0x00, 0x90 });
}

inline void safeModeOFF() {
	sequence_patch(gd::base + 0xf0624, { 0x75, 0x6c });
	sequence_patch(gd::base + 0xe53b6, { 0x0f, 0x85, 0x76, 0x01, 0x00, 0x00 });
	sequence_patch(gd::base + 0xe5419, { 0x0f, 0x85, 0x13, 0x01, 0x00, 0x00 });
}
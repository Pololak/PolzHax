#pragma once

#include <type_traits>
// assumes you have cocos2d in your path
#include <cocos2d.h>

namespace matdash {
	// Wrapper over CCArray with a templated type and range based for loop
	// This does *not* own the CCArray, nor does it care about its lifetime
	template <class T>
	class CCArrayOf {
		static_assert(std::is_pointer_v<T>, "T must be a pointer");
		static_assert(std::is_convertible_v<T, cocos2d::CCObject*>, "T must be a class that inherits CCObject");

		cocos2d::CCArray* m_arr;
	public:
		CCArrayOf(cocos2d::CCArray* arr) : m_arr(arr) {}
		CCArrayOf(const CCArrayOf&) = delete;
		void operator=(const CCArrayOf&) = delete;
		
		T* begin() { return reinterpret_cast<T*>(m_arr->data->arr); }
		T* end() { return reinterpret_cast<T*>(m_arr->data->arr) + m_arr->count(); }

		auto size() const { return m_arr->count(); }
		T operator[](size_t index) { return static_cast<T>(m_arr->objectAtIndex(index)); }

		// returns the raw CCArray*
		cocos2d::CCArray* array() const { return m_arr; }
	};

	template <class T>
	auto modernize(cocos2d::CCArray* array) {
		return CCArrayOf<T>(array);
	}

	namespace detail {
		template <typename K, typename V>
		struct CCDictIterator {
			cocos2d::CCDictElement* m_ptr;
		public:
			explicit CCDictIterator(cocos2d::CCDictElement* p) : m_ptr(p) {}

			std::pair<K, V> operator*() {
				if constexpr (std::is_same<K, std::string>::value) {
					return { m_ptr->getStrKey(), static_cast<V>(m_ptr->getObject()) };
				} else {
					return { m_ptr->getIntKey(), static_cast<V>(m_ptr->getObject()) };
				}
			}

			auto& operator++() {
				m_ptr = reinterpret_cast<decltype(m_ptr)>(m_ptr->hh.next);
				return *this;
			}

			bool operator==(const CCDictIterator& other) { return m_ptr == other.m_ptr; };
			bool operator!=(const CCDictIterator& other) { return m_ptr != other.m_ptr; };
			bool operator!=(std::nullptr_t) { return m_ptr != nullptr; }
		};
	}

	// Wrapper over CCDictionary with a templated type and range based for loop
	// This does *not* own the CCDictionary, nor does it care about its lifetime
	template <typename Key, typename Value>
	class CCDictionaryOf {
		static_assert(std::is_same_v<Key, std::string> || std::is_same_v<Key, intptr_t>,
			"Key can only be std::string or intptr_t");

		static_assert(std::is_pointer_v<Value>, "Value must be a pointer");
		static_assert(std::is_convertible_v<Value, cocos2d::CCObject*>, "Value must be a class that inherits CCObject");
		cocos2d::CCDictionary* m_dict;
	public:
		CCDictionaryOf(cocos2d::CCDictionary* dict) : m_dict(dict) {}
		CCDictionaryOf(const CCDictionaryOf&) = delete;
		void operator=(const CCDictionaryOf&) = delete;

		auto begin() { return detail::CCDictIterator<Key, Value>(m_dict->m_pElements); }
		// do *not* use this!
		// this is only a placeholder for the iterator
		auto end() { return nullptr; }

		auto size() const { return m_dict->count(); }
		Value operator[](const Key& key) { return static_cast<Value>(m_dict->objectForKey(key)); }

		void insert(const Key& key, Value value) {
			m_dict->setObject(value, key);
		}

		// returns the raw CCDictionary*
		cocos2d::CCDictionary* dict() { return m_dict; }
	};

	template <class K, class V>
	auto modernize(cocos2d::CCDictionary* dict) {
		return CCDictionaryOf<K, V>(dict);
	}
}
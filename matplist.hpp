#pragma once
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <limits>

namespace matplist {
	struct Value {
		char name;
		std::string value;
	};

	static const auto max_ignore = std::numeric_limits<std::streamsize>::max();

	template <class S>
	class Dict {
		S& stream;
		bool done = false;
		char get_not(char c) {
			char out;
			while (stream && (stream.get(out), out) == c) {}
			return out;
		}
		using ScaryInner = std::pair<std::string, std::variant<Value, Dict>>;
		using Scary = std::optional<ScaryInner>;
		struct DictIterator {
			Dict& dict;
			Scary val = {};
			void next() {
				auto n = dict.next();
				val.swap(n);
			}
			DictIterator(Dict& dict) : dict(dict) {
				next();
			}
			bool operator!=(const std::nullptr_t&) { return val.has_value(); }
			auto& operator++() {
				next();
				return *this;
			}
			ScaryInner&& operator*() {
				return std::move(*val);
			}
		};
	public:
		Dict(S& stream) : stream(stream) {}
		Dict(const Dict&) = delete;
		Dict(Dict&& other) : stream(other.stream), done(other.done) {
			other.done = true;
		}
		~Dict() {
			if (!done) {
				while (next()) {}
			}
		}
		auto begin() { return DictIterator(*this); }
		auto end() { return nullptr; }
		Scary next() {
			if (!stream) return std::nullopt;
			stream.ignore(max_ignore, '<');
			char c = get_not(' ');
			if (c == '/') {
				char next = get_not(' ');
				if (next == 'd') {
					done = true;
					return std::nullopt;
				}
			}
			stream.ignore(max_ignore, '>');
			std::string key;
			std::getline(stream, key, '<');

			stream.ignore(max_ignore, '<');
			c = get_not(' ');
			if (c == 'd') {
				stream.ignore(max_ignore, '>');
				return std::pair(key, Dict(stream));
			}
			Value val;
			val.name = c;
			while (stream) {
				char c = stream.get();
				if (c == '/') {
					return std::pair(key, val);
				} else if (c == '>') break;
			}
			std::getline(stream, val.value, '<');
			stream.ignore(max_ignore, '>');
			return std::pair(key, val);
		}
		friend void swap(Dict& a, Dict& b) {
			std::swap(a.done, b.done);
		}
	};

	template <class S>
	Dict<S> parse(S& stream) {
		stream.ignore(max_ignore, '>');
		return Dict(stream);
	}
}
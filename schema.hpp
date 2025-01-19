#pragma once
#include <tuple>

// https://github.com/Erlkoenig90/map-macro/blob/msvc_varmacro_fix/map.h

   
/*
 * Copyright (C) 2012 William Swanson
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors or
 * their institutions shall not be used in advertising or otherwise to
 * promote the sale, use or other dealings in this Software without
 * prior written authorization from the authors.
 */

#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0(EVAL0(EVAL0(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL4(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL5(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))

#ifdef _MSC_VER
// MSVC needs more evaluations
#define EVAL6(...) EVAL5(EVAL5(EVAL5(__VA_ARGS__)))
#define EVAL(...)  EVAL6(EVAL6(__VA_ARGS__))
#else
#define EVAL(...)  EVAL5(__VA_ARGS__)
#endif

#define MAP_END(...)
#define MAP_OUT

#define EMPTY() 
#define DEFER(id) id EMPTY()

#define MAP_GET_END2() 0, MAP_END
#define MAP_GET_END1(...) MAP_GET_END2
#define MAP_GET_END(...) MAP_GET_END1
#define MAP_NEXT0(test, next, ...) next MAP_OUT
#define MAP_NEXT1(test, next) DEFER ( MAP_NEXT0 ) ( test, next, 0)
#define MAP_NEXT(test, next)  MAP_NEXT1(MAP_GET_END test, next)
#define MAP_INC(X) MAP_INC_ ## X

#define MAP0(f, x, peek, ...) f(x) DEFER ( MAP_NEXT(peek, MAP1) ) ( f, peek, __VA_ARGS__ ) 
#define MAP1(f, x, peek, ...) f(x) DEFER ( MAP_NEXT(peek, MAP0) ) ( f, peek, __VA_ARGS__ )

#define MAP0_UD(f, userdata, x, peek, ...) f(x,userdata) DEFER ( MAP_NEXT(peek, MAP1_UD) ) ( f, userdata, peek, __VA_ARGS__ ) 
#define MAP1_UD(f, userdata, x, peek, ...) f(x,userdata) DEFER ( MAP_NEXT(peek, MAP0_UD) ) ( f, userdata, peek, __VA_ARGS__ ) 

#define MAP0_UD_I(f, userdata, index, x, peek, ...) f(x,userdata,index) DEFER ( MAP_NEXT(peek, MAP1_UD_I) ) ( f, userdata, MAP_INC(index), peek, __VA_ARGS__ ) 
#define MAP1_UD_I(f, userdata, index, x, peek, ...) f(x,userdata,index) DEFER ( MAP_NEXT(peek, MAP0_UD_I) ) ( f, userdata, MAP_INC(index), peek, __VA_ARGS__ ) 

#define MAP_LIST0(f, x, peek, ...) , f(x) DEFER ( MAP_NEXT(peek, MAP_LIST1) ) ( f, peek, __VA_ARGS__ ) 
#define MAP_LIST1(f, x, peek, ...) , f(x) DEFER ( MAP_NEXT(peek, MAP_LIST0) ) ( f, peek, __VA_ARGS__ ) 
#define MAP_LIST2(f, x, peek, ...)   f(x) DEFER ( MAP_NEXT(peek, MAP_LIST1) ) ( f, peek, __VA_ARGS__ ) 

#define MAP_LIST0_UD(f, userdata, x, peek, ...) , f(x, userdata) DEFER ( MAP_NEXT(peek, MAP_LIST1_UD) ) ( f, userdata, peek, __VA_ARGS__ ) 
#define MAP_LIST1_UD(f, userdata, x, peek, ...) , f(x, userdata) DEFER ( MAP_NEXT(peek, MAP_LIST0_UD) ) ( f, userdata, peek, __VA_ARGS__ ) 
#define MAP_LIST2_UD(f, userdata, x, peek, ...)   f(x, userdata) DEFER ( MAP_NEXT(peek, MAP_LIST1_UD) ) ( f, userdata, peek, __VA_ARGS__ ) 

#define MAP_LIST0_UD_I(f, userdata, index, x, peek, ...) , f(x, userdata, index) DEFER ( MAP_NEXT(peek, MAP_LIST1_UD_I) ) ( f, userdata, MAP_INC(index), peek, __VA_ARGS__ ) 
#define MAP_LIST1_UD_I(f, userdata, index, x, peek, ...) , f(x, userdata, index) DEFER ( MAP_NEXT(peek, MAP_LIST0_UD_I) ) ( f, userdata, MAP_INC(index), peek, __VA_ARGS__ ) 
#define MAP_LIST2_UD_I(f, userdata, index, x, peek, ...)   f(x, userdata, index) DEFER ( MAP_NEXT(peek, MAP_LIST0_UD_I) ) ( f, userdata, MAP_INC(index), peek, __VA_ARGS__ ) 

/**
 * Applies the function macro `f` to each of the remaining parameters.
 */
#define MAP(f, ...) EVAL(MAP1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**
 * Applies the function macro `f` to each of the remaining parameters and
 * inserts commas between the results.
 */
#define MAP_LIST(f, ...) EVAL(MAP_LIST2(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**
 * Applies the function macro `f` to each of the remaining parameters and passes userdata as the second parameter to each invocation,
 * e.g. MAP_UD(f, x, a, b, c) evaluates to f(a, x) f(b, x) f(c, x)
 */
#define MAP_UD(f, userdata, ...) EVAL(MAP1_UD(f, userdata, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**
 * Applies the function macro `f` to each of the remaining parameters, inserts commas between the results,
 * and passes userdata as the second parameter to each invocation,
 * e.g. MAP_LIST_UD(f, x, a, b, c) evaluates to f(a, x), f(b, x), f(c, x)
 */
#define MAP_LIST_UD(f, userdata, ...) EVAL(MAP_LIST2_UD(f, userdata, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**
 * Applies the function macro `f` to each of the remaining parameters, passes userdata as the second parameter to each invocation,
 * and the index of the invocation as the third parameter,
 * e.g. MAP_UD_I(f, x, a, b, c) evaluates to f(a, x, 0) f(b, x, 1) f(c, x, 2)
 */
#define MAP_UD_I(f, userdata, ...) EVAL(MAP1_UD_I(f, userdata, 0, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**
 * Applies the function macro `f` to each of the remaining parameters, inserts commas between the results,
 * passes userdata as the second parameter to each invocation, and the index of the invocation as the third parameter,
 * e.g. MAP_LIST_UD_I(f, x, a, b, c) evaluates to f(a, x, 0), f(b, x, 1), f(c, x, 2)
 */
#define MAP_LIST_UD_I(f, userdata, ...) EVAL(MAP_LIST2_UD_I(f, userdata, 0, __VA_ARGS__, ()()(), ()()(), ()()(), 0))



struct schema_access {

};

template <class T, class Child>
struct SchemaBase : public schema_access {
	template <template <class> class Func>
	static auto& load_from(T& obj) {
		_loop<0, Func>(&obj);
		return obj;
	}

	template <template <class> class Func>
	static void save_to(const T& obj) {
		_loop<0, Func>(&obj);
	}

protected:
	template <size_t N>
	struct _type_at {
		using type = std::tuple_element_t<N, typename Child::types>;
	};
public:

	template <size_t N>
	using type_at = typename _type_at<N>::type;

	template <size_t N>
	static auto& value_at(T& obj) {
		return *reinterpret_cast<type_at<N>*>(reinterpret_cast<uintptr_t>(&obj) + Child::offsets[N]);
	}

	template <size_t N>
	static const auto& value_at(const T& obj) {
		return *reinterpret_cast<type_at<N>*>(reinterpret_cast<uintptr_t>(&obj) + Child::offsets[N]);
	}

	static constexpr size_t size() {
		return std::tuple_size_v<typename Child::types>;
	}

protected:
	template <size_t N, template <class> class Func, class U>
	static void _loop(U* obj) {
		Func<type_at<N>>::exec(value_at<N>(obj), Child::names[N]);
		if constexpr (N < size() - 1)
			_loop<N + 1, Func, U>(obj);
	}
};

template <class>
struct GetSchema { using type = void; };

template <class T>
using get_schema = typename GetSchema<T>::type;

#define gen_name(name) _Schema___##name##____

#define _GEN_DECLTYPES(x, name) decltype(name::x)
#define _GEN_OFFSETS(x, name) offsetof(name, x)
#define _GEN_NAMES(x) #x

#define DEF_SCHEMA(name, ...) \
	struct gen_name(name) : public SchemaBase<name, gen_name(name)> { \
	public: \
		friend SchemaBase<name, gen_name(name)>; \
		using base = name; \
		using types = std::tuple<MAP_LIST_UD(_GEN_DECLTYPES, name, __VA_ARGS__)>; \
		static constexpr const size_t offsets[] = {MAP_LIST_UD(_GEN_OFFSETS, name, __VA_ARGS__)}; \
		static constexpr const char* names[] = {MAP_LIST(_GEN_NAMES, __VA_ARGS__)}; \
		static constexpr const char* my_name = #name; \
	}; \
	template <> \
	struct GetSchema<name> { using type = gen_name(name); };

namespace {
	template <size_t N, class S, class T>
	void dbg_schema_loop(const T* obj) {
		std::cout << "  " << S::names[N] << " = " << S::value_at<N>(obj) << std::endl;
		if constexpr (N < S::size() - 1)
			dbg_schema_loop<N + 1, S>(obj);
	}
}

template <class T>
void dbg_schema(const T& obj) {
	using S = get_schema<T>;
	std::cout << "schema " << S::my_name << " {" << std::endl;
	dbg_schema_loop<0, S>(&obj);
	std::cout << "}" << std::endl;
}

namespace {
	template <size_t Index, class Schema, class T, class Func>
	void visit_schema_loop(T& object, Func&& func) {
		// terrific
		func.template operator()<Index>(Schema::value_at<Index>(object));
		if constexpr (Index < Schema::size() - 1)
			visit_schema_loop<Index + 1, Schema>(object, func);
	}	
}

template <class T, class Func>
void visit_schema(T& object, Func&& func) {
	using Schema = get_schema<T>;
	visit_schema_loop<0, Schema>(object, func);
}
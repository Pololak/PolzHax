#pragma once
#include <utility>

namespace matdash {
	enum CallConv {
		Optcall,
		Membercall,
		Thiscall,
		Cdecl,
		Stdcall
	};

	namespace detail {
		template <class T, CallConv>
		struct ValueWrapper {
			T value;
			template <class... Args>
			ValueWrapper(Args&&... v) : value(std::forward<Args>(v)...) {}
			ValueWrapper(T v) : value(v) {}
			operator T(){ return value; }
		};

		template <CallConv cc>
		struct ValueWrapper<void, cc> {};
	}

	namespace cc {
		template <class T>
		using optcall = detail::ValueWrapper<T, Optcall>;

		template <class T>
		using membercall = detail::ValueWrapper<T, Membercall>;

		template <class T>
		using thiscall = detail::ValueWrapper<T, Thiscall>;

		// cant name it cdecl :(
		template <class T>
		using c_decl = detail::ValueWrapper<T, Cdecl>;

		template <class T>
		using stdcall = detail::ValueWrapper<T, Stdcall>;
	}

	namespace detail {
		template <class F>
		struct function_ret;

		template <class R, class... Args>
		struct function_ret<R(Args...)> {
			using type = R;
		};

		template <class T, CallConv cc>
		struct extract_cc_or {
			static constexpr auto value = cc;
			using type = T;
		};

		template <class T, CallConv c, CallConv idk>
		struct extract_cc_or<ValueWrapper<T, c>, idk> {
			static constexpr auto value = c;
			using type = T;
		};

		template <class F>
		struct wrap_member_fn {
			template <auto>
			static void wrap() {}
		};

		template <class R, class T, class... Args>
		struct wrap_member_fn<R(T::*)(Args...)> {
			template <auto func>
			static R wrap(T* self, Args... args) {
				return (self->*func)(args...);
			}
		};

		template <class F>
		struct clean_fn_type { using type = F; };

		template <class R, class... Args>
		struct clean_fn_type<R(*)(Args...)> {
			using type = R(Args...);
		};

		template <class R, class T, class... Args>
		struct clean_fn_type<R(T::*)(Args...)> {
			using type = R(T*, Args...);
		};

		template <class F>
		struct remove_value_wrapper;

		template <class R, class... Args>
		struct remove_value_wrapper<R(Args...)> {
			using ret = typename extract_cc_or<R, CallConv::Cdecl>::type;
			using type = ret(Args...);
		};

		template <bool>
		struct ternary {
			template <auto a, auto b>
			static constexpr auto value = a;
		};

		template <>
		struct ternary<false> {
			template <auto a, auto b>
			static constexpr auto value = b;
		};

		namespace wrappers {
			template <auto>
			static void* tramp = nullptr;

			template <class F>
			struct thiscall;

			template <class R, class... Args>
			struct thiscall<R(Args...)> {
				template <auto func>
				static R __thiscall wrap(Args... args) {
					if constexpr (std::is_same_v<R, void>)
						func(args...);
					else
						return func(args...);
				}
				template <auto func>
				static R invoke(Args... args) {
					return reinterpret_cast<R(__thiscall*)(Args...)>(tramp<func>)(args...);
				}
			};

			template <class F>
			struct c_decl;

			template <class R, class... Args>
			struct c_decl<R(Args...)> {
				template <auto func>
				static R __cdecl wrap(Args... args) {
					if constexpr (std::is_same_v<R, void>)
						func(args...);
					else
						return func(args...);
				}
				template <auto func>
				static R invoke(Args... args) {
					return reinterpret_cast<R(__cdecl*)(Args...)>(tramp<func>)(args...);
				}
			};

			template <class F>
			struct stdcall;

			template <class R, class... Args>
			struct stdcall<R(Args...)> {
				template <auto func>
				static R __stdcall wrap(Args... args) {
					if constexpr (std::is_same_v<R, void>)
						func(args...);
					else
						return func(args...);
				}
				template <auto func>
				static R invoke(Args... args) {
					return reinterpret_cast<R(__stdcall*)(Args...)>(tramp<func>)(args...);
				}
			};

			template <class F>
			struct membercall;

			template <class F>
			struct optcall;

			#include "gen.hpp"
		}

		template <CallConv conv>
		struct wrapper_for_cc {
			static_assert(conv == conv, "unknown cc");
		};

		template <>
		struct wrapper_for_cc<CallConv::Thiscall> {
			template <class T>
			using type = wrappers::thiscall<T>;
		};

		template <>
		struct wrapper_for_cc<CallConv::Cdecl> {
			template <class T>
			using type = wrappers::c_decl<T>;
		};

		template <>
		struct wrapper_for_cc<CallConv::Stdcall> {
			template <class T>
			using type = wrappers::stdcall<T>;
		};

		template <>
		struct wrapper_for_cc<CallConv::Membercall> {
			template <class T>
			using type = wrappers::membercall<T>;
		};

		template <>
		struct wrapper_for_cc<CallConv::Optcall> {
			template <class T>
			using type = wrappers::optcall<T>;
		};
	}
}
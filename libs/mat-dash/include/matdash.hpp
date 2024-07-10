#pragma once
#include "matdash/detail/impl.hpp"
#include <utility>

namespace matdash {
	inline void add_hook_impl(void* addr, void* detour, void** trampoline);

	template <auto func, CallConv conv, class T>
	void add_hook(const T address) {
		const auto addr = reinterpret_cast<void*>(address);
		static constexpr auto callable = detail::ternary<std::is_member_function_pointer_v<decltype(func)>>::template value< 
			&detail::wrap_member_fn<decltype(func)>::template wrap<func>,
			func>;
		using F = typename detail::clean_fn_type<decltype(func)>::type;
		using F1 = typename detail::remove_value_wrapper<F>::type;

		using wrapper = typename detail::wrapper_for_cc<conv>::template type<F1>;

		void* const func_addr = reinterpret_cast<void*>(&wrapper::template wrap<callable>);
		void** const tramp_addr = &detail::wrappers::template tramp<func>;

		add_hook_impl(addr, func_addr, tramp_addr);
	}

	template <auto func, class T>
	void add_hook(const T address) {
		using F = typename detail::clean_fn_type<decltype(func)>::type;
		using R = typename detail::function_ret<F>::type;
		using Info = detail::extract_cc_or<R, CallConv::Membercall>;
		add_hook<func, Info::value>(address);
	}

	template <auto func, CallConv conv, class... Args>
	decltype(auto) orig(Args&&... args) {
		using F = typename detail::clean_fn_type<decltype(func)>::type;
		using F1 = typename detail::remove_value_wrapper<F>::type;

		using wrapper = typename detail::wrapper_for_cc<conv>::template type<F1>;

		return wrapper::template invoke<func>(std::forward<Args>(args)...);
	}

	template <auto func, class... Args>
	decltype(auto) orig(Args&&... args) {
		using F = typename detail::clean_fn_type<decltype(func)>::type;
		using R = typename detail::function_ret<F>::type;
		using Info = typename detail::extract_cc_or<R, CallConv::Membercall>;
		return orig<func, Info::value>(args...);
	}
}

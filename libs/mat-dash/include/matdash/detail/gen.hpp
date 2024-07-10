
template <class R, class ...Args>
struct optcall<R(Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(Args... args) {
		return func(args...);
	}
	template <auto func>
	static decltype(auto) invoke(Args... args) {
		
		return reinterpret_cast<R(__fastcall*)(Args...)>(tramp<func>)(args...);
	}
};


template <class R, class A, class B, class C, class ...Args>
struct optcall<R(A, B, C, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(A a0, B a1, C a2, Args... args) {
		float a3;
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, B a1, C a2, float a3, Args... args) {
		__asm movss xmm3, a3
		return reinterpret_cast<R(__fastcall*)(A, B, C, Args...)>(tramp<func>)(a0, a1, a2, args...);
	}
};


template <class R, class A, class B, class ...Args>
struct optcall<R(A, B, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(A a0, B a1, Args... args) {
		float a2;
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, B a1, float a2, Args... args) {
		__asm movss xmm2, a2
		return reinterpret_cast<R(__fastcall*)(A, B, Args...)>(tramp<func>)(a0, a1, args...);
	}
};


template <class R, class A, class B, class ...Args>
struct optcall<R(A, B, float, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(A a0, B a1, Args... args) {
		float a2, a3;
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, B a1, float a2, float a3, Args... args) {
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return reinterpret_cast<R(__fastcall*)(A, B, Args...)>(tramp<func>)(a0, a1, args...);
	}
};


template <class R, class A, class ...Args>
struct optcall<R(A, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(A a0, Args... args) {
		float a1;
		__asm movss a1, xmm1
		return func(a0, a1, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, float a1, Args... args) {
		__asm movss xmm1, a1
		return reinterpret_cast<R(__fastcall*)(A, Args...)>(tramp<func>)(a0, args...);
	}
};


template <class R, class A, class C, class ...Args>
struct optcall<R(A, float, C, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(A a0, C a2, Args... args) {
		float a1, a3;
		__asm movss a1, xmm1
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, float a1, C a2, float a3, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm3, a3
		return reinterpret_cast<R(__fastcall*)(A, C, Args...)>(tramp<func>)(a0, a2, args...);
	}
};


template <class R, class A, class ...Args>
struct optcall<R(A, float, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(A a0, Args... args) {
		float a1, a2;
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, float a1, float a2, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		return reinterpret_cast<R(__fastcall*)(A, Args...)>(tramp<func>)(a0, args...);
	}
};


template <class R, class A, class ...Args>
struct optcall<R(A, float, float, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(A a0, Args... args) {
		float a1, a2, a3;
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, float a1, float a2, float a3, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return reinterpret_cast<R(__fastcall*)(A, Args...)>(tramp<func>)(a0, args...);
	}
};


template <class R, class ...Args>
struct optcall<R(float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(Args... args) {
		float a0;
		__asm movss a0, xmm0
		return func(a0, args...);
	}
	template <auto func>
	static decltype(auto) invoke(float a0, Args... args) {
		__asm movss xmm0, a0
		return reinterpret_cast<R(__fastcall*)(Args...)>(tramp<func>)(args...);
	}
};


template <class R, class B, class C, class ...Args>
struct optcall<R(float, B, C, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(B a1, C a2, Args... args) {
		float a0, a3;
		__asm movss a0, xmm0
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(float a0, B a1, C a2, float a3, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm3, a3
		return reinterpret_cast<R(__fastcall*)(B, C, Args...)>(tramp<func>)(a1, a2, args...);
	}
};


template <class R, class B, class ...Args>
struct optcall<R(float, B, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(B a1, Args... args) {
		float a0, a2;
		__asm movss a0, xmm0
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
	template <auto func>
	static decltype(auto) invoke(float a0, B a1, float a2, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm2, a2
		return reinterpret_cast<R(__fastcall*)(B, Args...)>(tramp<func>)(a1, args...);
	}
};


template <class R, class B, class ...Args>
struct optcall<R(float, B, float, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(B a1, Args... args) {
		float a0, a2, a3;
		__asm movss a0, xmm0
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(float a0, B a1, float a2, float a3, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return reinterpret_cast<R(__fastcall*)(B, Args...)>(tramp<func>)(a1, args...);
	}
};


template <class R, class ...Args>
struct optcall<R(float, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(Args... args) {
		float a0, a1;
		__asm movss a0, xmm0
		__asm movss a1, xmm1
		return func(a0, a1, args...);
	}
	template <auto func>
	static decltype(auto) invoke(float a0, float a1, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm1, a1
		return reinterpret_cast<R(__fastcall*)(Args...)>(tramp<func>)(args...);
	}
};


template <class R, class C, class ...Args>
struct optcall<R(float, float, C, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(C a2, Args... args) {
		float a0, a1, a3;
		__asm movss a0, xmm0
		__asm movss a1, xmm1
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(float a0, float a1, C a2, float a3, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm1, a1
		__asm movss xmm3, a3
		return reinterpret_cast<R(__fastcall*)(C, Args...)>(tramp<func>)(a2, args...);
	}
};


template <class R, class ...Args>
struct optcall<R(float, float, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(Args... args) {
		float a0, a1, a2;
		__asm movss a0, xmm0
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
	template <auto func>
	static decltype(auto) invoke(float a0, float a1, float a2, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		return reinterpret_cast<R(__fastcall*)(Args...)>(tramp<func>)(args...);
	}
};


template <class R, class ...Args>
struct optcall<R(float, float, float, float, Args...)> {
	template <auto func>
	static decltype(auto) __fastcall wrap(Args... args) {
		float a0, a1, a2, a3;
		__asm movss a0, xmm0
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(float a0, float a1, float a2, float a3, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return reinterpret_cast<R(__fastcall*)(Args...)>(tramp<func>)(args...);
	}
};


template <class R, class ...Args>
struct membercall<R(Args...)> {
	template <auto func>
	static decltype(auto) __thiscall wrap(Args... args) {
		return func(args...);
	}
	template <auto func>
	static decltype(auto) invoke(Args... args) {
		
		return reinterpret_cast<R(__thiscall*)(Args...)>(tramp<func>)(args...);
	}
};


template <class R, class A, class B, class C, class ...Args>
struct membercall<R(A, B, C, float, Args...)> {
	template <auto func>
	static decltype(auto) __thiscall wrap(A a0, B a1, C a2, Args... args) {
		float a3;
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, B a1, C a2, float a3, Args... args) {
		__asm movss xmm3, a3
		return reinterpret_cast<R(__thiscall*)(A, B, C, Args...)>(tramp<func>)(a0, a1, a2, args...);
	}
};


template <class R, class A, class B, class ...Args>
struct membercall<R(A, B, float, Args...)> {
	template <auto func>
	static decltype(auto) __thiscall wrap(A a0, B a1, Args... args) {
		float a2;
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, B a1, float a2, Args... args) {
		__asm movss xmm2, a2
		return reinterpret_cast<R(__thiscall*)(A, B, Args...)>(tramp<func>)(a0, a1, args...);
	}
};


template <class R, class A, class B, class ...Args>
struct membercall<R(A, B, float, float, Args...)> {
	template <auto func>
	static decltype(auto) __thiscall wrap(A a0, B a1, Args... args) {
		float a2, a3;
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, B a1, float a2, float a3, Args... args) {
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return reinterpret_cast<R(__thiscall*)(A, B, Args...)>(tramp<func>)(a0, a1, args...);
	}
};


template <class R, class A, class ...Args>
struct membercall<R(A, float, Args...)> {
	template <auto func>
	static decltype(auto) __thiscall wrap(A a0, Args... args) {
		float a1;
		__asm movss a1, xmm1
		return func(a0, a1, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, float a1, Args... args) {
		__asm movss xmm1, a1
		return reinterpret_cast<R(__thiscall*)(A, Args...)>(tramp<func>)(a0, args...);
	}
};


template <class R, class A, class C, class ...Args>
struct membercall<R(A, float, C, float, Args...)> {
	template <auto func>
	static decltype(auto) __thiscall wrap(A a0, C a2, Args... args) {
		float a1, a3;
		__asm movss a1, xmm1
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, float a1, C a2, float a3, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm3, a3
		return reinterpret_cast<R(__thiscall*)(A, C, Args...)>(tramp<func>)(a0, a2, args...);
	}
};


template <class R, class A, class ...Args>
struct membercall<R(A, float, float, Args...)> {
	template <auto func>
	static decltype(auto) __thiscall wrap(A a0, Args... args) {
		float a1, a2;
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, float a1, float a2, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		return reinterpret_cast<R(__thiscall*)(A, Args...)>(tramp<func>)(a0, args...);
	}
};


template <class R, class A, class ...Args>
struct membercall<R(A, float, float, float, Args...)> {
	template <auto func>
	static decltype(auto) __thiscall wrap(A a0, Args... args) {
		float a1, a2, a3;
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
	template <auto func>
	static decltype(auto) invoke(A a0, float a1, float a2, float a3, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return reinterpret_cast<R(__thiscall*)(A, Args...)>(tramp<func>)(a0, args...);
	}
};


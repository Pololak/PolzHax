void mod_main(HMODULE);

DWORD WINAPI __mat_dash_thread_function(void* module) {
	mod_main(reinterpret_cast<HMODULE>(module));
	return 0;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(module);
		if (auto handle = CreateThread(0, 0, __mat_dash_thread_function, module, 0, 0))
			CloseHandle(handle);
	}
	return TRUE;
}
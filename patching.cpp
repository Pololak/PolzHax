#include "patching.hpp"

template <typename T2>
void patch(uint32_t address, T2 data)
{
	DWORD old;
	VirtualProtect(reinterpret_cast<void*>(address), sizeof(T2), PAGE_EXECUTE_READWRITE, &old);
	memcpy(reinterpret_cast<void*>(address), &data, sizeof(T2));
	VirtualProtect(reinterpret_cast<void*>(address), sizeof(T2), old, &old);
}

void sequence_patch(uint32_t address, std::vector<uint8_t> data) {
	for (uint32_t i = 0; i < data.size(); i++) patch(address + i, (uint8_t)data[i]);
}

inline std::vector<uint8_t> patchBytesEx(uintptr_t const address, std::vector<uint8_t> const& bytes, bool hardOverwrite = false) {
	auto hProcess = GetCurrentProcess();
	auto nTarget = reinterpret_cast<LPVOID>(address);
	std::vector<uint8_t> ret(bytes.size());
	if (!ReadProcessMemory(hProcess, nTarget, ret.data(), ret.size(), nullptr)) return {};

	DWORD oldprotect;
	if (hardOverwrite) VirtualProtectEx(hProcess, nTarget, bytes.size(), PAGE_EXECUTE_READWRITE, &oldprotect);
	if (!WriteProcessMemory(hProcess, nTarget, bytes.data(), bytes.size(), nullptr)) return {};
	if (hardOverwrite) VirtualProtectEx(hProcess, nTarget, bytes.size(), oldprotect, &oldprotect);

	return ret;
}

static std::unordered_map<uintptr_t, std::vector<uint8_t>> _patchedBytes;

std::vector<uint8_t> patch(uintptr_t addr, std::vector<uint8_t> bytes, bool hardOverwrite) {
	if (!_patchedBytes[addr].size()) _patchedBytes[addr] = patchBytesEx(addr, bytes, hardOverwrite);
	return _patchedBytes[addr];
}

void unpatch(uintptr_t addr, bool hardOverwrite) {
	if (_patchedBytes[addr].size()) patchBytesEx(addr, _patchedBytes[addr], hardOverwrite);
	_patchedBytes.erase(addr);
}
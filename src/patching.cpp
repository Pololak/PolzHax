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
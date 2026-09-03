#pragma once
#include "pch.h"

template <typename T2>
void patch(uint32_t address, T2 data);

void sequence_patch(uint32_t address, std::vector<uint8_t> data);
std::vector<uint8_t> patch(uintptr_t addr, std::vector<uint8_t> bytes, bool hardOverwrite = false);
void unpatch(uintptr_t addr, bool hardOverwrite = false);
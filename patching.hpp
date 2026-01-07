#pragma once
#include "pch.h"

template <typename T2>
void patch(uint32_t address, T2 data);

void sequence_patch(uint32_t address, std::vector<uint8_t> data);
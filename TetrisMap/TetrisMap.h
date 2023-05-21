#pragma once
#include <array>
#include "../Dictionary/Dictionary.h"
static std::array<size_t, kArrH* kArrW> TetrisMap;
template<typename T>
requires std::integral<T> || std::floating_point<T>
size_t GetIndex(const T& row, const T& col)
{
	return size_t(row) * kArrH + size_t(col);
}
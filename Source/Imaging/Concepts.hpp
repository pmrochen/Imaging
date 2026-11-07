/*
 *	Name: Concepts
 *	Author: Pawel Mrochen
 */

#pragma once

#include <concepts>

namespace imaging {

struct Uninitialized {};
constexpr Uninitialized UNINITIALIZED{};

template<typename T>
concept Arithmetic = (std::floating_point<T> || std::integral<T>);

} // namespace imaging

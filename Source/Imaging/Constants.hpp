/*
 *	Name: Constants
 *	Author: Pawel Mrochen
 */

#pragma once

#include <limits>

#ifndef IMAGING_NATIVE_ORDER_RGBA
#if defined(_WIN32) && !defined(_XBOX) && !defined(_XBOX_ONE)
#define IMAGING_NATIVE_ORDER_RGBA 0
#else
#define IMAGING_NATIVE_ORDER_RGBA 1
#endif
#endif

#ifndef IMAGING_NATIVE_ORDER_BGRA
#define IMAGING_NATIVE_ORDER_BGRA (1 - IMAGING_NATIVE_ORDER_RGBA)
#endif

namespace imaging {

template<typename T>
struct Constants;

template<>
struct Constants<float>
{
	static constexpr float TOLERANCE/*ZERO_TOLERANCE*/ = 1e-6f;
	static constexpr float EPSILON = std::numeric_limits<float>::epsilon();
	static constexpr float INF = std::numeric_limits<float>::infinity();
	static constexpr float MINUS_INF = -std::numeric_limits<float>::infinity();
};

template<>
struct Constants<double>
{
	static constexpr double TOLERANCE/*ZERO_TOLERANCE*/ = 1e-15;
	static constexpr double EPSILON = std::numeric_limits<double>::epsilon();
	static constexpr double INF = std::numeric_limits<double>::infinity();
	static constexpr double MINUS_INF = -std::numeric_limits<double>::infinity();
};

} // namespace imaging

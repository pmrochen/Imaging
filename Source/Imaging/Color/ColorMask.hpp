/*
 *	Name: ColorMask
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace imaging {

enum class ColorMask
{
    NONE = 0x0,
    LUMINANCE = 0x1,
    RED = 0x1,
    GREEN = 0x2,
    BLUE = 0x4,
    ALPHA = 0x8,
    ALL = 0xF
};

} // namespace imaging

ENABLE_ENUM_BITWISE_OPERATORS(imaging::ColorMask);

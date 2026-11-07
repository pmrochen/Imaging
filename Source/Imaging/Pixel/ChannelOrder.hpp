/*
 *	Name: ChannelOrder
 *	Author: Pawel Mrochen
 */

#pragma once

#include "../Constants.hpp"

namespace imaging {

enum class ChannelOrder
{
    UNSPECIFIED,
	RGBA,
	BGRA,
#if IMAGING_NATIVE_ORDER_RGBA
	NATIVE = RGBA
#else
	NATIVE = BGRA
#endif
};

} // namespace imaging

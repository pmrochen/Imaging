/*
 *	Name: MipmapFlags
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace imaging {

enum class MipmapFlags
{
	NONE = 0,
	FLOATING_POINT_PRECISION = 1,
	LINEARIZE = 2,
	NORMALIZE = 4					// normal map only
};

ENABLE_ENUM_BITWISE_OPERATORS(MipmapFlags);

} // namespace imaging

/*
 *	Name: ResamplingFilter
 *	Author: Pawel Mrochen
 */

#pragma once

namespace imaging {

enum class ResamplingFilter
{
	NONE,
	POINT,		// NEAREST_NEIGHBOR
	BOX,
	LINEAR,		// TRIANGLE
	CUBIC,		// CATMULL_ROM
	MITCHELL,	// MITCHELL_NETRAVALI	// best for upscaling
	LANCZOS,	// LANCZOS_3			// best for downscaling
	KAISER								// best for mipmapping
};

} // namespace imaging

/*
 *	Name: ImageFormat
 *	Author: Pawel Mrochen
 */

#pragma once

namespace imaging {

enum class ImageFormat
{
    UNDETERMINED = 0,
	NATIVE = 1,
	BMP = 2,
	TGA/*TRUEVISION_TGA*/ = 3,
	PSD = 4,
	PNG = 5,
	JPEG = 6,
	DDS = 9,
	HDR/*RADIANCE_RGBE*/ = 10,
	OPENEXR = 11,
	WEBP = 12
};

} // namespace imaging

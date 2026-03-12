/*
 *	Name: PixelFormatInfo
 *	Author: Pawel Mrochen
 */

#include "PixelFormatInfo.hpp"

namespace imaging {

/*static*/ const PixelFormatInfo PixelFormatInfo::infos_[PixelFormat::COUNT] =
{
	//						chn col hdr		comp	float	packed	depth	quantization	sign
	{ "Unspecified",		0,	0,	false,	false,	false,	false,	0,		0u,				false },
	{ "Luminance8",			1,	1,	false,	false,	false,	false,	8,		0xFFFFFFFFu,	false },
	{ "Alpha8",				1,	0,	false,	false,	false,	false,	8,		0xFFFFFFFFu,	false },
	{ "Luminance8Alpha8",	2,	1,	false,	false,	false,	true,	16,		0xFFFFFFFFu,	false },
	{ "Bgr4",				3,	3,	false,	false,	false,	true,	16,		0xFFF0F0F0u,	false },
	{ "Bgra4",				4,	3,	false,	false,	false,	true,	16,		0xF0F0F0F0u,	false },
	{ "Bgr5",				3,	3,	false,	false,	false,	true,	16,		0xFFF8F8F8u,	false },
	{ "Bgr5A1",				4,	3,	false,	false,	false,	true,	16,		0x80F8F8F8u,	false },
	{ "B5G6R5",				3,	3,	false,	false,	false,	true,	16,		0xFFF8FCF8u,	false },
	{ "R8",					1,	1,	false,	false,	false,	false,	8,		0xFFFFFFFFu,	false },
	{ "Rg8",				2,	2,	false,	false,	false,	false,	16,		0xFFFFFFFFu,	false },
	{ "Rgb8",				3,	3,	false,	false,	false,	true,	32,		0xFFFFFFFFu,	false },
	{ "Rgba8",				4,	3,	false,	false,	false,	true,	32,		0xFFFFFFFFu,	false },
	{ "Bgr8",				3,	3,	false,	false,	false,	true,	32,		0xFFFFFFFFu,	false },
	{ "Bgra8",				4,	3,	false,	false,	false,	true,	32,		0xFFFFFFFFu,	false },
	{ "Rgb10A2",			4,	3,	true,	false,	false,	true,	32,		0xC0FFFFFFu,	false },
	{ "R16",				1,	1,	true,	false,	false,	false,	16,		0u,				false },
	{ "Rg16",				2,	2,	true,	false,	false,	false,	32,		0u,				false },
	{ "Rgba16",				4,	3,	true,	false,	false,	false,	64,		0u,				false },
	{ "R32",				1,	1,	true,	false,	false,	false,	32,		0u,				false },
	{ "Rg32",				2,	2,	true,	false,	false,	false,	64,		0u,				false },
	{ "Rgb32",				3,	3,	true,	false,	false,	false,	96,		0u,				false },
	{ "Rgba32",				4,	3,	true,	false,	false,	false,	128,	0u,				false },
	{ "R8i",				1,	1,	false,	false,	false,	false,	8,		0xFFFFFFFFu,	true },
	{ "Rg8i",				2,	2,	false,	false,	false,	false,	16,		0xFFFFFFFFu,	true },
	{ "Rgba8i",				4,	3,	false,	false,	false,	true,	32,		0xFFFFFFFFu,	true },
	{ "R16i",				1,	1,	true,	false,	false,	false,	16,		0u,				true },
	{ "Rg16i",				2,	2,	true,	false,	false,	false,	32,		0u,				true },
	{ "Rgba16i",			4,	3,	true,	false,	false,	false,	64,		0u,				true },
	{ "R32i",				1,	1,	true,	false,	false,	false,	32,		0u,				true },
	{ "Rg32i",				2,	2,	true,	false,	false,	false,	64,		0u,				true },
	{ "Rgb32i",				3,	3,	true,	false,	false,	false,	96,		0u,				true },
	{ "Rgba32i",			4,	3,	true,	false,	false,	false,	128,	0u,				true },
	{ "R11fG11fB10f",		3,	3,	true,	false,	true,	true,	32,		0u,				false },
	{ "R16f",				1,	1,	true,	false,	true,	false,	16,		0u,				true },
	{ "Rg16f",				2,	2,	true,	false,	true,	false,	32,		0u,				true },
	{ "Rgba16f",			4,	3,	true,	false,	true,	false,	64,		0u,				true },
	{ "R32f",				1,	1,	true,	false,	true,	false,	32,		0u,				true },
	{ "Rg32f",				2,	2,	true,	false,	true,	false,	64,		0u,				true },
	{ "Rgb32f",				3,	3,	true,	false,	true,	false,	96,		0u,				true },
	{ "Rgba32f",			4,	3,	true,	false,	true,	false,	128,	0u,				true },
	{ "Rgb9E5",				3,	3,	true,	false,	false,	true,	32,		0u,				false },
	{ "BC1",				4,	3,	false,	true,	false,	true,	4,		0x80F8FCF8u,	false },
	{ "BC2",				4,	3,	false,	true,	false,	true,	8,		0xF0F8FCF8u,	false },
	{ "BC3",				4,	3,	false,	true,	false,	true,	8,		0xFFF8FCF8u,	false },
	{ "BC4",				1,	1,	false,	true,	false,	true,	4,		0xFFFFFFFFu,	false },
	{ "BC5",				2,	2,	false,	true,	false,	true,	8,		0xFFFFFFFFu,	false },
	{ "BC6Hu",				3,	3,	true,	true,	true,	true,	8,		0u,				false },
	{ "BC6Hs",				3,	3,	true,	true,	true,	true,	8,		0u,				true },
	{ "BC7",				4,	3,	false,	true,	false,	true,	8,		0xFFFEFEFEu,	false }
	//						chn col hdr		comp	float	packed	depth	quantization
};

} // namespace imaging

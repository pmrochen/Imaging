/*
 *	Name: PixelFormatInfo
 *	Author: Pawel Mrochen
 */

#pragma once

#include <string>
#include <cstdint>
#include "ColorMask.hpp"
#include "PixelFormat.hpp"

namespace imaging {

struct PixelFormatInfo
{
	static const PixelFormatInfo& get(PixelFormat format) noexcept
	{
		unsigned int index = (unsigned int)format;
		return infos_[(index < (unsigned int)PixelFormat::COUNT) ? index : 0u];
	}

	const std::string& getName() const noexcept { return name; }
	ColorMask getChannels() const noexcept;
	int getNumberOfChannels() const noexcept { return nChannels; }
	int getNumberOfColorChannels() const noexcept { return nColorChannels; }
	bool hasAlphaChannel() const noexcept { return (nChannels > nColorChannels); }
	bool hasHighDynamicRange() const noexcept { return hdr; }
	bool isCompressed() const noexcept { return compressed; }
	bool isFloatingPoint() const noexcept { return floatingPoint; }
	bool isPacked/*Color*/() const noexcept { return packed; }
	//bool hasSharedExponent() const noexcept { return (value == RGB9_E5); }
	int getBitDepth() const noexcept { return bitDepth; }
	//int getSize() const noexcept { return bitDepth >> 3; }
	bool isSigned() const noexcept { return sign; }

	std::string name;
	int nChannels;
	int nColorChannels;
	bool hdr;
	bool compressed;
	bool floatingPoint;
	bool packed;
	int bitDepth;
	std::uint32_t quantizationMask;
	bool sign;

private:
	static const PixelFormatInfo infos_[(unsigned int)PixelFormat::COUNT];
};

} // namespace imaging

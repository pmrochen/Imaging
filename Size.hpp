/*
 *	Name: Size
 *	Author: Pawel Mrochen
 */

#pragma once

#include <istream>
#include <ostream>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <functional>
#include <utility>
#include <tuple>
#include <cstddef>
#include <cmath>

namespace imaging {

struct Uninitialized {};
constexpr Uninitialized UNINITIALIZED{};

template<typename T>
concept Arithmetic = (std::floating_point<T> || std::integral<T>); // #TODO Move to Concepts.hpp

template<typename T>
struct Constants;

template<>
struct Constants<float>
{
	static constexpr float TOLERANCE = 1e-6f;
};

template<>
struct Constants<double>
{
	static constexpr double TOLERANCE = 1e-15;
};

namespace templates {

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
struct Size
{
	using ComponentType = T;
	using ConstArg = const Size&;
	using ConstResult = const Size&;

	static constexpr int NUM_COMPONENTS = 2;

	constexpr Size() noexcept : width(), height() {}
	explicit Size(Uninitialized) noexcept {}
	constexpr Size(T width, T height) noexcept : width(width), height(height) {}
	explicit Size(const std::pair<T, T>& t) noexcept : width(t.first), height(t.second) {}
	template<Arithmetic U> explicit Size(const std::pair<U, U>& t) noexcept : width(T(t.first)), height(T(t.second)) {}
	explicit Size(const std::tuple<T, T>& t) noexcept : width(std::get<0>(t)), height(std::get<1>(t)) {}
	template<Arithmetic U> explicit Size(const std::tuple<U, U>& t) noexcept : width(T(std::get<0>(t))), height(T(std::get<1>(t))) {}
	template<Arithmetic U> explicit Size(const Size<U>& size) noexcept : width(T(size.width)), height(T(size.height)) {}

	T& operator[](int i) noexcept { return (&width)[i]; }
	const T& operator[](int i) const noexcept { return (&width)[i]; }

	Size operator+() const noexcept { return *this; }
	Size operator-() const noexcept { return Size(-width, -height); }
	Size& operator+=(const Size& size) noexcept { width += size.width; height += size.height; return *this; }
	Size& operator-=(const Size& size) noexcept { width -= size.width; height -= size.height; return *this; }
	Size& operator*=(const Size& size) noexcept { width *= size.width; height *= size.height; return *this; }
	Size& operator*=(T f) noexcept { width *= f; height *= f; return *this; }
	Size& operator/=(const Size& size) noexcept { width /= size.width; height /= size.height; return *this; }
	Size& operator/=(T f) noexcept;
	bool operator==(const Size& size) const noexcept { return (width == size.width) && (height == size.height); }
	bool operator!=(const Size& size) const noexcept { return !(*this == size); }
	
	template<typename A> void serialize(A& ar) { ar(width, height); }

	template<std::size_t I> T& get() noexcept;
	template<std::size_t I> const T& get() const noexcept;

	bool isZero() const noexcept { return (width == T()) && (height == T()); }
	bool isApproxZero() const noexcept requires std::floating_point<T>;
	bool approxEquals(const Size& size) const noexcept requires std::floating_point<T>;
	bool approxEquals(const Size& size, T tolerance) const noexcept requires std::floating_point<T>;
	bool allLessThan(const Size& size) const noexcept { return (width < size.width) && (height < size.height); }
	bool allLessThanEqual(const Size& size) const noexcept { return (width <= size.width) && (height <= size.height); }
	bool allGreaterThan(const Size& size) const noexcept { return (width > size.width) && (height > size.height); }
	bool allGreaterThanEqual(const Size& size) const noexcept { return (width >= size.width) && (height >= size.height); }
	bool anyLessThan(const Size& size) const noexcept { return (width < size.width) || (height < size.height); }
	bool anyLessThanEqual(const Size& size) const noexcept { return (width <= size.width) || (height <= size.height); }
	bool anyGreaterThan(const Size& size) const noexcept { return (width > size.width) || (height > size.height); }
	bool anyGreaterThanEqual(const Size& size) const noexcept { return (width >= size.width) || (height >= size.height); }
	bool isFinite() const noexcept requires std::floating_point<T>;
	Size& setZero() noexcept { width = T(); height = T(); return *this; }
	Size& set(T width, T height) noexcept { this->width = width; this->height = height; return *this; }
	Size& setMinimum(const Size& size1, const Size& size2);
	Size& setMaximum(const Size& size1, const Size& size2);
	template<std::floating_point U /*= T*/> U getAspectRatio() const noexcept;

	static const Size ZERO;

	T width;
	T height;
};

template<typename T> const Size<T> Size<T>::ZERO{};

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> operator+(const Size<T>& size1, const Size<T>& size2) noexcept
{
	return Size<T>(size1.width + size2.width, size1.height + size2.height);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> operator-(const Size<T>& size1, const Size<T>& size2) noexcept
{
	return Size<T>(size1.width - size2.width, size1.height - size2.height);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> operator*(const Size<T>& size1, const Size<T>& size2) noexcept
{
	return Size<T>(size1.width*size2.width, size1.height*size2.height);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> operator*(T f, const Size<T>& size) noexcept
{
	return Size<T>(f*size.width, f*size.height);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> operator*(const Size<T>& size, T f) noexcept
{
	return Size<T>(size.width*f, size.height*f);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> operator/(const Size<T>& size1, const Size<T>& size2) noexcept
{
	return Size<T>(size1.width/size2.width, size1.height/size2.height);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> operator/(T f, const Size<T>& size) noexcept
{
	return Size<T>(f/size.width, f/size.height);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> operator/(const Size<T>& size, T f) noexcept
{
	if constexpr (std::is_floating_point_size<T>)
		return operator*(size, T(1)/f);
	else
		return Size<T>(size.width/f, size.height/f);
}

template<typename T>
inline Size<T>& Size<T>::operator/=(T f)
{
	if constexpr (std::is_floating_point_v<T>)
	{
		return operator*=(T(1)/f);
	}
	else
	{
		width /= f;
		height /= f;
		return *this;
	}
}

template<typename C, typename T, typename U>
	requires std::floating_point<U> || std::integral<U>
inline std::basic_istream<C, T>& operator>>(std::basic_istream<C, T>& s, Size<U>& size)
{ 
	return s >> size.width >> std::ws >> size.height;
}

template<typename C, typename T, typename U>
	requires std::floating_point<U> || std::integral<U>
inline std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& s, const Size<U>& size)
{ 
	constexpr C WS(0x20);
	return s << size.width << WS << size.height;
}

template<typename T>
template<std::size_t I>
inline T& Size<T>::get()
{
	if constexpr (I == 0)
		return width;
	else if constexpr (I == 1)
		return height;
	static_assert(false);
}

template<typename T>
template<std::size_t I>
inline const T& Size<T>::get() const
{
	if constexpr (I == 0)
		return width;
	else if constexpr (I == 1)
		return height;
	static_assert(false);
}

template<typename T>
inline bool Size<T>::isApproxZero() const noexcept requires std::floating_point<T>
{ 
	return (std::fabs(width) < Constants<T>::TOLERANCE) && (std::fabs(height) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Size<T>::approxEquals(const Size<T>& size) const noexcept requires std::floating_point<T>
{ 
	return (std::fabs(size.width - width) < Constants<T>::TOLERANCE) && (std::fabs(size.height - height) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Size<T>::approxEquals(const Size<T>& size, T tolerance) const noexcept requires std::floating_point<T>
{ 
	return (std::fabs(size.width - width) < tolerance) && (std::fabs(size.height - height) < tolerance);
}

template<typename T>
inline bool Size<T>::isFinite() const noexcept requires std::floating_point<T> 
{ 
	return std::isfinite(width) && std::isfinite(height); 
}

template<typename T>
inline Size<T>& Size<T>::setMinimum(const Size<T>& size1, const Size<T>& size2)
{
	width = std::min(size1.width, size2.width);
	height = std::min(size1.height, size2.height);
	return *this;
}

template<typename T>
inline Size<T>& Size<T>::setMaximum(const Size<T>& size1, const Size<T>& size2)
{
	width = std::max(size1.width, size2.width);
	height = std::max(size1.height, size2.height);
	return *this;
}

template<typename T>
template<std::floating_point U>
inline U Size<T>::getAspectRatio() const
{
	return (height != T(0)) ? U((double)width/(double)height) : U();
}

template<std::size_t I, typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline T& get(Size<T>& size) noexcept
{
	if constexpr (I == 0)
		return size.width;
	else if constexpr (I == 1)
		return size.height;
	static_assert(false);
}

template<std::size_t I, typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline const T& get(const Size<T>& size) noexcept
{
	if constexpr (I == 0)
		return size.width;
	else if constexpr (I == 1)
		return size.height;
	static_assert(false);
}

template<std::size_t I, typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline T&& get(Size<T>&& size) noexcept
{
	if constexpr (I == 0)
		return size.width;
	else if constexpr (I == 1)
		return size.height;
	static_assert(false);
}

template<std::size_t I, typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline const T&& get(const Size<T>&& size) noexcept
{
	if constexpr (I == 0)
		return size.width;
	else if constexpr (I == 1)
		return size.height;
	static_assert(false);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> min(const Size<T>& size1, const Size<T>& size2)
{
	return Size<T>(std::min(size1.width, size2.width), std::min(size1.height, size2.height));
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> max(const Size<T>& size1, const Size<T>& size2)
{
	return Size<T>(std::max(size1.width, size2.width), std::max(size1.height, size2.height));
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> clamp(const Size<T>& size, const Size<T>& low, const Size<T>& high)
{
	return Size<T>(std::clamp(size.width, low.width, high.width), std::clamp(size.height, low.height, high.height));
}

} // namespace templates

using Size = templates::Size<int>;
using SizeArg = templates::Size<int>::ConstArg;
using SizeResult = templates::Size<int>::ConstResult;

using SizeF = templates::Size<float>;
using SizeFArg = templates::Size<float>::ConstArg;
using SizeFResult = templates::Size<float>::ConstResult;

using SizeD = templates::Size<double>;
using SizeDArg = templates::Size<double>::ConstArg;
using SizeDResult = templates::Size<double>::ConstResult;

} // namespace imaging

namespace std {

template<size_t I, typename T>
struct tuple_element;

template<size_t I, typename T>
struct tuple_element<I, ::imaging::templates::Size<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size;

template<typename T>
struct tuple_size<::imaging::templates::Size<T>> : integral_constant<size_t, 2>
{
};

template<typename T>
struct hash;

template<typename T>
struct hash<::imaging::templates::Size<T>>
{
	size_t operator()(const ::imaging::templates::Size<T>& size) const noexcept
	{
		hash<T> hasher;
		size_t seed = hasher(size.width) + 0x9e3779b9;
		seed ^= hasher(size.height) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

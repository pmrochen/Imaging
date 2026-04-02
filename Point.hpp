/*
 *	Name: Point
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
#include "Size.hpp" // for Uninitialized, Arithmetic, Constants

namespace imaging {
namespace templates {

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
struct Point
{
	using ComponentType = T;
	using ConstArg = const Point&;
	using ConstResult = const Point&;

	static constexpr int NUM_COMPONENTS = 2;

	constexpr Point() noexcept : x(), y() {}
	explicit Point(Uninitialized) noexcept {}
	constexpr Point(T x, T y) noexcept : x(x), y(y) {}
	explicit Point(const std::pair<T, T>& t) noexcept : x(t.first), y(t.second) {}
	template<Arithmetic U> explicit Point(const std::pair<U, U>& t) noexcept : x(T(t.first)), y(T(t.second)) {}
	explicit Point(const std::tuple<T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)) {}
	template<Arithmetic U> explicit Point(const std::tuple<U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))) {}
	template<Arithmetic U> explicit Point(const Point<U>& point) noexcept : x(T(point.x)), y(T(point.y)) {}

	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

	Point operator+() const noexcept { return *this; }
	Point operator-() const noexcept { return Point(-x, -y); }
	Point& operator+=(const Size& size) noexcept { x += size.width; y += size.height; return *this; }
	Point& operator-=(const Size& size) noexcept { x -= size.width; y -= size.height; return *this; }
	Point& operator*=(const Size& size) noexcept { x *= size.width; y *= size.height; return *this; }
	Point& operator*=(T f) noexcept { x *= f; y *= f; return *this; }
	Point& operator/=(const Size& size) noexcept { x /= size.width; y /= size.height; return *this; }
	Point& operator/=(T f) noexcept;
	bool operator==(const Point& point) const noexcept { return (x == point.x) && (y == point.y); }
	bool operator!=(const Point& point) const noexcept { return !(*this == point); }
	
	template<typename A> void serialize(A& ar) { ar(x, y); }

	template<std::size_t I> T& get() noexcept;
	template<std::size_t I> const T& get() const noexcept;

	bool isZero() const noexcept { return (x == T()) && (y == T()); }
	bool isApproxZero() const noexcept requires std::floating_point<T>;
	bool approxEquals(const Point& point) const noexcept requires std::floating_point<T>;
	bool approxEquals(const Point& point, T tolerance) const noexcept requires std::floating_point<T>;
	bool allLessThan(const Point& point) const noexcept { return (x < point.x) && (y < point.y); }
	bool allLessThanEqual(const Point& point) const noexcept { return (x <= point.x) && (y <= point.y); }
	bool allGreaterThan(const Point& point) const noexcept { return (x > point.x) && (y > point.y); }
	bool allGreaterThanEqual(const Point& point) const noexcept { return (x >= point.x) && (y >= point.y); }
	bool anyLessThan(const Point& point) const noexcept { return (x < point.x) || (y < point.y); }
	bool anyLessThanEqual(const Point& point) const noexcept { return (x <= point.x) || (y <= point.y); }
	bool anyGreaterThan(const Point& point) const noexcept { return (x > point.x) || (y > point.y); }
	bool anyGreaterThanEqual(const Point& point) const noexcept { return (x >= point.x) || (y >= point.y); }
	bool isFinite() const noexcept requires std::floating_point<T>;
	Point& setZero() noexcept { x = T(); y = T(); return *this; }
	Point& set(T x, T y) noexcept { this->x = x; this->y = y; return *this; }
	Point& setMinimum(const Point& point1, const Point& point2);
	Point& setMaximum(const Point& point1, const Point& point2);

	static const Point ZERO;

	T x;
	T y;
};

template<typename T> const Point<T> Point<T>::ZERO{};

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> operator+(const Point<T>& point1, const Point<T>& point2) noexcept
{
	return Size<T>(point1.x + point2.x, point1.y + point2.y);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator+(const Size<T>& size, const Point<T>& point) noexcept
{
	return Point<T>(size.width + point.x, size.height + point.y);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator+(const Point<T>& point, const Size<T>& size) noexcept
{
	return Point<T>(point.x + size.width, point.y + size.height);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Size<T> operator-(const Point<T>& point1, const Point<T>& point2) noexcept
{
	return Size<T>(point1.x - point2.x, point1.y - point2.y);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator-(const Size<T>& size, const Point<T>& point) noexcept
{
	return Point<T>(size.width - point.x, size.height - point.y);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator-(const Point<T>& point, const Size<T>& size) noexcept
{
	return Point<T>(point.x - size.width, point.y - size.height);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator*(const Size<T>& size, const Point<T>& point) noexcept
{
	return Point<T>(size.width*point.x, size.height*point.y);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator*(const Point<T>& point, const Size<T>& size) noexcept
{
	return Point<T>(point.x*size.width, point.y*size.height);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator*(T f, const Point<T>& point) noexcept
{
	return Point<T>(f*point.x, f*point.y);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator*(const Point<T>& point, T f) noexcept
{
	return Point<T>(point.x*f, point.y*f);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator/(const Size<T>& size, const Point<T>& point) noexcept
{
	return Point<T>(size.width/point.x, size.height/point.y);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator/(const Point<T>& point, const Size<T>& size) noexcept
{
	return Point<T>(point.x/size.width, point.y/size.height);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator/(T f, const Point<T>& point) noexcept
{
	return Point<T>(f/point.x, f/point.y);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> operator/(const Point<T>& point, T f) noexcept
{
	if constexpr (std::is_floating_point_v<T>)
		return operator*(point, T(1)/f);
	else
		return Point<T>(point.x/f, point.y/f);
}

template<typename T>
inline Point<T>& Point<T>::operator/=(T f) 
{ 
	if constexpr (std::is_floating_point_v<T>)
	{
		return operator*=(T(1)/f);
	}
	else
	{
		x /= f;
		y /= f;
		return *this;
	}
}

template<typename C, typename T, typename U>
	requires std::floating_point<U> || std::integral<U>
inline std::basic_istream<C, T>& operator>>(std::basic_istream<C, T>& s, Point<U>& point)
{ 
	return s >> point.x >> std::ws >> point.y;
}

template<typename C, typename T, typename U>
	requires std::floating_point<U> || std::integral<U>
inline std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& s, const Point<U>& point)
{ 
	constexpr C WS(0x20);
	return s << point.x << WS << point.y;
}

template<typename T>
template<std::size_t I>
inline T& Point<T>::get()
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	static_assert(false);
}

template<typename T>
template<std::size_t I>
inline const T& Point<T>::get() const
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	static_assert(false);
}

template<typename T>
inline bool Point<T>::isApproxZero() const noexcept requires std::floating_point<T>
{ 
	return (std::fabs(x) < Constants<T>::TOLERANCE) && (std::fabs(y) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Point<T>::approxEquals(const Point<T>& point) const noexcept requires std::floating_point<T>
{ 
	return (std::fabs(point.x - x) < Constants<T>::TOLERANCE) && (std::fabs(point.y - y) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Point<T>::approxEquals(const Point<T>& point, T tolerance) const noexcept requires std::floating_point<T>
{ 
	return (std::fabs(point.x - x) < tolerance) && (std::fabs(point.y - y) < tolerance);
}

template<typename T>
inline bool Point<T>::isFinite() const noexcept requires std::floating_point<T> 
{ 
	return std::isfinite(x) && std::isfinite(y); 
}

template<typename T>
inline Point<T>& Point<T>::setMinimum(const Point<T>& point1, const Point<T>& point2)
{
	x = std::min(point1.x, point2.x);
	y = std::min(point1.y, point2.y);
	return *this;
}

template<typename T>
inline Point<T>& Point<T>::setMaximum(const Point<T>& point1, const Point<T>& point2)
{
	x = std::max(point1.x, point2.x);
	y = std::max(point1.y, point2.y);
	return *this;
}

template<std::size_t I, typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline T& get(Point<T>& point) noexcept
{
	if constexpr (I == 0)
		return point.x;
	else if constexpr (I == 1)
		return point.y;
	static_assert(false);
}

template<std::size_t I, typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline const T& get(const Point<T>& point) noexcept
{
	if constexpr (I == 0)
		return point.x;
	else if constexpr (I == 1)
		return point.y;
	static_assert(false);
}

template<std::size_t I, typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline T&& get(Point<T>&& point) noexcept
{
	if constexpr (I == 0)
		return point.x;
	else if constexpr (I == 1)
		return point.y;
	static_assert(false);
}

template<std::size_t I, typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline const T&& get(const Point<T>&& point) noexcept
{
	if constexpr (I == 0)
		return point.x;
	else if constexpr (I == 1)
		return point.y;
	static_assert(false);
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> min(const Point<T>& point1, const Point<T>& point2)
{
	return Point<T>(std::min(point1.x, point2.x), std::min(point1.y, point2.y));
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> max(const Point<T>& point1, const Point<T>& point2)
{
	return Point<T>(std::max(point1.x, point2.x), std::max(point1.y, point2.y));
}

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
inline Point<T> clamp(const Point<T>& point, const Point<T>& low, const Point<T>& high)
{
	return Point<T>(std::clamp(point.x, low.x, high.x), std::clamp(point.y, low.y, high.y));
}

} // namespace templates

using Point = templates::Point<int>;
using PointArg = templates::Point<int>::ConstArg;
using PointResult = templates::Point<int>::ConstResult;

using PointF = templates::Point<float>;
using PointFArg = templates::Point<float>::ConstArg;
using PointFResult = templates::Point<float>::ConstResult;

using PointD = templates::Point<double>;
using PointDArg = templates::Point<double>::ConstArg;
using PointDResult = templates::Point<double>::ConstResult;

} // namespace imaging

namespace std {

template<size_t I, typename T>
struct tuple_element;

template<size_t I, typename T>
struct tuple_element<I, ::imaging::templates::Point<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size;

template<typename T>
struct tuple_size<::imaging::templates::Point<T>> : integral_constant<size_t, 2>
{
};

template<typename T>
struct hash;

template<typename T>
struct hash<::imaging::templates::Point<T>>
{
	size_t operator()(const ::imaging::templates::Point<T>& point) const noexcept
	{
		hash<T> hasher;
		size_t seed = hasher(point.x) + 0x9e3779b9;
		seed ^= hasher(point.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

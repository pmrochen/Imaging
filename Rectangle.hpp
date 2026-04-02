/*
 *	Name: Rectangle
 *	Author: Pawel Mrochen
 */

#pragma once

#include <istream>
#include <ostream>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <functional>
#include <cstddef>
#include <cmath>
#include "Color3.hpp" // for Uninitialized, Arithmetic, Constants

namespace imaging {
namespace templates {

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
struct Rectangle
{
	using ComponentType = T;
	using ConstArg = const Rectangle&;
	using ConstResult = const Rectangle&;

	constexpr Rectangle() noexcept : x(), y(), width(), height() {}
	explicit Rectangle(Uninitialized) noexcept {}
	constexpr Rectangle(T x, T y, T width, T height) noexcept : x(x), y(y), width(width), height(height) {}
	template<Arithmetic U> explicit Rectangle(const Rectangle<U>& rect) noexcept;

	bool operator==(const Rectangle& rect) const noexcept;
	bool operator!=(const Rectangle& rect) const noexcept { return !(*this == rect); }
	
	template<typename A> void serialize(A& ar) { ar(x, y, width, height); }

	bool isEmpty() const noexcept { return /*(x == T()) && (y == T()) &&*/ (width == T()) && (height == T()); }
	bool isZero() const noexcept { return (x == T()) && (y == T()) && (width == T()) && (height == T()); }
	bool isApproxZero() const noexcept requires std::floating_point<T>;
	bool approxEquals(const Rectangle& rect) const noexcept requires std::floating_point<T>;
	bool approxEquals(const Rectangle& rect, T tolerance) const noexcept requires std::floating_point<T>;
	bool isFinite() const noexcept requires std::floating_point<T>;
	T getX() const noexcept { return x; }
	void setX(T x) noexcept { this->x = x; }
	T getY() const noexcept { return y; }
	void setY(T y) noexcept { this->y = y; }
	T getWidth() const noexcept { return width; }
	void setWidth(T width) noexcept { this->width = width; }
	T getHeight() const noexcept { return height; }
	void setHeight(T height) noexcept { this->height = height; }
	T getLeft() const noexcept { return x; }
	T getTop() const noexcept { return y; }
	T getRight() const noexcept { return (x + width); }
	T getBottom() const noexcept { return (y + height); }
	Rectangle& setLocation(T x, T y) noexcept { this->x = x; this->y = y; return *this; }
	Rectangle& setSize(T width, T height) noexcept { this->width = width; this->height = height; return *this; }
	Rectangle& setZero() noexcept { x = T(); y = T(); width = T(); height = T(); return *this; }
	Rectangle& set(T x, T y, T width, T height) noexcept;
	Rectangle& setLeftTopRightBottom(T left, T top, T right, T bottom) noexcept;
	template<std::floating_point U /*= T*/> U getAspectRatio() const noexcept;
	Rectangle& inflate(T size) noexcept;
	Rectangle& inflate(T width, T height) noexcept;
	Rectangle& translate(T x, T y) noexcept { this->x += x; this->y += y; return *this; }
	Rectangle& setUnion(const Rectangle& a, const Rectangle& b) noexcept;
	Rectangle& setIntersection(const Rectangle& a, const Rectangle& b) noexcept;
	static Rectangle makeUnion(const Rectangle& a, const Rectangle& b) noexcept;
	static Rectangle makeIntersection(const Rectangle& a, const Rectangle& b) noexcept;
	bool contains(T x, T y) const noexcept;
	bool contains(const Rectangle& rect) const noexcept;
	bool intersects(const Rectangle& rect) const noexcept;

	static const Rectangle EMPTY;

	T x;
	T y;
	T width;
	T height;
};

template<typename T> const Rectangle<T> Rectangle<T>::EMPTY{};

template<typename T>
template<Arithmetic U>
inline Rectangle<T>::Rectangle(const Rectangle<U>& rect) : 
	x(T(rect.x)), 
	y(T(rect.y)), 
	width(T(rect.width)), 
	height(T(rect.height)) 
{
}

template<typename T>
inline bool Rectangle<T>::operator==(const Rectangle& rect) const 
{ 
	return (x == rect.x) && (y == rect.y) && (width == rect.width) && (height == rect.height); 
}

template<typename C, typename T, typename U>
	requires std::floating_point<U> || std::integral<U>
inline std::basic_istream<C, T>& operator>>(std::basic_istream<C, T>& s, Rectangle<U>& rect)
{ 
	return s >> rect.x >> std::ws >> rect.y >> std::ws >> rect.width >> std::ws >> rect.height;
}

template<typename C, typename T, typename U>
	requires std::floating_point<U> || std::integral<U>
inline std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& s, const Rectangle<U>& rect)
{ 
	constexpr C WS(0x20);
	return s << rect.x << WS << rect.y << WS << rect.width << WS << rect.height;
}

template<typename T>
inline bool Rectangle<T>::isApproxZero() const noexcept requires std::floating_point<T>
{ 
	return (std::fabs(x) < Constants<T>::TOLERANCE) && (std::fabs(y) < Constants<T>::TOLERANCE) && 
		(std::fabs(width) < Constants<T>::TOLERANCE) && (std::fabs(height) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Rectangle<T>::approxEquals(const Rectangle<T>& rect) const noexcept requires std::floating_point<T>
{ 
	return (std::fabs(rect.x - x) < Constants<T>::TOLERANCE) && (std::fabs(rect.y - y) < Constants<T>::TOLERANCE) &&
		(std::fabs(rect.width - width) < Constants<T>::TOLERANCE) && (std::fabs(rect.height - height) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Rectangle<T>::approxEquals(const Rectangle<T>& rect, T tolerance) const noexcept requires std::floating_point<T>
{ 
	return (std::fabs(rect.x - x) < tolerance) && (std::fabs(rect.y - y) < tolerance) &&
		(std::fabs(rect.width - width) < tolerance) && (std::fabs(rect.height - height) < tolerance);
}

template<typename T>
inline bool Rectangle<T>::isFinite() const noexcept requires std::floating_point<T> 
{ 
	return std::isfinite(x) && std::isfinite(y) && std::isfinite(width) && std::isfinite(height); 
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::set(T x, T y, T width, T height)
{ 
	this->x = x; 
	this->y = y; 
	this->width = width; 
	this->height = height; 
	return *this; 
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::setLeftTopRightBottom(T left, T top, T right, T bottom)
{ 
	x = left; 
	y = top; 
	width = right - left; 
	height = bottom - top; 
	return *this; 
}

template<typename T>
template<std::floating_point U>
inline U Rectangle<T>::getAspectRatio() const
{ 
	return (height != T(0)) ? U((double)width/(double)height) : U(); 
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::inflate(T size) 
{ 
	x -= size; 
	y -= size; 
	this->width += T(2)*size; 
	this->height += T(2)*size; 
	return *this; 
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::inflate(T width, T height) 
{ 
	x -= width; 
	y -= height; 
	this->width += T(2)*width; 
	this->height += T(2)*height; 
	return *this; 
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::setUnion(const Rectangle<T>& a, const Rectangle<T>& b)
{
	T x1 = std::min(a.x, b.x);
	T x2 = std::max(a.x + a.width, b.x + b.width);
	T y1 = std::min(a.y, b.y);
	T y2 = std::max(a.y + a.height, b.y + b.height);

	x = x1;
	y = y1;
	width = x2 - x1;
	height = y2 - y1;
	return *this;
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::setIntersection(const Rectangle<T>& a, const Rectangle<T>& b)
{
	T x1 = std::max(a.x, b.x);
	T x2 = std::min(a.x + a.width, b.x + b.width);
	T y1 = std::max(a.y, b.y);
	T y2 = std::min(a.y + a.height, b.y + b.height);

	if ((x2 >= x1) && (y2 >= y1))
	{
		x = x1;
		y = y1;
		width = x2 - x1;
		height = y2 - y1;
	}
	else
	{
		x = T();
		y = T();
		width = T();
		height = T();
	}

	return *this;
}

template<typename T>
/*static*/ inline Rectangle<T> Rectangle<T>::makeUnion(const Rectangle<T>& a, const Rectangle<T>& b)
{ 
	return Rectangle(Uninitialized()).setUnion(a, b);
}

template<typename T>
/*static*/ inline Rectangle<T> Rectangle<T>::makeIntersection(const Rectangle<T>& a, const Rectangle<T>& b)
{ 
	return Rectangle(Uninitialized()).setIntersection(a, b);
}

template<typename T>
inline bool Rectangle<T>::contains(T x, T y) const
{
	return (this->x <= x) && (x < (this->x + width)) && (this->y <= y) && (y < (this->y + height));
}

template<typename T>
inline bool Rectangle<T>::contains(const Rectangle<T>& rect) const
{
	return (x <= rect.x) && ((rect.x + rect.width) <= (x + width)) && (y <= rect.y) && ((rect.y + rect.height) <= (y + height));
}

template<typename T>
inline bool Rectangle<T>::intersects(const Rectangle<T>& rect) const
{
	return (rect.x < (x + width)) && (x < (rect.x + rect.width)) && (rect.y < (y + height)) && (y < (rect.y + rect.height));
}

} // namespace templates

using Rectangle = templates::Rectangle<int>;
using RectangleArg = templates::Rectangle<int>::ConstArg;
using RectangleResult = templates::Rectangle<int>::ConstResult;

} // namespace imaging

namespace std {

template<typename T>
struct hash;

template<typename T>
struct hash<::imaging::templates::Rectangle<T>>
{
	size_t operator()(const ::imaging::templates::Rectangle<T>& rect) const noexcept
	{
		hash<T> hasher;
		size_t seed = hasher(rect.x) + 0x9e3779b9;
		seed ^= hasher(rect.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(rect.width) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(rect.height) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

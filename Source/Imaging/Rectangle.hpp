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
#include "Point.hpp"
#include "Size.hpp"

namespace imaging {
namespace templates {

template<typename T>
	requires (std::floating_point<T> || std::integral<T>)
struct Rectangle
{
	using ComponentType = T;
	using ConstArg = const Rectangle&;
	using ConstResult = const Rectangle&;

	Rectangle() = default;
	explicit Rectangle(Uninitialized) noexcept : location(Uninitialized()), size(Uninitialized()) {}
	constexpr Rectangle(const Point<T>& location, const Size<T>& size) noexcept : location(location), size(size) {}
	constexpr explicit Rectangle(const Size<T>& size) noexcept : location(), size(size) {}
	constexpr Rectangle(T x, T y, T width, T height) noexcept : location(x, y), size(width, height) {}
	constexpr Rectangle(T width, T height) noexcept : location(), size(width, height) {}
	template<Arithmetic U> explicit Rectangle(const Rectangle<U>& rectangle) noexcept;

	bool operator==(const Rectangle& rectangle) const noexcept { return (location == rectangle.location) && (size == rectangle.size); }
	bool operator!=(const Rectangle& rectangle) const noexcept { return !(*this == rectangle); }
	
	template<typename A> void serialize(A& ar) { ar(location, size); }

	static Rectangle fromMinimumMaximum(const Point<T>& minimum, const Point<T>& maximum) noexcept;
	static Rectangle fromLeftTopRightBottom(T left, T top, T right, T bottom) noexcept;

	bool isEmpty() const noexcept { return size.isZero(); }
	bool isZero() const noexcept { return location.isZero() && size.isZero(); }
	bool isApproxZero() const noexcept requires std::floating_point<T>;
	bool approxEquals(const Rectangle& rectangle) const noexcept requires std::floating_point<T>;
	bool approxEquals(const Rectangle& rectangle, T tolerance) const noexcept requires std::floating_point<T>;
	bool isFinite() const noexcept requires std::floating_point<T>;
	Rectangle& setZero() noexcept { location.setZero(); size.setZero(); return *this; }
	Rectangle& set(const Point<T>& location, const Size<T>& size) noexcept { this->location = location; this->size = size; return *this; }
	Rectangle& set(T x, T y, T width, T height) noexcept { location.set(x, y); size.set(width, height); return *this; }
	const Point<T>& getLocation() const noexcept { return location; }
	void setLocation(const Point<T>& location) noexcept { this->location = location; }
	void setLocation(T x, T y) noexcept { location.set(x, y); }
	const Size<T>& getSize() const noexcept { return size; }
	void setSize(const Size<T>& size) noexcept { this->size = size; }
	void setSize(T width, T height) noexcept { size.set(width, height); }
	const Point<T>& getMinimum() const noexcept { return location; }
	Point<T> getMaximum() const noexcept { return location + size; }
	T getX() const noexcept { return location.x; }
	void setX(T x) noexcept { location.x = x; }
	T getY() const noexcept { return location.y; }
	void setY(T y) noexcept { location.y = y; }
	T getWidth() const noexcept { return size.width; }
	void setWidth(T width) noexcept { size.width = width; }
	T getHeight() const noexcept { return size.height; }
	void setHeight(T height) noexcept { size.height = height; }
	T getLeft() const noexcept { return location.x; }
	T getTop() const noexcept { return location.y; }
	T getRight() const noexcept { return location.x + size.width; }
	T getBottom() const noexcept { return location.y + size.height; }
	template<std::floating_point U /*= T*/> U getAspectRatio() const noexcept { return size.getAspectRatio<U>(); }
	Rectangle& inflate(const Size<T>& size) noexcept;
	Rectangle& inflate(T size) noexcept;
	Rectangle& inflate(T width, T height) noexcept;
	Rectangle& translate(const Point<T>& offset) noexcept { location += offset; return *this; }
	Rectangle& translate(T x, T y) noexcept { location.set(location.x + x, location.y + y); return *this; }
	Rectangle& setUnion(const Rectangle& a, const Rectangle& b) noexcept;
	Rectangle& setIntersection(const Rectangle& a, const Rectangle& b) noexcept;
	static Rectangle makeUnion(const Rectangle& a, const Rectangle& b) noexcept;
	static Rectangle makeIntersection(const Rectangle& a, const Rectangle& b) noexcept;
	bool contains(const Point<T>& point) const noexcept;
	bool contains(const Rectangle& rectangle) const noexcept;
	bool intersects(const Rectangle& rectangle) const noexcept;

	static const Rectangle EMPTY;

	Point<T> location;
	Size<T> size;
};

template<typename T> const Rectangle<T> Rectangle<T>::EMPTY{};

template<typename T>
template<Arithmetic U>
inline Rectangle<T>::Rectangle(const Rectangle<U>& rectangle) : 
	location(T(rectangle.x), T(rectangle.y)), 
	size(T(rectangle.width), T(rectangle.height)) 
{
}

template<typename C, typename T, typename U>
	requires std::floating_point<U> || std::integral<U>
inline std::basic_istream<C, T>& operator>>(std::basic_istream<C, T>& s, Rectangle<U>& rectangle)
{ 
	return s >> rectangle.location >> std::ws >> rectangle.size;
}

template<typename C, typename T, typename U>
	requires std::floating_point<U> || std::integral<U>
inline std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& s, const Rectangle<U>& rectangle)
{ 
	constexpr C WS(0x20);
	return s << rectangle.location << WS << rectangle.size;
}

template<typename T>
/*static*/ inline Rectangle<T> Rectangle<T>::fromMinimumMaximum(const Point<T>& minimum, const Point<T>& maximum)
{
	return Rectangle<T>(minimum, maximum - minimum);
}

template<typename T>
/*static*/ inline Rectangle<T> Rectangle<T>::fromLeftTopRightBottom(T left, T top, T right, T bottom)
{ 
	return Rectangle<T>(left, top, right - left, bottom - top);
}

template<typename T>
inline bool Rectangle<T>::isApproxZero() const noexcept requires std::floating_point<T>
{ 
	return location.isApproxZero() && size.isApproxZero();
}

template<typename T>
inline bool Rectangle<T>::approxEquals(const Rectangle<T>& rectangle) const noexcept requires std::floating_point<T>
{ 
	return location.approxEquals(rectangle.location) && size.approxEquals(rectangle.size);
}

template<typename T>
inline bool Rectangle<T>::approxEquals(const Rectangle<T>& rectangle, T tolerance) const noexcept requires std::floating_point<T>
{ 
	return location.approxEquals(rectangle.location, tolerance) && size.approxEquals(rectangle.size, tolerance);
}

template<typename T>
inline bool Rectangle<T>::isFinite() const noexcept requires std::floating_point<T> 
{ 
	return location.isFinite() && size.isFinite(); 
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::inflate(const Size<T>& size)
{
	return set(location - size, this->size + size + size);
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::inflate(T size) 
{ 
	return set(location.x - size, location.y - size, this->size.width + size + size, this->size.height + size + size);
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::inflate(T width, T height) 
{ 
	return set(location.x - width, location.y - height, size.width + width + width, size.height + height + height);
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::setUnion(const Rectangle<T>& a, const Rectangle<T>& b)
{
	Point<T> minimum = min(a.getMinimum(), b.getMinimum());
	Point<T> maximum = max(a.getMaximum(), b.getMaximum());
	return set(minimum, maximum - minimum);
}

template<typename T>
inline Rectangle<T>& Rectangle<T>::setIntersection(const Rectangle<T>& a, const Rectangle<T>& b)
{
	Point<T> minimum = max(a.getMinimum(), b.getMinimum());
	Point<T> maximum = min(a.getMaximum(), b.getMaximum());
	return set(minimum, max(maximum - minimum, Size<T>::ZERO));
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
inline bool Rectangle<T>::contains(const Point<T>& point) const
{
	return getMinimum().allLessThanEqual(point) && getMaximum().allGreaterThan(point);
}

template<typename T>
inline bool Rectangle<T>::contains(const Rectangle<T>& rectangle) const
{
	return getMinimum().allLessThanEqual(rectangle.getMinimum()) && maximum.allGreaterThanEqual(rectangle.getMaximum());
}

template<typename T>
inline bool Rectangle<T>::intersects(const Rectangle<T>& rectangle) const
{
	return getMinimum().allLessThan(rectangle.getMaximum()) && maximum.allGreaterThan(rectangle.getMinimum());
}

} // namespace templates

using Rectangle = templates::Rectangle<int>;
using RectangleArg = templates::Rectangle<int>::ConstArg;
using RectangleResult = templates::Rectangle<int>::ConstResult;

using RectangleF = templates::Rectangle<float>;
using RectangleFArg = templates::Rectangle<float>::ConstArg;
using RectangleFResult = templates::Rectangle<float>::ConstResult;

using RectangleD = templates::Rectangle<double>;
using RectangleDArg = templates::Rectangle<double>::ConstArg;
using RectangleDResult = templates::Rectangle<double>::ConstResult;

} // namespace imaging

namespace std {

template<typename T>
struct hash;

template<typename T>
struct hash<::imaging::templates::Rectangle<T>>
{
	size_t operator()(const ::imaging::templates::Rectangle<T>& rectangle) const noexcept
	{
		size_t seed = hash<typename ::imaging::templates::Point<T>>()(rectangle.location) + 0x9e3779b9;
		seed ^= hash<typename ::imaging::templates::Size<T>>()(rectangle.size) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

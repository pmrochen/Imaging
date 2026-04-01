/*
 *	Name: Pixel
 *	Author: Pawel Mrochen
 */

#pragma once

#include <type_traits>
#include <limits>
#include <algorithm>
#include <cstdint>

#ifndef HALF_ENABLE_CPP11_CONSTEXPR
#define HALF_ENABLE_CPP11_CONSTEXPR 1
#endif
#ifndef HALF_ENABLE_CPP11_NOEXCEPT
#define HALF_ENABLE_CPP11_NOEXCEPT 1
#endif
#ifndef HALF_ARITHMETIC_TYPE
#define HALF_ARITHMETIC_TYPE float
#endif
#include <half.hpp>

namespace imaging::pixel {

template<typename Type, int Size, int RBits, int GBits, int BBits, int ABits = 0, bool Compressed = false>
struct Base
{
    using DataType = Type;

    static constexpr bool COMPRESSED = Compressed;
    static constexpr bool FLOATING_POINT = std::is_floating_point_v<Type>;
    static constexpr bool HAS_ALPHA = (ABits > 0);
	static constexpr int BIT_DEPTH = RBits + GBits + BBits + ABits;
	static constexpr int NUM_COMPONENTS = (int)(RBits > 0) + (int)(GBits > 0) + (int)(BBits > 0) + (int)(ABits > 0);
    static constexpr int SIZE = Size;
    static constexpr int SIZE_BITS = Size*8;
    static constexpr int R_BITS = RBits;
	static constexpr int G_BITS = GBits;
	static constexpr int B_BITS = BBits;
	static constexpr int A_BITS = ABits;
	static constexpr int R_MAX = (int)((1ll << RBits) - 1ll);
	static constexpr int G_MAX = (int)((1ll << GBits) - 1ll);
	static constexpr int B_MAX = (int)((1ll << BBits) - 1ll);
	static constexpr int A_MAX = (int)((1ll << ABits) - 1ll);
};

template<typename Type, int NComponents>
struct Sequential : public Base<Type, sizeof(Type)*NComponents, (NComponents > 0) ? sizeof(Type)*8 : 0,
    (NComponents > 1) ? sizeof(Type)*8 : 0, (NComponents > 2) ? sizeof(Type)*8 : 0, (NComponents > 3) ? sizeof(Type)*8 : 0>
{
    using ComponentType = Type;
};

template<typename Type, int RBits, int GBits, int BBits, int ABits, int RShift, int GShift, int BShift, int AShift = 0>
struct Packed : public Base<Type, sizeof(Type), RBits, GBits, BBits, ABits>
{
    using PackedType = Type;

    //static constexpr bool PACKED_PIXEL = !std::is_void_v<Type>;
    static constexpr bool PACKED_PIXEL = (RShift > 0) || (GShift > 0) || (BShift > 0) || (AShift > 0);
	static constexpr int R_SHIFT = RShift;
	static constexpr int G_SHIFT = GShift;
	static constexpr int B_SHIFT = BShift;
	static constexpr int A_SHIFT = AShift;
	static constexpr int R_MASK = ((1 << RBits) - 1) << RShift;
	static constexpr int G_MASK = ((1 << GBits) - 1) << GShift;
	static constexpr int B_MASK = ((1 << BBits) - 1) << BShift;
	static constexpr int A_MASK = ((1 << ABits) - 1) << AShift;
};

template<typename Type, int RBits = sizeof(Type)*2, int GBits = sizeof(Type)*2, int BBits = sizeof(Type)*2>
struct PackedRgb : public Packed<Type, RBits, GBits, BBits, 0, 0, RBits, RBits + GBits> {};

template<typename Type, int RBits = sizeof(Type)*2, int GBits = sizeof(Type)*2, int BBits = sizeof(Type)*2, int ABits = sizeof(Type)*2>
struct PackedRgba : public Packed<Type, RBits, GBits, BBits, ABits, 0, RBits, RBits + GBits, RBits + GBits + BBits> {};

template<typename Type, int RBits = sizeof(Type)*2, int GBits = sizeof(Type)*2, int BBits = sizeof(Type)*2>
struct PackedBgr : public Packed<Type, RBits, GBits, BBits, 0, BBits + GBits, BBits, 0> {};

template<typename Type, int RBits = sizeof(Type)*2, int GBits = sizeof(Type)*2, int BBits = sizeof(Type)*2, int ABits = sizeof(Type)*2>
struct PackedBgra : public Packed<Type, RBits, GBits, BBits, ABits, BBits + GBits, BBits, 0, BBits + GBits + RBits> {};

struct Bgr4 : public PackedBgr<std::uint16_t, 4, 4, 4> {};
struct Bgra4 : public PackedBgra<std::uint16_t, 4, 4, 4, 4> {};
struct Bgr5 : public PackedBgr<std::uint16_t, 5, 5, 5> {};
struct Bgr5A1 : public PackedBgra<std::uint16_t, 5, 5, 5, 1> {};
struct B5G6R5 : public PackedBgr<std::uint16_t, 5, 6, 5> {};

struct R8 : public Sequential<std::uint8_t, 1> {};
struct Rg8 : public Sequential<std::uint8_t, 2> {};
struct Rgb8 : public PackedRgb<std::uint32_t, 8, 8, 8> {};
struct Rgba8 : public PackedRgba<std::uint32_t, 8, 8, 8, 8> {};
struct Bgr8 : public PackedBgr<std::uint32_t, 8, 8, 8> {};
struct Bgra8 : public PackedBgra<std::uint32_t, 8, 8, 8, 8> {};
struct Rgb10A2 : public PackedRgba<std::uint32_t, 10, 10, 10, 2> {};

struct R16 : public Sequential<std::uint16_t, 1> {};
struct Rg16 : public Sequential<std::uint16_t, 2> {};
struct Rgba16 : public Sequential<std::uint16_t, 4> {};

struct R32 : public Sequential<std::uint32_t, 1> {};
struct Rg32 : public Sequential<std::uint32_t, 2> {};
struct Rgb32 : public Sequential<std::uint32_t, 3> {};
struct Rgba32 : public Sequential<std::uint32_t, 4> {};

struct R8i : public Sequential<std::int8_t, 1> {};
struct Rg8i : public Sequential<std::int8_t, 2> {};
//struct Rgba8i : public PackedRgba<std::int32_t, 8, 8, 8, 8> {}; // #TODO

struct R16i : public Sequential<std::int16_t, 1> {};
struct Rg16i : public Sequential<std::int16_t, 2> {};
struct Rgba16i : public Sequential<std::int16_t, 4> {};

struct R32i : public Sequential<std::int32_t, 1> {};
struct Rg32i : public Sequential<std::int32_t, 2> {};
struct Rgb32i : public Sequential<std::int32_t, 3> {};
struct Rgba32i : public Sequential<std::int32_t, 4> {};

struct R16f : public Sequential<half_float::half, 1> {};
struct Rg16f : public Sequential<half_float::half, 2> {};
struct Rgba16f : public Sequential<half_float::half, 4> {};

struct R32f : public Sequential<float, 1> {};
struct Rg32f : public Sequential<float, 2> {};
struct Rgb32f : public Sequential<float, 3> {};
struct Rgba32f : public Sequential<float, 4> {};

template<typename TResult, bool Scale, int MaxValue, typename TSource>
inline TResult convert(TSource x)
{
    static_assert(std::is_integral_v<TResult> && (std::is_integral_v<TSource> || std::is_floating_point_v<TSource>));
    if constexpr (!Scale || (!std::is_floating_point_v<TSource> && std::is_unsigned_v<TSource> && ((int)std::numeric_limits<TSource>::max() == MaxValue)))
        return TResult(x);
    else if constexpr (std::is_floating_point_v<TSource>)
        return TResult(std::clamp(int(TSource(0.5) + x*TSource(MaxValue)), 0, MaxValue));
    else if constexpr (std::is_unsigned_v<TSource>)
        return std::min(TResult(x), TResult(MaxValue));
    else    
        return TResult(std::clamp(x, TSource(0), TSource(MaxValue))); // #FIXME
}

template<typename TPixel, bool Scale = false, typename TComponent>
inline typename TPixel::PackedType pack(TComponent r, TComponent g, TComponent b) noexcept
{
    static_assert(TPixel::R_BITS > 0);
    typename TPixel::PackedType v;
    if constexpr (TPixel::R_SHIFT > 0)
        v = convert<typename TPixel::PackedType, Scale, TPixel::R_MAX>(r) << TPixel::R_SHIFT;
    else
        v = convert<typename TPixel::PackedType, Scale, TPixel::R_MAX>(r); 
    if constexpr (TPixel::G_BITS > 0)
        v |= convert<typename TPixel::PackedType, Scale, TPixel::G_MAX>(g) << TPixel::G_SHIFT;
    if constexpr ((TPixel::B_BITS > 0) && (TPixel::B_SHIFT > 0))
        v |= convert<typename TPixel::PackedType, Scale, TPixel::B_MAX>(b) << TPixel::B_SHIFT;
    else if constexpr (TPixel::B_BITS > 0)
        v |= convert<typename TPixel::PackedType, Scale, TPixel::B_MAX>(b);
    if constexpr (TPixel::A_BITS > 0)
        v |= TPixel::A_MASK;
    return v;
}

template<typename TPixel, bool Scale = false, typename TComponent>
inline typename TPixel::PackedType pack(TComponent r, TComponent g, TComponent b, TComponent a) noexcept
{
    static_assert(TPixel::R_BITS > 0);
    typename TPixel::PackedType v;
    if constexpr (TPixel::R_SHIFT > 0)
        v = convert<typename TPixel::PackedType, Scale, TPixel::R_MAX>(r) << TPixel::R_SHIFT;
    else
        v = convert<typename TPixel::PackedType, Scale, TPixel::R_MAX>(r); 
    if constexpr (TPixel::G_BITS > 0)
        v |= convert<typename TPixel::PackedType, Scale, TPixel::G_MAX>(g) << TPixel::G_SHIFT;
    if constexpr ((TPixel::B_BITS > 0) && (TPixel::B_SHIFT > 0))
        v |= convert<typename TPixel::PackedType, Scale, TPixel::B_MAX>(b) << TPixel::B_SHIFT;
    else if constexpr (TPixel::B_BITS > 0)
        v |= convert<typename TPixel::PackedType, Scale, TPixel::B_MAX>(b);
    if constexpr (TPixel::A_BITS > 0)
        v |= convert<typename TPixel::PackedType, Scale, TPixel::A_MAX>(a) << TPixel::A_SHIFT;
    return v;
}

template<typename TResult, typename TComponent, int NComponents, typename TPixel>
inline TResult unpack(typename TPixel::PackedType c) noexcept
{
    static_assert((TPixel::R_BITS > 0) && (NComponents > 0) && (NComponents <= 4));
    typename TPixel::PackedType r;
    if constexpr (TPixel::R_SHIFT > 0)
        r = (c >> TPixel::R_SHIFT) & TPixel::R_MAX;
    else
        r = c & TPixel::R_MAX;
    if constexpr (NComponents > 1)
    {
        typename TPixel::PackedType g;
        if constexpr (TPixel::G_BITS > 0)
            g = (c >> TPixel::G_SHIFT) & TPixel::G_MAX;
        else
            g = {};
        if constexpr (NComponents > 2)
        {
            typename TPixel::PackedType b;
            if constexpr ((TPixel::B_BITS > 0) && (TPixel::B_SHIFT > 0))
                b = (c >> TPixel::B_SHIFT) & TPixel::B_MAX;
            else if constexpr (TPixel::B_BITS > 0)
                b = c & TPixel::B_MAX;
            else
                b = {};
            if constexpr (NComponents > 3)
            {
                typename TPixel::PackedType a;
                if constexpr ((TPixel::A_BITS > 0) && std::is_unsigned_v<typename TPixel::PackedType> && ((TPixel::A_SHIFT + TPixel::A_BITS) == TPixel::SIZE_BITS))
                    a = c >> TPixel::A_SHIFT;
                else if constexpr (TPixel::A_BITS > 0)
                    a = (c >> TPixel::A_SHIFT) & TPixel::A_MAX;
                else 
                    a = TPixel::A_MAX;
                return TResult(TComponent(r), TComponent(g), TComponent(b), TComponent(a));
            }
            return TResult(TComponent(r), TComponent(g), TComponent(b));
        }
        return TResult(TComponent(r), TComponent(g));
    }
    return TResult(TComponent(r));
}

} // namespace imaging::pixel

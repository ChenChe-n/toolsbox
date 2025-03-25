#pragma once
#include <cstdint>
#include <cfloat>
#include <bit>
#include <type_traits>

#define __NAMESPACE_NAME__ ts

#define __MATH_PI__	3.1415926535897932384626433832795
#define __MATH_E__	2.7182818284590452353602874713527

// 字节类型
using byte						= unsigned char;	

// 有符号整型
using i8						= int8_t;
inline constexpr i8 i8_max		= INT8_MAX;
inline constexpr i8 i8_min		= INT8_MIN;
using i16						= int16_t;
inline constexpr  i16 i16_max	= INT16_MAX;
inline constexpr  i16 i16_min	= INT16_MIN;
using i32						= int32_t;
inline constexpr  i32 i32_max	= INT32_MAX;
inline constexpr  i32 i32_min	= INT32_MIN;
using i64						= int64_t;
inline constexpr  i64 i64_max	= INT64_MAX;
inline constexpr  i64 i64_min	= INT64_MIN;

// 无符号整型
using u8						= uint8_t;
inline constexpr  u8 u8_max		= UINT8_MAX;
inline constexpr  u8 u8_min		= 0;
using u16						= uint16_t;
inline constexpr  u16 u16_max	= UINT16_MAX;
inline constexpr  u16 u16_min	= 0;
using u32						= uint32_t;
inline constexpr  u32 u32_max	= UINT32_MAX;
inline constexpr  u32 u32_min	= 0;
using u64						= uint64_t;
inline constexpr  u64 u64_max	= UINT64_MAX;
inline constexpr  u64 u64_min	= 0;

// 32bit浮点型
using f32						= float;
inline constexpr  f32 f32_max	= FLT_MAX;
inline constexpr  f32 f32_min	= FLT_MIN;
inline constexpr  f32 f32_inf	= std::bit_cast<f32>(u32(0x7f800000));
inline constexpr  f32 f32_nan	= std::bit_cast<f32>(u32(0x7fffffff));
inline constexpr  f32 f32_pi	= f32(__MATH_PI__);
inline constexpr  f32 f32_e		= f32(__MATH_E__);

// 64bit浮点型
using f64 = double;
inline constexpr  f64 f64_max	= DBL_MAX;
inline constexpr  f64 f64_min	= DBL_MIN;
inline constexpr  f64 f64_inf	= std::bit_cast<f64>(u64(0x7ff0000000000000));
inline constexpr  f64 f64_nan	= std::bit_cast<f64>(u64(0x7fffffffffffffff));
inline constexpr  f64 f64_pi	= f64(__MATH_PI__);
inline constexpr  f64 f64_e		= f64(__MATH_E__);


using size_t					= u64;
using st						= size_t;	

// 通用的带符号整数到无符号整数的转换函数
template <typename Signed, typename Unsigned>
inline constexpr  Unsigned signed_to_unsigned(Signed num) noexcept
{
	static_assert(std::is_signed_v<Signed>, "Signed must be a signed integer type");
	static_assert(std::is_unsigned_v<Unsigned>, "Unsigned must be an unsigned integer type");
	static_assert(sizeof(Signed) == sizeof(Unsigned), "Signed and Unsigned must have the same size");

	if (num >= 0)
	{
		return static_cast<Unsigned>(num);
	}
	return static_cast<Unsigned>(-num);
}

// 特化版本
inline constexpr  u8  i8_to_u8(i8 num)  noexcept { return signed_to_unsigned<i8, u8>(num); }
inline constexpr  u16 i16_to_u16(i16 num) noexcept { return signed_to_unsigned<i16, u16>(num); }
inline constexpr  u32 i32_to_u32(i32 num) noexcept { return signed_to_unsigned<i32, u32>(num); }
inline constexpr  u64 i64_to_u64(i64 num) noexcept { return signed_to_unsigned<i64, u64>(num); }

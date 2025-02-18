#pragma once
#include "using.hpp"

namespace num {
	// 十进制 (10^3) -> 千 (K)
	constexpr u64 K = 1000;
	// 十进制 (10^6) -> 百万 (M)
	constexpr u64 M = K * K;
	// 十进制 (10^9) -> 十亿 (B)
	constexpr u64 B = K * M;
	// 十进制 (10^12) -> 万亿 (T)
	constexpr u64 T = K * B;
	// 十进制 (10^15) -> 千万亿 (P)
	constexpr u64 P = K * T;
	// 十进制 (10^18) -> 百万亿 (E)
	constexpr u64 E = K * P;

	// 二进制 (2^10)
	constexpr u64 KB = 1024;
	// 二进制 (2^20)
	constexpr u64 MB = KB * KB;
	// 二进制 (2^30)
	constexpr u64 GB = KB * MB;
	// 二进制 (2^40)
	constexpr u64 TB = KB * GB;
	// 二进制 (2^50)
	constexpr u64 PB = KB * TB;
	// 二进制 (2^60)
	constexpr u64 EB = KB * PB;
}

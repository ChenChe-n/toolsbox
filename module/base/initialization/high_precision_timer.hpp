#pragma once

#include "../using.hpp"

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")

#define precision_timer_min 0.001
#define precision_timer_max 0.1
#define precision_timer_v1 0.001
#define precision_timer_v2 0.015625
#else
#define precision_timer_min 0.001
#define precision_timer_max 0.001
#define precision_timer_v1 0.001
#define precision_timer_v2 0.001
#endif

#define high_precision_timer_default_open 1 // 是否默认启用高精度定时器

class high_precision_timer
{
public:
	high_precision_timer() = default;
	~high_precision_timer() = default;

	// 启用高精度定时器
	static inline void enable(f64 seconds = precision_timer_v1)
	{
		// 限制精度范围
		if (seconds < precision_timer_min)
			seconds = precision_timer_min;
		if (seconds > precision_timer_max)
			seconds = precision_timer_max;

#ifdef _WIN32
		if (!enabled_)
		{
			timeBeginPeriod(static_cast<u32>(seconds * 1000));
			enabled_ = true;
			precision_timer_ = seconds;
		}
#endif
		return;
	}

	// 禁用高精度定时器
	static inline void disable()
	{
#ifdef _WIN32
		if (enabled_)
		{
			timeEndPeriod(static_cast<u32>(precision_timer_ * 1000));
			enabled_ = false;
		}
#endif
		return;
	}

private:
	// 调度器精度
	static inline f64 precision_timer_ = precision_timer_v2;
	// 记录高精度定时器是否已启用
	static inline bool enabled_ = []()
	{
#if high_precision_timer_default_open
			enable(precision_timer_v1)
				; return true;
#else
		return false;
#endif
	}();
};

#pragma once

#include "../base/using.hpp"
#include "../base/initialization/high_precision_timer.hpp"

#include <chrono>
#include <format>
#include <string>

namespace __NAMESPACE_NAME__
{
	class time
	{
	public:
		// 时间单位
		using days_ = std::chrono::duration<double, std::ratio<86400>>;
		// 小时
		using hours_ = std::chrono::duration<double, std::ratio<3600>>;
		// 分钟
		using minutes_ = std::chrono::duration<double, std::ratio<60>>;
		// 秒
		using seconds_ = std::chrono::duration<double>;
		// 毫秒
		using milliseconds_ = std::chrono::duration<double, std::milli>;
		// 微秒
		using microseconds_ = std::chrono::duration<double, std::micro>;
		// 纳秒
		using nanoseconds_ = std::chrono::duration<double, std::nano>;

		// 时间点类型
		using clock_ = std::chrono::high_resolution_clock;
		using time_point_ = std::chrono::time_point<clock_, seconds_>;

		// 获取当前时间点
		static inline time_point_ now() noexcept
		{
			return clock_::now();
		}

		// 获取程序运行时间（秒）
		static inline seconds_ elapsed_time() noexcept
		{
			return now() - program_start_time_;
		}

		// 获取程序启动时间点
		static inline time_point_ start_time() noexcept
		{
			return program_start_time_;
		}

		struct date
		{
			// utc 时区
			i16 utc_;
			// 年
			i16 year_;
			// 月
			i16 month_;
			// 日
			i16 day_;
			// 时
			i16 hour_;
			// 分
			i16 minute_;
			// 秒
			i16 second_;
			// 毫秒
			u16 milli_second_;
			// 微秒
			u16 micro_second_;
			// 纳秒
			u16 nano_second_;
		};

		static inline date get_current_date() {
			using namespace std::chrono;
			date d{};
		
			// 1. 获取当前时间点和本地时区
			auto sys_time = system_clock::now();
			auto local_zone = current_zone();
			zoned_time local_time{ local_zone, sys_time };
		
			// 2. 计算 UTC 时区偏移（小时）
			auto offset = local_time.get_info().offset;
			d.utc_ = static_cast<i16>(offset / 1h); // 转换为小时
		
			// 3. 分解日期（本地时间）
			auto local_days = floor<days>(local_time.get_local_time());
			year_month_day ymd{ local_days };
			d.year_  = static_cast<i16>(static_cast<int>(ymd.year()));
			d.month_ = static_cast<i16>(static_cast<unsigned>(ymd.month()));
			d.day_   = static_cast<i16>(static_cast<unsigned>(ymd.day()));
		
			// 4. 分解时间（包含亚秒部分）
			auto time_of_day = local_time.get_local_time() - local_days;
			auto sec = duration_cast<seconds>(time_of_day);
			time_of_day -= sec;
		
			d.hour_   = static_cast<i16>(duration_cast<hours>(sec).count() % 24);
			d.minute_ = static_cast<i16>(duration_cast<minutes>(sec).count() % 60);
			d.second_ = static_cast<i16>(sec.count() % 60);
		
			// 5. 分解毫秒、微秒、纳秒
			auto ms = duration_cast<milliseconds>(time_of_day);
			auto us = duration_cast<microseconds>(time_of_day - ms);
			auto ns = duration_cast<nanoseconds>(time_of_day - ms - us);
		
			d.milli_second_ = static_cast<u16>(ms.count());
			d.micro_second_ = static_cast<u16>(us.count());
			d.nano_second_  = static_cast<u16>(ns.count());
		
			return d;
		}
		time() = delete;
		~time() = delete;

	private:
		// 程序启动时间点
		static inline time_point_ program_start_time_ = now();
	};
}
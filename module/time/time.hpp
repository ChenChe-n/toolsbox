#pragma once

#include "../base/using.hpp"
#include "../base/size.hpp"
#include "../base/initialization/high_precision_timer.hpp"

#include <chrono>
#include <format>
#include <string>
#include <ctime>

namespace __NAMESPACE_NAME__
{
	class time
	{
	public:
		// 时间点类型
		using clock = std::chrono::high_resolution_clock;
		class date;
		class time_point;
		class duration;

		class duration
		{
		public:
			// 默认"0秒"构造函数
			constexpr inline duration() noexcept
			{
				duration_ = 0;
			}

			// 构造函数 chrono::duration
			template <typename Rep, typename Period>
			constexpr duration(const std::chrono::duration<Rep, Period> &dur) noexcept
			{
				duration_ = std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
			}
			// 构造函数 f64 seconds
			constexpr duration(f64 seconds) noexcept
			{
				duration_ = std::chrono::duration_cast<std::chrono::nanoseconds>(
								std::chrono::duration<f64>(seconds))
								.count();
			}
			// 构造函数 i64 nanoseconds
			constexpr duration(i64 nanoseconds) noexcept
			{
				duration_ = std::chrono::duration_cast<std::chrono::nanoseconds>(
								std::chrono::duration<i64, std::nano>(nanoseconds))
								.count();
			}
			// 获取纳秒
			constexpr inline i64 count()
			{
				return duration_;
			}

			// 从纳秒构造 i64 nanoseconds
			static constexpr inline duration from_nanoseconds(i64 nanoseconds) noexcept
			{
				return duration(nanoseconds);
			}
			// 从纳秒构造 f64 nanoseconds
			static constexpr inline duration from_nanoseconds(f64 nanoseconds) noexcept
			{
				return duration(nanoseconds / 1e9);
			}
			// 从纳秒构造 chrono::nanoseconds
			static constexpr inline duration from_nanoseconds(std::chrono::nanoseconds nanoseconds) noexcept
			{
				return duration(nanoseconds);
			}
			// 从微秒构造 i64 microseconds
			static constexpr inline duration from_microseconds(i64 microseconds) noexcept
			{
				return duration((microseconds * 1e3));
			}
			// 从微秒构造 f64 microseconds
			static constexpr inline duration from_microseconds(f64 microseconds) noexcept
			{
				return duration(microseconds / 1e6);
			}
			// 从微秒构造 chrono::microseconds
			static constexpr inline duration from_microseconds(std::chrono::microseconds microseconds) noexcept
			{
				return duration(microseconds);
			}
			// 从毫秒构造 i64 milliseconds
			static constexpr inline duration from_milliseconds(i64 milliseconds) noexcept
			{
				return duration(milliseconds * 1e6);
			}
			// 从毫秒构造 f64 milliseconds
			static constexpr inline duration from_milliseconds(f64 milliseconds) noexcept
			{
				return duration(milliseconds / 1e3);
			}
			// 从毫秒构造 chrono::milliseconds
			static constexpr inline duration from_milliseconds(std::chrono::milliseconds milliseconds) noexcept
			{
				return duration(milliseconds);
			}
			// 从秒构造 i64 seconds>
			static constexpr inline duration from_seconds(i64 seconds) noexcept
			{
				return duration(seconds * 1e9);
			}
			// 从秒构造 f64	seconds
			static constexpr inline duration from_seconds(f64 seconds) noexcept
			{
				return duration(seconds);
			}
			// 从秒构造 chrono::seconds
			static constexpr inline duration from_seconds(std::chrono::seconds seconds) noexcept
			{
				return duration(seconds);
			}

			// 隐式类型转换
			template <typename Rep, typename Period>
			constexpr inline operator std::chrono::duration<Rep, Period>() const noexcept
			{
				return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(std::chrono::nanoseconds(duration_));
			}

			// 转换为chrono::nanoseconds
			constexpr inline std::chrono::nanoseconds to_nanoseconds() const noexcept
			{
				std::chrono::nanoseconds temp = (*this);
				return temp;
			}
			// 转换为chrono::microseconds
			constexpr inline std::chrono::microseconds to_microseconds() const noexcept
			{
				std::chrono::microseconds temp = (*this);
				return temp;
			}
			// 转换为chrono::milliseconds
			constexpr inline std::chrono::milliseconds to_milliseconds() const noexcept
			{
				std::chrono::milliseconds temp = (*this);
				return temp;
			}
			// 转换为chrono::seconds
			constexpr inline std::chrono::seconds to_seconds() const noexcept
			{
				std::chrono::seconds temp = (*this);
				return temp;
			}
			// 转换为chrono::minutes
			constexpr inline std::chrono::minutes to_minutes() const noexcept
			{
				std::chrono::minutes temp = (*this);
				return temp;
			}
			// 转换为chrono::hours
			constexpr inline std::chrono::hours to_hours() const noexcept
			{
				std::chrono::hours temp = (*this);
				return temp;
			}
			// 转换为chrono::days
			constexpr inline std::chrono::days to_days() const noexcept
			{
				std::chrono::days temp = (*this);
				return temp;
			}

			// 获取纳秒_整数
			constexpr inline i64 nanoseconds() const noexcept
			{
				return duration_;
			}
			// 获取纳秒_浮点
			constexpr inline f64 fnanoseconds() const noexcept
			{
				return f64(duration_);
			}
			// 获取微秒_整数
			constexpr inline i64 microseconds() const noexcept
			{
				return duration_ / 1'000;
			}
			// 获取微秒_浮点
			constexpr inline f64 fmicroseconds() const noexcept
			{
				return f64(duration_) / 1'000.0;
			}
			// 获取毫秒_整数
			constexpr inline i64 milliseconds() const noexcept
			{
				return duration_ / 1'000'000;
			}
			// 获取毫秒_浮点
			constexpr inline f64 fmilliseconds() const noexcept
			{
				return f64(duration_) / 1'000'000.0;
			}
			// 获取秒_整数
			constexpr inline i64 seconds() const noexcept
			{
				return duration_ / 1'000'000'000;
			}
			// 获取秒_浮点
			constexpr inline f64 fseconds() const noexcept
			{
				return f64(duration_) / 1'000'000'000.0;
			}
			// 获取分钟_整数
			constexpr inline i64 minutes() const noexcept
			{
				return duration_ / 60'000'000'000;
			}
			// 获取分钟_浮点
			constexpr inline f64 fminutes() const noexcept
			{
				return f64(duration_) / 60'000'000'000.0;
			}
			// 获取小时_整数
			constexpr inline i64 hours() const noexcept
			{
				return duration_ / 3600'000'000'000;
			}
			// 获取小时_浮点
			constexpr inline f64 fhours() const noexcept
			{
				return f64(duration_) / 3600'000'000'000.0;
			}
			// 获取小时_浮点time_point
			constexpr inline i64 days() const noexcept
			{
				return duration_ / 86400'000'000'000;
			}
			// 获取天_浮点
			constexpr inline f64 fdays() const noexcept
			{
				return f64(duration_) / 86400'000'000'000;
			}

			// 等于运算符
			constexpr inline bool operator==(const duration &rhs) const noexcept
			{
				return this->duration_ == rhs.duration_;
			}
			// 不等于运算符
			constexpr inline bool operator!=(const duration &rhs) const noexcept
			{
				return this->duration_ != rhs.duration_;
			}
			// 小于运算符
			constexpr inline bool operator<(const duration &rhs) const noexcept
			{
				return this->duration_ < rhs.duration_;
			}
			// 大于运算符 time_point
			constexpr inline bool operator>(const duration &rhs) const noexcept
			{
				return this->duration_ > rhs.duration_;
			}
			// 小于等于运算符
			constexpr inline bool operator<=(const duration &rhs) const noexcept
			{
				return this->duration_ <= rhs.duration_;
			}
			// 大于等于运算符 duration_
			constexpr inline bool operator>=(const duration &rhs) const noexcept
			{
				return this->duration_ >= rhs.duration_;
			}

			// 加运算符
			constexpr inline duration operator+(const duration &rhs) const noexcept
			{
				return duration(this->duration_ + rhs.duration_);
			}
			// 减运算符
			constexpr inline duration operator-(const duration &rhs) const noexcept
			{
				return duration(this->duration_ - rhs.duration_);
			}
			// 加赋值运算符
			constexpr inline duration &operator+=(const duration &rhs) noexcept
			{
				this->duration_ += rhs.duration_;
				return *this;
			}
			// 减赋值运算符
			constexpr inline duration &operator-=(const duration &rhs) noexcept
			{
				this->duration_ -= rhs.duration_;
				return *this;
			}

		private:
			// 时间长度存储为纳秒（i64 可表示约 292 年）
			i64 duration_ = 0;
		};

		class time_point
		{
		public:
			// 默认构造函数
			constexpr inline time_point() noexcept
				: time_point_(clock::time_point())
			{
			}
			// 构造函数 time_point
			constexpr inline time_point(const time_point &time_point) noexcept
				: time_point_(time_point)
			{
			}
			// 构造函数 clock::time_point
			constexpr inline time_point(const clock::time_point &time_point) noexcept
				: time_point_(time_point)
			{
			}

			// 隐式转换 clock::time_point
			constexpr inline operator clock::time_point() const noexcept
			{
				return time_point_;
			}
			// 转换为chrono::time_point
			constexpr inline clock::time_point to_time_point() const noexcept
			{
				return time_point_;
			}
			// time_since_epoch
			constexpr inline duration time_since_epoch() const noexcept
			{
				return time_point_.time_since_epoch();
			}

			// 等于运算符
			constexpr inline bool operator==(const time_point &other) const noexcept
			{
				return this->time_point_ == other.time_point_;
			}
			// 不等于运算符
			constexpr inline bool operator!=(const time_point &other) const noexcept
			{
				return this->time_point_ != other.time_point_;
			}
			// 大于运算符
			constexpr inline bool operator>(const time_point &other) const noexcept
			{
				return this->time_point_ > other.time_point_;
			}
			// 大于等于运算符
			constexpr inline bool operator>=(const time_point &other) const noexcept
			{
				return this->time_point_ >= other.time_point_;
			}
			// 小于运算符
			constexpr inline bool operator<(const time_point &other) const noexcept
			{
				return this->time_point_ < other.time_point_;
			}
			// 小于等于运算符
			constexpr inline bool operator<=(const time_point &other) const noexcept
			{
				return this->time_point_ <= other.time_point_;
			}

			// 减运算符
			constexpr inline time_point operator-(const duration &rhs) const noexcept
			{
				return time_point(this->time_point_ - rhs.to_nanoseconds());
			}
			// 加运算符
			constexpr inline time_point operator+(const duration &rhs) const noexcept
			{
				return time_point(this->time_point_ + rhs.to_nanoseconds());
			}
			// 减赋值运算符
			constexpr inline time_point &operator+=(const duration &rhs) noexcept
			{
				this->time_point_ += rhs.to_nanoseconds();
				return *this;
			}
			// 加赋值运算符
			constexpr inline time_point &operator-=(const duration &rhs) noexcept
			{
				this->time_point_ -= rhs.to_nanoseconds();
				return *this;
			}
			// 获取时长
			constexpr inline duration operator-(const time_point &rhs) const noexcept
			{
				return duration(this->time_point_ - rhs.time_point_);
			}
			// 获取日期
			inline date to_date() const noexcept
			{
				return date(this->time_point_);
			}

		private:
			clock::time_point time_point_;
		};

		class date
		{
		public:
			// using date_info = tm;
			class date_info
			{
			public:
				// 默认构造函数
				constexpr inline date_info() noexcept
				{
					this->tm_ = {0};
				}
				// 构造函数 tm
				constexpr inline date_info(const tm &tm) noexcept
				{
					this->tm_ = tm;
				}
				// 构造函数 date_info
				constexpr inline date_info(const date_info &date_info) noexcept
				{
					this->tm_ = date_info.tm_;
				}
				// 赋值运算符
				constexpr inline date_info &operator=(const date_info &date_info) noexcept
				{
					this->tm_ = date_info.tm_;
					return *this;
				}
				// 赋值运算符
				constexpr inline date_info &operator=(const tm &tm) noexcept
				{
					this->tm_ = tm;
					return *this;
				}

				// 获取字符串
				// format如下：它们是区分大小写的。
				// %a	星期几的简写
				// %A	星期几的全称
				// %b	月分的简写
				// %B	月份的全称
				// %c	标准的日期的时间串
				// %C	年份的后两位数字
				// %d	十进制表示的每月的第几天
				// %D 	月/天/年
				// %e 	在两字符域中，十进制表示的每月的第几天
				// %F 	年-月-日
				// %g 	年份的后两位数字，使用基于周的年
				// %G 	年分，使用基于周的年
				// %h 	简写的月份名
				// %H 	24小时制的小时
				// %I 	12小时制的小时
				// %j 	十进制表示的每年的第几天
				// %m 	十进制表示的月份
				// %M 	十时制表示的分钟数
				// %n 	新行符
				// %p 	本地的AM或PM的等价显示
				// %r 	12小时的时间
				// %R 	显示小时和分钟：hh:mm
				// %S 	十进制的秒数
				// %t 	水平制表符
				// %T 	显示时分秒：hh:mm:ss
				// %u 	每周的第几天，星期一为第一天 （值从0到6，星期一为0）
				// %U 	第年的第几周，把星期日做为第一天（值从0到53）
				// %V 	每年的第几周，使用基于周的年
				// %w 	十进制表示的星期几（值从0到6，星期天为0）
				// %W 	每年的第几周，把星期一做为第一天（值从0到53）
				// %x 	标准的日期串
				// %X 	标准的时间串
				// %y 	不带世纪的十进制年份（值从0到99）
				// %Y 	带世纪部分的十进制年份
				// %z，%Z 时区名称，如果不能得到时区名称则返回空字符。
				// %% 	百分号
				inline std::string to_string(const std::string &str) const noexcept
				{
					char buffer[256] = { 0 };
					strftime(buffer, sizeof(buffer), str.c_str(), &this->tm_);
					std::string out(buffer);
					return out;
				}

			private:
				friend class date;
				tm tm_;
			};
			// 默认构造函数
			date() = delete;
			// 构造函数 time_point
			constexpr inline date(const time_point &time_point) noexcept
				: time_point_(time_point), is_update_(true)
			{
			}

			// 构造函数 date
			constexpr inline date(const date &date) noexcept
				: time_point_(date.time_point_), is_update_(date.is_update_), date_info_(date.date_info_)
			{
			}

			// 赋值运算符
			constexpr inline date &operator=(const time_point &time_point) noexcept
			{
				this->time_point_ = time_point;
				this->is_update_ = true;
				return *this;
			}
			// 赋值运算符
			constexpr inline date &operator=(const date &date) noexcept
			{
				this->time_point_ = date.time_point_;
				this->is_update_ = date.is_update_;
				return *this;
			}
			// 算术运算符
			constexpr inline date &operator+=(const duration &duration) noexcept
			{
				this->time_point_ += duration;
				this->is_update_ = true;
				return *this;
			}
			// 算术运算符
			constexpr inline date operator+(const duration &duration) noexcept
			{
				date temp = *this;
				temp.time_point_ += duration;
				temp.is_update_ = true;
				return temp;
			}
			// 获取日期
			inline date_info &get_date() noexcept
			{
				if (is_update_)
				{
					convert_time_point();
					is_update_ = false;
				}
				return date_info_;
			}

		private:
			// 时间点转日期
			inline void convert_time_point() noexcept
			{
				auto d1 = this->time_point_ - time::program_start_time_;
				auto d2 = time::program_start_sys_time_ + d1.to_seconds();
				time_t t = std::chrono::system_clock::to_time_t(d2);
#ifdef _WIN32
				// Windows 使用 localtime_s
				localtime_s(&(date_info_.tm_), &t);
#else
				// Linux/macOS 使用 tm_gmtoff
				localtime_r(&t, &(date_info_.tm_));
#endif
				return;
			}

			time_point time_point_;
			date_info date_info_;
			bool is_update_;
		};

		// 获取当前时间点
		static inline time_point now() noexcept
		{
			return clock::now();
		}

		// 获取程序运行时间
		static inline duration elapsed_time() noexcept
		{
			return now() - program_start_time_;
		}

		// 获取程序启动时间点
		static inline const time_point &start_time() noexcept
		{
			return program_start_time_;
		}

		// 获取程序启动时间点
		static inline const std::chrono::system_clock::time_point &start_sys_time() noexcept
		{
			return program_start_sys_time_;
		}

		// 禁止实例化
		time() = delete;
		~time() = delete;

	private:
		// 程序启动时间点
		const static inline time_point program_start_time_ = now();
		const static inline std::chrono::system_clock::time_point program_start_sys_time_ = std::chrono::system_clock::now();
	};
}
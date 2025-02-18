#pragma once

#include "../base/using.hpp"
#include "../base/initialization/high_precision_timer.hpp"

#include <chrono>

namespace __NAMESPACE_NAME__
{
	class time {
	public:
		// 时间单位
		using seconds		= std::chrono::duration<double>;				// 秒

		// 时间点类型
		using time_point = std::chrono::time_point<std::chrono::high_resolution_clock, seconds>;
		using clock = std::chrono::high_resolution_clock;

		// 获取当前时间点
		static inline time_point now() {
			return clock::now();
		}

		// 获取程序运行时间（秒）
		static inline seconds elapsed_time() {
			return now() - program_start_time_;
		}

		// 获取程序启动时间点
		static inline time_point start_time() {
			return program_start_time_;
		}

		time() = delete;
		~time() = delete;
	private:
		// 程序启动时间点
		static inline time_point program_start_time_ = now();
	};
}
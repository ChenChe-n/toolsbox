#pragma once

#include "../base/using.hpp"
#include "../time/time.hpp"

#include <atomic>
#include <condition_variable>
#include <thread>
#include <stdexcept>
#include <list>

namespace __NAMESPACE_NAME__
{
	namespace thread
	{

		// 休眠策略
		enum class sleep_level : uint8_t
		{
			level_v0, // 无休眠
			level_v1, // yield
			level_v2, // 1ms
			level_v3, // 10ms
			level_v4, // 100ms
			level_v5, // 1s
			level_v6, // 1m
			level_v7, // 1h
			level_v8  // 1d
		};

		// 根据休眠策略执行休眠
		inline void sleep_for(sleep_level level)
		{
			using namespace std::chrono;
			switch (level)
			{
			case sleep_level::level_v0:
				break; // 无操作
			case sleep_level::level_v1:
				std::this_thread::yield();
				break;
			case sleep_level::level_v2:
				std::this_thread::sleep_for(__NAMESPACE_NAME__::time::seconds(0.001));
				break;
			case sleep_level::level_v3:
				std::this_thread::sleep_for(__NAMESPACE_NAME__::time::seconds(0.01));
				break;
			case sleep_level::level_v4:
				std::this_thread::sleep_for(__NAMESPACE_NAME__::time::seconds(0.1));
				break;
			case sleep_level::level_v5:
				std::this_thread::sleep_for(__NAMESPACE_NAME__::time::seconds(1));
				break;
			case sleep_level::level_v6:
				std::this_thread::sleep_for(__NAMESPACE_NAME__::time::seconds(60));
				break;
			case sleep_level::level_v7:
				std::this_thread::sleep_for(__NAMESPACE_NAME__::time::seconds(60 * 60));
				break;
			case sleep_level::level_v8:
				std::this_thread::sleep_for(__NAMESPACE_NAME__::time::seconds(60 * 60 * 24));
				break;
			default:
				break;
			}
		}

		// 互斥锁
		class mutex_v1
		{
		public:
			explicit mutex_v1(sleep_level level = sleep_level::level_v2)
				: sleep_level_(level)
			{
				flag_.clear();
			}

			void lock()
			{
				while (flag_.test_and_set(std::memory_order_acquire))
				{
					sleep_for(sleep_level_);
				}
				return;
			}

			bool try_lock()
			{
				return !flag_.test_and_set(std::memory_order_acquire);
			}

			void unlock()
			{
				flag_.clear(std::memory_order_release);
				return;
			}

		private:
			std::atomic_flag flag_;
			sleep_level sleep_level_;
		};

		// 递归锁
		class mutex_v2
		{
		public:
			explicit mutex_v2(sleep_level level = sleep_level::level_v2)
				: sleep_level_(level), count_(0)
			{
			}

			void lock()
			{
				auto current_thread_id = std::this_thread::get_id();
				std::thread::id expected = std::thread::id();

				// 如果当前线程已经持有锁，增加计数
				if (thread_id_.load(std::memory_order_relaxed) == current_thread_id)
				{
					count_.fetch_add(1, std::memory_order_relaxed); // 原子操作
					return;
				}

				// 否则，尝试获取锁
				while (!thread_id_.compare_exchange_weak(expected, current_thread_id, std::memory_order_acquire))
				{
					expected = std::thread::id(); // 重置 expected
					sleep_for(sleep_level_);
				}

				// 获取锁成功，设置计数为 1
				count_.store(1, std::memory_order_relaxed);
			}

			bool try_lock()
			{
				auto current_thread_id = std::this_thread::get_id();
				std::thread::id expected = std::thread::id();

				// 如果当前线程已经持有锁，增加计数
				if (thread_id_.load(std::memory_order_relaxed) == current_thread_id)
				{
					count_.fetch_add(1, std::memory_order_relaxed); // 原子操作
					return true;
				}

				// 否则，尝试获取锁
				if (thread_id_.compare_exchange_strong(expected, current_thread_id, std::memory_order_acquire))
				{
					// 获取锁成功，设置计数为 1
					count_.store(1, std::memory_order_relaxed);
					return true;
				}
				return false;
			}

			void unlock()
			{
				auto current_thread_id = std::this_thread::get_id();
				if (thread_id_.load(std::memory_order_relaxed) != current_thread_id)
				{
					throw std::runtime_error("Attempt to unlock a mutex not owned by the current thread");
				}

				// 减少计数
				if (count_.fetch_sub(1, std::memory_order_release) == 1)
				{
					// 如果计数为 0，释放锁
					thread_id_.store(std::thread::id(), std::memory_order_release);
				}
			}

		private:
			std::atomic<std::thread::id> thread_id_;
			std::atomic<u64> count_;
			sleep_level sleep_level_;
		};

		// std::mutex
		using mutex_v3 = std::mutex;

		// 锁管理类
		template <typename Mutex>
		class lock
		{
		public:
			using mutex_type = Mutex;

			explicit lock(mutex_type &mutex) : mutex_(mutex)
			{
				mutex_.lock();
			}

			~lock()
			{
				mutex_.unlock();
			}

			// 禁止拷贝和赋值
			lock(const lock &) = delete;
			lock &operator=(const lock &) = delete;

			// 支持移动语义
			lock(lock &&other) noexcept : mutex_(other.mutex_)
			{
				other.mutex_ = nullptr;
			}

			lock &operator=(lock &&other) noexcept
			{
				if (this != &other)
				{
					mutex_ = other.mutex_;
					other.mutex_ = nullptr;
				}
				return *this;
			}

		private:
			mutex_type &mutex_;
		};
	}
}
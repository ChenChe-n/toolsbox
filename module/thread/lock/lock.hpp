#pragma once

#include "../../base/using.hpp"
#include "../../time/time.hpp"

#include <atomic>
#include <condition_variable>
#include <thread>
#include <stdexcept>

namespace __NAMESPACE_NAME__::thread
{
    inline void sleep_for(__NAMESPACE_NAME__::time::duration duration) noexcept
    {
        if (duration.count() < 0)
        {
            return;
        }
        if (duration.count() == 0.0)
        {
            std::this_thread::yield();
            return;
        }

        try
        {
            std::this_thread::sleep_for(duration.to_nanoseconds());
        }
        catch (const std::exception &e)
        {
            // std::cerr << e.what() << '\n';
        }

        return;
    }

    // 互斥锁
    class mutex_v1
    {
    public:
        // 禁止拷贝和赋值
        mutex_v1(const mutex_v1 &) = delete;
        mutex_v1 &operator=(const mutex_v1 &) = delete;

        // 禁止移动语义
        mutex_v1(mutex_v1 &&other) noexcept = delete;
        mutex_v1 &operator=(mutex_v1 &&other) noexcept = delete;

        // 构造函数
        inline mutex_v1(const __NAMESPACE_NAME__::time::duration duration = 0.001) noexcept
            : sleep_duration_(duration)
        {
            flag_.clear();
        }

        inline bool try_lock() noexcept
        {
            return !flag_.test_and_set(std::memory_order_acquire);
        }

        inline bool lock() noexcept
        {
            while (true)
            {
                if (try_lock())
                {
                    return true;
                }
                sleep_for(sleep_duration_);
            }
            return false;
        }

        inline bool try_lock_for(const __NAMESPACE_NAME__::time::duration duration) noexcept
        {
            auto start_time = __NAMESPACE_NAME__::time::now();
            auto end_time = start_time + duration;
            while (__NAMESPACE_NAME__::time::now() < end_time)
            {
                if (try_lock())
                {
                    return true;
                }
                sleep_for(sleep_duration_);
            }
            return false;
        }

        inline bool try_lock_until(const __NAMESPACE_NAME__::time::time_point time_point) noexcept
        {
            while (__NAMESPACE_NAME__::time::now() < time_point)
            {
                if (try_lock())
                {
                    return true;
                }
                sleep_for(sleep_duration_);
            }
            return false;
        }

        inline bool unlock() noexcept
        {
            flag_.clear(std::memory_order_release);
            return true;
        }

    private:
        std::atomic_flag flag_;
        __NAMESPACE_NAME__::time::duration sleep_duration_;
    };

    // 递归锁
    class mutex_v2
    {
    public:
        // 禁止拷贝和赋值
        mutex_v2(const mutex_v2 &) = delete;
        mutex_v2 &operator=(const mutex_v2 &) = delete;

        // 禁止移动语义
        mutex_v2(mutex_v2 &&other) noexcept = delete;
        mutex_v2 &operator=(mutex_v2 &&other) noexcept = delete;

        inline mutex_v2(__NAMESPACE_NAME__::time::duration duration = 0.001) noexcept
            : sleep_duration_(duration), count_(0)
        {
        }

        inline bool try_lock() noexcept
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

        inline bool lock() noexcept
        {
            while (true)
            {
                if (try_lock())
                {
                    return true;
                }
                sleep_for(sleep_duration_);
            }
            return false;
        }

        inline bool try_lock_for(const __NAMESPACE_NAME__::time::duration duration) noexcept
        {
            auto start_time = __NAMESPACE_NAME__::time::now();
            auto end_time = start_time + duration;
            while (__NAMESPACE_NAME__::time::now() < end_time)
            {
                if (try_lock())
                {
                    return true;
                }
                sleep_for(sleep_duration_);
            }
        }

        inline bool try_lock_until(const __NAMESPACE_NAME__::time::time_point time_point) noexcept
        {
            while (__NAMESPACE_NAME__::time::now() < time_point)
            {
                if (try_lock())
                {
                    return true;
                }
                sleep_for(sleep_duration_);
            }
            return false;
        }

        inline bool unlock() noexcept
        {
            auto current_thread_id = std::this_thread::get_id();
            if (thread_id_.load(std::memory_order_relaxed) != current_thread_id)
            {
                return false;
            }

            // 减少计数
            if (count_.fetch_sub(1, std::memory_order_release) == 1)
            {
                // 如果计数为 0，释放锁
                thread_id_.store(std::thread::id(), std::memory_order_release);
                return true;
            }
            return false;
        }

    private:
        std::atomic<std::thread::id> thread_id_;
        std::atomic<u64> count_;
        __NAMESPACE_NAME__::time::duration sleep_duration_;
    };

    // std::mutex
    using mutex_v3 = std::mutex;

    // 锁管理类
    template <typename Mutex>
    class lock
    {
    public:
        using mutex_type = Mutex;

        lock(mutex_type &mutex) noexcept
            : mutex_(&mutex)
        {
            if (mutex_ != nullptr)
            {
                (*mutex_).lock();
            }
        }

        ~lock() noexcept
        {
            if (mutex_ != nullptr)
            {
                (*mutex_).unlock();
            }
        }

        // 禁止拷贝和赋值
        lock(const lock &) = delete;
        lock &operator=(const lock &) = delete;

        // 支持移动语义
        lock(lock &&other) noexcept
            : mutex_(other.mutex_)
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
        mutex_type *mutex_;
    };

    template <typename mutex, typename T>
    class box_lock
    {
    public:
        box_lock() = delete;
        box_lock(const box_lock &) = delete;
        box_lock &operator=(const box_lock &) = delete;
        box_lock(box_lock &&) = delete;
        box_lock &operator=(box_lock &&) = delete;

        inline box_lock(mutex &mutex__, T &data) noexcept
            : mutex_(mutex__), data_(data)
        {
            mutex_.lock();
        }
        inline ~box_lock()
        {
            mutex_.unlock();
        }

        inline box_lock& operator=(const T &data){
            data_ = data;
            return *this;
        }
        inline box_lock& operator=(const T &&data){
            data_ = std::move(data);
            return *this;
        }

        T &data_;
    private:
        mutex &mutex_;
    };

}
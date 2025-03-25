#pragma once

#include "../lock/lock.hpp"

namespace __NAMESPACE_NAME__::thread
{
    template <typename T, typename mutex_type = mutex_v1>
    class box
    {
    public:
        box() = delete;
        box(const box &) = delete;
        box &operator=(const box &) = delete;
        box(box &&) = delete;
        box &operator=(box &&) = delete;

        inline box(T &&data)
            : mutex_(), data_(::std::forward<T>(data))
        {
        }

        inline box_lock<mutex_type,T> get()
        {
            return box_lock(mutex_, data_);
        }

        inline box &operator=(const T &data)
        {
            mutex_.lock();
            data_ = data;
            mutex_.unlock();
        }

        inline T copy()
        {
            mutex_.lock();
            T data = data_;
            mutex_.unlock();
            return data;
        }

    private:
        mutex_type mutex_;
        T data_;
    };

}
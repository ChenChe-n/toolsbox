#pragma once

#include "../lock/lock.hpp"

namespace __NAMESPACE_NAME__::thread
{
    template <typename T, typename Mutex_type = mutex_v1>
    class box
    {
        private:
        Mutex_type mutex_;
        T data_;
        using box_lock_ = box_lock<Mutex_type,T>; 
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

        inline box_lock_ get()
        {
            return box_lock(mutex_, data_);
        }

        inline box_lock_ data()
        {
            return box_lock(mutex_, data_);
        }
    };

}
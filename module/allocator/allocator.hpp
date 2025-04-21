#pragma once

#include "../base/using.hpp"
#include "../thread/lock/lock.hpp"
#include <stdexcept>
#include <unordered_map>
#include <map>
#include <atomic>
#include <cstring>

namespace __NAMESPACE_NAME__
{
    class allocator
    {
    public:
        inline allocator(u64 alloc_max_ = 0xffff) : alloc_max_(alloc_max_) {}
        inline ~allocator()
        {
            // 释放内存
            while (!map_.empty())
            {
                auto it = map_.begin();
                delete[] static_cast<byte *>(it->first);
                map_.erase(it);
            }
        }

        inline void *malloc(u64 size)
        {
            if (alloc_count_ + size > alloc_max_)
            {
                return nullptr;
            }
            try
            {
                auto ptr = static_cast<void *>(new byte[size]);

                alloc_count_ += size;
                map_[ptr] = size;

                return ptr;
            }
            catch (const std::bad_alloc &e)
            {
                // 处理内存分配失败
                return nullptr;
            }
        }

        inline void free(void *ptr)
        {
            if (ptr == nullptr)
            {
                return;
            }

            if (map_.count(ptr) == 0)
            {
                return;
            }
            delete[] static_cast<byte *>(ptr);
            alloc_count_ -= map_[ptr];
            map_.erase(ptr);
        }

        inline void *realloc(void *ptr, u64 size)
        {
            if (alloc_count_ + size > alloc_max_)
            {
                return nullptr;
            }
            if (ptr == nullptr)
            {
                return malloc(size);
            }
            if (size == 0)
            {
                free(ptr);
                return nullptr;
            }

            if (map_.count(ptr) == 0)
            {
                return nullptr;
            }

            try
            {
                auto new_ptr = static_cast<void *>(new byte[size]);
                u64 old_size = map_[ptr];
                memcpy(new_ptr, ptr, std::min(size, old_size));

                delete[] static_cast<byte *>(ptr);
                alloc_count_ -= old_size;
                map_.erase(ptr);

                alloc_count_ += size;
                map_[new_ptr] = size;

                return new_ptr;
            }
            catch (const std::bad_alloc &e)
            {
                // 处理内存分配失败
                return nullptr;
            }
        }

        inline const std::map<void *, u64> &get_map() const
        {
            return map_;
        }

        inline u64 get_count() const
        {
            return alloc_count_;
        }

        inline u64 set_alloc_max(u64 alloc_max)
        {
            alloc_max_ = std::max(alloc_max, alloc_count_);
            return alloc_max_;
        }
    private:
        u64 alloc_max_ = 0;
        u64 alloc_count_ = 0;
        std::map<void *, u64> map_;
    };
}
#pragma once

#include "../base/using.hpp"
#include <random>

namespace __NAMESPACE_NAME__
{
    class random_32
    {
    public:
        inline random_32()
            : gen_(rd_())
        {
        }
        inline random_32(u32 seed)
            : gen_(seed)
        {
        }
        inline void set_seed(u32 seed)
        {
            gen_ = std::mt19937(seed);
        }
        static inline u32 get_device()
        {
            return rd_();
        }
        inline u32 get_u32()
        {
            return dis_u32_(gen_);
        }
        inline u32 get_u32_distribution(u32 low, u32 high)
        {
            if (low >= high)
            {
                low = 0;
                high = u32_max;
            }
            std::uniform_int_distribution<u32> dis{low, high};
            return dis(gen_);
        }

    private:
        std::uniform_int_distribution<u32> dis_u32_{0, u32_max};
        static inline std::random_device rd_;
        std::mt19937 gen_;
    };
    class random_64
    {
    public:
        inline random_64()
            : gen_((u64(rd_()) << 32) | u64(rd_()))
        {
        }
        inline random_64(u64 seed)
            : gen_(seed)
        {
        }
        inline void set_seed(u64 seed)
        {
            gen_ = std::mt19937_64(seed);
        }
        static inline u64 get_device()
        {
            return ((u64(rd_()) << 32) | u64(rd_()));
        }

        inline u64 get_u64()
        {
            return dis_u64_(gen_);
        }
        inline u64 get_u64_distribution(u64 low, u64 high)
        {
            if (low >= high)
            {
                low = 0;
                high = u64_max;
            }
            std::uniform_int_distribution<u64> dis{low, high};
            return dis(gen_);
        }

    private:
        std::uniform_int_distribution<u64> dis_u64_{0, u64_max};
        static inline std::random_device rd_;
        std::mt19937_64 gen_;
    };
}
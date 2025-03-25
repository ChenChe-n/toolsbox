#pragma once

#include "../base/using.hpp"
#include <random>

namespace __NAMESPACE_NAME__
{
    class random
    {
    public:
        inline random()
        {
            gen = std::mt19937(rd());
        }
        inline void set_seed(u64 seed){
            gen = std::mt19937(seed);
        }
        static inline u64 get_device()
        {
            return rd();
        }
        inline u64 get_u64()
        {
            return dis_u64(gen);
        }

    private:
        std::uniform_int_distribution<u64> dis_u64{0, u64_max};
        static inline std::random_device rd;
        std::mt19937 gen;
    };
}
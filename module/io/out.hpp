#pragma once

#include "../base/using.hpp"
#include "../thread/lock/lock.hpp"

#include <string_view>
#include <memory>
#include <iostream>
#include <fstream>

namespace __NAMESPACE_NAME__::io
{
    class out_base
    {
    public:
        virtual inline bool out(const std::string_view str) noexcept = 0;
    };
    class out_std : public out_base
    {
    public:
        inline bool out(const std::string_view str) noexcept
        {
            std::cout << str;
            return true;
        }
    };
    class out_file : public out_base
    {
    public:
        inline bool out(const std::string_view str) noexcept
        {
            std::ofstream file("out.txt", std::ios::app);
            if (!file)
            {
                return false;
            }
            file << str;
            file.close();
            return true;
        }
    };

    class out
    {
    public:
        static inline out_base *std = new out_std();
        static inline out_base *file = new out_file();
    };
}
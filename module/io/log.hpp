#pragma once

#include "../base/using.hpp"
#include "../time/time.hpp"
#include "out.hpp"
#include <string_view>
#include <format>

namespace __NAMESPACE_NAME__::io
{
    class logging
    {
    private:
        ts::io::out_base *out_ = ts::io::out::std;
        enum class log_type
        {
            base,
            text,
            info,
            warning,
            error
        };
        inline std::string format(log_type logging_type, const std::string_view str)
        {
            auto mt = __NAMESPACE_NAME__::time::get_current_date();
            switch (logging_type)
            {
            case log_type::base:
                return std::format(
                    "{}\n",
                    str);
                break;
            case log_type::text:
                return std::format(
                    "\033[1;36m[TEXT]     {:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}  {:.03f}\033[0m \t{}\n", // 蓝色
                    mt.year_, mt.month_, mt.day_,
                    mt.hour_, mt.minute_, mt.second_, mt.milli_second_,
                    __NAMESPACE_NAME__::time::elapsed_time().count(),
                    str);
                break;
            case log_type::info:
                return std::format(
                    "\033[1;32m[INFO]     {:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}  {:.03f}\033[0m \t{}\n", // 绿色
                    mt.year_, mt.month_, mt.day_,
                    mt.hour_, mt.minute_, mt.second_, mt.milli_second_,
                    __NAMESPACE_NAME__::time::elapsed_time().count(),
                    str);
                break;
            case log_type::warning:
                return std::format(
                    "\033[1;33m[WARNING]  {:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}  {:.03f}\033[0m \t{}\n", // 黄色
                    mt.year_, mt.month_, mt.day_,
                    mt.hour_, mt.minute_, mt.second_, mt.milli_second_,
                    __NAMESPACE_NAME__::time::elapsed_time().count(),
                    str);
                break;
            case log_type::error:
                return std::format(
                    "\033[1;31m[ERROR]    {:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}  {:.03f}\033[0m \t{}\n", // 红色
                    mt.year_, mt.month_, mt.day_,
                    mt.hour_, mt.minute_, mt.second_, mt.milli_second_,
                    __NAMESPACE_NAME__::time::elapsed_time().count(),
                    str);
                break;
            default:
                return std::string(str);
                break;
            }
        }

        inline bool out(const std::string_view str) noexcept
        {
            if (out_->out(str))
            {
                return true;
            }
            return false;
        }

    public:
        inline void set_out(ts::io::out_base *out) noexcept
        {
            if (out == nullptr)
            {
                return;
            }
            out_ = out;
        }
        inline bool base(const std::string_view str) noexcept
        {
            return out(format(log_type::base, str));
        }

        inline bool text(const std::string_view str) noexcept
        {
            return out(format(log_type::text, str));
        }
        inline bool info(const std::string_view str) noexcept
        {
            return out(format(log_type::info, str));
        }
        inline bool warning(const std::string_view str) noexcept
        {
            return out(format(log_type::warning, str));
        }
        inline bool error(const std::string_view str) noexcept
        {
            return out(format(log_type::error, str));
        }
    };
}
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
            auto date = __NAMESPACE_NAME__::time::now().to_date().get_date().to_string("%Y-%m-%d %H:%M:%S");
            auto time = __NAMESPACE_NAME__::time::elapsed_time().fseconds();
            switch (logging_type)
            {
            case log_type::base:
                return std::format(
                    "{}\n",
                    str);
                break;
            case log_type::text:
                return std::format(
                    "\033[1;36m[TEXT]     {}  {:.06f}\033[0m \t{}\n", // 蓝色
                    date,
                    time,
                    str);
                break;
            case log_type::info:
                return std::format(
                    "\033[1;32m[INFO]     {}  {:.06f}\033[0m \t{}\n", // 绿色
                    date,
                    time,
                    str);
                break;
            case log_type::warning:
                return std::format(
                    "\033[1;33m[WARNING]  {}  {:.06f}\033[0m \t{}\n", // 黄色
                    date,
                    time,
                    str);
                break;
            case log_type::error:
                return std::format(
                    "\033[1;31m[ERROR]    {}  {:.06f}\033[0m \t{}\n", // 红色
                    date,
                    time,
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
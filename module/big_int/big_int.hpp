#pragma once

#include "../base/using.hpp"

#include <vector>
#include <sstream>

namespace __NAMESPACE_NAME__
{
    // 大整数类
    // 存储2^32进制的数据
    class big_int
    {
    public:
        enum class sign_state : u8
        {
            positive,          // 正数
            negative,          // 负数
            positive_overflow, // 正溢出
            negative_overflow, // 负溢出
            not_a_number,      // 非数字
            undefined          // 未定义
        };

        // 默认构造为0
        big_int();

        // 从 big_int 拷贝
        big_int(const big_int &num);

        // 从 big_int 移动
        big_int(big_int &&num) noexcept;

        // 从字符串构造
        big_int(const std::string &num);

        // 从数字构造
        big_int(i64 num);

        // 赋值
        big_int &operator=(const big_int &other);
        big_int &operator=(big_int &&other) noexcept;

        // 转换为字符串
        std::string to_string() const;
        std::string to_bit() const;
        std::string to_hex() const;
        std::string to_u32() const;

        // 单目运算符
        big_int operator-() const;
        big_int operator+() const;
        big_int &operator++();
        big_int &operator--();
        big_int operator++(int);
        big_int operator--(int);

        // 四则运算
        big_int operator+(const big_int &other) const;
        big_int operator-(const big_int &other) const;
        big_int operator*(const big_int &other) const;
        big_int operator/(const big_int &other) const;
        big_int operator%(const big_int &other) const;

        big_int operator+=(const big_int &other);
        big_int operator-=(const big_int &other);
        big_int operator*=(const big_int &other);
        big_int operator/=(const big_int &other);
        big_int operator%=(const big_int &other);

        static big_int division(const big_int &a, const big_int &b, big_int &remainder);
        static big_int abs(const big_int &num);

        // 比较运算
        bool operator==(const big_int &other) const;
        bool operator!=(const big_int &other) const;

        bool operator>(const big_int &other) const;
        bool operator<(const big_int &other) const;

        bool operator>=(const big_int &other) const;
        bool operator<=(const big_int &other) const;

        // 位运算
        big_int operator<<(u64 shift) const;
        big_int operator>>(u64 shift) const;

        big_int operator<<=(u64 shift);
        big_int operator>>=(u64 shift);

    private:
        std::vector<u32> data = {0};             // 数据存储 (32 位为一个块)
        sign_state state = sign_state::positive; // 符号状态
        u64 max_length = 8000;                   // 最大长度
    private:
        // 辅助方法
        static big_int abs_add(const big_int &a, const big_int &b);
        static big_int abs_sub(const big_int &a, const big_int &b);
        bool abs_less(const big_int &other) const;
        bool abs_greater(const big_int &other) const;
        void format();
        bool error(big_int &out) const;
        bool error(big_int &out, const big_int &other) const;
        // 将十进制大数转换为二进制，按 32 位存储
        std::vector<u32> static decimal_to_binary(const std::string &decimal);
        // 将二进制（按 32 位块存储）转换为十进制字符串
        std::string static binary_to_decimal(const std::vector<u32> &binary);
        // 判断字符串是否为有效大数
        bool static is_valid_number(const std::string &str);
    };
    constexpr inline static f64 log2_log10 = 3.3219281;
    constexpr inline static f64 log10_log2 = 0.3010301;
    const inline static big_int big_int_0 = big_int(0);
    const inline static big_int big_int_1 = big_int(1);
    const inline static big_int big_int_s1 = big_int(-1);
}
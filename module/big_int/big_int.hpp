#pragma once
#include "../base/using.hpp"

#include <vector>
#include <string_view>
#include <string>
#include <stdexcept>

namespace __NAMESPACE_NAME__
{
    class big_int
    {
    public:
        enum class sign
        {
            // 未初始化
            uninitialized,
            // 非数字
            not_a_number,
            // 正数
            positive,
            // 负数
            negative
        };
        // 构造函数
        big_int();
        big_int(const ::std::string_view &str);
        big_int(const i64 num);
        big_int(const big_int &other);
        big_int(big_int &&other);

        // 析构函数
        ~big_int();

        void swap(big_int &other);

        // 赋值运算符重载
        big_int &operator=(const i64 num);
        big_int &operator=(const big_int &other);
        big_int &operator=(big_int &&other);

        // 比较运算符重载
        bool operator==(const big_int &other) const;
        bool operator!=(const big_int &other) const;
        bool operator<(const big_int &other) const;
        bool operator>(const big_int &other) const;
        bool operator<=(const big_int &other) const;
        bool operator>=(const big_int &other) const;

        // 算术运运算符重载
        big_int operator+() const;
        big_int operator-() const;
        big_int &operator++();
        big_int &operator--();
        big_int operator++(int);
        big_int operator--(int);
        big_int &operator<<=(const u32 num);
        big_int &operator>>=(const u32 num);
        big_int &operator+=(const big_int &other);
        big_int &operator-=(const big_int &other);
        big_int &operator*=(const big_int &other);
        big_int &operator/=(const big_int &other);
        big_int &operator%=(const big_int &other);

        // 转换函数
        ::std::string to_dec() const;
        ::std::string to_hex() const;
        ::std::string to_oct() const;
        ::std::string to_bin() const;

    private:
        inline bool get_sign() const;
        inline static const big_int &abs_big(const big_int &a, const big_int &b);
        inline static const big_int &abs_small(const big_int &a, const big_int &b);
        // is_zero
        inline bool is_zero() const;
        // a < b
        inline static i8 less(const big_int &a, const big_int &b);
        // abs(a) < abs(b)
        inline static i8 abs_less(const big_int &a, const big_int &b);
        // a = a + b
        inline static void abs_add(big_int &a, const big_int &b);
        // a = a - b ( a >= b )
        inline static void abs_sub(big_int &a, const big_int &b);
        // a = a * b
        inline static void abs_mul(big_int &a, const big_int &b);
        // a = a % b , c = a / b
        inline static void abs_div(big_int &a, const big_int &b, big_int &c);
        // 格式化
        inline void format();

    private:
        // 数据 <低位数>>>>高位数>
        ::std::vector<u32> data_ = {};
        // 符号
        sign sign_ = sign::uninitialized;
        // 最大位数
        u32 max_bit_lenth_ = 65536;
    };
}
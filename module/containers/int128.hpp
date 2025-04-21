#pragma once
#include <cstdint>
#include <iostream>
#include <array>
#include <string>
#include <string_view>
#include <stdexcept>

class __int128_data
{
public:
    inline constexpr __int128_data() noexcept
        : data_({0})
    {
    }
    inline constexpr __int128_data(const uint64_t &lo, const uint64_t &hi = 0) noexcept
        : data_({uint32_t(lo & 0xffffffff), uint32_t(lo >> 32), uint32_t(hi & 0xffffffff), uint32_t(hi >> 32)})
    {
    }
    inline constexpr __int128_data(const __int128_data &data) noexcept
        : data_(data.data_)
    {
    }
    inline constexpr __int128_data &operator<<=(const uint64_t &shift) noexcept
    {
        uint8_t lo = shift % 32;
        uint8_t hi = 32 - lo;
        __int128_data temp;
        switch (shift / 32)
        {
        case 0:
            temp.data_[0] |= data_[0] << lo;
            temp.data_[1] |= data_[1] << lo;
            temp.data_[2] |= data_[2] << lo;
            temp.data_[3] |= data_[3] << lo;

            temp.data_[1] |= data_[0] >> hi;
            temp.data_[2] |= data_[1] >> hi;
            temp.data_[3] |= data_[2] >> hi;
            break;
        case 1:
            temp.data_[1] |= data_[0] << lo;
            temp.data_[2] |= data_[1] << lo;
            temp.data_[3] |= data_[2] << lo;

            temp.data_[2] |= data_[0] >> hi;
            temp.data_[3] |= data_[1] >> hi;
        case 2:
            temp.data_[2] |= data_[0] << lo;
            temp.data_[3] |= data_[1] << lo;

            temp.data_[3] |= data_[0] >> hi;
        case 3:
            temp.data_[3] |= data_[0] << lo;
            break;
        default:
            break;
        }
        *this = temp;
        return *this;
    }
    inline constexpr __int128_data &operator>>=(const uint64_t &shift) noexcept
    {
        uint8_t lo = shift % 32;
        uint8_t hi = 32 - lo;
        __int128_data temp;
        switch (shift / 32)
        {
        case 0:
            temp.data_[0] |= data_[0] >> lo;
            temp.data_[1] |= data_[1] >> lo;
            temp.data_[2] |= data_[2] >> lo;
            temp.data_[3] |= data_[3] >> lo;

            temp.data_[0] |= data_[1] << hi;
            temp.data_[1] |= data_[2] << hi;
            temp.data_[2] |= data_[3] << hi;
            break;
        case 1:
            temp.data_[0] |= data_[1] >> lo;
            temp.data_[1] |= data_[2] >> lo;
            temp.data_[2] |= data_[3] >> lo;

            temp.data_[0] |= data_[2] << hi;
            temp.data_[1] |= data_[3] << hi;
            break;
        case 2:
            temp.data_[0] |= data_[2] >> lo;
            temp.data_[1] |= data_[3] >> lo;

            temp.data_[0] |= data_[3] << hi;
            break;
        case 3:
            temp.data_[1] |= data_[3] >> lo;
            break;
        default:
            break;
        }
        *this = temp;
        return *this;
    }
    inline constexpr __int128_data &operator=(const __int128_data &data) noexcept
    {
        data_ = data.data_;
        return *this;
    }
    inline constexpr __int128_data &add_s(const uint64_t index, const uint32_t &data) noexcept
    {
        uint64_t sum = data;
        switch (index)
        {
        case 0:
            sum += data_[0];
            data_[0] = sum;
            sum >>= 32;
        case 1:
            sum += data_[1];
            data_[1] = sum;
            sum >>= 32;
        case 2:
            sum += data_[2];
            data_[2] = sum;
            sum >>= 32;
        case 3:
            sum += data_[3];
            data_[3] = sum;
            // sum >>= 32;
            break;
        }
        return *this;
    }
    inline constexpr __int128_data &add_s(const uint64_t index, const uint64_t &data) noexcept
    {
        uint64_t sum1 = data & 0xffffffff;
        uint64_t sum2 = data >> 32;
        switch (index)
        {
        case 0:
            sum1 += data_[0];
            data_[0] = sum1;

            sum2 += sum1 >> 32;
            sum1 = sum2;
        case 1:
            sum1 += data_[1];
            data_[1] = sum1;

            sum2 += sum1 >> 32;
            sum1 = sum2;
        case 2:
            sum1 += data_[2];
            data_[2] = sum1;

            sum2 += sum1 >> 32;
            sum1 = sum2;
        case 3:
            sum1 += data_[3];
            data_[3] = sum1;

            /*		sum2 += sum1 >> 32;
                    sum1 = sum2;*/
            break;
        }
        return *this;
    }
    inline constexpr __int128_data &add(const __int128_data &data) noexcept
    {
        uint64_t sum = 0;

        sum = data_[0];
        sum += data.data_[0];
        data_[0] = sum;
        sum >>= 32;

        sum += data_[1];
        sum += data.data_[1];
        data_[1] = sum;
        sum >>= 32;

        sum += data_[2];
        sum += data.data_[2];
        data_[2] = sum;
        sum >>= 32;

        sum += data_[3];
        sum += data.data_[3];
        data_[3] = sum;
        // sum >>= 32;

        return *this;
    }
    inline constexpr __int128_data &sub(const __int128_data &data) noexcept
    {
        uint32_t borrow = 0;
        for (size_t i = 0; i < 4; ++i)
        {
            int64_t current = data_[i];
            current -= data.data_[i];
            current -= borrow;
            if (current < 0)
            {
                borrow = 1;
                current += (1ULL << 32); // 补偿借位（2^32）
            }
            else
            {
                borrow = 0;
            }
            data_[i] = current;
        }
        return *this;
    }
    inline constexpr __int128_data &mul(const __int128_data &data) noexcept
    {
        __int128_data result;
        uint64_t temp[8] = {0}; // 临时存储中间结果

        // 计算所有位乘积
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                uint64_t product = static_cast<uint64_t>(data_[i]) * data.data_[j];
                temp[i + j] += product & 0xFFFFFFFF;
                temp[i + j + 1] += product >> 32;
            }
        }

        // 处理进位
        for (int i = 0; i < 3; ++i)
        {
            temp[i + 1] += temp[i] >> 32;
            temp[i] &= 0xFFFFFFFF;
        }

        // 复制结果
        for (int i = 0; i < 4; ++i)
        {
            data_[i] = static_cast<uint32_t>(temp[i]);
        }
        return *this;
    }
    // 函数为下面基于 Knuth “长除法” 的纯软件实现。
    // data_ 存储为小端 32‑bit limb：data_[0] 是最低 32 位，data_[3] 是最高 32 位。
    inline constexpr __int128_data &div(const __int128_data &D, __int128_data &Q)
    {
        // constexpr clz32
        constexpr auto clz32 = [](uint32_t x)
        {
            int n = 0;
            while (n < 32 && ((x & 0x80000000U) == 0))
            {
                x <<= 1;
                ++n;
            }
            return n;
        };

        // 1) 除零
        if (D.empty())
        {
            // return *this; // 或抛异常
            throw std::domain_error("division by zero");
        }

        // 2) 单 limb 优化：除数只有 data_[0]
        if (D.data_[1] == 0 && D.data_[2] == 0 && D.data_[3] == 0)
        {
            uint32_t d = D.data_[0];
            uint64_t rem = 0;
            __int128_data q{0, 0};

            // 注意：展开 i=3→0
            {
                uint64_t cur = (rem << 32) | data_[3];
                uint32_t qi = uint32_t(cur / d);
                rem = cur - uint64_t(qi) * d;
                q.data_[3] = qi;
            }
            {
                uint64_t cur = (rem << 32) | data_[2];
                uint32_t qi = uint32_t(cur / d);
                rem = cur - uint64_t(qi) * d;
                q.data_[2] = qi;
            }
            {
                uint64_t cur = (rem << 32) | data_[1];
                uint32_t qi = uint32_t(cur / d);
                rem = cur - uint64_t(qi) * d;
                q.data_[1] = qi;
            }
            {
                uint64_t cur = (rem << 32) | data_[0];
                uint32_t qi = uint32_t(cur / d);
                rem = cur - uint64_t(qi) * d;
                q.data_[0] = qi;
            }

            // 写回余数和商
            data_ = {uint32_t(rem), 0u, 0u, 0u};
            Q = q;
            return *this;
        }

        // 3) 通用 Knuth‑D（base=2^32），n=m=3，全展开
        // 3.1 归一化
        int s = clz32(D.data_[3]);
        uint32_t U0 = (s == 0 ? data_[0] : (data_[0] << s));
        uint32_t U1 = (s == 0 ? data_[1] : (data_[1] << s) | (data_[0] >> (32 - s)));
        uint32_t U2 = (s == 0 ? data_[2] : (data_[2] << s) | (data_[1] >> (32 - s)));
        uint32_t U3 = (s == 0 ? data_[3] : (data_[3] << s) | (data_[2] >> (32 - s)));
        uint32_t U4 = (s == 0 ? 0 : (data_[3] >> (32 - s)));

        uint32_t V0 = (s == 0 ? D.data_[0] : (D.data_[0] << s));
        uint32_t V1 = (s == 0 ? D.data_[1] : (D.data_[1] << s) | (D.data_[0] >> (32 - s)));
        uint32_t V2 = (s == 0 ? D.data_[2] : (D.data_[2] << s) | (D.data_[1] >> (32 - s)));
        uint32_t V3 = (s == 0 ? D.data_[3] : (D.data_[3] << s) | (D.data_[2] >> (32 - s)));

        uint32_t ql3 = 0, ql2 = 0, ql1 = 0, ql0 = 0;

        // 4) j=1
        {
            // 4.1 估商 q̂ = (U4:U3) ÷ V3
            uint64_t num = (uint64_t(U4) << 32) | U3;
            uint64_t den = V3;
            uint64_t qhat = num / den;
            uint64_t rhat = num - qhat * den;
            if (qhat >= (1ULL << 32))
            {
                qhat = (1ULL << 32) - 1;
                rhat = num - qhat * den;
            }
            // 修正
            while (qhat * V2 > ((rhat << 32) | U2))
            {
                --qhat;
                rhat += den;
                if (rhat >= (1ULL << 32))
                    break;
            }
            uint32_t q3 = uint32_t(qhat);

            // 4.2 U[1..4] -= q̂ * V[0..3]
            int64_t borrow = 0;
            // i=0
            {
                uint64_t p = qhat * V0;
                uint64_t sub = uint64_t(U1) - (p & 0xFFFFFFFF) - borrow;
                U1 = uint32_t(sub);
                borrow = (p >> 32) + ((sub >> 63) & 1);
            }
            // i=1
            {
                uint64_t p = qhat * V1;
                uint64_t sub = uint64_t(U2) - (p & 0xFFFFFFFF) - borrow;
                U2 = uint32_t(sub);
                borrow = (p >> 32) + ((sub >> 63) & 1);
            }
            // i=2
            {
                uint64_t p = qhat * V2;
                uint64_t sub = uint64_t(U3) - (p & 0xFFFFFFFF) - borrow;
                U3 = uint32_t(sub);
                borrow = (p >> 32) + ((sub >> 63) & 1);
            }
            // i=3
            {
                uint64_t p = qhat * V3;
                uint64_t sub = uint64_t(U4) - (p & 0xFFFFFFFF) - borrow;
                U4 = uint32_t(sub);
                borrow = (p >> 32) + ((sub >> 63) & 1);
            }
            // underflow?
            if (borrow)
            {
                // 回退：U[1..4] += V[0..3]
                uint64_t carry = 0;
                U1 = uint32_t(uint64_t(U1) + V0 + carry);
                carry = (uint64_t(U1) < V0 || carry && U1 == V0);
                U2 = uint32_t(uint64_t(U2) + V1 + carry);
                carry = (uint64_t(U2) < V1 || carry && U2 == V1);
                U3 = uint32_t(uint64_t(U3) + V2 + carry);
                carry = (uint64_t(U3) < V2 || carry && U3 == V2);
                U4 = uint32_t(uint64_t(U4) + V3 + carry);
                q3 -= 1;
            }
            ql3 = q3;
        }

        // 5) j=0（同理，只是 i=0..3）
        {
            uint64_t num = (uint64_t(U3) << 32) | U2;
            uint64_t den = V3;
            uint64_t qhat = num / den, rhat = num - qhat * den;
            if (qhat >= (1ULL << 32))
            {
                qhat = (1ULL << 32) - 1;
                rhat = num - qhat * den;
            }
            while (qhat * V2 > ((rhat << 32) | U1))
            {
                --qhat;
                rhat += den;
                if (rhat >= (1ULL << 32))
                    break;
            }
            uint32_t q2 = uint32_t(qhat);

            int64_t borrow = 0;
            {
                uint64_t p = qhat * V0;
                uint64_t sub = uint64_t(U2) - (p & 0xFFFFFFFF) - borrow;
                U2 = uint32_t(sub);
                borrow = (p >> 32) + ((sub >> 63) & 1);
            }
            {
                uint64_t p = qhat * V1;
                uint64_t sub = uint64_t(U3) - (p & 0xFFFFFFFF) - borrow;
                U3 = uint32_t(sub);
                borrow = (p >> 32) + ((sub >> 63) & 1);
            }
            {
                uint64_t p = qhat * V2;
                uint64_t sub = uint64_t(U4) - (p & 0xFFFFFFFF) - borrow;
                U4 = uint32_t(sub);
                borrow = (p >> 32) + ((sub >> 63) & 1);
            }
            {
                uint64_t p = qhat * V3;
                uint64_t sub = uint64_t(U0) - (p & 0xFFFFFFFF) - borrow;
                U0 = uint32_t(sub);
                borrow = (p >> 32) + ((sub >> 63) & 1);
            }

            if (borrow)
            {
                uint64_t carry = 0;
                U2 = uint32_t(uint64_t(U2) + V0 + carry);
                carry = (uint64_t(U2) < V0 || carry && U2 == V0);
                U3 = uint32_t(uint64_t(U3) + V1 + carry);
                carry = (uint64_t(U3) < V1 || carry && U3 == V1);
                U4 = uint32_t(uint64_t(U4) + V2 + carry);
                carry = (uint64_t(U4) < V2 || carry && U4 == V2);
                U0 = uint32_t(uint64_t(U0) + V3 + carry);
                q2 -= 1;
            }
            ql2 = q2;
        }

        // 6) 解除归一化，生成余数 R0..R3
        uint32_t R0 = s == 0 ? U0 : (U0 >> s) | (U1 << (32 - s));
        uint32_t R1 = s == 0 ? U1 : (U1 >> s) | (U2 << (32 - s));
        uint32_t R2 = s == 0 ? U2 : (U2 >> s) | (U3 << (32 - s));
        uint32_t R3 = s == 0 ? U3 : (U3 >> s) | (U4 << (32 - s));

        data_ = {R0, R1, R2, R3};
        Q.data_ = {ql0, ql1, ql2, ql3};
        return *this;
    }
    inline constexpr bool empty() const noexcept
    {
        return data_[0] == 0 and data_[1] == 0 and data_[2] == 0 and data_[3] == 0;
    }
    inline constexpr bool operator<(const __int128_data &num) const noexcept
    {
        if (data_[3] < num.data_[3])
        {
            return true;
        }
        else if (data_[3] == num.data_[3])
        {
            if (data_[2] < num.data_[2])
            {
                return true;
            }
            else if (data_[2] == num.data_[2])
            {
                if (data_[1] < num.data_[1])
                {
                    return true;
                }
                else if (data_[1] == num.data_[1])
                {
                    if (data_[0] < num.data_[0])
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

public:
    ::std::array<uint32_t, 4> data_;
};
class uint128
{
public:
    // 构造函数
    inline constexpr uint128() noexcept
        : data_({0})
    {
    }
    inline constexpr uint128(const uint64_t lo, const uint64_t hi = 0) noexcept
        : data_({lo, hi})
    {
    }
    inline constexpr uint128(const uint128 &data) noexcept
        : data_(data.data_)
    {
    }
    inline constexpr uint128(const ::std::string_view str)
        : data_({0})
    {
        for (char c : str)
        {
            if (c < '0' || c > '9')
                throw std::invalid_argument("Invalid character in numeric string");
            *this *= 10;
            *this += (c - '0');
        }
    }

    // 赋值运算符
    inline constexpr uint128 &operator=(const uint128 &data) noexcept
    {
        data_ = data.data_;
        return *this;
    }
    inline constexpr uint128 &operator=(const uint64_t &data) noexcept
    {
        data_ = {0};
        data_.add_s(0, data);
        return *this;
    }
    inline constexpr uint128 &operator=(const ::std::string_view str) noexcept
    {
        uint128 temp{str};
        *this = temp;
        return *this;
    }

    // 单目运算符
    inline constexpr uint128 operator~() const noexcept
    {
        uint128 result;
        result.data_.data_[0] = ~data_.data_[0];
        result.data_.data_[1] = ~data_.data_[1];
        result.data_.data_[2] = ~data_.data_[2];
        result.data_.data_[3] = ~data_.data_[3];
        return result;
    }

    // 算术运算符
    inline constexpr uint128 &operator+=(const uint128 &data) noexcept
    {
        data_.add(data.data_);
        return *this;
    }
    inline constexpr uint128 operator+(const uint128 &data) const noexcept
    {
        auto temp = *this;
        temp += data;
        return temp;
    }
    inline constexpr uint128 &operator-=(const uint128 &data) noexcept
    {
        data_.sub(data.data_);
        return *this;
    }
    inline constexpr uint128 operator-(const uint128 &data) const noexcept
    {
        auto temp = *this;
        temp -= data;
        return temp;
    }
    inline constexpr uint128 &operator*=(const uint128 &data) noexcept
    {
        data_.mul(data.data_);
        return *this;
    }
    inline constexpr uint128 operator*(const uint128 &data) const noexcept
    {
        auto temp = *this;
        temp *= data;
        return temp;
    }
    inline constexpr uint128 &operator/=(const uint128 &data) noexcept
    {
        uint128 temp1;
        data_.div(data.data_, temp1.data_);
        *this = temp1;
        return *this;
    }
    inline constexpr uint128 operator/(const uint128 &data) const noexcept
    {
        auto temp1 = *this;
        temp1 /= data;
        return temp1;
    }
    inline constexpr uint128 &operator%=(const uint128 &data) noexcept
    {
        uint128 temp1;
        data_.div(data.data_, temp1.data_);
        return *this;
    }
    inline constexpr uint128 operator%(const uint128 &data) const noexcept
    {
        auto temp1 = *this;
        temp1 %= data;
        return temp1;
    }

    // 比较运算符
    inline constexpr bool operator==(const uint128 &data) const noexcept
    {
        return data_.data_ == data.data_.data_;
    }
    inline constexpr bool operator!=(const uint128 &data) const noexcept
    {
        return data_.data_ != data.data_.data_;
    }
    inline constexpr bool operator<(const uint128 &data) const noexcept
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (data_.data_[i] < data.data_.data_[i])
                return true;
            else if (data_.data_[i] > data.data_.data_[i])
                return false;
        }
        return false;
    }
    inline constexpr bool operator<=(const uint128 &data) const noexcept
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (data_.data_[i] < data.data_.data_[i])
                return true;
            else if (data_.data_[i] > data.data_.data_[i])
                return false;
        }
        return true;
    }
    inline constexpr bool operator>(const uint128 &data) const noexcept
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (data_.data_[i] > data.data_.data_[i])
                return true;
            else if (data_.data_[i] < data.data_.data_[i])
                return false;
        }
        return false;
    }
    inline constexpr bool operator>=(const uint128 &data) const noexcept
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (data_.data_[i] > data.data_.data_[i])
                return true;
            else if (data_.data_[i] < data.data_.data_[i])
                return false;
        }
        return true;
    }

    // 位运算符
    inline constexpr uint128 &operator&=(const uint128 &data) noexcept
    {
        data_.data_[0] &= data.data_.data_[0];
        data_.data_[1] &= data.data_.data_[1];
        data_.data_[2] &= data.data_.data_[2];
        data_.data_[3] &= data.data_.data_[3];
        return *this;
    }
    inline constexpr uint128 operator&(const uint128 &data) const noexcept
    {
        auto temp = *this;
        temp &= data;
        return temp;
    }
    inline constexpr uint128 &operator|=(const uint128 &data) noexcept
    {
        data_.data_[0] |= data.data_.data_[0];
        data_.data_[1] |= data.data_.data_[1];
        data_.data_[2] |= data.data_.data_[2];
        data_.data_[3] |= data.data_.data_[3];
        return *this;
    }
    inline constexpr uint128 operator|(const uint128 &data) const noexcept
    {
        auto temp = *this;
        temp |= data;
        return temp;
    }
    inline constexpr uint128 &operator^=(const uint128 &data) noexcept
    {
        data_.data_[0] ^= data.data_.data_[0];
        data_.data_[1] ^= data.data_.data_[1];
        data_.data_[2] ^= data.data_.data_[2];
        data_.data_[3] ^= data.data_.data_[3];
        return *this;
    }
    inline constexpr uint128 operator^(const uint128 &data) const noexcept
    {
        auto temp = *this;
        temp ^= data;
        return temp;
    }
    inline constexpr uint128 &operator<<=(const uint64_t &shift) noexcept
    {
        data_ <<= shift;
        return *this;
    }
    inline constexpr uint128 operator<<(const uint64_t &shift) const noexcept
    {
        uint128 temp = *this;
        temp <<= shift;
        return temp;
    }
    inline constexpr uint128 &operator>>=(const uint64_t &shift) noexcept
    {
        data_ >>= shift;
        return *this;
    }
    inline constexpr uint128 operator>>(const uint64_t &shift) const noexcept
    {
        uint128 temp = *this;
        temp >>= shift;
        return temp;
    }

    // 转换函数
    inline constexpr explicit operator uint64_t() const noexcept
    {
        return uint64_t(data_.data_[0]) | (uint64_t(data_.data_[1]) << 32);
    }
    inline constexpr explicit operator int64_t() const noexcept
    {
        return uint64_t(data_.data_[0]) | (uint64_t(data_.data_[1]) << 32);
    }
    inline constexpr explicit operator double() const noexcept
    {
        return static_cast<double>(data_.data_[0]) +
               static_cast<double>(data_.data_[1]) * (4.294967296e9) +
               static_cast<double>(data_.data_[2]) * (4.294967296e9 * 4.294967296e9) +
               static_cast<double>(data_.data_[3]) * (4.294967296e9 * 4.294967296e9 * 4.294967296e9);
    }
    inline constexpr explicit operator float() const noexcept
    {
        return static_cast<double>(data_.data_[0]) +
               static_cast<double>(data_.data_[1]) * (4.294967296e9) +
               static_cast<double>(data_.data_[2]) * (4.294967296e9 * 4.294967296e9) +
               static_cast<double>(data_.data_[3]) * (4.294967296e9 * 4.294967296e9 * 4.294967296e9);
    }

    // 工具函数
    inline static constexpr uint128 mask_high_bits(uint64_t n) noexcept
    {
        if (n >= 128)
            return ~uint128();
        return (~uint128()) << (128 - n);
    }
    inline std::string to_string() const
    {
        std::string str;
        uint128 temp = *this;
        do
        {
            str.push_back('0' + uint64_t(temp % 10));
            temp /= 10;
        } while (temp != 0);
        std::reverse(str.begin(), str.end());
        return str.empty() ? "0" : str;
    }
    inline constexpr bool is_zero() const noexcept
    {
        return data_.empty();
    }

    friend class int128;
    inline friend std::ostream &operator<<(std::ostream &, const uint128 &);
    inline friend std::istream &operator>>(std::istream &, uint128 &);

private:
    __int128_data data_;
};
inline std::ostream &operator<<(std::ostream &os, const uint128 &value)
{
    return os << value.to_string();
}
inline std::istream &operator>>(std::istream &is, uint128 &value)
{
    std::string str;
    is >> str;
    value = uint128(str);
    return is;
}
using u128 = uint128;
inline constexpr u128 u128_max{"340282366920938463463374607431768211455"};
inline constexpr u128 u128_min{"0"};

class int128
{
public:
    // 构造函数
    inline constexpr int128() noexcept = default;
    inline constexpr int128(const int64_t value) noexcept
        : data_(static_cast<uint64_t>(value),
                value < 0 ? 0xFFFFFFFFFFFFFFFFULL : 0)
    {
    }
    inline constexpr int128(const uint128 &value) noexcept
        : data_(value)
    {
    }
    inline constexpr int128(const std::string_view str)
    {
        bool negative = false;
        size_t start = 0;

        if (!str.empty() && (str[0] == '-' || str[0] == '+'))
        {
            negative = (str[0] == '-');
            start = 1;
        }

        for (size_t i = start; i < str.size(); ++i)
        {
            char c = str[i];
            if (c < '0' || c > '9')
                throw std::invalid_argument("Invalid character in numeric string");
            data_ = data_ * uint128(10) + uint128(c - '0');
        }

        if (negative)
        {
            *this = -(*this);
        }
    }

    // 赋值运算符
    inline constexpr int128 &operator=(const int128 &data) noexcept
    {
        data_ = data.data_;
        return *this;
    }
    inline constexpr int128 &operator=(const uint128 &data) noexcept
    {
        data_ = data;
        return *this;
    }
    inline constexpr int128 &operator=(const std::string_view &str) noexcept
    {
        data_ = uint128(str);
        return *this;
    }

    // 单目运算符
    inline constexpr int128 operator-() const noexcept
    {
        return int128{(~data_ + uint128(1))};
    }
    inline constexpr int128 operator~() const noexcept
    {
        return int128{~data_};
    }

    // 算术运算符
    inline constexpr int128 &operator+=(const int128 &rhs) noexcept
    {
        data_ += rhs.data_;
        return *this;
    }
    inline constexpr int128 operator+(const int128 &rhs) const noexcept
    {
        auto temp = *this;
        temp += rhs;
        return temp;
    }
    inline constexpr int128 &operator-=(const int128 &rhs) noexcept
    {
        data_ -= rhs.data_;
        return *this;
    }
    inline constexpr int128 operator-(const int128 &rhs) const noexcept
    {
        auto temp = *this;
        temp -= rhs;
        return temp;
    }
    inline constexpr int128 &operator*=(const int128 &rhs) noexcept
    {
        const bool neg = is_negative() ^ rhs.is_negative();
        uint128 abs_lhs = abs().data_;
        uint128 abs_rhs = rhs.abs().data_;
        abs_lhs *= abs_rhs;
        data_ = neg ? (-(*(int128 *)&abs_lhs)).data_ : abs_lhs;
        return *this;
    }
    inline constexpr int128 operator*(const int128 &rhs) const noexcept
    {
        auto temp = *this;
        temp *= rhs;
        return temp;
    }
    inline constexpr int128 &operator/=(const int128 &rhs)
    {
        if (rhs == 0)
            throw std::runtime_error("Division by zero");

        const bool neg = is_negative() ^ rhs.is_negative();
        uint128 abs_lhs = abs().data_;
        uint128 abs_rhs = rhs.abs().data_;
        abs_lhs /= abs_rhs;
        data_ = neg ? (-(*(int128 *)&abs_lhs)).data_ : abs_lhs;
        return *this;
    }
    inline constexpr int128 operator/(const int128 &rhs) const noexcept
    {
        auto temp = *this;
        temp /= rhs;
        return temp;
    }
    inline constexpr int128 &operator%=(const int128 &rhs)
    {
        if (rhs == 0)
            throw std::runtime_error("Division by zero");

        bool is_negative = this->is_negative();
        // 计算被除数和除数的绝对值
        uint128 abs_value = abs().data_;
        const uint128 abs_rhs = rhs.abs().data_;
        // 计算余数的绝对值
        uint128 quotient_temp;
        abs_value.data_.div(abs_rhs.data_, quotient_temp.data_); // abs_value现在保存余数的绝对值
        // 设置结果，并根据原符号调整余数符号
        data_ = is_negative ? (-int128(abs_value)).data_ : abs_value;
        return *this;
    }
    inline constexpr int128 operator%(const int128 &rhs) const noexcept
    {
        auto temp = *this;
        temp %= rhs;
        return temp;
    }

    // 比较运算符
    inline constexpr bool operator==(const int128 &rhs) const noexcept
    {
        return data_ == rhs.data_;
    }
    inline constexpr bool operator!=(const int128 &rhs) const noexcept
    {
        return data_ != rhs.data_;
    }
    inline constexpr bool operator<(const int128 &rhs) const noexcept
    {
        if (is_negative() != rhs.is_negative())
        {
            return is_negative();
        }
        return is_negative() ? (data_ > rhs.data_) : (data_ < rhs.data_);
    }
    inline constexpr bool operator>(const int128 &rhs) const noexcept
    {
        return rhs < *this;
    }
    inline constexpr bool operator<=(const int128 &rhs) const noexcept
    {
        return !(*this > rhs);
    }
    inline constexpr bool operator>=(const int128 &rhs) const noexcept
    {
        return !(*this < rhs);
    }

    // 位运算符
    inline constexpr int128 &operator&=(const int128 &data) noexcept
    {
        data_.data_.data_[0] &= data.data_.data_.data_[0];
        data_.data_.data_[1] &= data.data_.data_.data_[1];
        data_.data_.data_[2] &= data.data_.data_.data_[2];
        data_.data_.data_[3] &= data.data_.data_.data_[3];
        return *this;
    }
    inline constexpr int128 operator&(const int128 &data) const noexcept
    {
        auto temp = *this;
        temp &= data;
        return temp;
    }
    inline constexpr int128 &operator|=(const int128 &data) noexcept
    {
        data_.data_.data_[0] |= data.data_.data_.data_[0];
        data_.data_.data_[1] |= data.data_.data_.data_[1];
        data_.data_.data_[2] |= data.data_.data_.data_[2];
        data_.data_.data_[3] |= data.data_.data_.data_[3];
        return *this;
    }
    inline constexpr int128 operator|(const int128 &data) const noexcept
    {
        auto temp = *this;
        temp |= data;
        return temp;
    }
    inline constexpr int128 &operator^=(const int128 &data) noexcept
    {
        data_.data_.data_[0] ^= data.data_.data_.data_[0];
        data_.data_.data_[1] ^= data.data_.data_.data_[1];
        data_.data_.data_[2] ^= data.data_.data_.data_[2];
        data_.data_.data_[3] ^= data.data_.data_.data_[3];
        return *this;
    }
    inline constexpr int128 operator^(const int128 &data) const noexcept
    {
        auto temp = *this;
        temp ^= data;
        return temp;
    }
    inline constexpr int128 operator<<(uint64_t shift) const noexcept
    {
        return int128(data_ << shift);
    }
    inline constexpr int128 &operator<<=(uint64_t shift) noexcept
    {
        data_ <<= shift;
        return *this;
    }
    inline constexpr int128 operator>>(uint64_t shift) const noexcept
    {
        // 算术右移
        if (is_negative())
        {
            int128 result = *this;
            result.data_ >>= shift;
            result.data_ |= uint128::mask_high_bits(shift);
            return result;
        }
        return int128(data_ >> shift);
    }
    inline constexpr int128 &operator>>=(uint64_t shift) noexcept
    {
        *this = *this >> shift;
        return *this;
    }

    // 转换函数
    inline explicit operator int64_t() const noexcept
    {
        const uint64_t val = static_cast<uint64_t>(data_);
        return is_negative() ? -static_cast<int64_t>(~val + 1) : val;
    }
    inline explicit operator double() const noexcept
    {
        return is_negative() ? -static_cast<double>(-*this) : static_cast<double>(data_);
    }

    // 工具函数
    inline constexpr bool is_negative() const noexcept
    {
        return (data_.data_.data_[3] & 0x80000000) != 0;
    }
    inline constexpr int128 abs() const noexcept
    {
        return is_negative() ? -*this : *this;
    }
    inline std::string to_string() const
    {
        if (is_negative())
        {
            return "-" + (-*this).data_.to_string();
        }
        return data_.to_string();
    }
    inline constexpr bool is_zero() const noexcept
    {
        return data_.data_.empty();
    }
    inline friend std::ostream &operator<<(std::ostream &, const int128 &);
    inline friend std::istream &operator>>(std::istream &, int128 &);

private:
    uint128 data_;
};
inline std::ostream &operator<<(std::ostream &os, const int128 &value)
{
    return os << value.to_string();
}
inline std::istream &operator>>(std::istream &is, int128 &value)
{
    std::string str;
    is >> str;
    value = int128(str);
    return is;
}
using i128 = int128;
inline constexpr i128 i128_max{"340282366920938463463374607431768211455"};
inline constexpr i128 i128_min{"0"};
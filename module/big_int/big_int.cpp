#include "big_int.hpp"
#include <iostream>
#include <bitset>
#include <sstream>

namespace __NAMESPACE_NAME__
{
    big_int::big_int()
    {
        this->sign_ = sign::positive;
        this->max_bit_lenth_ = 65536;
        this->data_.reserve(2);
        this->data_.push_back(0);
    }

    big_int::big_int(const std::string_view &str)
    {
        big_int temp1;
        constexpr u32 num_block_lenth = 18;
        const u32 block = str.size() / num_block_lenth;
        const u32 skip = str.size() % num_block_lenth;

        std::string temp2{str.substr(0, skip)};
        temp1 += big_int(atoll(temp2.c_str()));
        for (size_t i = 0; i < block; i++)
        {
            temp1 *= 1'000'000'000'000'000'000;
            temp2 = str.substr(skip + i * num_block_lenth, num_block_lenth);
            temp1 += big_int(atoll(temp2.c_str()));
        }
        *this = temp1;
        return;
    }
    big_int::big_int(const i64 num)
    {
        u64 abs_num = num < 0 ? -num : num;
        u32 high = abs_num >> 32;
        u32 low = abs_num & 0xffffffff;
        // std::cout << "num: " << num << " abs_num: " << abs_num << " low: " << low << " high: " << high << "\n";
        // std::cout << "bit: " << std::bitset<32>(high).to_string() << std::bitset<32>(low).to_string() << '\n';
        if (num == 0)
        {
            this->sign_ = sign::positive;
        }
        else if (num < 0)
        {
            this->sign_ = sign::negative;
        }
        else
        {
            this->sign_ = sign::positive;
        }

        this->data_.reserve(2);
        this->data_.push_back(low);
        if (high != 0)
        {
            this->data_.push_back(high);
        }

        this->max_bit_lenth_ = 65536;
    }
    big_int::big_int(const big_int &other)
    {
        this->data_ = other.data_;
        this->sign_ = other.sign_;
        this->max_bit_lenth_ = other.max_bit_lenth_;
        return;
    }
    big_int::big_int(big_int &&other)
    {
        this->data_.swap(other.data_);
        other.data_.clear();
        this->sign_ = other.sign_;
        other.sign_ = sign::uninitialized;
        this->max_bit_lenth_ = other.max_bit_lenth_;
        return;
    }
    big_int::~big_int()
    {
    }
    void big_int::swap(big_int &other)
    {
        this->data_.swap(other.data_);
        auto tmp = this->sign_;
        this->sign_ = other.sign_;
        other.sign_ = tmp;
        auto tmp2 = this->max_bit_lenth_;
        this->max_bit_lenth_ = other.max_bit_lenth_;
        other.max_bit_lenth_ = tmp2;
        return;
    }
    big_int &big_int::operator=(const i64 num)
    {
        u64 abs_num = num < 0 ? -num : num;
        u32 high = abs_num >> 32;
        u32 low = abs_num & 0xffffffff;

        if (num == 0)
        {
            this->sign_ = sign::positive;
        }
        else if (num < 0)
        {
            this->sign_ = sign::negative;
        }
        else
        {
            this->sign_ = sign::positive;
        }

        this->data_.reserve(2);
        this->data_.clear();
        this->data_.push_back(low);
        if (high != 0)
        {
            this->data_.push_back(high);
        }

        this->max_bit_lenth_ = 65536;
        return *this;
    }
    big_int &big_int::operator=(const big_int &other)
    {
        if (this != &other)
        {
            this->data_ = other.data_;
            this->sign_ = other.sign_;
            this->max_bit_lenth_ = other.max_bit_lenth_;
        }
        return *this;
    }
    big_int &big_int::operator=(big_int &&other)
    {
        if (this != &other)
        {
            this->data_.swap(other.data_);
            other.data_.clear();
            this->sign_ = other.sign_;
            other.sign_ = sign::uninitialized;
            this->max_bit_lenth_ = other.max_bit_lenth_;
        }
        return *this;
    }
    bool big_int::operator==(const big_int &other) const
    {
        auto &a = *this;
        auto &b = other;

        i8 a_sign = 0;
        if (a.sign_ == sign::positive)
        {
            a_sign = 1;
        }
        else if (a.sign_ == sign::negative)
        {
            a_sign = -1;
        }
        else
        {
            // return -1;
            throw ::std::runtime_error("big_int operator== a sign err");
        }
        i8 b_sign = 0;
        if (b.sign_ == sign::positive)
        {
            b_sign = 1;
        }
        else if (b.sign_ == sign::negative)
        {
            b_sign = -1;
        }
        else
        {
            // return -1;
            throw ::std::runtime_error("big_int operator== b sign err");
        }

        i32 size_a = a.data_.size();
        i32 size_b = b.data_.size();

        if (a_sign == b_sign and
            size_a == size_b)
        {
            return this->data_ == other.data_;
        }
        return false;
    }
    bool big_int::operator!=(const big_int &other) const
    {
        return !(*this == other);
    }
    bool big_int::operator<(const big_int &other) const
    {
        auto temp = less(*this, other);
        if (temp == -1)
        {
            return false;
        }
        return temp;
    }
    bool big_int::operator>(const big_int &other) const
    {
        auto temp = less(other, *this);
        if (temp == -1)
        {
            return false;
        }
        return temp;
    }
    bool big_int::operator<=(const big_int &other) const
    {
        auto temp = less(other, *this);
        if (temp == -1)
        {
            return false;
        }
        return !temp;
    }
    bool big_int::operator>=(const big_int &other) const
    {
        auto temp = less(*this, other);
        if (temp == -1)
        {
            return false;
        }
        return !temp;
    }

    big_int big_int::operator+() const
    {
        auto temp = *this;
        return temp;
    }

    big_int big_int::operator-() const
    {
        auto temp = *this;
        if (temp.sign_ == sign::positive)
        {
            temp.sign_ = sign::negative;
        }
        else if (temp.sign_ == sign::negative)
        {
            temp.sign_ = sign::positive;
        }
        else
        {
            // return -1;
            throw ::std::runtime_error("big_int operator- sign err");
        }
        return temp;
    }

    big_int &big_int::operator++()
    {
        *this += 1;
        return *this;
    }

    big_int &big_int::operator--()
    {
        *this -= 1;
        return *this;
    }

    big_int big_int::operator++(int)
    {
        auto temp = *this;
        ++*this;
        return temp;
    }

    big_int big_int::operator--(int)
    {
        auto temp = *this;
        --*this;
        return temp;
    }

    big_int &big_int::operator<<=(const u32 num)
    {
        if (num == 0)
        {
            return *this;
        }

        u32 old_block = this->data_.size();
        u32 block = num / 32; // 整块偏移量
        u32 bit = num % 32;   // 剩余位偏移量

        if (bit == 0)
        {
            // 如果没有剩余位偏移，直接扩展数据块
            this->data_.resize(old_block + block, 0);
            for (u32 i = old_block; i > 0; --i)
            {
                this->data_[i + block - 1] = this->data_[i - 1];
            }
            std::fill(this->data_.begin(), this->data_.begin() + block, 0);
        }
        else
        {
            // 处理非对齐位移
            this->data_.resize(old_block + block + 1, 0); // 预留空间
            for (u32 i = old_block; i > 0; --i)
            {
                this->data_[i + block] = (this->data_[i - 1] << bit) |
                                         ((i > 1) ? (this->data_[i - 2] >> (32 - bit)) : 0);
            }
            std::fill(this->data_.begin(), this->data_.begin() + block, 0);
        }

        // 格式化数据（去除多余的零）
        // this->format();

        return *this;
    }
    big_int &big_int::operator>>=(const u32 num)
    {
        if (num == 0)
        {
            return *this;
        }

        u32 old_block = this->data_.size();
        u32 block = num / 32; // 整块偏移量
        u32 bit = num % 32;   // 剩余位偏移量

        if (block >= old_block)
        {
            // 如果偏移量超过数据长度，清空数据并重置为 0
            this->data_.clear();
            this->data_.push_back(0);
            this->sign_ = sign::positive;
            return *this;
        }

        if (bit == 0)
        {
            // 如果没有剩余位偏移，直接移动数据块
            for (u32 i = 0; i < old_block - block; ++i)
            {
                this->data_[i] = this->data_[i + block];
            }
            this->data_.resize(old_block - block);
        }
        else
        {
            // 处理非对齐位移
            for (u32 i = 0; i < old_block - block - 1; ++i)
            {
                this->data_[i] = (this->data_[i + block] >> bit) |
                                 (this->data_[i + block + 1] << (32 - bit));
            }
            // 处理最后一个块
            this->data_[old_block - block - 1] = this->data_[old_block - 1] >> bit;

            // // 如果是负数，扩展符号位
            // if (this->sign_ == sign::negative)
            // {
            //     this->data_[old_block - block - 1] |= (0xFFFFFFFF << (32 - bit));
            // }

            this->data_.resize(old_block - block);
        }

        // 格式化数据（去除多余的零）
        this->format();

        return *this;
    }

    big_int &big_int::operator+=(const big_int &other)
    {
        auto a_sign = this->get_sign();
        auto b_sign = other.get_sign();

        if (a_sign == b_sign)
        {
            abs_add(*this, other);
        }
        else
        {
            bool sign__ = *this > other;
            auto &big = abs_big(*this, other);
            auto &small = abs_small(*this, other);
            if (&big == this)
            {
                abs_sub(*this, other);
                if (sign__)
                {
                    this->sign_ = sign::positive;
                }
                else
                {
                    this->sign_ = sign::negative;
                }
            }
            else
            {
                big_int temp = big;
                abs_sub(temp, small);
                if (sign__)
                {
                    temp.sign_ = sign::positive;
                }
                else
                {
                    temp.sign_ = sign::negative;
                }
                *this = std::move(temp);
            }
        }
        return *this;
    }

    big_int &big_int::operator-=(const big_int &other)
    {
        auto a_sign = this->get_sign();
        auto b_sign = other.get_sign();

        if (a_sign != b_sign)
        {
            abs_add(*this, other);
        }
        else
        {
            bool sign__ = *this > other;
            auto &big = abs_big(*this, other);
            auto &small = abs_small(*this, other);
            if (&big == this)
            {
                abs_sub(*this, other);
                if (sign__)
                {
                    this->sign_ = sign::positive;
                }
                else
                {
                    this->sign_ = sign::negative;
                }
            }
            else
            {
                big_int temp = big;
                abs_sub(temp, small);
                if (sign__)
                {
                    temp.sign_ = sign::positive;
                }
                else
                {
                    temp.sign_ = sign::negative;
                }
                *this = std::move(temp);
            }
        }
        return *this;
    }

    big_int &big_int::operator*=(const big_int &other)
    {
        auto a_sign = this->get_sign();
        auto b_sign = other.get_sign();

        if (a_sign != b_sign)
        {
            this->sign_ = sign::negative;
            abs_mul(*this, other);
        }
        else
        {
            this->sign_ = sign::positive;
            abs_mul(*this, other);
        }
        return *this;
    }

    big_int &big_int::operator/=(const big_int &other)
    {
        auto a_sign = this->get_sign();
        auto b_sign = other.get_sign();
        big_int tmp = 0;

        if (a_sign != b_sign)
        {
            this->sign_ = sign::negative;
            abs_div(*this, other, tmp);
        }
        else
        {
            this->sign_ = sign::positive;
            abs_div(*this, other, tmp);
        }
        this->swap(tmp);
        return *this;
    }

    big_int &big_int::operator%=(const big_int &other)
    {
        auto a_sign = this->get_sign();
        auto b_sign = other.get_sign();
        big_int tmp = 0;

        if (a_sign != b_sign)
        {
            this->sign_ = sign::negative;
            abs_div(*this, other, tmp);
        }
        else
        {
            this->sign_ = sign::positive;
            abs_div(*this, other, tmp);
        }
        return *this;
    }

    ::std::string big_int::to_dec() const
    {
        ::std::string out;
        if (this->sign_ == sign::uninitialized)
        {
            out += "no_init";
        }
        else if (this->sign_ == sign::not_a_number)
        {
            out += "NaN";
        }
        else if (this->sign_ == sign::positive)
        {
            // out += "+";
        }
        else if (this->sign_ == sign::negative)
        {
            out += "-";
        }
        else
        {
            throw ::std::runtime_error("big_int to_dec sign err");
        }

        if (this->data_.size() == 1)
        {
            out += ::std::to_string(this->data_[0]);
        }
        if (this->data_.size() == 2)
        {
            out += ::std::to_string(static_cast<u64>(this->data_[0]) + (static_cast<u64>(this->data_[1]) << 32));
        }
        if (this->data_.size() >= 3)
        {
            const big_int base = 1'000'000'000'000'000'000;
            // const big_int base = 10;
            big_int temp = *this;
            temp.sign_ = sign::positive;
            ::std::vector<::std::string> parts;
            big_int remainder;

            // 不断分解大数
            while (!temp.is_zero())
            {
                abs_div(temp, base, remainder);
                temp.data_.resize(2, 0);                                                                   // 计算商和余数
                parts.emplace_back(::std::to_string((u64)(temp.data_[0]) + ((u64)(temp.data_[1]) << 32))); // 存储余数
                temp.swap(remainder);
            }

            out += parts.back();
            // 拼接结果
            for (i32 i = parts.size() - 2; i >= 0; --i)
            {
                for (i32 j = 18 - parts[i].size(); j > 0; --j)
                {
                    out += '0';
                }
                out += parts[i];
            }
        }

        return out;
    }
    ::std::string big_int::to_hex() const
    {
        ::std::stringstream out;
        if (this->sign_ == sign::uninitialized)
        {
            out << "no_init";
        }
        else if (this->sign_ == sign::not_a_number)
        {
            out << "NaN";
        }
        else if (this->sign_ == sign::positive)
        {
            out << "+";
        }
        else if (this->sign_ == sign::negative)
        {
            out << "-";
        }
        else
        {
            throw ::std::runtime_error("big_int to_hex sign err");
        }

        out << ::std::hex;
        for (i32 i = this->data_.size() - 1; i >= 0; i--)
        {
            out << this->data_[i];
        }
        out << ::std::dec;

        return out.str();
    }
    ::std::string big_int::to_oct() const
    {
        ::std::stringstream out;
        if (this->sign_ == sign::uninitialized)
        {
            out << "no_init";
        }
        else if (this->sign_ == sign::not_a_number)
        {
            out << "NaN";
        }
        else if (this->sign_ == sign::positive)
        {
            out << "+";
        }
        else if (this->sign_ == sign::negative)
        {
            out << "-";
        }
        else
        {
            throw ::std::runtime_error("big_int to_oct sign err");
        }

        out << ::std::oct;
        for (i32 i = this->data_.size() - 1; i >= 0; i--)
        {
            out << this->data_[i];
        }
        out << ::std::dec;

        return out.str();
    }
    ::std::string big_int::to_bin() const
    {
        ::std::string out;
        if (this->sign_ == sign::uninitialized)
        {
            out += "no_init";
        }
        else if (this->sign_ == sign::not_a_number)
        {
            out += "NaN";
        }
        else if (this->sign_ == sign::positive)
        {
            out += "+";
        }
        else if (this->sign_ == sign::negative)
        {
            out += "-";
        }
        else
        {
            throw ::std::runtime_error("big_int to_bin sign err");
        }

        for (i32 i = this->data_.size() - 1; i >= 0; i--)
        {
            out += ::std::bitset<32>(this->data_[i]).to_string();
        }

        return out;
    }
    inline bool big_int::get_sign() const
    {
        switch (this->sign_)
        {
        case sign::positive:
            return true;
        case sign::negative:
            return false;
        default:
            throw std::runtime_error("big_int get_sign: invalid sign state");
        }
        return 0;
    }
    inline const big_int &big_int::abs_big(const big_int &a, const big_int &b)
    {
        i32 a_size = a.data_.size();
        i32 b_size = b.data_.size();
        if (a_size > b_size)
        {
            return a;
        }
        if (a_size < b_size)
        {
            return b;
        }
        // 相等
        for (i32 i = a_size - 1; i >= 0; i--)
        {
            if (a.data_[i] == b.data_[i])
            {
                continue;
            }
            if (a.data_[i] > b.data_[i])
            {
                return a;
            }
            if (a.data_[i] < b.data_[i])
            {
                return b;
            }
        }
        return a;
    }
    inline const big_int &big_int::abs_small(const big_int &a, const big_int &b)
    {
        i32 a_size = a.data_.size();
        i32 b_size = b.data_.size();
        if (a_size > b_size)
        {
            return b;
        }
        if (a_size < b_size)
        {
            return a;
        }
        // 相等
        for (i32 i = a_size - 1; i >= 0; i--)
        {
            if (a.data_[i] == b.data_[i])
            {
                continue;
            }
            if (a.data_[i] > b.data_[i])
            {
                return b;
            }
            if (a.data_[i] < b.data_[i])
            {
                return a;
            }
        }
        return b;
    }
    inline bool big_int::is_zero() const
    {
        if (this->data_.size() == 1 and this->data_[0] == 0)
        {
            return true;
        }
        return false;
    }
    inline i8 big_int::less(const big_int &a, const big_int &b)
    {
        i8 a_sign = 0;
        if (a.sign_ == sign::positive)
        {
            a_sign = 1;
        }
        else if (a.sign_ == sign::negative)
        {
            a_sign = -1;
        }
        else
        {
            return -1;
        }
        i8 b_sign = 0;
        if (b.sign_ == sign::positive)
        {
            b_sign = 1;
        }
        else if (b.sign_ == sign::negative)
        {
            b_sign = -1;
        }
        else
        {
            return -1;
        }

        i32 size_a = a.data_.size();
        i32 size_b = b.data_.size();

        if (a_sign < b_sign)
        {
            return 1;
        }
        if (a_sign > b_sign)
        {
            return 0;
        }
        if (a_sign == b_sign)
        {
            if (a_sign == 1)
            {
                if (size_a < size_b)
                {
                    return 1;
                }
                if (size_a > size_b)
                {
                    return 0;
                }
                if (size_a == size_b)
                {
                    for (i32 i = size_a - 1; i >= 0; i--)
                    {
                        if (a.data_[i] == b.data_[i])
                        {
                            continue;
                        }
                        else
                        {
                            if (a.data_[i] < b.data_[i])
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    // 相等
                    return 0;
                }
            }
            if (a_sign == -1)
            {
                if (size_a < size_b)
                {
                    return 0;
                }
                if (size_a > size_b)
                {
                    return 1;
                }
                if (size_a == size_b)
                {
                    for (i32 i = size_a - 1; i >= 0; i--)
                    {
                        if (a.data_[i] == b.data_[i])
                        {
                            continue;
                        }
                        else
                        {
                            if (a.data_[i] < b.data_[i])
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    // 相等
                    return 0;
                }
            }
        }

        return -1;
    }
    inline i8 big_int::abs_less(const big_int &a, const big_int &b)
    {
        auto a_size = a.data_.size();
        auto b_size = b.data_.size();
        if (a_size > b_size)
        {
            return 0;
        }
        else if (a_size < b_size)
        {
            return 1;
        }
        else if (a_size == b_size)
        {
            for (i32 i = a_size - 1; i >= 0; i--)
            {
                if (a.data_[i] == b.data_[i])
                {
                    continue;
                }
                else
                {
                    if (a.data_[i] < b.data_[i])
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            return -1;
        }
        return -1;
    }
    inline void big_int::abs_add(big_int &a, const big_int &b)
    {
        u64 carry = 0;
        i32 size_b = b.data_.size();
        i32 size_a = a.data_.size();
        if (size_a < size_b)
        {
            a.data_.resize(size_b, 0);
            size_a = a.data_.size();
        }
        i32 i = 0;
        for (; i < size_b; i++)
        {
            carry += static_cast<u64>(a.data_[i]) + static_cast<u64>(b.data_[i]);
            a.data_[i] = carry & u32_max;
            carry >>= 32;
        }

        for (; carry != 0 and i < size_a; i++)
        {
            carry += a.data_[i];
            a.data_[i] = carry & u32_max;
            carry >>= 32;
        }

        if (carry != 0)
        {
            a.data_.push_back(carry);
        }

        a.format();
        return;
    }
    inline void big_int::abs_sub(big_int &a, const big_int &b)
    {
        // 确保 a >= b，否则抛出异常
        if (&a == &abs_small(a, b))
        {
            throw std::runtime_error("big_int::abs_sub: a < b");
        }

        u64 borrow = 0; // 借位标志

        // 处理 b 的每一位
        for (i32 i = 0; i < b.data_.size(); ++i)
        {
            u64 temp = static_cast<u64>(a.data_[i]) - static_cast<u64>(b.data_[i]) - borrow;
            a.data_[i] = temp & u32_max; // 保留低 32 位
            borrow = (temp >> 32) & 0x1; // 提取借位标志
        }

        // 处理 a 的剩余位
        for (i32 i = b.data_.size(); i < a.data_.size(); ++i)
        {
            u64 temp = static_cast<u64>(a.data_[i]) - borrow;
            a.data_[i] = temp & u32_max; // 保留低 32 位
            borrow = (temp >> 32) & 0x1; // 提取借位标志
        }

        a.format();
        return;
    }
    inline void big_int::abs_mul(big_int &a, const big_int &b)
    {
        auto a_size = a.data_.size();
        auto b_size = b.data_.size();

        if (a.is_zero() or b.is_zero())
        {
            a.data_.clear();
            a.sign_ = sign::positive;
            return;
        }

        ::std::vector<u64> temp(a.data_.size() + b.data_.size() + 1, 0);
        for (u32 i = 0; i < a_size; i++)
        {
            for (u32 j = 0; j < b_size; j++)
            {
                u64 carry = static_cast<u64>(a.data_[i]) * static_cast<u64>(b.data_[j]);
                temp[i + j] += carry & u32_max;
                temp[i + j + 1] += carry >> 32;
            }
        }

        a.data_.resize(temp.size() - 1);
        for (u32 i = 0; i < temp.size() - 1; i++)
        {
            a.data_[i] = temp[i] & 0xffffffff;
            temp[i + 1] += temp[i] >> 32;
        }

        a.format();
        return;
    }
    inline void big_int::abs_div(big_int &a, const big_int &b, big_int &c)
    {
        if (b.is_zero())
        {
            throw std::runtime_error("big_int::abs_div: division by zero");
        }

        c = 0;

        // 快速路径：如果 b == 1，则商为 a，余数为 0
        if (b.data_.size() == 1 && b.data_[0] == 1)
        {
            a.swap(c);
            return;
        }
        // std::cout << "aaaaa:1\n";
        // std::cout << a.to_dec() << '/' << b.to_dec() << '\n';
        // 如果 a < b，则商为 0，余数为 a
        if (abs_less(a, b))
        {
            // std::cout << "a < b\n";
            c = 0;
            return;
        }
        // std::cout << "aaaaa:2\n";
        // 商
        auto &result = c;
        // 余数
        auto &remainder = a;
        // 临时除数
        big_int temp = b;
        // 临时倍率
        big_int temp_multiplier = 1;
        // number 1
        big_int num_1 = 1;
        // std::cout << "aaaaa:3\n";
        // 快速左移
        if (remainder.data_.size() > temp.data_.size() + 1)
        {
            size_t shift_blocks = (remainder.data_.size() - temp.data_.size() - 1);
            temp <<= shift_blocks * 32;
            temp_multiplier <<= shift_blocks * 32;
        }

        // 循环左移，直到 temp >= a
        while (temp < remainder)
        {
            temp <<= 1;
            temp_multiplier <<= 1;
        }

        // 右移到恰好小于等于 remainder
        temp >>= 1;
        temp_multiplier >>= 1;

        // std::cout << temp.to_dec() << std::endl;

        // 开始除法
        while ((temp_multiplier >= num_1))
        {

            if (remainder >= temp)
            {
                remainder -= temp;
                result += temp_multiplier;
            }

            temp >>= 1;
            temp_multiplier >>= 1;
        }

        return;
    }

    inline void big_int::format()
    {
        // 去除多余的0
        while (this->data_.size() >= 2 and
               this->data_.back() == 0)
        {
            this->data_.pop_back();
        }
    }
}
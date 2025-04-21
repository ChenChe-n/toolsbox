#pragma once

#include "../base/using.hpp"
#include <vector>
#include <string_view>
#include <string>
#include <stdexcept> // 添加异常处理

namespace __NAMESPACE_NAME__
{
    class big_uint
    {
    public:
        inline constexpr big_uint()
            : data_({0})
        {
        }
        inline constexpr big_uint(const ::std::string_view &str)
            : data_({0})
        {
            format();
        }
        inline constexpr big_uint(::std::vector<u32> &&vec)
            : data_(::std::move(vec))
        {
            format();
        }
        inline constexpr big_uint(const u64 &num)
            : data_(num >= u32_max ? ::std::vector<u32>{u32(num), u32(num >> 32)} : ::std::vector<u32>{u32(num)})
        {
        }
        inline constexpr big_uint(const big_uint &num)
            : data_(num.data_)
        {
        }
        inline constexpr big_uint(big_uint &&num)
            : data_(::std::move(num.data_))
        {
        }

        inline ::std::string to_dec() const
        {
            return result.empty() ? "0" : result;
        }

        inline constexpr big_uint &operator=(const big_uint &num)
        {
            data_ = num.data_;
            return *this;
        }
        inline constexpr big_uint &operator=(big_uint &&num)
        {
            data_.swap(num.data_);
            num.data_.clear();
            return *this;
        }

        inline constexpr bool operator==(const big_uint &num) const
        {
            return data_ == num.data_;
        }
        inline constexpr bool operator!=(const big_uint &num) const
        {
            return data_ != num.data_;
        }
        inline constexpr bool operator<(const big_uint &num) const
        {
            u64 a_size = data_.size();
            u64 b_size = num.data_.size();

            if (a_size < b_size)
            {
                return true;
            }
            if (a_size > b_size)
            {
                return false;
            }
            for (i64 i = a_size - 1; i >= 0; i++)
            {
                if (data_[i] < num.data_[i])
                {
                    return true;
                }
                if (data_[i] > num.data_[i])
                {
                    return false;
                }
            }
            return false;
        }
        inline constexpr bool operator>(const big_uint &num) const
        {
            return num < *this;
        }
        inline constexpr bool operator<=(const big_uint &num) const
        {
            return !(*this > num);
        }
        inline constexpr bool operator>=(const big_uint &num) const
        {
            return !(*this < num);
        }

        inline constexpr big_uint &operator<<=(const u64 &num)
        {
            if (num == 0)
                return *this;

            u64 block_shift = num / 32;
            u32 bit_shift = num % 32;

            // 处理块位移
            if (block_shift > 0)
            {
                data_.insert(data_.begin(), block_shift, 0u);
            }

            // 处理位位移
            if (bit_shift != 0)
            {
                u64 carry = 0;
                for (auto &block : data_)
                {
                    u64 temp = (static_cast<u64>(block) << bit_shift) | carry;
                    block = static_cast<u32>(temp & 0xFFFFFFFFu);
                    carry = temp >> 32;
                }
                if (carry != 0)
                {
                    data_.push_back(static_cast<u32>(carry));
                }
            }

            format();
            return *this;
        }
        inline constexpr big_uint operator<<(const u64 &num) const
        {
            auto result = *this;
            return result <<= num;
        }
        inline constexpr big_uint &operator>>=(const u64 &num)
        {
            if (num == 0)
                return *this;

            u64 total_shift = num;
            u64 block_shift = total_shift / 32;
            u32 bit_shift = total_shift % 32;

            // 处理块位移
            if (block_shift >= data_.size())
            {
                data_.clear();
                data_.push_back(0);
                return *this;
            }
            else if (block_shift > 0)
            {
                data_.erase(data_.begin(), data_.begin() + block_shift);
            }

            // 处理位位移
            if (bit_shift != 0)
            {
                u64 carry = 0;
                for (size_t i = data_.size(); i > 0; --i)
                {
                    size_t idx = i - 1;
                    u64 current = data_[idx];
                    u64 combined = (current >> bit_shift) | (carry << (32 - bit_shift));
                    carry = current & ((1ULL << bit_shift) - 1);
                    data_[idx] = static_cast<u32>(combined);
                }
            }

            format();
            return *this;
        }
        inline constexpr big_uint operator>>(const u64 &num) const
        {
            auto result = *this;
            return result >>= num;
        }

        inline constexpr big_uint &operator+=(const big_uint &num)
        {
            u64 sum = 0;
            u64 size = num.data_.size();

            data_.resize(std::max(data_.size(), num.data_.size()) + 1, 0);

            u64 i = 0;
            for (; i < size; i++)
            {
                sum += data_[i];
                sum += num.data_[i];
                data_[i] = sum;
                sum >>= 32;
            }

            while (sum)
            {
                sum += data_[i];
                data_[i] = sum;
                sum >>= 32;
                i++;
            }

            format();
            return *this;
        }
        inline constexpr big_uint operator+(const big_uint &num) const
        {
            auto result = *this;
            return result += num;
        }
        inline constexpr big_uint &operator-=(const big_uint &num)
        {
            if (*this < num)
            {
                data_.clear();
                data_.emplace_back(0);
                return *this;
            }

            i64 sub = 0;
            u64 size = num.data_.size();

            u64 i = 0;
            for (; i < size; i++)
            {
                sub += data_[i];
                sub -= num.data_[i];
                data_[i] = *(u64 *)&sub;
                sub >>= 32;
            }

            while (sub)
            {
                sub += data_[i];
                data_[i] = *(u64 *)&sub;
                sub >>= 32;
                i++;
            }

            format();
            return *this;
        }
        inline constexpr big_uint operator-(const big_uint &num) const
        {
            auto result = *this;
            return result -= num;
        }
        inline constexpr big_uint &operator*=(const big_uint &num)
        {
            u64 sum1 = 0;
            u64 a_size = data_.size();
            u64 b_size = num.data_.size();

            data_.resize(a_size + b_size, 0);

            i64 i = a_size - 1, j = b_size - 1;
            while (true)
            {
                if (i > j and i > 0)
                {
                    u64 sum = data_[i];
                    sum *= num.data_[j];
                    data_[i + j] = 0;
                    add(i + j, sum);
                    i--;
                }
                else if (j > 0)
                {
                    u64 sum = data_[i];
                    sum *= num.data_[j];
                    data_[i + j] = 0;
                    add(i + j, sum);
                    j--;
                }
                else
                {
                    u64 sum = data_[i];
                    sum *= num.data_[j];
                    data_[i + j] = 0;
                    add(i + j, sum);
                    break;
                }
            }

            format();
            return *this;
        }
        inline constexpr big_uint operator*(const big_uint &num) const
        {
            auto result = *this;
            return result *= num;
        }

        // 除法核心函数，返回商和余数
        // a 被除数/余数，b 除数，c 商
        static inline void divide(big_uint a, const big_uint &b, big_uint &c)
        {
            std::cerr << ",";
            c = 0;
            auto &remainder = a; // 余数初始化为a的值

            if (b == 0)
                throw std::domain_error("Division by zero");
            if (remainder < b)
                return;

            big_uint temp1 = b;
            big_uint temp2 = 1;

            // 寻找最大的temp1 <= remainder
            while (temp1 <= (remainder >> 1)) // 避免溢出
            {
                temp1 <<= 1;
                temp2 <<= 1;
            }

            while (temp2 != 0)
            {
                std::cerr << ".";
                if (remainder >= temp1)
                {
                    remainder -= temp1;
                    c += temp2;
                }
                temp1 >>= 1;
                temp2 >>= 1;
            }
            std::cerr << "\n";
        }

        inline  big_uint &operator/=(const big_uint &other)
        {
            big_uint temp;
            divide(*this, other, temp);
            *this = std::move(temp);
            return *this;
        }
        inline  big_uint operator/(const big_uint &other) const
        {
            big_uint temp = *this;
            temp /= other;
            return temp;
        }
        inline  big_uint &operator%=(const big_uint &other)
        {
            big_uint temp;
            divide(*this, other, temp);
            return *this;
        }
        inline  big_uint operator%(const big_uint &other) const
        {
            big_uint temp = *this;
            temp %= other;
            return temp;
        }

    private:
        inline constexpr void add(u64 index, u64 num)
        {
            while (num)
            {
                data_[index] += num;
                index++;
                num >>= 32;
            }
        }
        inline constexpr void format()
        {
            // 去除前导0
            while (data_.size() >= 2 and data_.back() == 0)
            {
                data_.pop_back();
            }
        }
        ::std::vector<u32> data_;
    };
}
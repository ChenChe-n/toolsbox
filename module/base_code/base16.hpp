#pragma once

#include "../base/using.hpp"
#include <string>
#include <array>
#include <vector>

class base16
{
public:
    // 编码base16
    constexpr static inline ::std::string encode_c(const byte *data, const u64 size) noexcept
    {
        ::std::string out = {};
        for (u64 i = 0; i < size; i++)
        {
            push_str(out, __encode__(data[i]));
        }
        return out;
    }
    // 编码base16
    constexpr static inline ::std::string encode(const ::std::string &data)
    {
        return encode_c(static_cast<const byte *>(static_cast<const void *>(data.data())), data.size());
    }
    // 编码base16
    constexpr static inline ::std::string encode(const ::std::vector<byte> &data)
    {
        return encode_c(static_cast<const byte *>(data.data()), data.size());
    }
    // 解码base16
    constexpr static inline ::std::vector<byte> decoder_c(const byte *data, const u64 size) noexcept
    {
        ::std::vector<byte> out = {};
        if (!is_base16_c(data, size))
        {
            return out;
        }
        for (u64 i = 0; i < size; i += 2)
        {
            push_vec(out, __decoder__({ data[i],data[i + 1] }));
        }
        return out;
    }
    // 解码base16
    constexpr static inline ::std::vector<byte> decoder(const ::std::string &data)
    {
        return decoder_c(static_cast<const byte *>(static_cast<const void *>(data.data())), data.size());
    }
    // 解码base16
    constexpr static inline ::std::vector<byte> decoder(const ::std::vector<byte> &data)
    {
        return decoder_c(static_cast<const byte *>(data.data()), data.size());
    }
    // 判断是否是base16
    constexpr static inline bool is_base16_c(const byte *data, const u64 size) noexcept
    {
        if (size % 2 != 0)
        {
            return false;
        }
        if (size == 0)
        {
            return false;
        }
        for (u64 i = 0; i < size; i++)
        {
            if (base16_char[data[i]] == 0)
            {
                return false;
            }
        }
        return true;
    }
    // 判断是否是base16
    constexpr static inline bool is_base16(const ::std::string &data)
    {
        return is_base16_c(static_cast<const byte *>(static_cast<const void *>(data.data())), data.size());
    }
    // 判断是否是base16
    constexpr static inline bool is_base16(const ::std::vector<byte> &data)
    {
        return is_base16_c(static_cast<const byte *>(data.data()), data.size());
    }

private:
    constexpr static inline ::std::array<byte, 2> __encode__(const byte &data) noexcept
    {
        ::std::array<byte, 2> out = {char_to_base16[(data >> 4) & bit4],char_to_base16[data & bit4]};
        return out;
    }
    constexpr static inline byte __decoder__(const ::std::array<byte, 2> &data) noexcept
    {
        byte out = (base16_to_char[data[0]] << 4) | base16_to_char[data[1]];

        return out;
    }
    constexpr static inline void push_str(::std::string &str, const ::std::array<byte, 2> &data)
    {
        str.push_back(data[0]);
        str.push_back(data[1]);
        return;
    }
    constexpr static inline void push_vec(::std::vector<byte> &vec, const byte &data)
    {
        vec.push_back(data);
        return;
    }
    constexpr static inline ::std::array<byte, 16> char_to_base16 = []()
    {
        ::std::array<byte, 16> out = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        return out;
    }();
    constexpr static inline ::std::array<byte, 128> base16_to_char = []()
    {
        ::std::array<byte, 128> out = {0};
        out.fill(0);
        for (int i = 0; i < 16; i++)
        {
            out[char_to_base16[i]] = i;
        }
        return out;
    }();
    constexpr static inline ::std::array<byte, 256> base16_char = []()
    {
        ::std::array<byte, 256> out = {0};
        for (int i = 0; i < 16; i++)
        {
            out[char_to_base16[i]] = 1;
        }
        return out;
    }();
    constexpr static inline byte bit4 = 0b1111;
    constexpr static inline byte bit8 = 0b11111111;
};
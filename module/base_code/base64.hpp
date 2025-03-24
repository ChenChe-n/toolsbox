#pragma once

#include "../base/using.hpp"
#include <string>
#include <array>
#include <vector>

class base64
{
public:
    // 编码base64
    constexpr static inline ::std::string encode_c(const byte* data, const u64 size) noexcept
    {
        ::std::string out = {};
        const u64 k = (size / 3) * 3;
        out.reserve((size / 3) * 4 + 4);
        // 构造成3一组的base64
        for (u64 i = 0; i < k; i += 3)
        {
            push_str(out, __encode__({ data[i], data[i + 1], data[i + 2] }));
        }
        // 处理尾部
        if (size % 3 != 0)
        {
            auto temp = __encode__({ k + 0 < size ? data[k + 0] : byte(0),
                                                            k + 1 < size ? data[k + 1] : byte(0),
                                                            k + 2 < size ? data[k + 2] : byte(0) });

            if (k + 2 >= size)
            {
                temp[3] = '=';
            }
            if (k + 1 >= size)
            {
                temp[2] = '=';
            }
            if (k + 0 >= size)
            {
                temp[1] = '=';
                temp[0] = '=';
            }
            push_str(out, temp);
        }
        return out;
    }
    // 编码base64
    constexpr static inline ::std::string encode(const ::std::string& data)
    {
        return encode_c(reinterpret_cast<const byte*>(data.data()), data.size());
    }
    // 编码base64
    constexpr static inline ::std::string encode(const ::std::vector<byte>& data)
    {
        return encode_c(reinterpret_cast<const byte*>(data.data()), data.size());
    }
    // 解码base64
    constexpr static inline ::std::vector<byte> decoder_c(const byte* data, const u64 size) noexcept
    {
        ::std::vector<byte> out = {};
        if (!is_base64_c(data, size))
        {
            return out;
        }
        const u64 k = data[size - 1] == '=' ? size - 4 : size;
        if (size % 4 != 0)
        {
            return out;
        }
        out.reserve((size / 4) * 3 + 4);
        // 解码成4一组的base64
        for (u64 i = 0; i < k; i += 4)
        {
            push_vec(out, __decoder__({ data[i + 0], data[i + 1], data[i + 2], data[i + 3] }));
        }
        // 处理尾部
        if (k < size)
        {
            auto temp = __decoder__({ data[k + 0], data[k + 1], data[k + 2], data[k + 3] });
            push_vec(out, temp);
            if (data[k + 3] == '=')
            {
                out.pop_back();
            }
            if (data[k + 2] == '=')
            {
                out.pop_back();
            }
        }
        return out;
    }
    // 解码base64
    constexpr static inline ::std::vector<byte> decoder(const ::std::string& data)
    {
        return decoder_c(reinterpret_cast<const byte*>(data.data()), data.size());
    }
    // 解码base64
    constexpr static inline ::std::vector<byte> decoder(const ::std::vector<byte>& data)
    {
        return decoder_c(reinterpret_cast<const byte*>(data.data()), data.size());
    }
    // 判断是否是base64
    constexpr static inline bool is_base64_c(const byte* data, const u64 size) noexcept
    {
        if (size % 4 != 0) {
            return false;
        }
        if (size < 4) {
            return false;
        }
        u64 k = size;
        if (data[k - 1] == '=')
        {
            if (data[k - 2] == '=')
            {
                --k;
            }
            --k;
        }
        if (data[k - 1] == '=')
        {
            return false;
        }
        for (u64 i = 0; i < k; i += 4)
        {
            if (base64_char[data[i]] == 0)
            {
                return false;
            }
        }
        return true;
    }
    // 判断是否是base64
    constexpr static inline bool is_base64(const ::std::string& data) {
        return is_base64_c(reinterpret_cast<const byte*>(data.data()), data.size());
    }
    // 判断是否是base64
    constexpr static inline bool is_base64(const ::std::vector<byte>& data) {
        return is_base64_c(reinterpret_cast<const byte*>(data.data()), data.size());
    }
private:
    constexpr static inline ::std::array<byte, 4> __encode__(const ::std::array<byte, 3>& data) noexcept
    {
        ::std::array<byte, 4> out = { 0 };
        u32 temp = 0;

        temp |= data[0];
        temp <<= 8;
        temp |= data[1];
        temp <<= 8;
        temp |= data[2];

        out[0] = char_to_base64[(temp >> 18) & bit6];
        out[1] = char_to_base64[(temp >> 12) & bit6];
        out[2] = char_to_base64[(temp >> 6) & bit6];
        out[3] = char_to_base64[temp & bit6];

        return out;
    }
    constexpr static inline ::std::array<byte, 3> __decoder__(const ::std::array<byte, 4>& data) noexcept
    {
        ::std::array<byte, 3> out = { 0 };
        u32 temp = 0;

        temp |= base64_to_char[data[0]];
        temp <<= 6;
        temp |= base64_to_char[data[1]];
        temp <<= 6;
        temp |= base64_to_char[data[2]];
        temp <<= 6;
        temp |= base64_to_char[data[3]];

        out[0] = (temp >> 16) & bit8;
        out[1] = (temp >> 8) & bit8;
        out[2] = temp & bit8;

        return out;
    }
    constexpr static inline void push_str(::std::string& str, const ::std::array<byte, 4>& data)
    {
        str.push_back(data[0]);
        str.push_back(data[1]);
        str.push_back(data[2]);
        str.push_back(data[3]);
        return;
    }
    constexpr static inline void push_vec(::std::vector<byte>& vec, const ::std::array<byte, 3>& data)
    {
        vec.push_back(data[0]);
        vec.push_back(data[1]);
        vec.push_back(data[2]);
        return;
    }
    constexpr static inline ::std::array<byte, 64> char_to_base64 = []()
        {
            ::std::array<byte, 64> out = {
                    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };
            return out;
        }();
    constexpr static inline ::std::array<byte, 128> base64_to_char = []()
        {
            ::std::array<byte, 128> out = { 0 };
            out.fill(0);
            for (int i = 0; i < 64; i++)
            {
                out[char_to_base64[i]] = i;
            }
            return out;
        }();
    constexpr static inline ::std::array<byte, 256> base64_char = []()
        {
            ::std::array<byte, 256> out = { 0 };
            for (int i = 0; i < 64; i++)
            {
                out[char_to_base64[i]] = 1;
            }
            return out;
        }();
    constexpr static inline byte bit6 = 0b111111;
    constexpr static inline byte bit8 = 0b11111111;
};
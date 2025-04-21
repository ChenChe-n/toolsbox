#pragma once

#include "../base/using.hpp"
#include "../random/random.hpp"

#include <array>
#include <vector>

namespace __NAMESPACE_NAME__::encoding
{
    class xor_encryption
    {
    public:
        static inline void encode_c(byte *data, const u64 size, const u64 seed)
        {
            ::std::vector<byte> out;
            __NAMESPACE_NAME__::random_64 rd{seed};
            out.reserve(size);
            for (u64 i = 0; i < size; i++)
            {
                data[i] ^= rd.get_u64();
            }
            return;
        }
        //
        static inline ::std::vector<byte> encode(const ::std::vector<byte> &data, const u64 seed)
        {
            ::std::vector<byte> out = data;
            __NAMESPACE_NAME__::random_64 rd{seed};
            for (u64 i = 0; i < out.size(); i++)
            {
                out[i] ^= rd.get_u64();
            }
            return out;
        }
        //
        static inline ::std::string encode(const ::std::string &data, const u64 seed)
        {
            ::std::string out = data;
            __NAMESPACE_NAME__::random_64 rd{seed};
            for (u64 i = 0; i < out.size(); i++)
            {
                out[i] ^= rd.get_u64();
            }
            return out;
        }
    };
}
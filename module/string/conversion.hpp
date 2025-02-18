#pragma once

#include "../base/using.hpp"

#include <string>
#include <string_view>

namespace __NAMESPACE_NAME__
{
	enum class encoding : u16
	{
		// UTF
		utf_8 = 0,
		utf_16_be = 1,
		utf_16_le = 2,
		utf_32 = 3,
		// GBK
		gbk = 10,
		gb2312 = 10,
		gb18030 = 10,
	};
	namespace string
	{
        inline std::u32string utf8_to_utf32(const std::u8string_view& str) {
            u64 skip_count = 0;
            std::u32string out;

            while (skip_count < str.size()) {
                char32_t result = 0;
                char8_t first_byte = str[skip_count];

                // 1-byte UTF-8 (ASCII)
                if (first_byte <= 0b01111111) {
                    result = first_byte;
                    skip_count += 1;
                    out.push_back(result);
                    continue;
                }

                // 2-byte UTF-8
                if ((first_byte & 0b11100000) == 0b11000000 && str.size() - skip_count >= 2) {
                    char8_t second_byte = str[skip_count + 1];
                    if ((second_byte & 0b11000000) != 0b10000000) return std::u32string();  // 无效字节

                    result = ((first_byte & 0b00011111) << 6) | (second_byte & 0b00111111);
                    skip_count += 2;
                    out.push_back(result);
                    continue;
                }

                // 3-byte UTF-8
                if ((first_byte & 0b11110000) == 0b11100000 && str.size() - skip_count >= 3) {
                    char8_t second_byte = str[skip_count + 1];
                    char8_t third_byte = str[skip_count + 2];

                    if ((second_byte & 0b11000000) != 0b10000000 ||
                        (third_byte & 0b11000000) != 0b10000000) return std::u32string();  // 无效字节

                    result = ((first_byte & 0b00001111) << 12) |
                        ((second_byte & 0b00111111) << 6) |
                        (third_byte & 0b00111111);
                    skip_count += 3;
                    out.push_back(result);
                    continue;
                }

                // 4-byte UTF-8
                if ((first_byte & 0b11111000) == 0b11110000 && str.size() - skip_count >= 4) {
                    char8_t second_byte = str[skip_count + 1];
                    char8_t third_byte = str[skip_count + 2];
                    char8_t fourth_byte = str[skip_count + 3];

                    if ((second_byte & 0b11000000) != 0b10000000 ||
                        (third_byte & 0b11000000) != 0b10000000 ||
                        (fourth_byte & 0b11000000) != 0b10000000) return std::u32string();  // 无效字节

                    result = ((first_byte & 0b00000111) << 18) |
                        ((second_byte & 0b00111111) << 12) |
                        ((third_byte & 0b00111111) << 6) |
                        (fourth_byte & 0b00111111);
                    skip_count += 4;
                    out.push_back(result);
                    continue;
                }

                // 发现非法字符，返回空字符串
                return std::u32string();
            }

            return out;
        }

        inline std::u8string utf32_to_utf8(const std::u32string_view str) {
            std::u8string result;

            for (char32_t ch : str) {
                if (ch <= 0x7F) {
                    // 1 字节: 0xxxxxxx
                    result += static_cast<char8_t>(ch);
                }
                else if (ch <= 0x7FF) {
                    // 2 字节: 110xxxxx 10xxxxxx
                    result += static_cast<char8_t>(0b11000000 | ((ch >> 6) & 0b00011111));
                    result += static_cast<char8_t>(0b10000000 | (ch & 0b00111111));
                }
                else if (ch <= 0xFFFF) {
                    // 3 字节: 1110xxxx 10xxxxxx 10xxxxxx
                    result += static_cast<char8_t>(0b11100000 | ((ch >> 12) & 0b00001111));
                    result += static_cast<char8_t>(0b10000000 | ((ch >> 6) & 0b00111111));
                    result += static_cast<char8_t>(0b10000000 | (ch & 0b00111111));
                }
                else if (ch <= 0x10FFFF) {
                    // 4 字节: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    result += static_cast<char8_t>(0b11110000 | ((ch >> 18) & 0b00000111));
                    result += static_cast<char8_t>(0b10000000 | ((ch >> 12) & 0b00111111));
                    result += static_cast<char8_t>(0b10000000 | ((ch >> 6) & 0b00111111));
                    result += static_cast<char8_t>(0b10000000 | (ch & 0b00111111));
                }
                else {
                    // 超出 Unicode 规范范围 (U+10FFFF)，跳过或替换为 '?'
                    result += u8"�";
                }
            }

            return result;
        }
	
	}
}

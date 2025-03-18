#include "module/all_module.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <format>
#include <map>
#include <unordered_map>
#include <thread>
#include <string_view>
#include <SFML/Graphics.hpp>

bool ppp(const std::string_view str1, const std::string_view str2)
{
    if (str1.size() != str2.size())
    {
        return false;
    }
    for (size_t i = 0; i < str1.size(); i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }

    return true;
}

int main()
{
    auto out = ts::io::logging();
    // for (size_t i = 0; i < 256; i++)
    // {
    //     out.error("error\t错误");
    //     out.warning("warning\t警告");
    //     out.info("info\t信息");
    //     out.text("text\t文本");
    //     out.base("base\t基本");
    // }
    std::string str_a = "1235771957861985767";
    std::string str_b = "41192561297461946195";
    std::string str_c = "50904612123914409968303665756671752760";
    // if (ppp(a_py, b_cpp))
    // {
    //     std::cout << "a==b\n";
    // }
    // else
    // {
    //     std::cout << "a!=b\n";
    // }
    ts::big_int a(str_a);
    ts::big_int b(str_b);
    ts::big_int c(str_c);
    out.info(a.to_dec());
    out.info(b.to_dec());
    out.info(c.to_dec());
    a *= b;
    out.info(a.to_dec());
    out.info((a == c) ? "a==b" : "a!=b");

    return 0;
}
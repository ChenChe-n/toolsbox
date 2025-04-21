#include "module/all.hpp"
#include <array>
#include <map>
#include <cassert>
#include <cstdint>
#include <climits>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>

// #include "module/containers/vm.hpp"
#include "module/containers/int128.hpp"
int main()
{
    auto out = ts::io::logging();
    u64 i1 = 1;
    u64 q1 = 1024 * 1024 * 1024;
    std::cin >> q1;

    auto t1 = ts::time::now();
    {
        __int128_t a = i1;
        for (size_t i = 0; i < q1; i++)
        {
            a += i;
            a -= 13;
            a *= 72;
            a /= 17;
            a ^= a++ % 14816;
            a ^= a-- << 9;
            a ^= a >> 5;
            a ++;
            ++a;
        }
        out.info(std::format("{}", a));
    }
    auto t2 = ts::time::now();
    {
        int128 a = i1;
        for (size_t i = 0; i < q1; i++)
        {
            a += i;
            a -= 13;
            a *= 72;
            a /= 17;
            a ^= a++ % 14816;
            a ^= a-- << 9;
            a ^= a >> 5;
            a ++;
            ++a;
        }
        out.info(std::format("{}", a.to_string()));
    }
    auto t3 = ts::time::now();
    out.info(std::format("__int128_t : {}", (t2 - t1).fseconds()));
    out.info(std::format("int128 : {}", (t3 - t2).fseconds()));

    return 0;
}
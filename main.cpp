#include "module/all.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <climits>

int main()
{
    ts::thread::mutex_v1 m1;
    auto out = ts::io::logging();
    ts::thread::box<u64> box{0};
    out.info("initOk");
    std::thread t1([&](){
        for (u64 i = 0; i < 100'000'000; ++i) {
            auto b = box.get();
            auto temp = b.data_;
            temp += 1;
            temp *= 3;
            temp /= 2;
            b = temp;
        } });
    std::thread t2([&](){
        for (u64 i = 0; i < 100'000'000; ++i) {
            auto b = box.get();
            auto temp = b.data_;
            temp += 1;
            temp *= 3;
            temp /= 2;
            b = temp;
        } });
    // for (u64 i = 0; i < 100'000; ++i)
    // {
    //     auto b = box.get();
    //     out.info("box.data_ = " + std::to_string(b.data_));
    // }
    t1.join();
    t2.join();
    out.info("box.data_ = " + std::to_string(box.get().data_));
    u64 o = 0;
    for (size_t i = 0; i < 200'000'000; i++)
    {
        o += 1;
        o *= 3;
        o /= 2;
    }
    out.info("o = " + std::to_string(o));
    
    return 0;
}
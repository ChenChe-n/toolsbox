#include "module/all.hpp"
#include <format>
#include <iostream>
#include <thread>
int main()
{
    auto out = ts::io::logging();
    while (true)
    {
        out.info("test");
        ts::thread::sleep_for(ts::time::duration(0.001));
    }
    return 0;
}
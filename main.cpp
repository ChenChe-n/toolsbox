#include "module/all_module.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <format>

i32 main() {
    std::cerr << "程序运行时间: " << ts::time::elapsed_time()<< std::endl;

    auto t1 = ts::time::now();
    ts::thread::mutex_v1 mutex;
    i64 shared_data = 0;

    auto worker = [&mutex, &shared_data](int id) {
        for (int i = 0; i < 50'000'000; ++i) {
            {
                ts::thread::lock lock(mutex); // 加锁
                shared_data = shared_data * 3 / 2 + 1;
            }
        }
        };

    std::vector<std::thread> threads;
    for (int i = 0; i < 2; ++i) {
        std::cout << "Thread " << i << " started\n";
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }
    auto t2 = ts::time::now();

    std::cout << "Final shared_data: " << shared_data << "\n";
    auto t3 = ts::time::now();
    i64 n = 0;
    for (i64 i = 0; i < 100'000'000; ++i) {
        n = n * 3 / 2 + 1;
    }
    auto t4 = ts::time::now();
    std::cout << "Final n: " << n << "\n";


    std::cerr << "程序运行时间: " << ts::time::elapsed_time()<< std::endl;
    std::cerr << "lock运行时间: " << (t2 - t1) << "s" << std::endl;
    std::cerr << "not_运行时间: " << (t4 - t3) << "s" << std::endl;

    for (size_t i = 0; i < 64; i++)
    {
        std::this_thread::sleep_for(ts::time::seconds(0.001));
        std::cerr << "程序运行时间: " << ts::time::elapsed_time()<< std::endl;
    }

    return 0;
}
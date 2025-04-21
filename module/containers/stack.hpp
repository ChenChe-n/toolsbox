#pragma once

#include "../base/using.hpp"
#include <cstring>   // 添加memcpy头文件
#include <stdexcept> // 添加异常处理

namespace __NAMESPACE_NAME__::containers
{
    class stack
    {
    public:
        // 构造函数
        explicit stack() = default;
        // 构造函数
        explicit stack(u64 capacity) : data_ptr_(new byte[capacity]),
                                            data_capacity_(capacity)
        {
        }

        // 禁用拷贝构造/赋值（RAII改进）
        stack(const stack &) = delete;
        stack &operator=(const stack &) = delete;

        // 移动语义支持
        stack(stack &&other) noexcept : data_ptr_(other.data_ptr_),
                                        data_top_(other.data_top_),
                                        data_capacity_(other.data_capacity_)
        {
            other.data_ptr_ = nullptr;
            other.reset();
        }

        stack &operator=(stack &&other) noexcept
        {
            if (this != &other)
            {
                delete[] data_ptr_;
                data_ptr_ = other.data_ptr_;
                data_top_ = other.data_top_;
                data_capacity_ = other.data_capacity_;
                other.data_ptr_ = nullptr;
                other.reset();
            }
            return *this;
        }

        ~stack()
        {
            delete[] data_ptr_;
        }

        // 核心功能改进
        template <typename T>
        void push(const T &data)
        {
            static_assert(std::is_trivially_copyable_v<T>,
                          "Stack only supports trivially copyable types");

            constexpr size_t align = alignof(T);
            constexpr size_t size = sizeof(T);

            // 对齐计算
            u64 new_top = (data_top_ + align - 1) & ~(align - 1);

            // 容量检查（增加安全余量）
            if ((new_top + size) > data_capacity_)
            {
                expand_capacity(new_top + size + 128); // 预分配额外空间
            }

            // 安全写入（避免strict aliasing）
            std::memcpy(data_ptr_ + new_top, &data, size);
            data_top_ = new_top + size;
        }

        template <typename T>
        T pop()
        {
            if (data_top_ == 0)
            {
                throw std::out_of_range("Stack underflow");
            }

            constexpr size_t align = alignof(T);
            constexpr size_t size = sizeof(T);

            // 计算实际存储位置
            u64 actual_top = data_top_ - size;
            u64 aligned_top = actual_top & ~(align - 1);

            // 校验对齐有效性
            if (aligned_top > actual_top || (actual_top - aligned_top) >= align)
            {
                throw std::runtime_error("Data alignment corruption");
            }

            T value;
            std::memcpy(&value, data_ptr_ + aligned_top, size);
            data_top_ = aligned_top;
            return value;
        }

        // 新增功能方法
        template <typename T>
        T &top()
        {
            if (data_top_ == 0)
            {
                throw std::out_of_range("Stack is empty");
            }

            constexpr size_t align = alignof(T);
            constexpr size_t size = sizeof(T);

            u64 actual_top = data_top_ - size;
            u64 aligned_top = actual_top & ~(align - 1);

            return *reinterpret_cast<T *>(data_ptr_ + aligned_top);
        }

        u64 size() const noexcept { return data_top_; }
        bool empty() const noexcept { return data_top_ == 0; }
        u64 capacity() const noexcept { return data_capacity_; }

        void clear() noexcept { data_top_ = 0; }

    private:
        void expand_capacity(u64 required)
        {
            u64 new_capacity = std::max(data_capacity_ * 2, required);
            byte *new_ptr = new byte[new_capacity];

            if (data_ptr_)
            {
                std::memcpy(new_ptr, data_ptr_, data_top_);
                delete[] data_ptr_;
            }

            data_ptr_ = new_ptr;
            data_capacity_ = new_capacity;
        }

        void reset() noexcept
        {
            data_top_ = 0;
            data_capacity_ = 0;
        }

        byte *data_ptr_ = nullptr;
        u64 data_top_ = 0;
        u64 data_capacity_ = 0;
    };
}
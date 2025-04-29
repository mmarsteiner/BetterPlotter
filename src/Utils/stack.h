//
// Created by Matthew on 6/06/2024.
//

#pragma once

#include <cstdint>
#include <cstdlib>

#define INIT_SIZE 10

namespace bp {

template <typename T>
class Stack {
    size_t top;
    size_t max;
    T** data;

    public:
    Stack(uint32_t initSize);
    Stack();
    ~Stack();
    void Push(T* el);
    T* Pop();

    T* Peek() const;
    bool IsEmpty() const;
    size_t Size() const;
    T* Get(size_t index) const;
    T** GetData() const;
};
}  // namespace bp

#include "stack.tpp"

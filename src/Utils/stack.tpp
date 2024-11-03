//
// Created by Matthew on 6/06/2024.
//

#pragma once

#include "stack.h"

namespace bp {
template <typename T>
Stack<T>::Stack(uint32_t initSize) {
    top = 0;
    max = initSize;
    data = static_cast<T**>(malloc(initSize * sizeof(*data)));
}

template <typename T>
Stack<T>::Stack() : Stack(INIT_SIZE) {}

template <typename T>
Stack<T>::~Stack() {
    free(data);
}

template <typename T>
void Stack<T>::Push(T* el) {
    if (top >= max) {
        max *= 2;
        data = static_cast<T**>(realloc(data, max * sizeof(*data)));
    }
    data[top++] = el;
}

template <typename T>
T* Stack<T>::Pop() {
    if (IsEmpty()) {
        top = 0;
        return nullptr;
    }
    T* ret = data[top - 1];
    top--;
    return ret;
}

template <typename T>
T* Stack<T>::Peek() const {
    if (IsEmpty()) {
        return nullptr;
    }
    return data[top - 1];
}

template <typename T>
bool Stack<T>::IsEmpty() const {
    return top <= 0;
}

template <typename T>
size_t Stack<T>::Size() const {
    return top;
}

template <typename T>
T* Stack<T>::Get(size_t index) const {
    if (index >= top) {
        return nullptr;
    }
    return data[index];
}

template <typename T>
T** Stack<T>::GetData() const {
    return data;
}
}  // namespace bp
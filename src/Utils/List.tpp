#pragma once

#include "List.h"

namespace bp {

template <typename E>
List<E>::List(size_t initCapacity) : capacity{initCapacity}, size{0} {
    data = static_cast<E**>(malloc(capacity * sizeof(E*)));
}

template <typename E>
List<E>::List() : List(INIT_SIZE) {}

template <typename E>
List<E>::~List() {
    free(data);
}

template <typename E>
void List<E>::EnsureCapacity() {
    if (size == capacity) {
        capacity *= 2;
        data = static_cast<E**>(realloc(data, capacity * sizeof(E*)));
    }
}

template <typename E>
void List<E>::Add(E* element) {
    Add(size, element);
}

template <typename E>
void List<E>::Add(size_t index, E* element) {
    if (index < 0 || index > size) {
        return;
    }
    EnsureCapacity();
    for (size_t i = size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = element;
    ++size;
}

template <typename E>
void List<E>::AddAll(E** elements, size_t numElements) {
    for (size_t i = 0; i < numElements; ++i) {
        Add(elements[i]);
    }
}

template <typename E>
E* List<E>::Get(size_t index) const {
    if (index >= size) {
        return nullptr;
    }
    return data[index];
}

template <typename E>
bool List<E>::IsEmpty() const {
    return size == 0;
}

template <typename E>
E* List<E>::Remove(size_t index) {
    if (index >= size) {
        return nullptr;
    }
    E* removed = data[index];
    for (size_t i = index; i < size; ++i) {
        data[index] = data[index + 1];
    }
    data[size - 1] = nullptr;  // not strictly necessary but may prevent confusion
    --size;
    return removed;
}

template <typename E>
E* List<E>::RemoveLast() {
    return Remove(size - 1);
}

template <typename E>
E* List<E>::Set(size_t index, E* element) {
    if (index >= size) {
        return nullptr;
    }
    E* overwritten = data[index];
    data[index] = element;
    return overwritten;
}

template <typename E>
size_t List<E>::Size() const {
    return size;
}

}  // namespace bp
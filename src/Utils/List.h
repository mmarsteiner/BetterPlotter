#pragma once

#include <cstdint>
#include <cstdlib>

#include "Error.h"

#define INIT_SIZE 10

namespace bp {

/**
 * Simple array-based list
 *
 * @tparam E type stored by the list
 * @author Matt Marsteiner
 */
template <typename E>
class List {
    size_t capacity;
    size_t size;
    E** data;
    void EnsureCapacity();

    public:
    /**
     * Constructs a `List` with an initial capacity
     *
     * @param initCapacity Initial capacity for the list
     */
    List(size_t initCapacity);

    /**
     * Constructs a `List` with an initial capacity of 10
     */
    List();

    ~List();

    /**
     * Adds the element to the end of the list
     *
     * @param element element to add to the list
     */
    void Add(E* element);

    /**
     * Adds an element to the list at the given index
     *
     * @param index Index to add the element at
     * @param element Element to add to the list
     *
     * @return Error code: either `ERR_SUCCESS` if the operation was successful or `ERR_IDX_OUT_OF_BOUNDS` if the provided index was out of bounds for the List
     */
    Error Add(size_t index, E* element);

    /**
     * Adds all the provided elements to the list
     *
     * @param elements Array of pointers to the elements that should be added to the list
     * @param numElements Number of elements in `elements`
     */
    void AddAll(E** elements, size_t numElements);

    /**
     * Gets the element from the list at the provided index
     *
     * @param index Index of the element to retrieve
     *
     * @return element at `index` or `nullptr` if the index is out of bounds
     */
    E* Get(size_t index) const;

    /**
     * Determines if the list is empty
     *
     * @return `true` if the list is empty or `false` otherwise
     */
    bool IsEmpty() const;

    /**
     * Removes the element from the list at the provided index
     *
     * @param index Index of the element to remove
     *
     * @return The element that was removed or `nullptr` if the index was out of bounds
     */
    E* Remove(size_t index);

    /**
     * Removes the last element from the list
     *
     * @return The element that was removed
     */
    E* RemoveLast();

    /**
     * Sets the element at the provided index
     *
     * @param index Index of the element to set
     * @param element Pointer to the new element
     *
     * @return the element that was overwritten or `nullptr` if the index was out of bounds
     */
    E* Set(size_t index, E* element);

    /**
     * Gets the size of the list
     *
     * @return number of elements in the list
     */
    size_t Size() const;
};
}  // namespace bp

#include "List.tpp"

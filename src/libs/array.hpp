#pragma once

#include "logger.hpp"

template <typename T, int N> struct Array {
    static constexpr int maxElements = N;
    int count = 0;
    T elements[N];

    T& operator[](int idx)
    {
        FP_ASSERT(idx >= 0, "idx negative!");
        FP_ASSERT(idx < count, "Idx out of bounds!");
        return elements[idx];
    }

    int add(T element)
    {
        FP_ASSERT(count < maxElements, "Array Full!");
        elements[count] = element;
        return count++;
    }

    void remove_idx_and_swap(int idx)
    {
        FP_ASSERT(idx >= 0, "idx negative!");
        FP_ASSERT(idx < count, "idx out of bounds!");
        elements[idx] = elements[--count];
    }

    void clear()
    {
        count = 0;
    }

    bool is_full()
    {
        return count == N;
    }
};
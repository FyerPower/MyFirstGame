#pragma once

// ###############################################
// #tag Includes
// ###############################################

#include "logger.hpp"

// ###############################################
// #tag Implementation
// ###############################################

template <typename T, int N> struct Array {
    static constexpr int maxElements = N;
    int count = 0;
    T elements[N];

    T& operator[](int idx)
    {
        Logger::asssert((idx >= 0), "idx negative!");
        Logger::asssert((idx < count), "Idx out of bounds!");
        return elements[idx];
    }

    int add(T element)
    {
        Logger::asssert((count < maxElements), "Array Full!");
        elements[count] = element;
        return count++;
    }

    void remove_idx_and_swap(int idx)
    {
        Logger::asssert((idx >= 0), "idx negative!");
        Logger::asssert((idx < count), "idx out of bounds!");
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
#pragma once

// ###############################################
// #tag Includes
// ###############################################

// ###############################################
// #tag Implementation
// ###############################################

template <typename T, int N> struct Array {
    static constexpr int maxElements = N;
    int count = 0;
    T elements[N];

    T& operator[](int idx)
    {
        PLOG_ASSERT((idx >= 0), "idx negative!");
        PLOG_ASSERT((idx < count), "Idx out of bounds!");
        return elements[idx];
    }

    int add(T element)
    {
        PLOG_ASSERT((count < maxElements), "Array Full!");
        elements[count] = element;
        return count++;
    }

    void remove_idx_and_swap(int idx)
    {
        PLOG_ASSERT((idx >= 0), "idx negative!");
        PLOG_ASSERT((idx < count), "idx out of bounds!");
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
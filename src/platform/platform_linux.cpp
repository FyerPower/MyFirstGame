#pragma once

#include "../libs/logger.hpp"

bool platform_create_window(int width, int height, char* title)
{
    // TODO: Implement Linux Platform Function
}

void platform_update_window()
{
    // TODO: Implement Linux Platform Function
}

void platform_swap_buffers()
{
    // TODO: Implement Linux Platform Function
}

void* platform_load_dynamic_library(char* dll)
{
    // TODO: Implement Linux Platform Function
}

void* platform_load_dynamic_function(void* dll, char* funName)
{
    // TODO: Implement Linux Platform Function
}

bool platform_free_dynamic_library(void* dll)
{
    // TODO: Implement Linux Platform Function
}
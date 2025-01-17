#pragma once

// ###############################################
// #tag Globals
// ###############################################

static bool isRunning = true;

// ###############################################
// #tag Functions
// ###############################################

bool platform_create_window(int width, int height, const char* title);
void platform_update_window();
void* platform_load_gl_function(const char* funcName);
void platform_swap_buffers();
void* platform_load_dynamic_library(const char* dll);
void* platform_load_dynamic_function(void* dll, const char* funName);
bool platform_free_dynamic_library(void* dll);
void platform_fill_keycode_lookup_table();
void platform_set_vsync(bool vSync);
#pragma once

#include <internal.h>
#include <mimas/mimas.h>

mimas_bool mimas_platform_init(Mimas_Backend backend,
                               Mimas_Init_Options const* options);
void mimas_platform_terminate(void);

Mimas_Window* mimas_platform_create_window(Mimas_Window_Create_Info);
void mimas_platform_destroy_window(Mimas_Window* window);

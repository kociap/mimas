#ifndef MIMAS_MIMAS_PLATFORM_H_INCLUDE
#define MIMAS_MIMAS_PLATFORM_H_INCLUDE

#include <internal.h>
#include <mimas/mimas.h>

mimas_bool mimas_platform_init(Mimas_Backend backend,
                               Mimas_Init_Options const* options);
void mimas_platform_terminate(Mimas_Backend backend);

Mimas_Window* mimas_platform_create_window(Mimas_Window_Create_Info);
void mimas_platform_destroy_window(Mimas_Window* window);

#endif // !MIMAS_MIMAS_PLATFORM_H_INCLUDE

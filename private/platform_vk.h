#ifndef MIMAS_PLATFORM_VK_H_INCLUDE
#define MIMAS_PLATFORM_VK_H_INCLUDE

#include <mimas/mimas_vk.h>

mimas_bool mimas_platform_init_with_vk();
void mimas_platform_terminate_with_vk();
char const** mimas_platform_get_vk_extensions();

#endif // !MIMAS_PLATFORM_VK_H_INCLUDE

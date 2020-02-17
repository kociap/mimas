#ifndef MIMAS_MIMAS_VK_H_INCLUDE
#define MIMAS_MIMAS_VK_H_INCLUDE

#include <mimas/mimas.h>

MIMAS_EXTERN_C_BEGIN

MIMAS_API mimas_bool mimas_init_with_vk();

/*
 * Return: A NULL terminated list of vk extension names.
 */
MIMAS_API char const** mimas_get_vk_extensions();

// void mimas_create_vk_surface(VkInstance*, VkAllocationCallbacks const*, VkSurfaceKHR*);

MIMAS_EXTERN_C_END

#endif // !MIMAS_MIMAS_VK_H_INCLUDE

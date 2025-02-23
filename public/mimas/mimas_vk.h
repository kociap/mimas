#pragma once

#include <mimas/mimas.h>

MIMAS_EXTERN_C_BEGIN

typedef struct VkInstance_T* VkInstance;
typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
struct VkAllocationCallbacks;

// mimas_init_with_vk
//
// Initializes mimas to work with Vulkan.
// The entire process is per monitor DPI aware.
//
// Returns:
// mimas_true if successfully initialized. mimas_false otherwise.
//
MIMAS_API mimas_bool mimas_init_with_vk(Mimas_Init_Options const* options);

// mimas_get_vk_extensions
//
// Returns:
// A NULL terminated list of vk extension names.
//
MIMAS_API mimas_char8 const**
mimas_get_vk_extensions(mimas_i32* extension_count);

// mimas_create_vk_surface
//
// Cast the return value to VkResult.
//
// Returns:
// VK_SUCCESS on success.
// VK_ERROR_OUT_OF_HOST_MEMORY or VK_ERROR_OUT_OF_DEVICE_MEMORY on failure.
//
mimas_i32 mimas_create_vk_surface(
  Mimas_Window* window, VkInstance vkinstance,
  struct VkAllocationCallbacks const* vkallocationcallbacks,
  VkSurfaceKHR* vksurface);

MIMAS_EXTERN_C_END

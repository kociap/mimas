#ifndef MIMAS_MIMAS_VK_H_INCLUDE
#define MIMAS_MIMAS_VK_H_INCLUDE

#include <mimas/mimas.h>

MIMAS_EXTERN_C_BEGIN

typedef struct VkInstance_T* VkInstance;
typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
struct VkAllocationCallbacks;

// Returns: mimas_true if successfully initialized. mimas_false otherwise.
//
MIMAS_API mimas_bool mimas_init_with_vk(void);

// mimas_get_vk_extensions
// Returns: A NULL terminated list of vk extension names.
//
MIMAS_API char const** mimas_get_vk_extensions(mimas_i32* extension_count);

// mimas_create_vk_surface
// Returns: VK_SUCCESS on success, VK_ERROR_OUT_OF_HOST_MEMORY or VK_ERROR_OUT_OF_DEVICE_MEMORY on failure.
//
mimas_i32 mimas_create_vk_surface(Mimas_Window* window, VkInstance vkinstance, struct VkAllocationCallbacks const* vkallocationcallbacks, VkSurfaceKHR* vksurface);

MIMAS_EXTERN_C_END

#endif // !MIMAS_MIMAS_VK_H_INCLUDE

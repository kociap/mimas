#ifndef MIMAS_PLATFORM_VK_H_INCLUDE
#define MIMAS_PLATFORM_VK_H_INCLUDE

#include <mimas/mimas_vk.h>

typedef mimas_u32 VkFlags;

typedef enum VkStructureType {
  VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR = 1000004000,
  VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR = 1000005000,
  VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR = 1000006000,
  VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR = 1000008000,
  VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR = 1000009000,
  VK_STRUCTURE_TYPE_MAX_ENUM = 0x7FFFFFFF
} VkStructureType;

typedef enum VkResult {
  VK_SUCCESS = 0,
  VK_ERROR_OUT_OF_HOST_MEMORY = -1,
  VK_ERROR_OUT_OF_DEVICE_MEMORY = -2,
  VK_RESULT_MAX_ENUM = 0x7FFFFFFF
} VkResult;

mimas_bool mimas_platform_init_vk_backend(void);
void mimas_platform_terminate_vk_backend(void);
mimas_char8 const**
mimas_platform_get_vk_extensions(mimas_i32* extension_count);
VkResult mimas_platform_create_vk_surface(Mimas_Window*, VkInstance,
                                          struct VkAllocationCallbacks const*,
                                          VkSurfaceKHR*);

#endif // !MIMAS_PLATFORM_VK_H_INCLUDE

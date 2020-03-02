#include <linux/x11/platform.h>
#include <platform_vk.h>
#include <internal.h>
#include <dlfcn.h>

static void* vulkan_module = NULL;

typedef struct VkXlibSurfaceCreateInfoKHR {
    VkStructureType sType;
    void const* pNext;
    VkFlags flags;
    Display* dpy;
    Window window;
} VkXlibSurfaceCreateInfoKHR;


typedef void (*PFN_vkVoidFunction)(void);
typedef PFN_vkVoidFunction (*PFN_vkGetInstanceProcAddr)(VkInstance, char const*);
static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

typedef VkResult (*PFN_vkCreateXlibSurfaceKHR)(VkInstance, VkXlibSurfaceCreateInfoKHR const*, struct VkAllocationCallbacks const*, VkSurfaceKHR*);

mimas_bool mimas_platform_init_vk_backend() {
    vulkan_module = dlopen("vulkan.so", RTLD_LAZY | RTLD_LOCAL);
    if (vulkan_module) {
        vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(vulkan_module, "vkGetInstanceProcAddr");
        return mimas_true;
    } else {
        // TODO: Error
        return mimas_false;
    }
}

void mimas_platform_terminate_vk_backend() {
    dlclose(vulkan_module);
}

char const** mimas_platform_get_vk_extensions(mimas_i32* extension_count) {
    static char const* vk_win_extensions[] = { "VK_KHR_surface", "VK_KHR_xlib_surface", NULL };
    *extension_count = 2;
    return vk_win_extensions;
}

mimas_i32 mimas_platform_create_vk_surface(Mimas_Window* const window, VkInstance const instance, struct VkAllocationCallbacks const* allocation_callbacks, VkSurfaceKHR* surface) {
    PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateXlibSurfaceKHR");
    Mimas_X11_Window* const native_window = (Mimas_X11_Window*)window->native_window;
    VkXlibSurfaceCreateInfoKHR const vk_win32_info = {
        .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .window = native_window->window,
        .dpy = native_window->display,
    };
    return vkCreateXlibSurfaceKHR(instance, &vk_win32_info, allocation_callbacks, surface);
}

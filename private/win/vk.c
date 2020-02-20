#include <win/platform.h>
#include <platform_vk.h>
#include <internal.h>

static HMODULE vulkan_module = NULL;

typedef struct VkWin32SurfaceCreateInfoKHR {
    VkStructureType sType;
    void const* pNext;
    VkFlags flags;
    HINSTANCE hinstance;
    HWND hwnd;
} VkWin32SurfaceCreateInfoKHR;

typedef void (*PFN_vkVoidFunction)(void);
typedef PFN_vkVoidFunction (*PFN_vkGetInstanceProcAddr)(VkInstance, char const*);
static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

typedef VkResult (*PFN_vkCreateWin32SurfaceKHR)(VkInstance, VkWin32SurfaceCreateInfoKHR const*, struct VkAllocationCallbacks const*, VkSurfaceKHR*);

mimas_bool mimas_platform_init_vk_backend() {
    vulkan_module = LoadLibraryW(L"vulkan-1.dll");
    if(vulkan_module) {
        vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vulkan_module, "vkGetInstanceProcAddr");
        return mimas_true;
    } else {
        // TODO: Error
        return mimas_false;
    }
}

void mimas_platform_terminate_vk_backend() {
    FreeLibrary(vulkan_module);
}

char const** mimas_platform_get_vk_extensions() {
    static char const* vk_win_extensions[] = {"VK_KHR_surface", "VK_KHR_win32_surface", NULL};
    return vk_win_extensions;
}

mimas_i32 mimas_platform_create_vk_surface(Mimas_Window* const window, VkInstance const instance, struct VkAllocationCallbacks const* allocation_callbacks, VkSurfaceKHR* surface) {
    PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    VkWin32SurfaceCreateInfoKHR const vk_win32_info = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .hinstance = NULL,
        .hwnd = native_window->handle,
    };
    return vkCreateWin32SurfaceKHR(instance, &vk_win32_info, allocation_callbacks, surface);
}

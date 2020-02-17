#include <win/platform.h>

static HMODULE vulkan_module = NULL;

// typedef (*PFN_vkGetInstanceProcAddr);
// static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

mimas_bool mimas_platform_init_with_vk() {
    vulkan_module = LoadLibraryW(L"vulkan-1.dll");
    if(vulkan_module) {
        // GetProcAddress
        return mimas_true;
    } else {
        // TODO: Error
        return mimas_false;
    }
}

void mimas_platform_terminate_with_vk() {
    FreeLibrary(vulkan_module);
}

char const** mimas_platform_get_vk_extensions() {
    static char const* vk_win_extensions[] = {"VK_KHR_surface", "VK_KHR_win32_surface", NULL};
    return vk_win_extensions;
}

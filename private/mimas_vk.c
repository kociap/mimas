#include <mimas/mimas_gl.h>
#include <internal.h>
#include <platform_vk.h>
#include <platform.h>

mimas_bool mimas_init_with_vk() {
    _mimas_init_internal(MIMAS_BACKEND_VK);
    mimas_bool const res =  mimas_platform_init(MIMAS_BACKEND_VK);
    if(!res) {
        _mimas_terminate_internal();
    }
    return res;
}

char const** mimas_get_vk_extensions(mimas_i32* extension_count) {
    return mimas_platform_get_vk_extensions(extension_count);
}

mimas_i32 mimas_create_vk_surface(Mimas_Window* const window, VkInstance const instance, struct VkAllocationCallbacks const* allocation_callbacks, VkSurfaceKHR* const surface) {
    return mimas_platform_create_vk_surface(window, instance, allocation_callbacks, surface);
}

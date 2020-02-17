#include <mimas/mimas_gl.h>
#include <internal.h>
#include <platform_vk.h>

mimas_bool mimas_init_with_vk() {
    _mimas_init_internal(MIMAS_BACKEND_VK);
    mimas_bool const res =  mimas_platform_init_with_vk();
    if(!res) {
        _mimas_terminate_internal();
    }
    return res;
}

char const** mimas_get_vk_extensions() {
    return mimas_platform_get_vk_extensions();
}

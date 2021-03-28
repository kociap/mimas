target_sources(mimas
    PRIVATE    
    "${CMAKE_CURRENT_SOURCE_DIR}/internal.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/internal.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/mimas_vk.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/mimas_gl.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/mimas.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/platform_vk.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/platform_gl.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/platform.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils.h"
)

target_include_directories(mimas PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

if(WIN32)
    target_sources(mimas
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/win/cursor.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/display.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/file_dialog.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/gl.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/input.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/platform.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/time.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/vk.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/wgl.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/wgl.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/window.c"
    )
    target_link_libraries(mimas PRIVATE dwmapi gdi32 comdlg32 propsys shlwapi)

    if(BUILD_SHARED_LIBS)
        target_compile_definitions(mimas PRIVATE MIMAS_BUILDING_DLL=1)
    endif()
else()
    target_sources(mimas
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/linux/time.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/linux/x11/platform.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/linux/x11/vk.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/linux/x11/window.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/linux/x11/init.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/linux/x11/init.c"
    )
endif()
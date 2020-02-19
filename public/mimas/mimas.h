#ifndef MIMAS_MIMAS_H_INCLUDE
#define MIMAS_MIMAS_H_INCLUDE

#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
    #define MIMAS_WIN
#endif

#if MIMAS_WIN
    #if defined(MIMAS_BUILDING_DLL)
        #define MIMAS_API __declspec(dllexport)
    #elif defined(MIMAS_DLL)
        #define MIMAS_API __declspec(dllimport)
    #else
        #define MIMAS_API 
    #endif
#else 
    #define MIMAS_API
#endif

#if defined(__cplusplus)
    #define MIMAS_EXTERN_C_BEGIN extern "C" {
    #define MIMAS_EXTERN_C_END }
#else 
    #define MIMAS_EXTERN_C_BEGIN
    #define MIMAS_EXTERN_C_END
#endif

MIMAS_EXTERN_C_BEGIN

typedef enum Mimas_Key {
    MIMAS_KEY_UNKNOWN = -1,

    MIMAS_KEY_0 = 48,
    MIMAS_KEY_1 = 49,
    MIMAS_KEY_2 = 50,
    MIMAS_KEY_3 = 51,
    MIMAS_KEY_4 = 52,
    MIMAS_KEY_5 = 53,
    MIMAS_KEY_6 = 54,
    MIMAS_KEY_7 = 55,
    MIMAS_KEY_8 = 56,
    MIMAS_KEY_9 = 57,

    MIMAS_KEY_A = 65,
    MIMAS_KEY_B = 66,
    MIMAS_KEY_C = 67,
    MIMAS_KEY_D = 68,
    MIMAS_KEY_E = 69,
    MIMAS_KEY_F = 70,
    MIMAS_KEY_G = 71,
    MIMAS_KEY_H = 72,
    MIMAS_KEY_I = 73,
    MIMAS_KEY_J = 74,
    MIMAS_KEY_K = 75,
    MIMAS_KEY_L = 76,
    MIMAS_KEY_M = 77,
    MIMAS_KEY_N = 78,
    MIMAS_KEY_O = 79,
    MIMAS_KEY_P = 80,
    MIMAS_KEY_Q = 81,
    MIMAS_KEY_R = 82,
    MIMAS_KEY_S = 83,
    MIMAS_KEY_T = 84,
    MIMAS_KEY_U = 85,
    MIMAS_KEY_V = 86,
    MIMAS_KEY_W = 87,
    MIMAS_KEY_X = 88,
    MIMAS_KEY_Y = 89,
    MIMAS_KEY_Z = 90,
} Mimas_Key;

typedef enum Mimas_Key_Action {
    MIMAS_KEY_RELEASE,
    MIMAS_KEY_PRESS,
    MIMAS_KEY_REPEAT,
} Mimas_Key_Action;

typedef int mimas_bool;
#define mimas_true 1
#define mimas_false 0

typedef char mimas_i8;
typedef unsigned char mimas_u8;
typedef short mimas_i16;
typedef unsigned short mimas_u16;
typedef int mimas_i32;
typedef unsigned int mimas_u32;
typedef long long mimas_i64;
typedef unsigned long long mimas_u64;

typedef enum Mimas_Cursor_Mode {
    MIMAS_CURSOR_NORMAL,
    MIMAS_CURSOR_CAPTURED,
    MIMAS_CURSOR_VIRTUAL,
} Mimas_Cursor_Mode;

typedef struct Mimas_Window Mimas_Window;

MIMAS_API void mimas_terminate();

MIMAS_API void mimas_poll_events();

typedef struct Mimas_Window_Create_Info {
    mimas_i32 width;
    mimas_i32 height;
    char const* title;
    mimas_bool decorated;
} Mimas_Window_Create_Info;

MIMAS_API Mimas_Window* mimas_create_window(Mimas_Window_Create_Info);
MIMAS_API void mimas_destroy_window(Mimas_Window* window);

/*
 * activated parameter is 1 if activated, 0 if deactivated.
 */
typedef void(*mimas_window_activate_callback)(Mimas_Window* window, mimas_i32 activated, void* user_data);
MIMAS_API void mimas_set_window_activate_callback(Mimas_Window* window, mimas_window_activate_callback callback, void* user_data);

/*
 * x and y are the screen coordinates of the cursor.
 */
typedef void(*mimas_window_cursor_pos_callback)(Mimas_Window* window, mimas_i32 x, mimas_i32 y, void* user_data);
MIMAS_API void mimas_set_window_cursor_pos_callback(Mimas_Window* window, mimas_window_cursor_pos_callback callback, void* user_data);

typedef void(* mimas_window_key_callback)(Mimas_Window* window, Mimas_Key key, Mimas_Key_Action state, void* user_data);
MIMAS_API void mimas_set_window_key_callback(Mimas_Window* window, mimas_window_key_callback callback, void* user_data);

MIMAS_API void mimas_set_window_pos(Mimas_Window* window, mimas_i32 x, mimas_i32 y);
MIMAS_API void mimas_get_window_pos(Mimas_Window* window, mimas_i32* x, mimas_i32* y);
MIMAS_API void mimas_set_window_content_pos(Mimas_Window* window, mimas_i32 x, mimas_i32 y);
MIMAS_API void mimas_get_window_content_pos(Mimas_Window* window, mimas_i32* x, mimas_i32* y);
MIMAS_API void mimas_set_window_content_size(Mimas_Window* window, mimas_i32 width, mimas_i32 height);
MIMAS_API void mimas_get_window_content_size(Mimas_Window* window, mimas_i32* width, mimas_i32* height);

MIMAS_API void mimas_show_window(Mimas_Window* window);
MIMAS_API void mimas_hide_window(Mimas_Window* window);
MIMAS_API void mimas_restore_window(Mimas_Window* window);
MIMAS_API void mimas_minimize_window(Mimas_Window* window);
MIMAS_API void mimas_maximize_window(Mimas_Window* window);

MIMAS_API void mimas_swap_buffers(Mimas_Window* window);
MIMAS_API void mimas_set_swap_interval(mimas_i32);
MIMAS_API mimas_i32 mimas_get_swap_interval();

MIMAS_API void mimas_set_cursor_mode(Mimas_Window* window, Mimas_Cursor_Mode);
MIMAS_API void mimas_get_cursor_pos(mimas_i32* x, mimas_i32* y);

MIMAS_EXTERN_C_END

#endif // !MIMAS_MIMAS_H_INCLUDE

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

typedef enum Mimas_Key {
    MIMAS_KEY_UNKNOWN = -1,

    MIMAS_MOUSE_LEFT_BUTTON,
    MIMAS_MOUSE_RIGHT_BUTTON,
    MIMAS_MOUSE_MIDDLE_BUTTON,
    MIMAS_MOUSE_THUMB_BUTTON_1,
    MIMAS_MOUSE_THUMB_BUTTON_2,
    MIMAS_MOUSE_WHEEL,
    MIMAS_MOUSE_HORIZ_WHEEL,

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

    MIMAS_KEY_SLASH,
    MIMAS_KEY_COMMA,
    MIMAS_KEY_DOT,
    MIMAS_KEY_SEMICOLON,
    MIMAS_KEY_LEFT_BRACKET,
    MIMAS_KEY_RIGHT_BRACKET,
    MIMAS_KEY_MINUS,
    MIMAS_KEY_EQUALS,
    MIMAS_KEY_APOSTROPHE,
    MIMAS_KEY_TICK,
    MIMAS_KEY_BACKWARD_SLASH,

    MIMAS_KEY_F1,
    MIMAS_KEY_F2,
    MIMAS_KEY_F3,
    MIMAS_KEY_F4,
    MIMAS_KEY_F5,
    MIMAS_KEY_F6,
    MIMAS_KEY_F7,
    MIMAS_KEY_F8,
    MIMAS_KEY_F9,
    MIMAS_KEY_F10,
    MIMAS_KEY_F11,
    MIMAS_KEY_F12,

    MIMAS_KEY_LEFT,
    MIMAS_KEY_RIGHT,
    MIMAS_KEY_UP,
    MIMAS_KEY_DOWN,

    MIMAS_KEY_CAPS_LOCK,
    MIMAS_KEY_TAB,
    MIMAS_KEY_PAGE_UP,
    MIMAS_KEY_PAGE_DOWN,
    MIMAS_KEY_HOME,
    MIMAS_KEY_END,
    MIMAS_KEY_INSERT,
    MIMAS_KEY_DELETE,
    MIMAS_KEY_BACKSPACE,
    MIMAS_KEY_SPACE,
    MIMAS_KEY_ENTER,
    MIMAS_KEY_ESCAPE,

    MIMAS_KEY_LEFT_SHIFT,
    MIMAS_KEY_RIGHT_SHIFT,
    MIMAS_KEY_LEFT_CONTROL,
    MIMAS_KEY_RIGHT_CONTROL,
    MIMAS_KEY_LEFT_ALT,
    MIMAS_KEY_RIGHT_ALT,

    MIMAS_KEY_NUMPAD_0,
    MIMAS_KEY_NUMPAD_1,
    MIMAS_KEY_NUMPAD_2,
    MIMAS_KEY_NUMPAD_3,
    MIMAS_KEY_NUMPAD_4,
    MIMAS_KEY_NUMPAD_5,
    MIMAS_KEY_NUMPAD_6,
    MIMAS_KEY_NUMPAD_7,
    MIMAS_KEY_NUMPAD_8,
    MIMAS_KEY_NUMPAD_9,
    MIMAS_KEY_NUMPAD_ADD,
    MIMAS_KEY_NUMPAD_SUBTRACT,
    MIMAS_KEY_NUMPAD_DIVIDE,
    MIMAS_KEY_NUMPAD_MULTIPLY,
    MIMAS_KEY_NUMPAD_DECIMAL,
    MIMAS_KEY_NUMPAD_ENTER,
    MIMAS_KEY_NUMLOCK,

    // TODO: Those keys are not directly mappable.
    //       We have to translate them from a sequence of scan codes.
    MIMAS_KEY_PAUSE,
    MIMAS_KEY_PRINT_SCREEN,
    MIMAS_KEY_SCROLL_LOCK,
} Mimas_Key;

typedef enum Mimas_Key_Action {
    MIMAS_KEY_RELEASE,
    MIMAS_KEY_PRESS,
    MIMAS_KEY_REPEAT,
} Mimas_Key_Action;

typedef enum Mimas_Mouse_Button_Action {
    MIMAS_MOUSE_BUTTON_RELEASE,
    MIMAS_MOUSE_BUTTON_PRESS,
    MIMAS_MOUSE_BUTTON_DOUBLECLICK,
} Mimas_Mouse_Button_Action;

typedef enum Mimas_Hittest_Result {
    MIMAS_HITTEST_TOP,
    MIMAS_HITTEST_BOTTOM,
    MIMAS_HITTEST_LEFT,
    MIMAS_HITTEST_RIGHT,
    MIMAS_HITTEST_TOP_LEFT,
    MIMAS_HITTEST_TOP_RIGHT,
    MIMAS_HITTEST_BOTTOM_LEFT,
    MIMAS_HITTEST_BOTTOM_RIGHT,
    MIMAS_HITTEST_CLIENT,
    MIMAS_HITTEST_TITLEBAR,
    MIMAS_HITTEST_MINIMIZE,
    MIMAS_HITTEST_MAXIMIZE,
    MIMAS_HITTEST_CLOSE,
    MIMAS_HITTEST_NOWHERE,
} Mimas_Hittest_Result;

typedef struct Mimas_Rect {
    mimas_i32 left;
    mimas_i32 top;
    mimas_i32 right;
    mimas_i32 bottom;
} Mimas_Rect;

typedef struct Mimas_Display Mimas_Display;
typedef struct Mimas_Window Mimas_Window;

typedef struct Mimas_Display_Settings {
    mimas_i64 width;
    mimas_i64 height;
    mimas_i64 refresh_rate;
} Mimas_Display_Settings;

MIMAS_API void mimas_terminate(void);

MIMAS_API void mimas_poll_events(void);

MIMAS_API Mimas_Display* mimas_get_primary_display(void);
MIMAS_API Mimas_Display** mimas_get_displays(mimas_i64* count);
MIMAS_API Mimas_Display_Settings mimas_get_display_settings(Mimas_Display* display);

// Makes a window non-exclusive fullscreen on given display.
// If display is NULL, the window will be restored to normal mode. The window will not be restored to the previous size.
//   If you want to restore the window to its previous size, store its size before you fullscreen the window. 
//   After you restore the window to normal state, call mimas_set_window_content_size with the saved values.
// The window will always be shown after this function finishes, but will not be active. 
//   If you want to hide the widnow, see mimas_hide_window.
//
MIMAS_API void mimas_fullscreen_window(Mimas_Window* window, Mimas_Display* display);

typedef struct Mimas_Window_Create_Info {
    mimas_i32 width;
    mimas_i32 height;
    char const* title;
    mimas_bool decorated;
} Mimas_Window_Create_Info;

MIMAS_API Mimas_Window* mimas_create_window(Mimas_Window_Create_Info);
MIMAS_API void mimas_destroy_window(Mimas_Window* window);
MIMAS_API mimas_bool mimas_close_requested(Mimas_Window* window);

typedef struct Mimas_Callback {
    void* callback;
    void* user_data;
} Mimas_Callback;

// activated parameter is 1 if activated, 0 if deactivated.
//
typedef void (*mimas_window_activate_callback)(Mimas_Window* window, mimas_i32 activated, void* user_data);
MIMAS_API void mimas_set_window_activate_callback(Mimas_Window* window, mimas_window_activate_callback callback, void* user_data);
MIMAS_API Mimas_Callback mimas_get_window_activate_callback(Mimas_Window* window);


typedef void (*mimas_window_resize_callback)(Mimas_Window* window, mimas_i32 w, mimas_i32 h, void* user_data);
MIMAS_API void mimas_set_window_resize_callback(Mimas_Window* window, mimas_window_resize_callback, void* user_data);
MIMAS_API Mimas_Callback mimas_get_window_resize_callback(Mimas_Window* window);

// x and y are the screen coordinates of the cursor.
//
typedef void (*mimas_window_cursor_pos_callback)(Mimas_Window* window, mimas_i32 x, mimas_i32 y, void* user_data);
MIMAS_API void mimas_set_window_cursor_pos_callback(Mimas_Window* window, mimas_window_cursor_pos_callback callback, void* user_data);
MIMAS_API Mimas_Callback mimas_get_cursor_pos_callback(Mimas_Window* window);

typedef void(*mimas_window_mouse_button_callback)(Mimas_Window* window, Mimas_Key button, Mimas_Mouse_Button_Action action, void* user_data);
MIMAS_API void mimas_set_window_mouse_button_callback(Mimas_Window* window, mimas_window_mouse_button_callback callback, void* user_data);
MIMAS_API Mimas_Callback mimas_get_window_mouse_button_callback(Mimas_Window* window);

typedef void(*mimas_window_scroll_callback)(Mimas_Window* window, mimas_i32 d_x, mimas_i32 d_y, void* user_data);
MIMAS_API void mimas_set_window_scroll_callback(Mimas_Window* window, mimas_window_scroll_callback callback, void* user_data);
MIMAS_API Mimas_Callback mimas_get_window_scroll_callback(Mimas_Window* window);

typedef void (*mimas_window_key_callback)(Mimas_Window* window, Mimas_Key key, Mimas_Key_Action action, void* user_data);
MIMAS_API void mimas_set_window_key_callback(Mimas_Window* window, mimas_window_key_callback callback, void* user_data);
MIMAS_API Mimas_Callback mimas_get_window_key_callback(Mimas_Window* window);

typedef void(*mimas_window_close_requested)(Mimas_Window* window, void* user_data);
MIMAS_API void mimas_set_window_close_requested_callback(Mimas_Window* window, void* user_data);
MIMAS_API Mimas_Callback mimas_get_window_close_requested_callback(Mimas_Window* window);

// Set custom hit function for the native window to define custom resize, drag, minimize, maximize and close behaviour.
//
typedef Mimas_Hittest_Result (*mimas_window_hittest)(Mimas_Window* window, mimas_i32 cursor_x, mimas_i32 cursor_y, Mimas_Rect window_rect, Mimas_Rect client_rect);
MIMAS_API void mimas_set_window_hittest(Mimas_Window* window, mimas_window_hittest callback);
MIMAS_API Mimas_Callback mimas_get_window_hittest(Mimas_Window* window);

MIMAS_API void mimas_set_window_pos(Mimas_Window* window, mimas_i32 x, mimas_i32 y);
MIMAS_API void mimas_get_window_pos(Mimas_Window* window, mimas_i32* x, mimas_i32* y);
MIMAS_API void mimas_set_window_content_pos(Mimas_Window* window, mimas_i32 x, mimas_i32 y);
MIMAS_API void mimas_get_window_content_pos(Mimas_Window* window, mimas_i32* x, mimas_i32* y);
MIMAS_API void mimas_set_window_content_size(Mimas_Window* window, mimas_i32 width, mimas_i32 height);
MIMAS_API void mimas_get_window_content_size(Mimas_Window* window, mimas_i32* width, mimas_i32* height);
MIMAS_API mimas_bool mimas_is_window_active(Mimas_Window* window);

MIMAS_API void mimas_show_window(Mimas_Window* window);
MIMAS_API void mimas_hide_window(Mimas_Window* window);
MIMAS_API void mimas_restore_window(Mimas_Window* window);
MIMAS_API void mimas_minimize_window(Mimas_Window* window);
MIMAS_API void mimas_maximize_window(Mimas_Window* window);

// Returns: MIMAS_KEY_RELEASE when the key is released, MIMAS_KEY_PRESS when the key is pressed.
//
MIMAS_API Mimas_Key_Action mimas_get_key(Mimas_Key key);

// Confines the cursor to an area inside window defined by region.
//
// region is in screen coordinates relative to top-left corner of the client area of the window
// and will be cropped so that it doesn't extend outside the window. 
// If region is NULL, the cursor will be released.
//
MIMAS_API void mimas_clip_cursor(Mimas_Window* window, Mimas_Rect const* region);

MIMAS_API void mimas_lock_cursor(Mimas_Window* window);
MIMAS_API void mimas_unlock_cursor(Mimas_Window* window);

MIMAS_API void mimas_get_cursor_pos(mimas_i32* x, mimas_i32* y);
MIMAS_API void mimas_set_cursor_pos(mimas_i32 x, mimas_i32 y);

typedef enum Mimas_Standard_Cursor {
    MIMAS_CURSOR_ARROW,
    MIMAS_CURSOR_RESIZE_RIGHT,
    MIMAS_CURSOR_RESIZE_LEFT,
    MIMAS_CURSOR_RESIZE_TOP,
    MIMAS_CURSOR_RESIZE_BOTTOM,
    MIMAS_CURSOR_RESIZE_TOP_LEFT,
    MIMAS_CURSOR_RESIZE_TOP_RIGHT,
    MIMAS_CURSOR_RESIZE_BOTTOM_LEFT,
    MIMAS_CURSOR_RESIZE_BOTTOM_RIGHT,
} Mimas_Standard_Cursor;

typedef struct Mimas_Cursor Mimas_Cursor;

MIMAS_API Mimas_Cursor* mimas_create_standard_cursor(Mimas_Standard_Cursor cursor);
MIMAS_API void mimas_destroy_cursor(Mimas_Cursor* cursor);

// If cursor is NULL, the system cursor will be hidden.
//
MIMAS_API void mimas_set_cursor(Mimas_Window* window, Mimas_Cursor* cursor);

// File dialog

typedef enum Mimas_File_Dialog_Flags {
    // Allows the file dialog to pick files
    MIMAS_FILE_DIALOG_PICK_FILES = 1,
    // Allows the file dialog to pick folders
    MIMAS_FILE_DIALOG_PICK_FOLDERS = 2,
    // Opens an "open file" dialog. You must specify either this or MIMAS_FILE_DIALOG_SAVE, or mimas_open_file_dialog will return NULL
    MIMAS_FILE_DIALOG_OPEN = 4,
    // Opens a "save file" dialog. You must specify either this or MIMAS_FILE_DIALOG_OPEN, or mimas_open_file_dialog will return NULL
    MIMAS_FILE_DIALOG_SAVE = 8
} Mimas_File_Dialog_Flags;

typedef struct Mimas_File_Filter {
    // Display name of the filter
    char const* name;
    // The actual filter string. An example of a filter string accepting only png files is "*.png"
    // To filter for multiple types in the same filter string you can separate filters by a semicolon: "*.png;*.jpg"
    char const* filter;
} Mimas_File_Filter;

// Opens a native file dialog. For an explanation on the different flags and filters, see comments for Mimas_File_Dialog_Flags and 
// Mimas_File_Filter. If the file selection is canceled or no file was selected, this function returns NULL. Otherwise it returns a
// pointer to a character array that has to be freed by the user, or NULL if the function failed for any other reason
MIMAS_API char* mimas_open_file_dialog(Mimas_File_Dialog_Flags flags, Mimas_File_Filter* filters, mimas_u64 filter_count, char const* default_path);

// Time 

typedef struct Mimas_System_Time {
    mimas_u16 year;
    // 1 through 12
    mimas_u16 month;
    // 1 through 31
    mimas_u16 day;
    // 0 (Sunday) through 6 (Saturday)
    mimas_u16 day_of_week;
    // 0 through 23
    mimas_u16 hour;
    // 0 through 59
    mimas_u16 minutes;
    // 0 through 59
    mimas_u16 seconds;
    // 0 through 999
    mimas_u16 milliseconds;
} Mimas_System_Time;

MIMAS_API Mimas_System_Time mimas_get_utc_system_time(void);
MIMAS_API Mimas_System_Time mimas_get_local_system_time(void);

// Returns time in seconds
MIMAS_API double mimas_get_time(void);

MIMAS_EXTERN_C_END

#endif // !MIMAS_MIMAS_H_INCLUDE

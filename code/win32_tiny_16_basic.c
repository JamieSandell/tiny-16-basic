#include <windows.h>

#define internal_function static
#define global static

#define UNUSED_PARAMETER(P) P

struct window
{
    HINSTANCE instance;
    HWND handle;
};

global const int failure = 1;
global struct window main_window;
global const int success = 0;

internal_function
LRESULT CALLBACK
win32_main_window_callback(HWND window_handle,
                           UINT message,
                           WPARAM w_param,
                           LPARAM l_param)
{
    LRESULT result = 0;
    
    switch (message)
    {
        case WM_ACTIVATEAPP:
        {
            
        } break;
        
        case WM_CLOSE:
        {
            DestroyWindow(main_window.handle);
        } break;
        
        case WM_DESTROY:
        {
            PostQuitMessage(success);
        } break;
        
        case WM_SIZE:
        {
        } break;
        
        default:
        {
            result = DefWindowProc(window_handle, message, w_param, l_param);
        } break;
    }
    
    return result;
}

int WINAPI
WinMain
(
 HINSTANCE instance,
 HINSTANCE prev_instance,
 LPSTR command_line,
 int show_command
 )
{
    UNUSED_PARAMETER(command_line);
    WNDCLASSA window_class = {0};
    const char class_name[] = "Tiny16BasicWindowClass";
    
    if (!prev_instance)
    {
        window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        window_class.lpfnWndProc = win32_main_window_callback;
        window_class.hInstance = instance;
        // window_class.hIcon; // TODO: Supply and icon
        window_class.lpszClassName = class_name;
        
        if (!RegisterClass(&window_class))
        {
            return failure;
        }
    }
    
    HWND handle = CreateWindowA(
                                class_name,
                                "Tiny16Basic",
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                NULL,
                                NULL,
                                NULL,
                                NULL);
    
    if (handle == NULL)
    {
        // TODO: Logging
        return failure;
    }
    
    main_window.instance = window_class.hInstance;
    main_window.handle = handle;
    
    ShowWindow(main_window.handle, show_command);
    UpdateWindow(main_window.handle);
    
    BOOL result;
    MSG msg = {0};
    
    while ((result = GetMessage(&msg, main_window.handle, 0, 0)))
    {
        if (result == -1)
        {
            // TODO: handle error, exit?
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    return (int)msg.wParam;
}


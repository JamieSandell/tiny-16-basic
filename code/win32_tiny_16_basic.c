#include <stdint.h>
#include <windows.h>

#define internal_function static
#define global static

#define u8 uint8_t
#define u32 uint32_t

#define UNUSED_PARAMETER(P) P

struct win32_back_buffer
{
    BITMAPINFO info;
    void *memory;
    int width;
    int height;
    int bytes_per_pixel;
};

struct window
{
    HINSTANCE instance;
    HWND handle;
};

global struct win32_back_buffer back_buffer;
global HDC bitmap_dc;
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
            RECT client_rect;
            if (!GetClientRect(main_window.handle, &client_rect))
            {
                // TODO: handle error, logging
            }
            else
            {
                void *back_buffer = VirtualAlloc(
                                                 NULL,
                                                 bytes_per_pixel * resolution_width * resolution_height,
                                                 MEM_COMMIT | MEM_RESERVE,
                                                 PAGE_READWRITE);
                
                if (back_buffer == NULL)
                {
                    // TODO: handle error, logging
                    PostQuitMessage(failure);
                }
                
                u32*start = (u32*)back_buffer;
                u32*location;
                u8* pixel;
                
                for (int y = 0; y < resolution_height; ++y)
                {
                    location = start + (y * resolution_height);
                    for (int x = 0; x < resolution_width; ++x)
                    {
                        pixel = (u8*)location;
                        *pixel++ = 0xFF; // BB
                        *pixel++ = 0x00; // GG
                        *pixel++ = 0x00; // RR
                        *pixel = 0xFF; // A
                    }
                }
                
                HDC dc = GetDC(main_window.handle);
                
                if (dc == NULL)
                {
                    // TODO: handle error, logging
                    PostQuitMessage(failure);
                }
                
                if (!ReleaseDC(main_window.handle, dc))
                {
                    // TODO: handle error, logging
                }
                
                if(!VirtualFree(back_buffer, 0, MEM_RELEASE))
                {
                    // TODO: handle error, logging
                }
            }
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
    
    HDC device_context = GetDC(main_window.handle);
    
    if (device_context == NULL)
    {
        // TODO: Logging
        PostQuitMessage(failure);
    }
    
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


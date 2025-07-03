#include <stdint.h>
#include <windows.h>

#define internal_function static
#define global static

#define u8 uint8_t
#define u32 uint32_t

#define UNUSED_PARAMETER(P) P

struct win32_offscreen_buffer
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

global struct win32_offscreen_buffer global_back_buffer;
global const int failure = 1;
global struct window main_window;
global const int resolution_height = 270;
global const int resolution_width = 480;
global const int success = 0;
global void *test_bitmap;

internal_function
void
win32_render_back_buffer(void);

internal_function
LRESULT CALLBACK
win32_main_window_callback(HWND window_handle,
                           UINT message,
                           WPARAM w_param,
                           LPARAM l_param);

int WINAPI
WinMain
(
 HINSTANCE instance,
 HINSTANCE prev_instance,
 LPSTR command_line,
 int show_command
 )
{
    global_back_buffer.width = resolution_width;
    global_back_buffer.height = resolution_height;
    global_back_buffer.bytes_per_pixel = 4; // TODO: No magic numbers
    
    global_back_buffer.memory = VirtualAlloc(
                                             NULL,
                                             global_back_buffer.bytes_per_pixel * global_back_buffer.width * global_back_buffer.height,
                                             MEM_COMMIT | MEM_RESERVE,
                                             PAGE_READWRITE);
    
    if (global_back_buffer.memory == NULL)
    {
        // TODO: handle error, logging
        PostQuitMessage(failure);
    }
    
    global_back_buffer.info.bmiHeader.biSize = sizeof(global_back_buffer.info.bmiHeader);
    global_back_buffer.info.bmiHeader.biWidth = global_back_buffer.width;
    global_back_buffer.info.bmiHeader.biHeight = global_back_buffer.height;
    global_back_buffer.info.bmiHeader.biPlanes = 1;
    global_back_buffer.info.bmiHeader.biBitCount = (WORD)(global_back_buffer.bytes_per_pixel * 8); //TODO: No magic numbers
    global_back_buffer.info.bmiHeader.biCompression = BI_RGB;
    
    u32*start = (u32*)global_back_buffer.memory;
    u32*location;
    u8* pixel;
    
    for (int y = 0; y < resolution_height; ++y)
    {
        for (int x = 0; x < resolution_width; ++x)
        {
            location = start + (y * resolution_width) + x;
            pixel = (u8*)location;
            *pixel++ = 0xFF; // BB
            *pixel++ = 0x00; // GG
            *pixel++ = 0x00; // RR
            *pixel = 0xFF; // A
        }
    }
    
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
    
    HANDLE file_handle = CreateFileA(
                                     "../assets/basic.bmp",
                                     GENERIC_READ,
                                     FILE_SHARE_READ,
                                     NULL,
                                     OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL,
                                     NULL);
    
    if (file_handle == NULL)
    {
        GetLastError();
    }
    
    BITMAPFILEHEADER bmp_file_header = {0};
    void *bmp_file_header_buffer = VirtualAlloc(
                                                NULL,
                                                14,
                                                MEM_COMMIT | MEM_RESERVE,
                                                PAGE_READWRITE);
    unsigned long bytes_read = 0;
    if (!ReadFile(
                  file_handle,
                  bmp_file_header_buffer,
                  14,
                  &bytes_read,
                  NULL))
    {
        GetLastError();
    }
    else
    {
        bmp_file_header.
    }
    
    win32_render_back_buffer();
    
    BOOL result; // TODO: Don't use BOOL
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
        
        case WM_PAINT:
        {
            win32_render_back_buffer();
        } break;
        
        default:
        {
            result = DefWindowProc(window_handle, message, w_param, l_param);
        } break;
    }
    
    return result;
}

internal_function
void
win32_render_back_buffer(void)
{
    HDC device_context = GetDC(main_window.handle);
    
    if (device_context == NULL)
    {
        // TODO: Logging
        PostQuitMessage(failure);
    }
    
    RECT rect;
    if (!GetClientRect(main_window.handle, &rect))
    {
        //TODO: Logging
        PostQuitMessage(GetLastError());
    }
    
    if(!StretchDIBits(
                      device_context,
                      rect.left,
                      rect.top,
                      rect.right - rect.left,
                      rect.bottom - rect.top,
                      0,
                      0,
                      global_back_buffer.width,
                      global_back_buffer.height,
                      global_back_buffer.memory,
                      &global_back_buffer.info,
                      DIB_RGB_COLORS,
                      SRCCOPY
                      ))
    {
        // TODO: Logging
        PostQuitMessage(GetLastError());
    }
    
    if(!ReleaseDC(main_window.handle, device_context))
    {
        // TODO: Logging
        PostQuitMessage(failure);
    }
}


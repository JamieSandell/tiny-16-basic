#include <windows.h>

#define UNUSED_PARAMETER(P) P

static const int failure = 0;
static const int success = 1;

struct window
{
    HINSTANCE instance;
    HWND handle;
};

LRESULT CALLBACK
main_window_callback(HWND window_handle,
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
        } break;
        
        case WM_DESTROY:
        {
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

int CALLBACK
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
    const string class_name = "Tiny16BasicWindowClass";
    
    if (!prev_instance)
    {
        window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        window_class.lpfnWndProc = main_window_callback;
        window_class.hInstance = instance;
        // window_class.hIcon; // TODO: Supply and icon
        window_class.lpszClassName = class_name;
        
        if (!RegisterClass(&window_class))
        {
            return failure;
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
                                    NULL);
        
        if (handle == NULL)
        {
            // TODO: Logging
            return failure;
        }
        
        
    }
    
    return success;
}


#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

#define global_variable static
#define local_persist static
global_variable size_t WM_PAINTCALLS = 0;
global_variable size_t WM_SIZECALLS = 0;
global_variable bool Running;


global_variable HBITMAP BitmapHandle;
global_variable int BitmapWidth;
global_variable int BitmapHeight;



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Win_32UpdateMainWindow(HWND hwnd,HDC deviceContext);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    
	const wchar_t CLASS_NAME[] = L"UniqWindow";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = NULL;   
    wc.style = CS_HREDRAW|CS_VREDRAW;
	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		  0,                              
        wc.lpszClassName,                 
        L"Learn to Program Windows",    
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,         
        NULL,       
        hInstance, 
        NULL    
        );

	if(hwnd) {
	ShowWindow(hwnd, nCmdShow);	

	MSG msg = {};
    Running = true;
    while(GetMessage(&msg, 0,0,0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
	return 0;
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (uMsg)
    {
    case WM_CREATE: {
        //Check CreateProcessA to create console that is independent, because you can't close console right now without closing main proccess which console is attach to via AllocConsole
        if(AllocConsole()) {
          freopen("CONIN$", "r", stdin); 
          freopen("CONOUT$", "w", stdout); 
          freopen("CONOUT$", "w", stderr); 
          printf_s("WM_CREATE\n");   
        } 
        OutputDebugStringA("Hello");
    } break;
    case WM_DESTROY: {
    // Running = false;
    PostQuitMessage(0);
    }
    break;
    case WM_PAINT: {   
        ++WM_PAINTCALLS;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

      // PatBlt(hdc, ps.rcPaint.top, ps.rcPaint.left, ps.rcPaint.right, ps.rcPaint.bottom,BLACKNESS);
         Win_32UpdateMainWindow(hwnd,hdc);

        EndPaint(hwnd, &ps);

        HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cd = {};
        cd.X = 0;
        cd.Y = 2;
        SetConsoleCursorPosition(hd, cd);

        printf_s("WM_PAINT: %d", WM_PAINTCALLS);
        
        } break;
    case WM_SIZE: {
        ++WM_SIZECALLS;

        HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cd = {};
        cd.X = 0;
        cd.Y = 3;
        SetConsoleCursorPosition(hd, cd);

        printf_s("WM_SIZE: %d", WM_SIZECALLS);
        OutputDebugStringA("WN_SIZE");
    } break;
    case WM_CLOSE:
     {
        // Running = false;
        OutputDebugStringA("WM_CLOSE\n");
        DestroyWindow(hwnd);
    } break;
    default:
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    return result;
    }
}

void Win_32UpdateMainWindow(HWND hwnd, HDC hdc)
{
    local_persist BITMAPINFO BitmapInfo = {};

    RECT rc;
    GetClientRect(hwnd,&rc);
    int X = rc.left;
    int Y = rc.top;

    int Width = rc.right;
    int Height = rc.bottom;

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = (Height);
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
    
    local_persist void* BitmapMemory = nullptr;

    if(BitmapMemory) {
        VirtualFree(BitmapMemory, 0, MEM_RELEASE);
    }

    BitmapMemory = VirtualAlloc(0,(Height * Width * 4), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
     unsigned char *Pixel = (unsigned char*)BitmapMemory;
       // 
       // Pixel in memory: BB GG RR XX
       // 
    for (int i = 0; i < Width * Height; i++) {
        *Pixel = i + 33;
        ++Pixel;
        *Pixel = i + 2;
        ++Pixel;
        *Pixel = i + 13;
        ++Pixel;
        *Pixel = 0;
        ++Pixel;
    }
    StretchDIBits(hdc,
    0, 0, 
    Width, Height,
    0,0,
    BitmapInfo.bmiHeader.biWidth, BitmapInfo.bmiHeader.biHeight,
    BitmapMemory, 
    &BitmapInfo,  
    DIB_RGB_COLORS, SRCCOPY
    );
}

#include <windows.h>
#include <stdio.h>
#include "engine.h"

#define NUM_LINES 21
#define LINE_LENGTH 20
#define PADDING 10
#define ID_SHOWHIDEKEY 1
#define ID_HIDEKEY 2
#define OUTPUT_TEMPLATE "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" \
                        "______________________\n" 
#include "resource.h"

const char g_szClassName[] = "PopupCalculator";

/*
    Interface with the internal calculator engine.
    Relay the input to the calculator engine
    (with exceptions, see below) and
    return the output from the engine.
    If there's no calculator engine
    (the first call or the previous one was
    destroyed), create one.
    Exceptions to input:
    ' ' (space character, 32): Just return the output
    EOF:                       Destroy the calculator engine
*/
char *InterfaceWithCalculator(int input) {
    static calc_state_t *state;
    if (state == NULL) {
        state = calc_new();
    }

    if (input == ' ') {
        return calc_output(state);
    } else if (input != EOF) {
        calc_input(state, input);
        return calc_output(state);
    } else {
        calc_destroy(state);
        state = NULL;
        return "";
    }
}

/*
    Calculate the size of the calculator's
    output area, using OEM_FIXED_FONT. The
    size is stored in (width, height).
*/
void CalculateSize(int *width, int *height) {
    HDC hdc = GetDC(NULL);
    RECT rect = {0};
    SelectObject(hdc, GetStockObject(OEM_FIXED_FONT));
    DrawTextEx(hdc,
               OUTPUT_TEMPLATE,
               -1,
               &rect,
               DT_CALCRECT | DT_EXTERNALLEADING,
               NULL);
    *width = (int)(rect.right - rect.left);
    *height = (int)(rect.bottom - rect.top);
    ReleaseDC(NULL, hdc);
}

/*
    Either
		- Repaint the window or
		- Provide the text used for repainting the window.

	To repaint the window, set text to NULL and hwnd to the
	window you want to paint. Do this only on the WM_PAINT
	message.

	To provide the text for repainting, set text to a valid
	string. In that context, the value of hwnd is not used.
*/
void RepaintWindowUsingText(HWND hwnd, char *text) {
    static char *provided_text;

	if (text != NULL) {
		provided_text = text;
	} else {
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rc;
		
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);
		rc.left += PADDING;
		rc.top += PADDING;
		rc.right -= PADDING;
		rc.bottom -= PADDING;
		SelectObject(hdc, GetStockObject(OEM_FIXED_FONT));
    	DrawTextEx(hdc,
        	       provided_text,
         	       -1,
          	       &rc,
           	       DT_EXTERNALLEADING,
               	   NULL);
		EndPaint(hwnd, &ps);
	}
}

/*
    Toggle the minimized/restored state
    of the window with handle hwnd. If
    hwnd is NULL, use the previously used
    hwnd.
*/
void ToggleWindowState(HWND hwnd, int show) {
    static HWND savedHwnd;

    if (hwnd != NULL) {
        savedHwnd = hwnd;
    }

    if (show == 1) {
        ShowWindow(savedHwnd, SW_RESTORE);
        RedrawWindow(savedHwnd, NULL, NULL,
                            RDW_INVALIDATE | RDW_UPDATENOW);
    } else {
        ShowWindow(savedHwnd, SW_HIDE);
    }
}

/*
    Process the input from
    WM_KEYUP (or WM_KEYDOWN) message
    and use it to control the calculator.
*/
LRESULT CALLBACK HandleKeyboardInput(int nCode,
                                     WPARAM wParam,
                                     LPARAM lParam)
{
    char *output;
    char c = '\0';
    static int window_hidden = 0;

    if (nCode >= 0 && 
        (wParam == WM_KEYUP 
            || wParam == WM_SYSKEYUP
            || wParam == WM_KEYDOWN
            || wParam == WM_SYSKEYDOWN)) {
        PKBDLLHOOKSTRUCT evt = (PKBDLLHOOKSTRUCT)lParam;
        // Map virtual key manually to bypass numlock
        WPARAM vk            = MapVirtualKey(evt->scanCode, MAPVK_VSC_TO_VK_EX);

        // Numpad keys have extended flag unset
        // ... except numpad divide and return, which HAS the extended flag set
        if (  (!(evt->flags & LLKHF_EXTENDED) 
                && (vk == VK_RETURN))
           || (!(evt->flags & LLKHF_EXTENDED)
                && (vk == VK_OEM_MINUS))
           || ((evt->flags & LLKHF_EXTENDED) 
                && (vk != VK_RETURN)
                && (vk != VK_OEM_MINUS))) {
            vk = 0;
        }

        switch (vk) {
            case VK_INSERT:       c = '0'; break;
            case VK_END:          c = '1'; break;
            case VK_DOWN:         c = '2'; break;
            case VK_NEXT:         c = '3'; break;
            case VK_LEFT:         c = '4'; break;
            case VK_CLEAR:        c = '5'; break;
            case VK_RIGHT:        c = '6'; break;
            case VK_HOME:         c = '7'; break;
            case VK_UP:           c = '8'; break;
            case VK_PRIOR:        c = '9'; break;
            case VK_DELETE:       c = '.'; break;
            case VK_MULTIPLY:     c = '*'; break;
            case VK_ADD:          c = '+'; break;
            case VK_SUBTRACT:     c = '-'; break;
            case VK_OEM_MINUS:    c = '/'; break; // Don't ask...
            case VK_RETURN:       c = '='; break;
            default:              ToggleWindowState(NULL, 0);
                                  window_hidden = 1;
                                  return 
                                    CallNextHookEx(NULL,
                                                   nCode,
                                                   wParam,
                                                   lParam);       
        }

        if (wParam == WM_KEYUP) {
            output = InterfaceWithCalculator((int)c);

        	RepaintWindowUsingText(NULL, output);
            ToggleWindowState(NULL, 1);
            window_hidden = 0;
        }
        return 1;
    } else {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
}

/* Process window messages */
LRESULT CALLBACK WndProc(HWND hwnd,
                         UINT msg,
                         WPARAM wParam,
                         LPARAM lParam) {
    switch (msg) {
		/* Paint the window */
		case WM_PAINT:
			RepaintWindowUsingText(hwnd, NULL);
			break;
        /* Handle minimize hotkey */
        case WM_HOTKEY:
            if (wParam == ID_SHOWHIDEKEY) {
                ToggleWindowState(hwnd, 0);
            } else if (wParam == ID_HIDEKEY) {
                ToggleWindowState(hwnd, 1);
            }
            break;
        /* Handle window closing properly */
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        /* Pass control to Windows */
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* Entry point */
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nShowCmd) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;
	HANDLE hMutex;
    HHOOK hhkLowLevelKbd;

	hMutex = CreateMutex(NULL, FALSE, g_szClassName);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return 1;
	}

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_POPUPCALC));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_POPUPCALC));
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    int width, height;
    CalculateSize(&width, &height);

    hwnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        g_szClassName,
        "Popup Calculator",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width + 2*PADDING + PADDING/2, height + 2*PADDING + PADDING/2,
        NULL, NULL, hInstance, NULL);
    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    /* Run program */
    ShowWindow(hwnd, nShowCmd);
    SetWindowPos(hwnd,
                 HWND_TOPMOST,
                 0,
                 0,
                 0,
                 0,
                 SWP_NOMOVE | SWP_NOSIZE);
                 
	ToggleWindowState(hwnd, 1);

    hhkLowLevelKbd = SetWindowsHookEx(WH_KEYBOARD_LL,
                                      HandleKeyboardInput,
                                      hInstance,
                                      0);
	if (hhkLowLevelKbd == NULL) {
        MessageBox(NULL, "Unable to install low level keyboard hook", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
		CloseHandle(hMutex);
		return 0;
	}

    UpdateWindow(hwnd);
    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKbd);
	CloseHandle(hMutex);
    return msg.wParam;
}

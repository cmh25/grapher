#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "about.h"
#include "main.h"
#include "view.h"
#include "globals.h"

TCHAR *g_szAppTitle = "grapher";
HINSTANCE g_hinst;
static HWND m_hwndMain;

static BOOL InitInstance(int);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                   _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    MSG msg;
    HACCEL hAccelTable;

    g_hinst = hInstance;

    main_register_class();
    view_register_class();

    if(!InitInstance(nCmdShow)) {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(g_hinst, (LPCSTR)IDR_ACCELERATOR);

    while(GetMessage(&msg, NULL, 0, 0)) {
        if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

BOOL InitInstance(int nCmdShow) {
    
    m_hwndMain = main_create();
    if(!m_hwndMain) return FALSE;

    ShowWindow(m_hwndMain, nCmdShow);
    UpdateWindow(m_hwndMain);

    return TRUE;
}

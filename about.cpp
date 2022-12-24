#include "about.h"
#include "resource.h"

LRESULT CALLBACK about_wndproc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC dc;
    HICON icon;
    switch(message) {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        if(LOWORD(wParam) == IDOK) {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    case WM_PAINT:
        dc = BeginPaint(hDlg, &ps);
        icon = LoadIcon(g_hinst, MAKEINTRESOURCEA(IDI_ICON1));
        DrawIconEx(dc, 10, 10, icon, 0, 0, 0, NULL, DI_NORMAL);
        DestroyIcon(icon);
        EndPaint(hDlg, &ps);
        break;
    }
    return FALSE;
}

#include "options.h"
#include "resource.h"
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include "view.h"

LRESULT CALLBACK buttonProc(HWND hWnd, UINT uMsg, WPARAM wParam,
    LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

LRESULT CALLBACK colors_wndproc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static CHOOSECOLOR cc;
    static COLORREF custom[16] = { RGB(0,0,0) };

    switch (message) {
    case WM_INITDIALOG:
        SetWindowSubclass(GetDlgItem(hDlg, IDC_BUTTON_PAPER_COLOR), buttonProc, 0, 0);
        SetWindowSubclass(GetDlgItem(hDlg, IDC_BUTTON_AXIS_COLOR), buttonProc, 1, 0);
        SetWindowSubclass(GetDlgItem(hDlg, IDC_BUTTON_DOTS_COLOR), buttonProc, 2, 0);
        SetWindowSubclass(GetDlgItem(hDlg, IDC_BUTTON_GRAPH_COLOR), buttonProc, 3, 0);
        ZeroMemory(&cc, sizeof(cc));
        cc.lStructSize = sizeof(cc);
        cc.hwndOwner = hDlg;
        cc.lpCustColors = custom;
        cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            EndDialog(hDlg, LOWORD(wParam));
            view_redraw();
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        case IDC_BUTTON_PAPER_COLOR:
            cc.rgbResult = view_get_paperc();
            ChooseColor(&cc);
            view_set_paperc(cc.rgbResult);
            break;
        case IDC_BUTTON_AXIS_COLOR:
            cc.rgbResult = view_get_axisc();
            ChooseColor(&cc);
            view_set_axisc(cc.rgbResult);
            break;
        case IDC_BUTTON_DOTS_COLOR:
            cc.rgbResult = view_get_dotsc();
            ChooseColor(&cc);
            view_set_dotsc(cc.rgbResult);
            break;
        case IDC_BUTTON_GRAPH_COLOR:
            cc.rgbResult = view_get_graphc();
            ChooseColor(&cc);
            view_set_graphc(cc.rgbResult);
            break;
        case IDC_BUTTON_DEFAULT:
            view_set_default_colors();
            RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            break;
        }
    }
    return FALSE;
}

/* subclassed button control that draws itself */
LRESULT CALLBACK buttonProc(HWND hWnd, UINT uMsg, WPARAM wParam,
    LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    PAINTSTRUCT ps;
    HDC dc;
    RECT r;
    HBRUSH br;
    int res;
    switch (uMsg) {
    case WM_PAINT:
        dc = BeginPaint(hWnd, &ps);
        if (uIdSubclass == 0)
            br = CreateSolidBrush(view_get_paperc());
        else if (uIdSubclass == 1)
            br = CreateSolidBrush(view_get_axisc());
        else if (uIdSubclass == 2)
            br = CreateSolidBrush(view_get_dotsc());
        else if (uIdSubclass == 3)
            br = CreateSolidBrush(view_get_graphc());
        GetClientRect(hWnd, &r);
        res = FillRect(dc, &r, br);
        DeleteObject(br);
        EndPaint(hWnd, &ps);
        break;
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
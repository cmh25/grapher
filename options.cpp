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

LRESULT CALLBACK graphing_wndproc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static CHOOSECOLOR cc;
    static COLORREF custom[16] = { RGB(0,0,0) };
    HWND cb1, s2, s3;
    int cs, px, py;

    switch (message) {
    case WM_INITDIALOG:
        cb1 = GetDlgItem(hDlg, IDC_COMBO1);
        SendMessage(cb1, CB_ADDSTRING, 0, (LPARAM)"0.1");
        SendMessage(cb1, CB_ADDSTRING, 0, (LPARAM)"0.01");
        SendMessage(cb1, CB_ADDSTRING, 0, (LPARAM)"0.001");
        SendMessage(cb1, CB_ADDSTRING, 0, (LPARAM)"0.0001");
        SendMessage(cb1, CB_ADDSTRING, 0, (LPARAM)"0.00001");
        SendMessage(cb1, CB_SETCURSEL, view_get_accuracyi(), 0);
        s2 = GetDlgItem(hDlg, IDC_SPIN2);
        SendMessage(s2, UDM_SETRANGE, 0, MAKELPARAM(100, 0));
        SendMessage(s2, UDM_SETPOS, 0, view_get_pixelsinonex());
        s3 = GetDlgItem(hDlg, IDC_SPIN3);
        SendMessage(s3, UDM_SETRANGE, 0, MAKELPARAM(100, 0));
        SendMessage(s3, UDM_SETPOS, 0, view_get_pixelsinoney());
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            cb1 = GetDlgItem(hDlg, IDC_COMBO1);
            cs = (int)SendMessage(cb1, CB_GETCURSEL, 0, 0);
            s2 = GetDlgItem(hDlg, IDC_SPIN2);
            px = (int)SendMessage(s2, UDM_GETPOS, 0, 0);
            s3 = GetDlgItem(hDlg, IDC_SPIN3);
            py = (int)SendMessage(s3, UDM_GETPOS, 0, 0);
            EndDialog(hDlg, LOWORD(wParam));
            view_set_accuracyi(cs);
            view_set_pixelsinonex(px);
            view_set_pixelsinoney(py);
            view_redraw();
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        case IDC_BUTTON_DEFAULT:
            view_set_default_graphopt();
            cb1 = GetDlgItem(hDlg, IDC_COMBO1);
            SendMessage(cb1, CB_SETCURSEL, view_get_accuracyi(), 0);
            s2 = GetDlgItem(hDlg, IDC_SPIN2);
            SendMessage(s2, UDM_SETPOS, 0, view_get_pixelsinonex());
            s3 = GetDlgItem(hDlg, IDC_SPIN3);
            SendMessage(s3, UDM_SETPOS, 0, view_get_pixelsinoney());
            return TRUE;
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
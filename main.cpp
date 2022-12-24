#include "main.h"
#include "resource.h"
#include "view.h"
#include "about.h"
#include "options.h"
#include <commctrl.h>
#include <process.h>
#include <stdio.h>
#include "expression.h"

int g_sizing;

static HWND    m_hwnd;
static HWND    m_hwndRB;
static HWND    m_hwndSB;
static HWND    m_hwndView;
static HWND    m_hwndTB;
static HWND    m_hwndCB;

static HWND CreateToolBar(HWND);
static HWND CreateStatusBar(HWND);
static HWND CreateRebar(HWND);
static HWND CreateComboBox(HWND);

ATOM main_register_class() {
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(wcex);
    wcex.lpszClassName = "main";
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = main_wndproc;
    wcex.hInstance = g_hinst;
    wcex.hIcon = LoadIcon(g_hinst,  MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszMenuName = g_szAppTitle;
    wcex.hIconSm = LoadIcon(g_hinst, MAKEINTRESOURCE(IDI_ICON1));
    return RegisterClassEx(&wcex);
}

HWND main_create() {
    HMENU hMenu = LoadMenu(g_hinst, MAKEINTRESOURCE(IDR_MENU_MAIN));

    m_hwnd = CreateWindow("main",
                          g_szAppTitle,
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, 0,
                          CW_USEDEFAULT, 0,
                          NULL, hMenu,
                          g_hinst, NULL);
    return m_hwnd;
}

LRESULT CALLBACK main_wndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rc, rr, rs;
    int rrHeight, sbHeight, x, y, width, height;
    char sbt[512];

    switch(message) {
    case WM_CREATE:
        m_hwndRB = CreateRebar(hWnd);
        m_hwndSB = CreateStatusBar(hWnd);
        m_hwndView = view_create(hWnd);
        break;
    case WM_DESTROY:
        DestroyWindow(m_hwndView);
        DestroyWindow(m_hwndSB);
        DestroyWindow(m_hwndRB);
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        switch(wmId) {
        case ID_FILE_EXIT:
            DestroyWindow(m_hwnd);
            break;
        case ID_BUTTON_SAVE:
        case ID_FILE_SAVEAS:
            view_save();
            break;
        case ID_BUTTON_CLEAR:
            view_drawgrid();
            break;
        case ID_BUTTON_GRAPH:
            view_graph_expression();
            break;
        case ID_HELP_ABOUT:
			DialogBox(g_hinst, (LPCTSTR)IDD_ABOUT, hWnd, (DLGPROC)about_wndproc);
            break;
        case ID_OPTIONS_COLORS:
            DialogBox(g_hinst, (LPCTSTR)IDD_COLORS, hWnd, (DLGPROC)colors_wndproc);
            break;
        case ID_COMBOBOX:
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_SIZE:
        SendMessage(m_hwndSB, WM_SIZE, wParam, lParam);
        SendMessage(m_hwndRB, WM_SIZE, wParam, lParam);
        GetClientRect(m_hwnd, &rc);
        GetWindowRect(m_hwndRB, &rr);
        GetWindowRect(m_hwndSB, &rs);
        rrHeight = rr.bottom - rr.top - 2;
        sbHeight = rs.bottom - rs.top - 1;
        x = 0;
        y = rrHeight;
        width = rc.right - rc.left;
        height = rc.bottom - rc.top - rrHeight - sbHeight - 1;
        SetWindowPos(m_hwndView, HWND_TOP, x, y, width, height, SWP_SHOWWINDOW);
        SendMessage(m_hwndView, WM_SIZE, wParam, lParam);
        sprintf_s(sbt, 512, "%dx%d", width, height);
        SendMessage(m_hwndSB, SB_SETTEXT, MAKELONG(0,0), (LPARAM)sbt);
        break;
    case WM_ENTERSIZEMOVE:
        g_sizing = 1;
        break;
    case WM_EXITSIZEMOVE:
        g_sizing = 0;
        view_graph_expression();
        break;
    case WM_NOTIFY:
        switch(wParam) {
        case ID_REBAR:
            if(((LPNMHDR)lParam)->code == RBN_HEIGHTCHANGE)
                SendMessage(m_hwnd, WM_SIZE, 0, 0);
            break;
        default:
            break;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
   }
   return 0;
}

HWND CreateStatusBar(HWND hwndParent) {
    HWND hwndSB;
    int a[5] = { 60, 200, 300, 500, 600 };

    hwndSB = CreateWindow(STATUSCLASSNAME,
                          NULL,
                          WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | SBARS_SIZEGRIP,
                          0, 0, 0, 0,
                          hwndParent,
                          (HMENU)ID_STATUS,
                          g_hinst, NULL);
    SendMessage(hwndSB, SB_SETPARTS, 5, (LPARAM)a);
    return hwndSB;
}

HWND CreateRebar(HWND hwndParent) {
    HWND hwndRB, hwndTB;
    REBARBANDINFO rbbi;
    INITCOMMONCONTROLSEX icex;
    RECT tbr;
    
    icex.dwSize = sizeof(icex);
    icex.dwICC  = ICC_BAR_CLASSES | ICC_COOL_CLASSES | ICC_USEREX_CLASSES;
    InitCommonControlsEx(&icex);

    hwndRB = CreateWindowEx(WS_EX_TOOLWINDOW,
                            REBARCLASSNAME,
                            NULL,
                            WS_VISIBLE | WS_BORDER | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
                              RBS_VARHEIGHT | RBS_BANDBORDERS,
                            0, 0, 0, 0,
                            hwndParent,
                            0,
                            g_hinst,
                            NULL );

    hwndTB = CreateToolBar(hwndRB);

    // Get the height of the toolbar.
    DWORD dwBtnSize = (DWORD)SendMessage(hwndTB, TB_GETBUTTONSIZE, 0, 0);

    ZeroMemory(&rbbi, sizeof(rbbi));
    rbbi.cbSize = REBARBANDINFO_V3_SIZE;
    rbbi.fMask = RBBIM_COLORS | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE;
    rbbi.clrFore = GetSysColor(COLOR_BTNTEXT);
    rbbi.clrBack = GetSysColor(COLOR_BTNFACE);
    rbbi.fStyle = RBBS_GRIPPERALWAYS | RBBS_CHILDEDGE;
    rbbi.hwndChild = hwndTB;
    rbbi.cxMinChild = 3*LOWORD(dwBtnSize);
    rbbi.cyMinChild = 10+HIWORD(dwBtnSize);

    SendMessage(hwndRB, RB_INSERTBAND, (WPARAM)0, (LPARAM)&rbbi);

    m_hwndCB = CreateComboBox(hwndRB);
    GetWindowRect(m_hwndCB, &tbr);
    rbbi.hwndChild = m_hwndCB;
    rbbi.cxMinChild = tbr.right - tbr.left - 1;
    rbbi.cyMinChild = tbr.bottom - tbr.top - 1;
    rbbi.lpText = "y=";
    rbbi.fMask |= RBBIM_TEXT;
    SendMessage(hwndRB, RB_INSERTBAND, (WPARAM)1, (LPARAM)&rbbi);

    return hwndRB;
}

HWND CreateToolBar(HWND hWndParent) {
    const int numButtons = 3;
    const int bitmapSize = 16;

    // Create the toolbar.
    HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
        WS_CHILD | CCS_NORESIZE | TBSTYLE_TOOLTIPS | TBSTYLE_TRANSPARENT,
        0, 0, 0, 0,
        hWndParent, NULL, g_hinst, NULL);

    if (hWndToolbar == NULL)
        return NULL;

    // Create the image list.
    HIMAGELIST hil = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR16, numButtons, 0);
    SendMessage(hWndToolbar, TB_SETIMAGELIST, 0, (LPARAM)hil);

    // Load the button images.
    TBADDBITMAP tb = { g_hinst, IDR_TOOLBAR_MAIN };
    SendMessage(hWndToolbar, TB_ADDBITMAP, numButtons, (LPARAM)&tb);
    TBBUTTON tbButtons[] =
    { { 0, ID_BUTTON_SAVE,  TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)"save" },
      { 1, ID_BUTTON_CLEAR, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)"clear" },
      { 2, ID_BUTTON_GRAPH, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)"graph" } };
    SendMessage(hWndToolbar, TB_SETMAXTEXTROWS, 0, 0);
    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

    return hWndToolbar; 
}

HWND CreateComboBox(HWND hwndParent) {
    HWND h = CreateWindow(WC_COMBOBOX, TEXT(""),
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL,
        0,0,100,200, hwndParent, NULL, g_hinst,
        NULL);
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"x^2");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"x^3");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)".001x^3");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"1/x");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(x^2-5x-5)/(x^2+5x+5)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"x+1/x^3");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(x^2-2x)/(x^2-9)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"sin(x)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"cos(x)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"tan(x)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"csc(x)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"sec(x)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"cot(x)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"abs(x)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"sqrt(x)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"log(x)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"ln(x)");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"e^x");
    SendMessage(h, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(x-pi)^2");
    SendMessage(h, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
    view_set_expression("x^2");
    return h;
}

void main_get_cbtext(TCHAR *b) {
    SendMessage(m_hwndCB, WM_GETTEXT, 100, (LPARAM)b);
}
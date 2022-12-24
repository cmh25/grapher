#include "view.h"
#include <math.h>
#include "globals.h"
#include "expression.h"
#include "main.h"
#include "bitmap.h"

static HWND     m_hwnd;
static COLORREF m_crpaper = RGB(255, 255, 255);
static COLORREF m_craxis = RGB(0, 0, 0);
static COLORREF m_crdots = RGB(0, 0, 0);
static COLORREF m_crgraph = RGB(0, 0, 255);
static HBRUSH m_brpaper;
static HBRUSH m_braxis;
static HBRUSH m_brdots;
static HBRUSH m_brgraph;
static TCHAR m_expression[256];
static int m_pixelsinonex = 10;
static int m_pixelsinoney = 10;
static float m_accuracy = 0.001f;
static int exp_index, exp_max;

ATOM view_register_class() {
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(wcex);
    wcex.lpszClassName = "view";
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = view_wndproc;
    wcex.hInstance = g_hinst;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    return RegisterClassEx(&wcex);
}

HWND view_create(HWND hwndParent) {
    m_hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "view", g_szAppTitle,
                          WS_CHILD | WS_VISIBLE,
                          0, 0, 0, 0,
                          hwndParent, NULL,
                          g_hinst, NULL);

    SendMessage(m_hwnd, WM_SIZE, 0, 0);

    return m_hwnd;
}

LRESULT CALLBACK view_wndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    static RECT rv, rc, rr, rs;
    static int rrHeight, sbHeight, x, y;
    static BOOLEAN bFirstPaint = TRUE;

    switch(message) {
    case WM_CREATE:
        view_set_expression("x^2");
        m_brpaper = CreateSolidBrush(m_crpaper);
        break;
    case WM_DESTROY:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        view_drawgrid();
        if(!g_sizing) view_graph_expression();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}

COLORREF view_get_paperc() {
    return m_crpaper;
}

void view_set_paperc(COLORREF c) {
    m_crpaper = c;
    DeleteObject(m_brpaper);
    m_brpaper = CreateSolidBrush(c);
}

COLORREF  view_get_axisc() {
    return m_craxis;
}

void view_set_axisc(COLORREF c) {
    m_craxis = c;
    DeleteObject(m_braxis);
    m_braxis = CreateSolidBrush(c);
}

COLORREF  view_get_dotsc() {
    return m_crdots;
}

void view_set_dotsc(COLORREF c) {
    m_crdots = c;
    DeleteObject(m_brdots);
    m_brdots = CreateSolidBrush(c);
}

COLORREF view_get_graphc() {
    return m_crgraph;
}

void view_set_graphc(COLORREF c) {
    m_crgraph = c;
    DeleteObject(m_brgraph);
    m_brgraph = CreateSolidBrush(c);
}

void view_set_default_colors() {
    m_crpaper = RGB(255, 255, 255);
    m_craxis = RGB(0, 0, 0);
    m_crdots = RGB(0, 0, 0);
    m_crgraph = RGB(0, 0, 255);
    DeleteObject(m_brpaper);
    DeleteObject(m_braxis);
    DeleteObject(m_brdots);
    DeleteObject(m_brgraph);
    m_brpaper = CreateSolidBrush(m_crpaper);
    m_braxis = CreateSolidBrush(m_crpaper);
    m_brdots = CreateSolidBrush(m_crdots);
    m_brgraph = CreateSolidBrush(m_crgraph);
}

TCHAR* view_get_expression() {
    return m_expression;
}

void view_set_expression(TCHAR* e) {
    strcpy_s(m_expression, 256, e);
}

void view_redraw() {
    RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void view_drawgrid() {
    RECT rect;
    int x;
    int y;
    HDC dc;
    HPEN p;

    GetClientRect(m_hwnd, &rect);
    dc = GetDC(m_hwnd);
    FillRect(dc, &rect, m_brpaper);
    p = CreatePen(PS_SOLID, 0, m_craxis);
    SelectObject(dc, p);

    MoveToEx(dc, 0, rect.bottom/2, 0);
    LineTo(dc, rect.right,rect.bottom/2);
    MoveToEx(dc, rect.right / 2, 0,0);
    LineTo(dc, rect.right / 2, rect.bottom);

    //Q1
    for (x = rect.right / 2 + m_pixelsinonex; x <= rect.right; x += m_pixelsinonex)
    {
        for (y = rect.bottom / 2 - m_pixelsinoney; y >= rect.top; y -= m_pixelsinoney)
        {
            SetPixel(dc, x, y, m_crdots);
        }
    }
    //Q2
    for (x = rect.right / 2 - m_pixelsinonex; x >= rect.left; x -= m_pixelsinonex)
    {
        for (y = rect.bottom / 2 - m_pixelsinoney; y >= rect.top; y -= m_pixelsinoney)
        {
            SetPixel(dc, x, y, m_crdots);
        }
    }
    //Q3
    for (x = rect.right / 2 - m_pixelsinonex; x >= rect.left; x -= m_pixelsinonex)
    {
        for (y = rect.bottom / 2 + m_pixelsinoney; y <= rect.bottom; y += m_pixelsinoney)
        {
            SetPixel(dc, x, y, m_crdots);
        }
    }
    //Q4
    for (x = rect.right / 2 + m_pixelsinonex; x <= rect.right; x += m_pixelsinonex)
    {
        for (y = rect.bottom / 2 + m_pixelsinoney; y <= rect.bottom; y += m_pixelsinoney)
        {
            SetPixel(dc, x, y, m_crdots);
        }
    }

    DeleteObject(p);
    ReleaseDC(m_hwnd, dc);
    return;
}

void view_graph_expression() {
    double x;
    double y;
    double startx;
    double endx;
    double lowy;
    double highy;
    RECT rect;
    HDC dc;
    GetClientRect(m_hwnd, &rect);
    dc = GetDC(m_hwnd);

    startx = (0 - rect.right / 2) / m_pixelsinonex - 1;
    endx = (rect.right / 2) / m_pixelsinonex + 1;
    lowy = (0 - rect.bottom / 2) / m_pixelsinoney - 1;
    highy = (rect.bottom / 2) / m_pixelsinoney + 1;

    main_get_cbtext(m_expression);
    switch (ParseExpression(m_expression, &exp_index, &exp_max))
    {
    case 0:
        break;
    case 1:
        MessageBox(m_hwnd, "You didn't enter an expression!", "error", MB_OK);
        return;
    case 2:
    case 3:
        MessageBox(m_hwnd, "Illegal character in expression!","error",MB_OK);
        return;
    case 4:
        MessageBox(m_hwnd, "Unmatching parenthises!","error",MB_OK);
        return;
    default:
        break;
    }

    for (x = startx; x <= endx; x += m_accuracy)
    {
        exp_index = 0;
        y = -(GetExpression(m_expression, &exp_index, &exp_max, x));
        if (y >= lowy && y <= highy)
        {
            SetPixel(dc, (int)((x * m_pixelsinonex + rect.right / 2) + .5),
                (int)((y * m_pixelsinoney + rect.bottom / 2) + .5), m_crgraph);
        }
    }

    ReleaseDC(m_hwnd, dc);
}

void view_save() {
    HDC wdc, cdc;
    HBITMAP bm;
    RECT r;
    int w, h;
    PBITMAPINFO pbmi;
    OPENFILENAME ofn;
    char sz[256];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = sz;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(sz);
    ofn.lpstrFilter = "All (*.*)\0*.*\0Bitmap (*.bmp)\0*.bmp\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrDefExt = "bmp";
    ofn.Flags = 0;

    if (!GetSaveFileName(&ofn)) return;

    wdc = GetDC(m_hwnd);
    cdc = CreateCompatibleDC(wdc);
    GetClientRect(m_hwnd, &r);
    w = r.right - r.left;
    h = r.bottom - r.top;
    bm = CreateCompatibleBitmap(wdc, w, h);
    SelectObject(cdc, bm);
    BitBlt(cdc, 0, 0, w, h, wdc, 0, 0, SRCCOPY);
    pbmi = CreateBitmapInfoStruct(m_hwnd, bm);
    CreateBMPFile(m_hwnd, sz, pbmi, bm, cdc);
    DeleteObject(bm);
    ReleaseDC(m_hwnd, wdc);
    DeleteDC(cdc);
}
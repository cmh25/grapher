/* https://learn.microsoft.com/en-us/windows/win32/gdi/storing-an-image */
#ifndef BITMAP_H
#define BITMAP_H
#include <windows.h>
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
#endif /* BITMAP_H */
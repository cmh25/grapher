#ifndef MAIN_H
#define MAIN_H

#include "globals.h"

ATOM main_register_class();
HWND main_create();
LRESULT CALLBACK main_wndproc(HWND, UINT, WPARAM, LPARAM);
void main_get_cbtext(TCHAR* b);
void main_set_statusxy(int x, int y);

#endif /* MAIN_H */

#ifndef MAIN_H
#define MAIN_H

#include "globals.h"

ATOM main_register_class();
HWND main_create();
LRESULT CALLBACK main_wndproc(HWND, UINT, WPARAM, LPARAM);
void main_get_cbtext(TCHAR* b);

#endif /* MAIN_H */

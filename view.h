#ifndef VIEW_H
#define VIEW_H

#include "globals.h"

ATOM view_register_class();
HWND view_create(HWND);
LRESULT CALLBACK view_wndproc(HWND, UINT, WPARAM, LPARAM);
COLORREF view_get_paperc();
void view_set_paperc(COLORREF c);
COLORREF view_get_axisc();
void view_set_axisc(COLORREF c);
COLORREF view_get_dotsc();
void view_set_dotsc(COLORREF c);
COLORREF view_get_graphc();
void view_set_graphc(COLORREF c);
void view_set_default_colors();
void view_set_default_graphopt();
int view_get_accuracyi();
void view_set_accuracyi(int a);
int view_get_pixelsinonex();
void view_set_pixelsinonex(int x);
int view_get_pixelsinoney();
void view_set_pixelsinoney(int y);
TCHAR* view_get_expression();
void view_set_expression(TCHAR *e);
void view_redraw();
void view_drawgrid();
void view_graph_expression();
void view_save();

#endif /* VIEW_H */

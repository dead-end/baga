/*
 * MIT License
 *
 * Copyright (c) 2020 dead-end
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef INC_LIB_CURSES_H_
#define INC_LIB_CURSES_H_

#include <stdbool.h>
#include <ncurses.h>
#include <menu.h>

#include "lib_s_point.h"

/******************************************************************************
 * The definition of the functions.
 *****************************************************************************/

#define KEY_ESC 27

/******************************************************************************
 * The definition of the functions.
 *****************************************************************************/

void lc_curses_init();

void lc_curses_finish();

void lc_mouse_init(const mmask_t mask, const bool moving);

void lc_mouse_finish(const bool moving);

WINDOW* lc_win_create_full();

void lc_win_del(WINDOW *win);

void lc_win_refresh(WINDOW *win);

bool lc_win_is_inside(WINDOW *win, const int row, const int col);

s_point lc_win_pos_center(WINDOW *win_parent, const s_point *dim);

s_point lc_win_pos_center_bottom(WINDOW *win_parent, const s_point *dim);

int lc_menu_cur_item_idx(MENU *menu);

void lc_menu_set_cur_item_idx(MENU *menu, const int idx);

#endif /* INC_LIB_CURSES_H_ */

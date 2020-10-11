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

#ifndef INC_S_TARR_H_
#define INC_S_TARR_H_

#include <stdbool.h>
#include <ncurses.h>

#include "lib_s_point.h"
#include "lib_s_tchar.h"

#include "s_area.h"

/******************************************************************************
 * The structure represents a two dimensional array. The data is stored in a
 * one dimensional array. We have a macro to access an element.
 *****************************************************************************/

typedef struct {

	s_point dim;

	s_tchar *arr;

} s_tarr;

/******************************************************************************
 * The function declarations.
 *****************************************************************************/

s_tarr* s_tarr_new(const int row, const int col);

void s_tarr_free(s_tarr **tarr);

void s_tarr_set(s_tarr *tarr, const s_tchar tchar);

void s_tarr_set_gradient(s_tarr *tarr, const wchar_t chr, const short fg_color, const short *bg_colors);

void s_tarr_set_bg(s_tarr *tarr, const s_area *area, const short *bg_colors);

void s_tarr_cp_fg(s_tarr *to, const s_tarr *from, const s_point pos);

void s_tarr_cp(s_tarr *to_arr, const s_tarr *from_arr, const s_point pos);

s_point s_tarr_ul_pos_get(const s_tarr *tarr, s_point cur_pos, const bool reverse);

void s_tarr_print_pos(WINDOW *win, const s_tarr *ta_fg, const s_point pos_fg, const s_tarr *ta_bg);

/******************************************************************************
 * The macro to access the elements of the array.
 *****************************************************************************/

#define s_tarr_get(t,r,c) ((t)->arr[ (r) * (t)->dim.col + (c) ])

#endif /* INC_S_TARR_H_ */

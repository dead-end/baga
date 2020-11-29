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

#ifndef INC_S_BOARD_H_
#define INC_S_BOARD_H_

#include <ncurses.h>

#include "lib_s_point.h"
#include "s_tarr.h"

/******************************************************************************
 * The function contains the data necessary for the board.
 *****************************************************************************/

typedef struct {

	//
	// The s_tarr with the foreground data.
	//
	s_tarr *fg;

	//
	// The s_tarr with the background data.
	//
	s_tarr *bg;

	//
	// The window of the board.
	//
	WINDOW *win;

} s_board;

/******************************************************************************
 * Definition of functions and macros.
 *****************************************************************************/

void s_board_init(s_board *board, const s_point dim);

void s_board_free(s_board *board);

void s_board_win_refresh(const s_board *board, const bool do_sleep);

void s_board_trv_del(const s_board *board, const s_tarr *tmpl, const s_point tmpl_pos);

void s_board_trv_mv_line(const s_board *board, const s_tarr *tmpl, s_point *tmpl_pos, const s_point target);

void s_board_trv_mv_to(const s_board *board, const s_tarr *tmpl, s_point *tmpl_pos, const e_dir dir);

#endif /* INC_S_BOARD_H_ */

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

#include <ncurses.h>

#include "lib_logging.h"
#include "lib_color_pair.h"

#include "s_tmpl_points.h"
#include "s_tmpl_checker.h"
#include "nc_board.h"
#include "s_color_def.h"

/******************************************************************************
 * Definitions of the various areas of the board. An area has a position and a
 * dimension. The main board has the following four areas:
 *
 * outer | inner | inner | bear
 * board | bar   | board | off
 *****************************************************************************/

// TODO: static for currently unused
//
// Outer board
//
static s_area _area_board_outer = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL },

.dim = { .row = BOARD_HALF_ROW, .col = BOARD_HALF_COL } };

//
// Inner bar
//
s_area _area_bar_inner = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL + BOARD_HALF_COL + BORDER_COL },

.dim = { .row = BOARD_HALF_ROW, .col = CHECKER_COL } };

//
// Inner board
//
static s_area _area_board_inner = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL + BOARD_HALF_COL + BORDER_COL + CHECKER_COL + BORDER_COL },

.dim = { .row = BOARD_HALF_ROW, .col = BOARD_HALF_COL } };

//
// bear off area (right bar)
//
s_area _area_bear_off = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL + BOARD_HALF_COL + BORDER_COL + CHECKER_COL + BORDER_COL + BOARD_HALF_COL + BORDER_COL },

.dim = { .row = BOARD_HALF_ROW, .col = CHECKER_COL } };

/******************************************************************************
 *
 *****************************************************************************/

static s_tarr *_nc_board_bg;

static s_tarr *_nc_board_fg;

static s_point _points_pos[POINTS_NUM];

// --------------------------

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_init_bg(s_tarr *board_bg, const s_area *area_board_outer, const s_area *area_board_inner) {

	//
	// Complete board
	//
	short color_bar_bg[BOARD_ROW];

	s_color_def_gradient(color_bar_bg, BOARD_ROW, "#1a0d00", "#663500");

	s_tarr_set_gradient(board_bg, EMPTY, 0, color_bar_bg);

	//
	// Inner / outer board
	//
	short color_board_bg[BOARD_HALF_ROW];

	s_color_def_gradient(color_board_bg, BOARD_HALF_ROW, "#ffe6cc", "#ff9933");

	s_tarr_set_bg(board_bg, area_board_outer, color_board_bg);

	s_tarr_set_bg(board_bg, area_board_inner, color_board_bg);
}

// ---------------------------------------------------------------------------
// Interface functions
// ---------------------------------------------------------------------------

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_init() {

	_nc_board_bg = s_tarr_new(BOARD_ROW, BOARD_COL);

	_nc_board_fg = s_tarr_new(BOARD_ROW, BOARD_COL);
	//
	// Checker template
	//
	//s_tmpl_checker_init(&_tmpl_checker);

	s_tmpl_checker_create();

	nc_board_init_bg(_nc_board_bg, &_area_board_outer, &_area_board_inner);

	//
	// Points positions
	//
	s_tmpl_points_set_pos(_points_pos, &_area_board_outer, &_area_board_inner);

	//
	// Add points to the board
	//

	s_tmpl_point_add_2_tarr(_nc_board_bg, _points_pos);

	//
	//
	//
	s_tarr_set(_nc_board_fg, ( s_tchar ) { TCHAR_CHR_UNUSED, -1, -1 });
}

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_free() {

	log_debug_str("Freeing resources!");

	s_tmpl_checker_free();

	s_tarr_free(&_nc_board_fg);

	s_tarr_free(&_nc_board_bg);
}

#define CHECKER_ROW_HALF 1

/******************************************************************************
 *
 *****************************************************************************/

static s_point s_board_cp_tmpl(const s_tarr *tmpl, s_point pos, const bool reverse) {

	if (reverse) {
		pos.row = pos.row - tmpl->dim.row + 1;
	}

	s_tarr_cp(_nc_board_fg, tmpl, pos);

	if (reverse) {
		pos.row--;
	} else {
		pos.row += tmpl->dim.row;
	}

	return pos;
}

#define pos_next(r,h,ir) ((ir) ? (r) - (h): (r) + (h) )

/******************************************************************************
 *
 *****************************************************************************/

void s_board_points_add_checkers(const int idx, const e_owner owner, const int num) {

	s_point pos = { .row = _points_pos[idx].row + CHECKER_OFFSET_ROW, .col = _points_pos[idx].col + CHECKER_OFFSET_COL };

	const bool reverse = (idx >= 12);

	const s_tarr *tmpl;

	for (int i = 0; i < num; i++) {

		tmpl = s_tmpl_checker_get_tmpl(owner, num, i, reverse);

		if (tmpl == NULL) {
			continue;
		}

		pos = s_board_cp_tmpl(tmpl, pos, reverse);
	}
}

/******************************************************************************
 * The function prints the board. This is done with the fixed background board
 * and the foreground board, which contains the checkers. If there is a non
 * space character in the foreground, we print the foreground, otherwise we
 * print the background.
 *****************************************************************************/

void nc_board_print() {

	//
	// The color pair.
	//
	short cp;

	//
	// The character to be printed.
	//
	wchar_t chr;

	const s_tchar *bg, *fg;

	for (int row = 0; row < BOARD_ROW; row++) {
		for (int col = 0; col < BOARD_COL; col++) {

			fg = &s_tarr_get(_nc_board_fg, row, col);

			if (s_tchar_is_defined(fg)) {

				cp = cp_color_pair_get(fg->fg, fg->bg);
				chr = s_tarr_get(_nc_board_fg,row,col).chr;

			} else {

				bg = &s_tarr_get(_nc_board_bg, row, col);
				cp = cp_color_pair_get(bg->fg, bg->bg);
				chr = bg->chr;
			}

			attrset(COLOR_PAIR(cp));

			mvprintw(row, col, "%lc", chr);
		}
	}
}

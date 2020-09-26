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

static s_nc_board _nc_board_bg;

static s_nc_board _nc_board_fg;

static s_tmpl_checker _tmpl_checker;

static s_point _points_pos[POINTS_NUM];

/******************************************************************************
 * The function sets the board with a default character and a foreground and
 * background color.
 *****************************************************************************/

static void nc_board_set_tchar(s_nc_board *board, const s_tchar tchar) {

	for (int row = 0; row < BOARD_ROW; row++) {
		for (int col = 0; col < BOARD_COL; col++) {

			board->arr[row][col] = tchar;
		}
	}
}

/******************************************************************************
 * The function sets the board with a default color and a foreground color and
 * a background color gradient.
 *****************************************************************************/

static void nc_board_init_gradient(s_nc_board *board, const wchar_t chr, const short fg_color, const short *bg_colors) {

	for (int row = 0; row < BOARD_ROW; row++) {
		for (int col = 0; col < BOARD_COL; col++) {

			board->arr[row][col].chr = chr;
			board->arr[row][col].fg = fg_color;
			board->arr[row][col].bg = bg_colors[row];
		}
	}
}

/******************************************************************************
 * The function sets the background color with a gradient for a given area.
 *****************************************************************************/

static void nc_board_set_area_bg(s_nc_board *board, const s_area *area_board, const short *bg_colors) {

	//
	// Store the position of the area.
	//
	const int pos_row = area_board->pos.row;
	const int pos_col = area_board->pos.col;

	for (int row = 0; row < area_board->dim.row; row++) {
		for (int col = 0; col < area_board->dim.col; col++) {

			board->arr[pos_row + row][pos_col + col].bg = bg_colors[row];
		}
	}
}

// --------------------------

/******************************************************************************
 * The function adds a points template to the background board. The lower
 * points have to be added upside down.
 *****************************************************************************/

static void s_board_points_add_templ(s_nc_board *board, const s_points_tchar *points_tchar, const bool reverse, const s_point *pos) {

	s_tchar *board_tchar;
	const s_tchar *tmpl_tchar;

	for (int row = 0; row < POINTS_ROW; row++) {
		for (int col = 0; col < POINTS_COL; col++) {

			board_tchar = &board->arr[pos->row + row][pos->col + col];

			//
			// Get the template character for an index, which may be reversed.
			//
			if (reverse) {
				tmpl_tchar = &points_tchar->tchar[reverse_idx(POINTS_ROW, row)][col];
			} else {
				tmpl_tchar = &points_tchar->tchar[row][col];
			}

			board_tchar->chr = tmpl_tchar->chr;
			board_tchar->fg = tmpl_tchar->fg;
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void s_board_points_add(s_nc_board *board, const s_tmpl_points *points_templ, const s_point *points_pos) {

	for (int i = 0; i < POINTS_NUM; i++) {
		s_board_points_add_templ(board, s_tmpl_point_get(points_templ, i), (i < 12), &points_pos[i]);
	}
}

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_init_bg(s_nc_board *board_bg, const s_area *area_board_outer, const s_area *area_board_inner) {

	//
	// Complete board
	//
	short color_bar_bg[BOARD_ROW];

	s_color_def_gradient(color_bar_bg, BOARD_ROW, "#1a0d00", "#663500");

	nc_board_init_gradient(board_bg, EMPTY, 0, color_bar_bg);

	//
	// Inner / outer board
	//
	short color_board_bg[BOARD_HALF_ROW];

	s_color_def_gradient(color_board_bg, BOARD_HALF_ROW, "#ffe6cc", "#ff9933");

	nc_board_set_area_bg(board_bg, area_board_outer, color_board_bg);

	nc_board_set_area_bg(board_bg, area_board_inner, color_board_bg);
}

// ---------------------------------------------------------------------------
// Interface functions
// ---------------------------------------------------------------------------

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_init() {

	//
	// Checker template
	//
	s_tmpl_checker_init(&_tmpl_checker);

	nc_board_init_bg(&_nc_board_bg, &_area_board_outer, &_area_board_inner);

	//
	// Points positions
	//
	s_tmpl_points_set_pos(_points_pos, &_area_board_outer, &_area_board_inner);

	//
	// Points template
	//
	s_tmpl_points tmpl_points;
	s_tmpl_points_init(&tmpl_points);

	s_board_points_add(&_nc_board_bg, &tmpl_points, _points_pos);

	//
	//
	//
	nc_board_set_tchar(&_nc_board_fg, ( s_tchar ) { EMPTY, 0, 0 });
}

/******************************************************************************
 *
 *****************************************************************************/

// TODO: comments
#define CHECKER_OFFSET_COL 1

void s_board_add_checker(const int checker_idx, const e_owner owner) {

	const s_checker_tchar *templ = &_tmpl_checker.checker[owner];

	const s_point *checker_pos = &_points_pos[checker_idx];

	for (int row = 0; row < CHECKER_ROW; row++) {
		for (int col = 0; col < CHECKER_COL; col++) {

			//
			// copy the struct
			//
			_nc_board_fg.arr[checker_pos->row + row][checker_pos->col + CHECKER_OFFSET_COL + col] = templ->tchar[row][col];
		}
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

			fg = &_nc_board_fg.arr[row][col];

			if (fg->chr == EMPTY) {

				bg = &_nc_board_bg.arr[row][col];

				cp = cp_color_pair_get(bg->fg, bg->bg);
				chr = bg->chr;

			} else {
				cp = cp_color_pair_get(fg->fg, fg->bg);
				chr = _nc_board_fg.arr[row][col].chr;
			}

			attrset(COLOR_PAIR(cp));

			mvprintw(row, col, "%lc", chr);
		}
	}
}

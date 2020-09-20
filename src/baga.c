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

#include <bg_board.h>
#include <locale.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>

#include "nc_board.h"
#include "s_tmpl_points.h"

#include "lib_logging.h"
#include "lib_color.h"
#include "lib_color_pair.h"
#include "lib_s_tchar.h"
#include "lib_s_point.h"

#include "s_color_def.h"
#include "s_tmpl_checker.h"

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory.
 *****************************************************************************/

static void exit_callback() {

	endwin();
}

/******************************************************************************
 * The method initializes the application.
 *****************************************************************************/

static void init() {

	//
	// Set the locale to allow utf8.
	//
	if (setlocale(LC_CTYPE, "") == NULL) {
		log_exit_str("Unable to set the locale.");
	}

	//
	// Register callback for log_exit calls.
	//
	log_atexit(exit_callback);

	if (initscr() == NULL) {
		log_exit_str("Unable to init screen!");
	}

	if (start_color() == ERR) {
		log_exit_str("Unable to start color!");
	}

	curs_set(0);
}

// --------------------------------------------

/******************************************************************************
 *
 *****************************************************************************/

//
// Outer board
//
s_area _area_board_outer = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL },

.dim = { .row = BOARD_HALF_ROW, .col = BOARD_HALF_COL }

};

//
// Inner bar
//
s_area _area_bar_inner = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL + BOARD_HALF_COL + BORDER_COL },

.dim = { .row = BOARD_HALF_ROW, .col = CHECKER_COL }

};

//
// Inner board
//
s_area _area_board_inner = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL + BOARD_HALF_COL + BORDER_COL + CHECKER_COL + BORDER_COL },

.dim = { .row = BOARD_HALF_ROW, .col = BOARD_HALF_COL }

};

//
// bear off area (right bar)
//
s_area _area_bar_exit = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL + BOARD_HALF_COL + BORDER_COL + CHECKER_COL + BORDER_COL + BOARD_HALF_COL + BORDER_COL },

.dim = { .row = BOARD_HALF_ROW, .col = CHECKER_COL }

};

/******************************************************************************
 *
 *****************************************************************************/

// TODO:
/******************************************************************************
 *
 *****************************************************************************/

static s_nc_board _nc_board_bg;

static s_nc_board _nc_board_fg;

/******************************************************************************
 *
 *****************************************************************************/

void s_board_add_checker(s_nc_board *board, const s_point *checker_pos, const s_checker_tchar *templ) {

	s_point point;

	point.row = checker_pos->row + 0;
	point.col = checker_pos->col + 1;

	for (int row = 0; row < CHECKER_ROW; row++) {
		for (int col = 0; col < CHECKER_COL; col++) {

			board->arr[point.row + row][point.col + col].fg = templ->tchar[row][col].fg;
			board->arr[point.row + row][point.col + col].bg = templ->tchar[row][col].bg;
			board->arr[point.row + row][point.col + col].chr = templ->tchar[row][col].chr;
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void s_board_add_templ(s_nc_board *board, const s_points_tchar *points_tchar, const bool reverse, const s_point *pos) {

	s_tchar *b_chr;
	const s_tchar *p_chr;

	for (int row = 0; row < POINTS_ROW; row++) {
		for (int col = 0; col < POINTS_COL; col++) {

			b_chr = &board->arr[pos->row + row][pos->col + col];

			if (reverse) {
				p_chr = &points_tchar->tchar[reverse_idx(POINTS_ROW, row)][col];
			} else {
				p_chr = &points_tchar->tchar[row][col];
			}

			b_chr->chr = p_chr->chr;
			b_chr->fg = p_chr->fg;
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void s_board_triangle_add(s_nc_board *board, const s_tmpl_points *points_templ, const s_point *points_pos) {

	for (int i = 0; i < POINTS_NUM; i++) {
		s_board_add_templ(board, &points_templ->points[i % 2], (i < 12), &points_pos[i]);
	}
}

/******************************************************************************
 * The main function.
 *****************************************************************************/

int main() {
	log_debug_str("Starting baga...");

	init();

	//
	// Checker template
	//
	s_tmpl_checker tmpl_checker;
	s_tmpl_checker_init(&tmpl_checker);

	//
	// Points template
	//
	s_tmpl_points tmpl_points;
	s_tmpl_points_init(&tmpl_points);

	s_bg_board bg_board;
	bg_board_new_game(&bg_board);

	nc_board_init_bg(&_nc_board_bg, &_area_board_outer, &_area_board_inner);

	//
	// Points positions
	//
	s_point points_pos[POINTS_NUM];
	s_tmpl_points_set_pos(points_pos, &_area_board_outer, &_area_board_inner);

	s_board_triangle_add(&_nc_board_bg, &tmpl_points, points_pos);

	//
	//
	//
	nc_board_init_empty(&_nc_board_fg, ( s_tchar ) { EMPTY, 0, 0 });

	s_board_add_checker(&_nc_board_fg, &points_pos[23], &tmpl_checker.checker[OWNER_BLACK]);

	s_board_add_checker(&_nc_board_fg, &points_pos[1], &tmpl_checker.checker[OWNER_WHITE]);

	//
	// Print the initialized board
	//
	nc_board_print(&_nc_board_fg, &_nc_board_bg);

	getch();

	log_debug_str("Ending baga...");
}

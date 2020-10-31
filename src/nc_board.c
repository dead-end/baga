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
#include "s_area.h"

#include "s_tmpl_points.h"
#include "s_tmpl_checker.h"
#include "nc_board.h"
#include "s_color_def.h"
#include "direction.h"

#include "s_point_layout.h"

// todo: comment, file, ...

#define MS_SLEEP 80
//#define MS_SLEEP 550

#define ms_sleep() if (napms(MS_SLEEP) != OK) { log_exit_str("sleep failed!"); }

#define nc_board_refresh(w) if (wrefresh(w) == ERR) { log_exit_str("Unable to refresh window!"); }

#define TRAVEL_ROW POINTS_ROW + 2

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

// -------------------------------- START

/******************************************************************************
 * The function initializes the background of the board.
 *****************************************************************************/

static void nc_board_init_bg(s_tarr *board_bg, const s_area *area_board_outer, const s_area *area_board_inner) {

	//
	// Set the background of the complete board.
	//
	short color_bar_bg[BOARD_ROW];

	s_color_def_gradient(color_bar_bg, BOARD_ROW, "#1a0d00", "#663500");

	s_tarr_set_gradient(board_bg, EMPTY, 0, color_bar_bg);

	//
	// Set the background of the inner / outer board.
	//
	short color_board_bg[BOARD_HALF_ROW];

	s_color_def_gradient(color_board_bg, BOARD_HALF_ROW, "#ffe6cc", "#ff9933");

	s_tarr_set_bg(board_bg, area_board_outer->pos, area_board_outer->dim, color_board_bg);

	s_tarr_set_bg(board_bg, area_board_inner->pos, area_board_inner->dim, color_board_bg);
}

/******************************************************************************
 * The function allocates the resources for the board.
 *****************************************************************************/

static void nc_board_alloc() {

	log_debug_str("Allocating resources!");

	_nc_board_bg = s_tarr_new(BOARD_ROW, BOARD_COL);

	_nc_board_fg = s_tarr_new(BOARD_ROW, BOARD_COL);

	s_tmpl_checker_create();
}

/******************************************************************************
 * The function frees the resources for the board.
 *****************************************************************************/

void nc_board_free() {

	log_debug_str("Freeing resources!");

	s_tarr_free(&_nc_board_fg);

	s_tarr_free(&_nc_board_bg);

	s_tmpl_checker_free();
}

// -------------------------------- END

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_init() {

	nc_board_alloc();

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

#define CHECKER_ROW_HALF 1

/******************************************************************************
 *
 *****************************************************************************/

void s_board_points_add_checkers(const int idx, const e_owner owner, const int num) {

	// todo check second parameter
	const s_point_layout layout = s_point_layout_get(num, E_UNCOMP);

	s_point pos = { .row = _points_pos[idx].row + CHECKER_OFFSET_ROW, .col = _points_pos[idx].col + CHECKER_OFFSET_COL };

	const bool reverse = (idx >= 12);

	const s_tarr *tmpl;

	for (int i = 0; i < num; i++) {

		tmpl = s_tmpl_checker_get_tmpl(owner, layout, i, reverse);

		if (tmpl == NULL) {
			continue;
		}

		pos = s_tarr_cp_pos(_nc_board_fg, tmpl, pos, reverse);
	}
}

/******************************************************************************
 * The function prints the board.
 *****************************************************************************/

void nc_board_print() {

	s_tarr_print_area(stdscr, _nc_board_fg, _nc_board_bg, (s_point ) { 0, 0 }, _nc_board_fg->dim);

	nc_board_refresh(stdscr);
}

/******************************************************************************
 * The function copies the traveler to the foreground at a given position and
 * prints the traveler area.
 *****************************************************************************/

static void travleler_print(const s_tarr *tmpl, const s_point tmpl_pos) {

	s_tarr_cp(_nc_board_fg, tmpl, tmpl_pos);

	s_tarr_print_area(stdscr, _nc_board_fg, _nc_board_bg, tmpl_pos, tmpl->dim);
}

/******************************************************************************
 * The function deletes the traveler from the foreground at a given position
 * and prints the (empty) traveler area.
 *****************************************************************************/

static void traveler_del(const s_tarr *tmpl, const s_point tmpl_pos) {

	s_tarr_del(_nc_board_fg, tmpl, tmpl_pos);

	s_tarr_print_area(stdscr, _nc_board_fg, _nc_board_bg, tmpl_pos, tmpl->dim);
}

/******************************************************************************
 * The function is called with the template of the checker and moves the
 * checker from a position to a target position. It is assumed that this is a
 * horizontal or vertical movement. This means the rows or columns of the
 * current position have to be the same.
 *****************************************************************************/

static void traveler_move_line(const s_tarr *tmpl, s_point *tmpl_pos, const s_point target) {

#ifdef DEBUG

	//
	//
	//
	if (s_point_same(tmpl_pos, &target)) {
		log_exit_str("Already the same!");
	}
#endif

	const s_point dir = direction_get(*tmpl_pos, target);

	while (true) {

		direction_mov_to(tmpl_pos, dir);

		//
		// Copy the checker template to the position
		//
		travleler_print(tmpl, *tmpl_pos);

		nc_board_refresh(stdscr);

		ms_sleep();

		log_debug_str("");

		//
		// Delete the checker template from the position
		//
		traveler_del(tmpl, *tmpl_pos);

		//
		// If the current position is the target we are done. It is important
		// that the traveler is deleted before we return.
		//
		if (s_point_same(tmpl_pos, &target)) {
			log_debug_str("same");
			return;
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

void travler_move(const int idx_from, const int num_from, const int idx_to, const int num_to, const e_owner owner) {

	const s_point pos_from = { .row = _points_pos[idx_from].row + CHECKER_OFFSET_ROW, .col = _points_pos[idx_from].col + CHECKER_OFFSET_COL };

	const s_point pos_to = { .row = _points_pos[idx_to].row + CHECKER_OFFSET_ROW, .col = _points_pos[idx_to].col + CHECKER_OFFSET_COL };

	const s_tarr *tmpl = s_tmpl_checker_get_travler(owner);
	log_debug("traveler color: %d", tmpl->arr[0].bg);

	s_point tmpl_pos;

	s_point target;

	//
	// Phase 1
	//
	if (num_from <= CHECK_DIS_FULL) {

		s_board_points_add_checkers(idx_from, owner, num_from - 1);

		tmpl_pos = s_tmpl_checker_last_pos(pos_from, idx_from, num_from);

		s_tarr_cp(_nc_board_fg, tmpl, tmpl_pos);

		const s_area area = s_tmpl_checker_point_area(pos_from, nc_board_is_upper(idx_from));
		s_tarr_print_area(stdscr, _nc_board_fg, _nc_board_bg, area.pos, area.dim);

		nc_board_refresh(stdscr);

		traveler_del(tmpl, tmpl_pos);

	} else {
		log_exit_str("Unimplemented!");
	}

	ms_sleep();

	//
	// Move to traveler line
	//
	target.row = TRAVEL_ROW;
	target.col = tmpl_pos.col;
	traveler_move_line(tmpl, &tmpl_pos, target);

	//
	// Move along the traveler line
	//
	target.row = TRAVEL_ROW;
	target.col = pos_to.col;
	traveler_move_line(tmpl, &tmpl_pos, target);

	//
	// Move from traveler line
	//
	const s_point last_pos = s_tmpl_checker_last_pos(pos_to, idx_to, num_to + 1);

	target.row = last_pos.row;
	target.col = last_pos.col;
	traveler_move_line(tmpl, &tmpl_pos, target);

	//
	// Phase
	//
	if (num_to < CHECK_DIS_FULL) {
		s_board_points_add_checkers(idx_to, owner, num_to + 1);

		const s_area area = s_tmpl_checker_point_area(pos_to, nc_board_is_upper(idx_to));
		s_tarr_print_area(stdscr, _nc_board_fg, _nc_board_bg, area.pos, area.dim);

		nc_board_refresh(stdscr);

	} else {
		log_exit_str("Unimplemented!");
	}
}

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_test() {
	log_debug_str("start");

	travler_move(4, 2, 2, 3, OWNER_BLACK);

	travler_move(15, 4, 7, 4, OWNER_WHITE);

}

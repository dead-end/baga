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

static WINDOW *_win_board;

// todo: comment, file, ...

//#define MS_SLEEP 80
#define MS_SLEEP 500

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
 * The upper left corners for upper points and the lower left corners for lower
 * points.
 *****************************************************************************/

static s_point _points_pos[POINTS_NUM];

/******************************************************************************
 * The points are wider than the checkers. To get the position of the checker
 * on the point we can use this macro.
 *****************************************************************************/

#define point_pos_checker(i) { .row = _points_pos[i].row + CHECKER_OFFSET_ROW, .col = _points_pos[i].col + CHECKER_OFFSET_COL }

/******************************************************************************
 * We have two two-dimensional arrays, which represents the foreground and the
 * background. The background contains the board and the foreground the
 * checkers on the board. So the foreground is mostly transparent.
 *****************************************************************************/

static s_tarr *_nc_board_bg;

static s_tarr *_nc_board_fg;

/******************************************************************************
 * The function initializes the background of the board.
 *****************************************************************************/

static void nc_board_init_bg(s_tarr *board_bg, const s_area *area_board_outer, const s_area *area_board_inner) {

	//
	// Set the background of the complete board.
	//
	short color_bar_bg[BOARD_ROW];

	// TODO: choose final color
	//s_color_def_gradient(color_bar_bg, BOARD_ROW, "#1a0d00", "#663500");

	s_color_def_gradient(color_bar_bg, BOARD_ROW, "#22222f", "#55555f");

	// TODO: choose final color
	//s_color_def_gradient(color_bar_bg, BOARD_ROW, "#44444f", "#77777f");

	s_tarr_set_gradient(board_bg, EMPTY, 0, color_bar_bg);

	//
	// Set the background of the inner / outer board.
	//
	short color_board_bg[BOARD_HALF_ROW];

	// TODO: choose final color
	//s_color_def_gradient(color_board_bg, BOARD_HALF_ROW, "#ffe6cc", "#ff9933");

	s_color_def_gradient(color_board_bg, BOARD_HALF_ROW, "#88888f", "#cccccf");

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

/******************************************************************************
 * The function initializes the board.
 *****************************************************************************/

void nc_board_init() {

	_win_board = stdscr;

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
	// Initialize the foreground board as unset.
	//
	s_tarr_set(_nc_board_fg, ( s_tchar ) { TCHAR_CHR_UNUSED, -1, -1 });
}

/******************************************************************************
 * The function adds the checkers to the foreground board.
 *****************************************************************************/

void s_board_points_add_checkers(const int idx, const e_owner owner, const int num, const e_compressed compressed) {

	const s_point_layout layout = s_point_layout_get(num, compressed);

	const bool reverse = !s_point_layout_is_upper(idx);

	const s_tarr *tmpl;

	s_point pos = point_pos_checker(idx);

	for (int i = 0; i < num; i++) {

		//
		// Get the next template, which is null, if the checker is not visible.
		//
		if ((tmpl = s_tmpl_checker_get_tmpl(owner, layout, i, reverse)) == NULL) {
			continue;
		}

		//
		// The function returns the position of the next checker.
		//
		pos = s_tarr_cp_pos(_nc_board_fg, tmpl, pos, reverse);
	}
}

/******************************************************************************
 * The function does a refresh of the board window and if required it sleeps
 * for an animation.
 *****************************************************************************/

static void win_board_refresh(const bool do_sleep) {

	if (wrefresh(_win_board) == ERR) {
		log_exit_str("Unable to refresh window!");
	}

	if (!do_sleep) {
		return;
	}

	if (napms(MS_SLEEP) != OK) {
		log_exit_str("sleep failed!");
	}
}

/******************************************************************************
 * The function prints the board.
 *****************************************************************************/

void nc_board_print() {

	s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, (s_point ) { 0, 0 }, _nc_board_fg->dim);

	win_board_refresh(false);
}

/******************************************************************************
 * The function copies the traveler to the foreground at a given position and
 * prints the traveler area.
 *****************************************************************************/

static void travleler_print(const s_tarr *tmpl, const s_point tmpl_pos) {

	s_tarr_cp(_nc_board_fg, tmpl, tmpl_pos);

	s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, tmpl_pos, tmpl->dim);
}

/******************************************************************************
 * The function deletes the traveler from the foreground at a given position
 * and prints the (empty) traveler area.
 *****************************************************************************/

static void traveler_del(const s_tarr *tmpl, const s_point tmpl_pos) {

	s_tarr_del(_nc_board_fg, tmpl, tmpl_pos);

	s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, tmpl_pos, tmpl->dim);
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
	// Ensure that calling to this function is necessary.
	//
	if (s_point_same(tmpl_pos, &target)) {
		log_exit_str("Already the same!");
	}
#endif

	const s_point dir = direction_to(*tmpl_pos, target);

	while (true) {

		direction_mov_to(tmpl_pos, dir);

		//
		// Copy the checker template to the position
		//
		travleler_print(tmpl, *tmpl_pos);

		win_board_refresh(true);

		//
		// Delete the checker template from the position
		//
		traveler_del(tmpl, *tmpl_pos);

		//
		// If the current position is the target we are done. It is important
		// that the traveler is deleted before we return.
		//
		if (s_point_same(tmpl_pos, &target)) {
			log_debug_str("The traveler reached the target position.");
			return;
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void traveler_move_to(const s_tarr *tmpl, s_point *tmpl_pos, const e_dir dir) {

	direction_mov_to(tmpl_pos, direction_get(dir));

	s_tarr_cp(_nc_board_fg, tmpl, *tmpl_pos);
}

/******************************************************************************
 *
 *****************************************************************************/

void travler_move(const int idx_from, const int num_from, const int idx_to, const int num_to, const e_owner owner) {
	s_area area;

	const s_point pos_from = point_pos_checker(idx_from);

	const s_point pos_to = point_pos_checker(idx_to);

	const s_tarr *tmpl = s_tmpl_checker_get_travler(owner);

	s_point tmpl_pos;

	const bool from_is_upper = s_point_layout_is_upper(idx_from);
	const bool to_is_upper = s_point_layout_is_upper(idx_to);

	//
	// Phase 1
	//
	area = s_point_layout_ext_area(pos_from, from_is_upper);

	if (num_from <= CHECK_DIS_FULL) {

		//
		// Write the decreased checkers (uncompressed)
		//
		s_board_points_add_checkers(idx_from, owner, num_from - 1, E_UNCOMP);

		//
		// Add the traveler
		//
		tmpl_pos = s_point_layout_pos_full(pos_from, from_is_upper, E_UNCOMP, min(num_from, CHECK_DIS_FULL));
		s_tarr_cp(_nc_board_fg, tmpl, tmpl_pos);

		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);

	} else {

		//
		// Write the decreased checkers (compressed)
		//
		s_board_points_add_checkers(idx_from, owner, num_from - 1, E_COMP);

		//
		// Add the traveler
		//
		tmpl_pos = s_point_layout_pos_full(pos_from, from_is_upper, E_COMP, CHECK_DIS_FULL + 1);
		s_tarr_cp(_nc_board_fg, tmpl, tmpl_pos);

		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);

		win_board_refresh(true);

		traveler_del(tmpl, tmpl_pos);

		//
		// Move the traveler
		//
		traveler_move_to(tmpl, &tmpl_pos, from_is_upper ? E_DIR_DOWN : E_DIR_UP);

		//
		// Write the decreased checkers (uncompressed)
		//
		s_board_points_add_checkers(idx_from, owner, num_from - 1, E_UNCOMP);

		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);
	}

	win_board_refresh(true);

	traveler_del(tmpl, tmpl_pos);

	//
	// Phase: walk the line
	//
	traveler_move_line(tmpl, &tmpl_pos, (s_point ) { .row = TRAVEL_ROW, .col = tmpl_pos.col });

	traveler_move_line(tmpl, &tmpl_pos, (s_point ) { .row = TRAVEL_ROW, .col = pos_to.col });

	const s_point last_pos = s_point_layout_pos_full(pos_to, to_is_upper, E_UNCOMP, min(num_to + 1, CHECK_DIS_FULL + 1));

	traveler_move_line(tmpl, &tmpl_pos, (s_point ) { .row = last_pos.row, .col = last_pos.col });

	//
	// Phase: arrival
	//
	area = s_point_layout_ext_area(pos_to, to_is_upper);

	if (num_to < CHECK_DIS_FULL) {

		//
		// Write the increased checker, which overwrites the traveler, so it
		// does not have to be deleted.
		//
		s_board_points_add_checkers(idx_to, owner, num_to + 1, E_UNCOMP);
		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);

	} else {

		traveler_del(tmpl, tmpl_pos);
		traveler_move_to(tmpl, &tmpl_pos, to_is_upper ? E_DIR_UP : E_DIR_DOWN);
		s_board_points_add_checkers(idx_to, owner, num_to, E_COMP);
		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);

		//
		// Do pause
		//
		win_board_refresh(true);

		traveler_del(tmpl, tmpl_pos);

		s_board_points_add_checkers(idx_to, owner, num_to + 1, E_UNCOMP);
		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);
	}

	win_board_refresh(true);
}

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_test() {
	log_debug_str("start");

	travler_move(4, 6, 2, 4, OWNER_BLACK);

	travler_move(2, 5, 4, 5, OWNER_BLACK);

	travler_move(15, 6, 7, 4, OWNER_WHITE);

	travler_move(7, 5, 15, 5, OWNER_WHITE);

}

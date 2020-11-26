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

#include "lib_curses.h"
#include "lib_logging.h"
#include "nc_board.h"

#include "s_area.h"
#include "s_tmpl_points.h"
#include "s_tmpl_checker.h"
#include "nc_board.h"
#include "s_color_def.h"
#include "direction.h"

#include "s_point_layout.h"

static WINDOW *_win_board;

// todo: comment, file, ...

#define MS_SLEEP 40
//#define MS_SLEEP 500

#define TRAVEL_ROW POINTS_ROW + 2

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

static void nc_board_init_bg(s_tarr *board_bg, const s_board_areas *board_areas) {

	//
	// Set the background of the complete board.
	//
	short color_bar_bg[board_areas->board_dim.row];

	// TODO: choose final color
	s_color_def_gradient(color_bar_bg, board_areas->board_dim.row, "#22222f", "#55555f");

	s_tarr_set_gradient(board_bg, EMPTY, 0, color_bar_bg);

	// TODO: param
	// TODO: color
	s_tarr_set_bg(board_bg, board_areas->bear_off.pos, board_areas->bear_off.dim, color_bar_bg, true);

	//
	// Set the background of the inner / outer board.
	//
	short color_board_bg[board_areas->board_outer.dim.row];

	// TODO: choose final color
	s_color_def_gradient(color_board_bg, board_areas->board_outer.dim.row, "#88888f", "#cccccf");

	s_tarr_set_bg(board_bg, board_areas->board_outer.pos, board_areas->board_outer.dim, color_board_bg, false);

	s_tarr_set_bg(board_bg, board_areas->board_inner.pos, board_areas->board_inner.dim, color_board_bg, false);
}

/******************************************************************************
 * The function allocates the resources for the board.
 *****************************************************************************/

static void nc_board_alloc(const s_point board_dim) {

	log_debug_str("Allocating resources!");

	_nc_board_bg = s_tarr_new(board_dim.row, board_dim.col);

	_nc_board_fg = s_tarr_new(board_dim.row, board_dim.col);

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

void nc_board_init(const s_board_areas *board_areas) {

	_win_board = stdscr;

	nc_board_alloc(board_areas->board_dim);

	nc_board_init_bg(_nc_board_bg, board_areas);

	//
	// Add points to the board
	//
	s_tmpl_point_add_2_tarr(_nc_board_bg, s_pos_get_points());

	//
	// Initialize the foreground board as unset.
	//
	s_tarr_set(_nc_board_fg, S_TCHAR_UNUSED);
}

/******************************************************************************
 * The function adds the checkers to the foreground board.
 *****************************************************************************/

void s_board_points_add_checkers_pos(s_pos pos, const e_owner owner, const int num, const e_compressed compressed) {

	const s_point_layout layout = s_point_layout_get(num, compressed);

	const s_tarr *tmpl;

	for (int i = 0; i < num; i++) {

		//
		// Get the next template, which is null, if the checker is not visible.
		//
		if ((tmpl = s_tmpl_checker_get_tmpl(owner, layout, i, !pos.is_upper)) == NULL) {
			continue;
		}

		//
		// The function returns the position of the next checker.
		//
		pos.pos = s_tarr_cp_pos(_nc_board_fg, tmpl, pos.pos, !pos.is_upper);
	}
}

/******************************************************************************
 *
 *****************************************************************************/

void s_board_add_checkers(const s_field field, const e_owner owner, const int num) {

	const s_pos pos_tmp = s_pos_get_checker(field);

	s_board_points_add_checkers_pos(pos_tmp, owner, num, E_UNCOMP);
}

/******************************************************************************
 * The function does a refresh of the board window and if required it sleeps
 * for an animation.
 *****************************************************************************/

static void win_board_refresh(const bool do_sleep) {

	lc_win_refresh(_win_board);

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

	//
	// Move the cursor to a save place and do the refreshing. If the cursor
	// is not moved a flickering can occur. (I am not sure if this is necessary
	// for this game, but I had trouble with it in the past)
	//
	if (wmove(_win_board, 0, 0) == ERR) {
		log_exit_str("Unable to move the cursor!");
	}

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

	s_tarr_del(_nc_board_fg, tmpl->dim, tmpl_pos);

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

	//
	// Ensure that we do not go over the target. This means a simple distance
	// between the traveler position and the target position should be smaller
	// every time.
	//
	int dist = s_point_dist(*tmpl_pos, target);
#endif

	const s_point dir = direction_to(*tmpl_pos, target);

	while (true) {

		direction_mov_to(tmpl_pos, dir);

#ifdef DEBUG

		//
		// Compare the current and the last distances to ensure that the
		// current will be smaller.
		//
		dist = s_point_smaller_dist(*tmpl_pos, target, dist);
#endif

		log_debug("cur: %d/%d target: %d/%d", tmpl_pos->row, tmpl_pos->col, target.row, target.col);

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
 * The function moves the checker in the given direction.
 *****************************************************************************/

static void traveler_move_to(const s_tarr *tmpl, s_point *tmpl_pos, const e_dir dir) {

	direction_mov_to(tmpl_pos, direction_get(dir));

	s_tarr_cp(_nc_board_fg, tmpl, *tmpl_pos);
}

/******************************************************************************
 *
 *****************************************************************************/

static void travler_move(const s_pos *checker_from, const int num_from, const s_pos *checker_to, const int num_to, const e_owner owner) {
	s_area area;

	const s_tarr *tmpl = s_tmpl_checker_get_travler(owner);

	s_point tmpl_pos;

	//
	// Phase 1
	//
	area = s_point_layout_ext_area(checker_from);

	if (num_from <= CHECK_DIS_FULL) {

		//
		// Write the decreased checkers (uncompressed)
		//
		s_board_points_add_checkers_pos(*checker_from, owner, num_from - 1, E_UNCOMP);

		//
		// Add the traveler
		//
		tmpl_pos = s_point_layout_pos_full(checker_from, E_UNCOMP, min(num_from, CHECK_DIS_FULL));
		s_tarr_cp(_nc_board_fg, tmpl, tmpl_pos);

		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);

	} else {

		//
		// Write the decreased checkers (compressed)
		//
		s_board_points_add_checkers_pos(*checker_from, owner, num_from - 1, E_COMP);

		//
		// Add the traveler
		//
		tmpl_pos = s_point_layout_pos_full(checker_from, E_COMP, CHECK_DIS_FULL + 1);
		s_tarr_cp(_nc_board_fg, tmpl, tmpl_pos);

		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);

		win_board_refresh(true);

		traveler_del(tmpl, tmpl_pos);

		//
		// Move the traveler
		//
		traveler_move_to(tmpl, &tmpl_pos, checker_from->is_upper ? E_DIR_DOWN : E_DIR_UP);

		//
		// Write the decreased checkers (uncompressed)
		//
		s_board_points_add_checkers_pos(*checker_from, owner, num_from - 1, E_UNCOMP);

		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);
	}

	win_board_refresh(true);

	traveler_del(tmpl, tmpl_pos);

	//
	// Phase: walk the line
	//
	traveler_move_line(tmpl, &tmpl_pos, (s_point ) { .row = TRAVEL_ROW, .col = tmpl_pos.col });

	traveler_move_line(tmpl, &tmpl_pos, (s_point ) { .row = TRAVEL_ROW, .col = checker_to->pos.col });

	const s_point last_pos = s_point_layout_pos_full(checker_to, E_UNCOMP, min(num_to + 1, CHECK_DIS_FULL + 1));

	traveler_move_line(tmpl, &tmpl_pos, (s_point ) { .row = last_pos.row, .col = last_pos.col });

	//
	// Phase: arrival
	//
	area = s_point_layout_ext_area(checker_to);

	if (num_to < CHECK_DIS_FULL) {

		//
		// Write the increased checker, which overwrites the traveler, so it
		// does not have to be deleted.
		//
		s_board_points_add_checkers_pos(*checker_to, owner, num_to + 1, E_UNCOMP);
		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);

	} else {

		traveler_del(tmpl, tmpl_pos);
		traveler_move_to(tmpl, &tmpl_pos, checker_to->is_upper ? E_DIR_UP : E_DIR_DOWN);

		s_board_points_add_checkers_pos(*checker_to, owner, num_to, E_COMP);
		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);

		//
		// Do pause
		//
		win_board_refresh(true);

		traveler_del(tmpl, tmpl_pos);

		s_board_points_add_checkers_pos(*checker_to, owner, num_to + 1, E_UNCOMP);
		s_tarr_print_area(_win_board, _nc_board_fg, _nc_board_bg, area.pos, area.dim);
	}

	win_board_refresh(true);
}

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_test() {
	log_debug_str("start");

	s_pos pos_from, pos_to;

	pos_from = s_pos_get_checker((s_field ) { E_FIELD_POINTS, 11 });
	pos_to = s_pos_get_checker((s_field ) { E_FIELD_POINTS, 10 });

	travler_move(&pos_from, 5, &pos_to, 0, OWNER_WHITE);

	pos_from = s_pos_get_checker((s_field ) { E_FIELD_POINTS, 0 });
	pos_to = s_pos_get_checker((s_field ) { E_FIELD_BEAR_OFF, OWNER_WHITE });

	travler_move(&pos_from, 2, &pos_to, 0, OWNER_WHITE);

	pos_from = s_pos_get_checker((s_field ) { E_FIELD_POINTS, 0 });
	pos_to = s_pos_get_checker((s_field ) { E_FIELD_BAR, OWNER_WHITE });

	travler_move(&pos_from, 1, &pos_to, 0, OWNER_WHITE);
}

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

#include "lib_logging.h"
#include "lib_curses.h"
#include "direction.h"
#include "s_board.h"

#define MS_SLEEP 40

/******************************************************************************
 * The function initializes the s_board struct.
 *****************************************************************************/

void s_board_init(s_board *board, const s_point dim) {

	log_debug_str("Allocating resource!");

	board->bg = s_tarr_new(dim.row, dim.col);

	board->fg = s_tarr_new(dim.row, dim.col);

	board->win = stdscr;
}

/******************************************************************************
 * The function frees the s_board struct.
 *****************************************************************************/

void s_board_free(s_board *board) {

	log_debug_str("Freeing resources!");

	s_tarr_free(&board->fg);

	s_tarr_free(&board->bg);
}

/******************************************************************************
 * The function does a refresh of the board window and if required it sleeps
 * for an animation.
 *****************************************************************************/

void s_board_win_refresh(const s_board *board, const bool do_sleep) {

	lc_win_refresh(board->win);

	if (!do_sleep) {
		return;
	}

	if (napms(MS_SLEEP) != OK) {
		log_exit_str("sleep failed!");
	}
}

/******************************************************************************
 * The function copies the traveler to the foreground at a given position and
 * prints the traveler area.
 *****************************************************************************/

static void s_board_trv_print(const s_board *board, const s_tarr *tmpl, const s_point tmpl_pos) {

	s_tarr_cp(board->fg, tmpl, tmpl_pos);

	s_tarr_print_area(board->win, board->fg, board->bg, tmpl_pos, tmpl->dim);
}

/******************************************************************************
 * The function deletes the traveler from the foreground at a given position
 * and prints the (empty) traveler area.
 *****************************************************************************/

void s_board_trv_del(const s_board *board, const s_tarr *tmpl, const s_point tmpl_pos) {

	s_tarr_del(board->fg, tmpl->dim, tmpl_pos);

	s_tarr_print_area(board->win, board->fg, board->bg, tmpl_pos, tmpl->dim);
}

/******************************************************************************
 * The function is called with the template of the checker and moves the
 * checker from a position to a target position. It is assumed that this is a
 * horizontal or vertical movement. This means the rows or columns of the
 * current position have to be the same.
 *****************************************************************************/

void s_board_trv_mv_line(const s_board *board, const s_tarr *tmpl, s_point *tmpl_pos, const s_point target) {

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
		s_board_trv_print(board, tmpl, *tmpl_pos);

		s_board_win_refresh(board, true);

		//
		// Delete the checker template from the position
		//
		s_board_trv_del(board, tmpl, *tmpl_pos);

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

void s_board_trv_mv_to(const s_board *board, const s_tarr *tmpl, s_point *tmpl_pos, const e_dir dir) {

	direction_mov_to(tmpl_pos, direction_get(dir));

	s_tarr_cp(board->fg, tmpl, *tmpl_pos);
}

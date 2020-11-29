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
#include "lib_utils.h"
#include "lib_curses.h"
#include "s_color_def.h"
#include "s_area.h"
#include "s_tmpl_points.h"
#include "s_tmpl_checker.h"
#include "s_point_layout.h"
#include "direction.h"
#include "nc_board.h"
#include "nc_board.h"
#include "s_board.h"

static WINDOW *_win_board;

// todo: comment, file, ...

#define TRAVEL_ROW POINTS_ROW + 2

/******************************************************************************
 * We have two two-dimensional arrays, which represents the foreground and the
 * background. The background contains the board and the foreground the
 * checkers on the board. So the foreground is mostly transparent.
 *****************************************************************************/

// TODO: not necessary
//static s_tarr *_nc_board_bg;
//static s_tarr *_nc_board_fg;
/******************************************************************************
 *
 *****************************************************************************/

static s_board _board;

/******************************************************************************
 * The function initializes the background of the board.
 *****************************************************************************/

static void nc_board_init_bg(const s_game_cfg *game_cfg, s_tarr *board_bg, const s_board_areas *board_areas) {

	//
	// Set the background of the complete board.
	//
	short color_bar_bg[board_areas->board_dim.row];

	s_color_def_gradient(color_bar_bg, board_areas->board_dim.row, game_cfg->clr_board_start, game_cfg->clr_board_end);

	s_tarr_set_gradient(board_bg, EMPTY, 0, color_bar_bg);

	// TODO: param
	// TODO: color
	s_tarr_set_bg(board_bg, board_areas->bear_off.pos, board_areas->bear_off.dim, color_bar_bg, true);

	//
	// Set the background of the inner / outer board.
	//
	short color_board_bg[board_areas->board_outer.dim.row];

	s_color_def_gradient(color_board_bg, board_areas->board_outer.dim.row, game_cfg->clr_outer_inner_start, game_cfg->clr_outer_inner_end);

	s_tarr_set_bg(board_bg, board_areas->board_outer.pos, board_areas->board_outer.dim, color_board_bg, false);

	s_tarr_set_bg(board_bg, board_areas->board_inner.pos, board_areas->board_inner.dim, color_board_bg, false);
}

/******************************************************************************
 * The function allocates the resources for the board.
 *****************************************************************************/

static void nc_board_alloc(const s_game_cfg *game_cfg, const s_point board_dim) {

	log_debug_str("Allocating resources!");

	s_board_init(&_board, board_dim);

	// TODO: not necessary
	//_nc_board_bg = _board.bg;

	//_nc_board_fg = _board.fg;

	s_tmpl_checker_create(game_cfg);
}

/******************************************************************************
 * The function frees the resources for the board.
 *****************************************************************************/

void nc_board_free() {

	log_debug_str("Freeing resources!");

	s_board_free(&_board);

	s_tmpl_checker_free();
}

/******************************************************************************
 * The function initializes the board.
 *****************************************************************************/

void nc_board_init(const s_game_cfg *game_cfg, const s_board_areas *board_areas) {

	_win_board = stdscr;

	nc_board_alloc(game_cfg, board_areas->board_dim);

	//nc_board_init_bg(game_cfg, _nc_board_bg, board_areas);
	nc_board_init_bg(game_cfg, _board.bg, board_areas);

	//
	// Add points to the board
	//
	s_tmpl_point_create(game_cfg);

	s_tmpl_point_add_2_tarr(_board.bg, s_pos_get_points());

	s_tmpl_point_free();

	//
	// Initialize the foreground board as unset.
	//
	//s_tarr_set(_nc_board_fg, S_TCHAR_UNUSED);
	s_tarr_set(_board.fg, S_TCHAR_UNUSED);
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
		//pos.pos = s_tarr_cp_pos(_nc_board_fg, tmpl, pos.pos, !pos.is_upper);
		pos.pos = s_tarr_cp_pos(_board.fg, tmpl, pos.pos, !pos.is_upper);
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
 * The function prints the board.
 *****************************************************************************/

void nc_board_print() {

	s_tarr_print_area(_board.win, _board.fg, _board.bg, (s_point ) { 0, 0 }, _board.fg->dim);

	//
	// Move the cursor to a save place and do the refreshing. If the cursor
	// is not moved a flickering can occur. (I am not sure if this is necessary
	// for this game, but I had trouble with it in the past)
	//
	if (wmove(_win_board, 0, 0) == ERR) {
		log_exit_str("Unable to move the cursor!");
	}

	s_board_win_refresh(&_board, false);
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
		tmpl_pos = s_point_layout_pos_full(checker_from, E_UNCOMP, lu_min(num_from, CHECK_DIS_FULL));
		s_tarr_cp(_board.fg, tmpl, tmpl_pos);

		s_tarr_print_area(_board.win, _board.fg, _board.bg, area.pos, area.dim);

	} else {

		//
		// Write the decreased checkers (compressed)
		//
		s_board_points_add_checkers_pos(*checker_from, owner, num_from - 1, E_COMP);

		//
		// Add the traveler
		//
		tmpl_pos = s_point_layout_pos_full(checker_from, E_COMP, CHECK_DIS_FULL + 1);
		s_tarr_cp(_board.fg, tmpl, tmpl_pos);

		s_tarr_print_area(_board.win, _board.fg, _board.bg, area.pos, area.dim);

		s_board_win_refresh(&_board, true);

		s_board_trv_del(&_board, tmpl, tmpl_pos);

		//
		// Move the traveler
		//
		s_board_trv_mv_to(&_board, tmpl, &tmpl_pos, checker_from->is_upper ? E_DIR_DOWN : E_DIR_UP);

		//
		// Write the decreased checkers (uncompressed)
		//
		s_board_points_add_checkers_pos(*checker_from, owner, num_from - 1, E_UNCOMP);

		s_tarr_print_area(_board.win, _board.fg, _board.bg, area.pos, area.dim);
	}

	s_board_win_refresh(&_board, true);

	s_board_trv_del(&_board, tmpl, tmpl_pos);

	//
	// Phase: walk the line
	//
	s_board_trv_mv_line(&_board, tmpl, &tmpl_pos, (s_point ) { .row = TRAVEL_ROW, .col = tmpl_pos.col });

	s_board_trv_mv_line(&_board, tmpl, &tmpl_pos, (s_point ) { .row = TRAVEL_ROW, .col = checker_to->pos.col });

	const s_point last_pos = s_point_layout_pos_full(checker_to, E_UNCOMP, lu_min(num_to + 1, CHECK_DIS_FULL + 1));

	s_board_trv_mv_line(&_board, tmpl, &tmpl_pos, (s_point ) { .row = last_pos.row, .col = last_pos.col });

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
		s_tarr_print_area(_board.win, _board.fg, _board.bg, area.pos, area.dim);

	} else {

		s_board_trv_del(&_board, tmpl, tmpl_pos);
		s_board_trv_mv_to(&_board, tmpl, &tmpl_pos, checker_to->is_upper ? E_DIR_UP : E_DIR_DOWN);

		s_board_points_add_checkers_pos(*checker_to, owner, num_to, E_COMP);
		s_tarr_print_area(_board.win, _board.fg, _board.bg, area.pos, area.dim);

		//
		// Do pause
		//
		s_board_win_refresh(&_board, true);

		s_board_trv_del(&_board, tmpl, tmpl_pos);

		s_board_points_add_checkers_pos(*checker_to, owner, num_to + 1, E_UNCOMP);
		s_tarr_print_area(_board.win, _board.fg, _board.bg, area.pos, area.dim);
	}

	s_board_win_refresh(&_board, true);
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

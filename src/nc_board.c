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

#include "s_fieldset.h"
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
#include "s_board.h"
#include "e_owner.h"
#include "rules.h"

// todo: comment, file, ...

#define TRAVEL_ROW POINTS_ROW + 2

/******************************************************************************
 * The s_board struct with the foreground and background arrays and the window.
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

	//
	// Set the background of the bear off area, which is the reverse normal
	// background.
	//
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

static void nc_board_alloc(WINDOW *win, const s_game_cfg *game_cfg, const s_point board_dim) {

	log_debug_str("Allocating resources!");

	s_board_init(win, &_board, board_dim);

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

void nc_board_init(WINDOW *win, const s_game_cfg *game_cfg, const s_board_areas *board_areas) {

	nc_board_alloc(win, game_cfg, board_areas->board_dim);

	nc_board_init_bg(game_cfg, _board.bg, board_areas);

	//
	// Add points to the board
	//
	s_tmpl_point_create(game_cfg);

	s_tmpl_point_add_2_tarr(_board.bg, s_board_areas_get_points());

	s_tmpl_point_free();

	//
	// Initialize the foreground board as unset.
	//
	s_tarr_set(_board.fg, S_TCHAR_UNUSED);
}

/******************************************************************************
 * The function adds the checkers to the foreground board.
 *****************************************************************************/

void s_board_points_add_checkers_pos(const s_board *board, s_pos pos, const e_owner owner, const int num, const e_compressed compressed) {

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
		pos.pos = s_tarr_cp_pos(board->fg, tmpl, pos.pos, !pos.is_upper);
	}
}

/******************************************************************************
 * The function adds the checkers from the fieldset to the fields of a given
 * type.
 *****************************************************************************/

static void nc_board_add_checker(s_fieldset *fieldset, const int num, const e_field_type type) {
	const s_field *field;
	s_pos pos_tmp;

	for (int i = 0; i < num; i++) {

		field = s_fieldset_get(fieldset, type, i);

		//
		// We ignore fields without checker.
		//
		if (field->num != 0) {

			//
			// Get the position of the checkers on the board and add them.
			//
			pos_tmp = s_board_areas_get_checker(field->id);
			s_board_points_add_checkers_pos(&_board, pos_tmp, field->owner, field->num, E_UNCOMP);
		}
	}
}

/******************************************************************************
 * The function adds the checkers from the fieldset to the fields of all types.
 *
 * No checkers are removed from the board.
 *****************************************************************************/

void nc_board_points_add_checker(s_fieldset *fieldset) {

	nc_board_add_checker(fieldset, POINTS_NUM, E_FIELD_POINTS);

	nc_board_add_checker(fieldset, BARS_NUM, E_FIELD_BAR);

	nc_board_add_checker(fieldset, BEAR_OFF_NUM, E_FIELD_BEAR_OFF);
}

/******************************************************************************
 * The function resets and prints the board..
 *****************************************************************************/

void nc_board_reset(s_fieldset *fieldset) {

	//
	// Delete the current foreground.
	//
	s_tarr_set(_board.fg, S_TCHAR_UNUSED);

	//
	// Add the checker to the (empty) foreground.
	//
	nc_board_points_add_checker(fieldset);

	//
	// Print the board.
	//
	nc_board_print_win();
}

/******************************************************************************
 * The function prints the board window.
 *****************************************************************************/

void nc_board_print_win() {

	s_tarr_print_area(_board.win, _board.fg, _board.bg, (s_point ) { 0, 0 }, _board.fg->dim);

	//
	// Move the cursor to a save place and do the refreshing. If the cursor
	// is not moved a flickering can occur. (I am not sure if this is necessary
	// for this game, but I had trouble with it in the past)
	//
	if (wmove(_board.win, 0, 0) == ERR) {
		log_exit_str("Unable to move the cursor!");
	}

	s_board_win_refresh(&_board, false);
}

/******************************************************************************
 *
 *****************************************************************************/

static void travler_move(const s_board *board, const s_pos *checker_from, const int num_from, const s_pos *checker_to, const int num_to, const e_owner owner) {
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
		s_board_points_add_checkers_pos(board, *checker_from, owner, num_from - 1, E_UNCOMP);

		//
		// Add the traveler
		//
		tmpl_pos = s_point_layout_pos_full(checker_from, E_UNCOMP, lu_min(num_from, CHECK_DIS_FULL));
		s_tarr_cp(board->fg, tmpl, tmpl_pos);

		s_tarr_print_area(board->win, board->fg, board->bg, area.pos, area.dim);

	} else {

		//
		// Write the decreased checkers (compressed)
		//
		s_board_points_add_checkers_pos(board, *checker_from, owner, num_from - 1, E_COMP);

		//
		// Add the traveler
		//
		tmpl_pos = s_point_layout_pos_full(checker_from, E_COMP, CHECK_DIS_FULL + 1);
		s_tarr_cp(board->fg, tmpl, tmpl_pos);

		s_tarr_print_area(board->win, board->fg, board->bg, area.pos, area.dim);

		s_board_win_refresh(board, true);

		s_board_trv_del(board, tmpl, tmpl_pos);

		//
		// Move the traveler
		//
		s_board_trv_mv_to(board, tmpl, &tmpl_pos, checker_from->is_upper ? E_DIR_DOWN : E_DIR_UP);

		//
		// Write the decreased checkers (uncompressed)
		//
		s_board_points_add_checkers_pos(board, *checker_from, owner, num_from - 1, E_UNCOMP);

		s_tarr_print_area(board->win, board->fg, board->bg, area.pos, area.dim);
	}

	s_board_win_refresh(board, true);

	s_board_trv_del(board, tmpl, tmpl_pos);

	//
	// Phase: walk the line
	//
	s_board_trv_mv_line(board, tmpl, &tmpl_pos, (s_point ) { .row = TRAVEL_ROW, .col = tmpl_pos.col });

	s_board_trv_mv_line(board, tmpl, &tmpl_pos, (s_point ) { .row = TRAVEL_ROW, .col = checker_to->pos.col });

	const s_point last_pos = s_point_layout_pos_full(checker_to, E_UNCOMP, lu_min(num_to + 1, CHECK_DIS_FULL + 1));
	s_board_trv_mv_line(board, tmpl, &tmpl_pos, (s_point ) { .row = last_pos.row, .col = last_pos.col });

	//
	// Phase: arrival
	//
	area = s_point_layout_ext_area(checker_to);

	if (num_to < CHECK_DIS_FULL) {

		//
		// Write the increased checker, which overwrites the traveler, so it
		// does not have to be deleted.
		//
		s_board_points_add_checkers_pos(board, *checker_to, owner, num_to + 1, E_UNCOMP);
		s_tarr_print_area(board->win, board->fg, board->bg, area.pos, area.dim);

	} else {

		s_board_trv_del(board, tmpl, tmpl_pos);
		s_board_trv_mv_to(board, tmpl, &tmpl_pos, checker_to->is_upper ? E_DIR_UP : E_DIR_DOWN);

		s_board_points_add_checkers_pos(board, *checker_to, owner, num_to, E_COMP);
		s_tarr_print_area(board->win, board->fg, board->bg, area.pos, area.dim);

		//
		// Do pause
		//
		s_board_win_refresh(board, true);

		s_board_trv_del(board, tmpl, tmpl_pos);

		s_board_points_add_checkers_pos(board, *checker_to, owner, num_to + 1, E_UNCOMP);
		s_tarr_print_area(board->win, board->fg, board->bg, area.pos, area.dim);
	}

	s_board_win_refresh(board, true);
}

/******************************************************************************
 *
 *****************************************************************************/
// TODO: proper name and description
static void traveler_mv(s_field *field_src, s_field *field_dst) {

	//
	// Get the positions on the board.
	//
	const s_pos pos_from = s_board_areas_get_checker(field_src->id);
	const s_pos pos_to = s_board_areas_get_checker(field_dst->id);

	//
	// Move the checker on the board (animation).
	//
	travler_move(&_board, &pos_from, field_src->num, &pos_to, field_dst->num, field_src->owner);

	//
	// Move the checker on the game.
	//
	s_field_mv(field_src, field_dst);
}

/******************************************************************************
 * The function is called with the s_field_id from a mouse event.
 *****************************************************************************/
// TODO: working state
void nc_board_process(s_status *status, s_fieldset *fieldset, const s_field_id id) {

	//
	// If the game ended, there is nothing to do.
	//
	if (s_status_is_end(status)) {
		log_debug_str("Game ended!");
		return;
	}

	//
	// Get the source field. It returns NULL is the field is not valid.
	//
	s_field *field_src = rules_get_field_src(fieldset, status, id);
	if (field_src == NULL) {
		return;
	}

	//
	// Get the destination field. It returns NULL is the field is not valid.
	//
	s_field *field_dst = rules_can_mv(fieldset, status, field_src);
	if (field_dst == NULL) {
		log_debug_str("No target field found");
		return;
	}

	//
	// Hit the opponent checker if necessary.
	//
	const e_owner owner_other = e_owner_other(status->turn);
	if (field_dst->owner == owner_other) {
		const s_field_id field_other = { .type = E_FIELD_BAR, .idx = owner_other };
		traveler_mv(field_dst, s_fieldset_get_by_id(fieldset, field_other));
		s_status_set_phase(status, owner_other, E_PHASE_BAR);
	}

	traveler_mv(field_src, field_dst);

	rules_update_phase(fieldset, status);

	s_status_next_dice(status);
}

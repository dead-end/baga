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
#include "s_game.h"
#include "nc_board.h"
#include "rules.h"

/******************************************************************************
 * The function initializes the s_game struct. No checker are placed.
 *****************************************************************************/

void s_game_init(s_game *game) {

	//
	// Initialize the points
	//
	for (int i = 0; i < POINTS_NUM; i++) {

		s_field_set_full(game->point[i], E_FIELD_POINTS, i, 0, E_OWNER_NONE);
	}

	s_field_set_full(game->bear_off[E_OWNER_TOP], E_FIELD_BEAR_OFF, E_OWNER_TOP, 0, E_OWNER_TOP);

	s_field_set_full(game->bear_off[E_OWNER_BOT], E_FIELD_BEAR_OFF, E_OWNER_BOT, 0, E_OWNER_BOT);

	s_field_set_full(game->reenter[E_OWNER_TOP], E_FIELD_BAR, E_OWNER_TOP, 0, E_OWNER_TOP);

	s_field_set_full(game->reenter[E_OWNER_BOT], E_FIELD_BAR, E_OWNER_BOT, 0, E_OWNER_BOT);
}

/******************************************************************************
 * The function resets the s_game struct and sets the checker for the new game.
 *****************************************************************************/

void s_game_new_game(s_game *game, s_status *status) {

	//
	// Set all to 0
	//
	s_game_init(game);

	//
	// Index 0
	//
	s_field_set(game->point[s_field_idx_rel( E_OWNER_BOT, 0)], 2, E_OWNER_BOT);
	s_field_set(game->point[s_field_idx_rel( E_OWNER_TOP, 0)], 2, E_OWNER_TOP);

	//
	// Index 11
	//
	s_field_set(game->point[s_field_idx_rel( E_OWNER_BOT, 11)], 5, E_OWNER_BOT);
	s_field_set(game->point[s_field_idx_rel( E_OWNER_TOP, 11)], 5, E_OWNER_TOP);

	//
	// Index 16
	//
	s_field_set(game->point[s_field_idx_rel( E_OWNER_BOT, 16)], 3, E_OWNER_BOT);
	s_field_set(game->point[s_field_idx_rel( E_OWNER_TOP, 16)], 3, E_OWNER_TOP);

	//
	// Index 11
	//
	s_field_set(game->point[s_field_idx_rel( E_OWNER_BOT, 18)], 5, E_OWNER_BOT);
	s_field_set(game->point[s_field_idx_rel(E_OWNER_TOP, 18)], 5, E_OWNER_TOP);

	//
	// Reset the status for a new game.
	//
	// TODO: correct here?
	s_status_start(status);
}

/******************************************************************************
 * The function prints the checkers to the board.
 *****************************************************************************/

void s_game_print(const s_game *game) {

	for (int i = 0; i < POINTS_NUM; i++) {

		if (game->point[i].num != 0) {

			s_board_add_checkers(&game->point[i]);
		}
	}
}

/******************************************************************************
 * The function returns a pointer to the field with the given id.
 *****************************************************************************/

s_field* s_game_get(s_game *game, const s_field_id id) {

#ifdef DEBUG

	//
	// Ensure that the field id is valid
	//
	s_field_id_ensure_valid(id);
#endif

	switch (id.type) {

	case E_FIELD_BAR:
		return &game->reenter[id.idx];

	case E_FIELD_BEAR_OFF:
		return &game->bear_off[id.idx];

	case E_FIELD_POINTS:
		return &game->point[id.idx];

	default:
		log_exit("Unknown type: %d", id.type)
		;
	}
}

/******************************************************************************
 *
 *****************************************************************************/
// TODO: unit tests
// TODO: maybe a macro
bool s_game_is_min_out(const s_game *game, const s_status *status, const int dice) {

	const int idx_rel = rules_min_rel_idx(game, status);

	return idx_rel + dice > POINTS_NUM - 1;
}

/******************************************************************************
 * The function is called with a source field and the status and it computes
 * the index of the destination field.
 *****************************************************************************/
// TODO: missing: bear-off phase
// TODO: Name
s_field_id s_field_get_dst_id(const s_game *game, const s_field *field_src, const s_status *status) {
	s_field_id id_dst;

	//
	// Get the value from the active dice.
	//
	const int dice = s_dices_get_value(&status->dices);

	//
	// Get the source index of the field with respect to bars.
	//
	const int idx_src = s_field_get_src_idx(field_src);

	//
	// Compute the destination index.
	//
	id_dst.idx = s_field_idx_add_abs(field_src->owner, idx_src, dice);
	id_dst.type = E_FIELD_POINTS;

	log_debug("field index src: %d dst: %d dice: %d", idx_src, id_dst.idx, dice);

	if (s_field_idx_is_out(field_src->owner, id_dst.idx)) {

		if (status->player_phase[status->turn] == E_PHASE_BEAR_OFF && (s_field_idx_is_ex_out(field_src->owner, id_dst.idx) || s_game_is_min_out(game, status, dice))) {
			s_field_id_set(id_dst, E_FIELD_BEAR_OFF, status->turn);

		} else {
			log_debug("out of range dst: %d src: %d", id_dst.idx, idx_src);
			s_field_id_set(id_dst, E_FIELD_NONE, -1);
		}
	}

	return id_dst;
}

/******************************************************************************
 * The function is called with a source field and the result from a dice. The
 * source field is identified by a mouse event, so everything is possible.
 *
 * If the field is valid and contains a checker, the function checks if it can
 * move the given number of fields. If so, it returns the destination field. If
 * not it returns NULL.
 *****************************************************************************/

// TODO: the function is not complete.
// - ensure that it is the turn of the owner of the checker.
s_field* s_game_can_mv(s_game *game, s_status *status, const s_field *field_src) {

	if (!s_field_is_valid_src(field_src, status)) {
		log_debug_str("Source not valid!");
		return NULL;
	}

	const s_field_id id_dst = s_field_get_dst_id(game, field_src, status);
	if (id_dst.type == E_FIELD_NONE) {
		log_debug_str("no dest!");
		return NULL;
	}

	s_field *field_dst = s_game_get(game, id_dst);

	//
	// num > 1 => hit
	//
	if (field_dst->owner != E_OWNER_NONE && field_dst->owner != field_src->owner && field_dst->num > 1) {
		log_debug_str("diff owner!");
		return NULL;
	}

	log_debug("Owner: %d, idx: %d", field_src->owner, field_src->num);

	return field_dst;
}

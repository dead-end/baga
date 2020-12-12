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

/******************************************************************************
 * The function initializes the s_game struct. No checker are placed.
 *****************************************************************************/

void s_game_init(s_game *game) {

	//
	// Initialize the points
	//
	for (int i = 0; i < POINTS_NUM; i++) {

		s_field_set_full(game->point[i], E_FIELD_POINTS, i, 0, OWNER_NONE);
	}

	s_field_set_full(game->bear_off[OWNER_BLACK], E_FIELD_BEAR_OFF, OWNER_BLACK, 0, OWNER_BLACK);

	s_field_set_full(game->bear_off[OWNER_WHITE], E_FIELD_BEAR_OFF, OWNER_WHITE, 0, OWNER_WHITE);

	s_field_set_full(game->reenter[OWNER_BLACK], E_FIELD_BAR, OWNER_BLACK, 0, OWNER_BLACK);

	s_field_set_full(game->reenter[OWNER_WHITE], E_FIELD_BAR, OWNER_WHITE, 0, OWNER_WHITE);
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
	s_field_set(game->point[s_field_rel_idx(status, OWNER_WHITE, 0)], 2, OWNER_WHITE);
	s_field_set(game->point[s_field_rel_idx(status, OWNER_BLACK, 0)], 2, OWNER_BLACK);

	//
	// Index 11
	//
	s_field_set(game->point[s_field_rel_idx(status, OWNER_WHITE, 11)], 5, OWNER_WHITE);
	s_field_set(game->point[s_field_rel_idx(status, OWNER_BLACK, 11)], 5, OWNER_BLACK);

	//
	// Index 16
	//
	s_field_set(game->point[s_field_rel_idx(status, OWNER_WHITE, 16)], 3, OWNER_WHITE);
	s_field_set(game->point[s_field_rel_idx(status, OWNER_BLACK, 16)], 3, OWNER_BLACK);

	//
	// Index 11
	//
	s_field_set(game->point[s_field_rel_idx(status, OWNER_WHITE, 18)], 5, OWNER_WHITE);
	s_field_set(game->point[s_field_rel_idx(status, OWNER_BLACK, 18)], 5, OWNER_BLACK);

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
		return NULL;
	}

	const s_field_id id_dst = s_field_get_dst_id(field_src, status);
	s_field *field_dst = s_game_get(game, id_dst);

	//
	// num > 1 => hit
	//
	if (field_dst->owner != OWNER_NONE && field_dst->owner != field_src->owner && field_dst->num > 1) {
		log_debug_str("diff owner!");
		return NULL;
	}

	log_debug("Owner: %d, idx: %d", field_src->owner, field_src->num);

	return field_dst;
}

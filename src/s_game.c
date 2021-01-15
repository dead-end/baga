/*
 * MIT License
 *
 * Copyright (c) 2021 dead-end
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
 * The function sets a field and returns a pointer to that field.
 *****************************************************************************/

s_field* s_game_set(s_game *game, const e_field_type type, const int idx, const e_owner owner, const int num) {

	s_field *field;

	switch (type) {

	case E_FIELD_BAR:
		field = &game->reenter[idx];
		if (idx != owner) {
			log_exit("idx: %d owner: %d", idx, owner);
		}
		break;

	case E_FIELD_BEAR_OFF:
		field = &game->bear_off[idx];
		if (idx != owner) {
			log_exit("idx: %d owner: %d", idx, owner);
		}
		break;

	case E_FIELD_POINTS:
		field = &game->point[idx];
		field->owner = owner;
		break;

	default:
		log_exit("Unknown type: %d", type)
		;
	}

	field->num = num;

	return field;
}

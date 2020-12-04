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
 * The definition of the index for black and white. White is playing from upper
 * right to lower right and black is playing in reverse direction.
 *****************************************************************************/

#define idx_white(i) (i)

#define idx_black(i) lu_reverse_idx(POINTS_NUM,i)

/******************************************************************************
 * Two macros to set fields.
 *****************************************************************************/

#define s_game_set_full(p,t,i,n,o) \
	(p).id.type = (t); \
	(p).id.idx = (i); \
	(p).num = (n); \
	(p).owner = (o)

#define s_game_set(p,n,o) \
	(p).num = (n); \
	(p).owner = (o)

/******************************************************************************
 * The function initializes the s_game struct. No checker are placed.
 *****************************************************************************/

void s_game_init(s_game *game) {

	//
	// Initialize the points
	//
	for (int i = 0; i < POINTS_NUM; i++) {

		s_game_set_full(game->point[i], E_FIELD_POINTS, i, 0, OWNER_NONE);
	}

	s_game_set_full(game->bear_off[OWNER_BLACK], E_FIELD_BEAR_OFF, OWNER_BLACK, 0, OWNER_BLACK);

	s_game_set_full(game->bear_off[OWNER_BLACK], E_FIELD_BEAR_OFF, OWNER_WHITE, 0, OWNER_WHITE);

	s_game_set_full(game->reenter[OWNER_BLACK], E_FIELD_BAR, OWNER_BLACK, 0, OWNER_BLACK);

	s_game_set_full(game->reenter[OWNER_BLACK], E_FIELD_BAR, OWNER_WHITE, 0, OWNER_WHITE);
}

/******************************************************************************
 * The function resets the s_game struct and sets the checker for the new game.
 *****************************************************************************/

void s_game_new_game(s_game *game) {

	//
	// Set all to 0
	//
	s_game_init(game);

	//
	// Index 0
	//
	s_game_set(game->point[idx_white(0)], 2, OWNER_WHITE);
	s_game_set(game->point[idx_black(0)], 2, OWNER_BLACK);

	//
	// Index 5
	//
	s_game_set(game->point[idx_white(5)], 5, OWNER_WHITE);
	s_game_set(game->point[idx_black(5)], 5, OWNER_BLACK);

	//
	// Index 7
	//
	s_game_set(game->point[idx_white(7)], 5, OWNER_WHITE);
	s_game_set(game->point[idx_black(7)], 5, OWNER_BLACK);

	//
	// Index 11
	//
	s_game_set(game->point[idx_white(11)], 5, OWNER_WHITE);
	s_game_set(game->point[idx_black(11)], 5, OWNER_BLACK);
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
 * The function moves a checker from a source to a destination field.
 *****************************************************************************/

void s_game_mv(s_field *field_src, s_field *field_dst) {

	//
	// If there is not a checker on the destination we have to set the new
	// owner.
	//
	if (field_dst->id.type == E_FIELD_POINTS && field_dst->num == 0) {
		field_dst->owner = field_src->owner;
	}

	field_dst->num++;
	field_src->num--;

	//
	// If no checker is left on the source field, we delete the owner.
	//
	if (field_src->id.type == E_FIELD_POINTS && field_src->num == 0) {
		field_src->owner = OWNER_NONE;
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
s_field* s_game_can_mv(s_game *game, const s_field *field_src, const int num) {
	int idx_src;

	//
	// If a checker reached the bear off area, it cannot moved again.
	//
	if (field_src->id.type == E_FIELD_BEAR_OFF) {
		return NULL;
	}

	//
	// If there is no checker on the field, there is nothing to do.
	//
	if (field_src->num == 0) {
		return NULL;
	}

	if (field_src->id.type == E_FIELD_BAR) {
		idx_src = (field_src->owner == OWNER_WHITE) ? -1 : 16;
	} else {
		idx_src = field_src->id.idx;
	}

	int idx_dst = (field_src->owner == OWNER_WHITE) ? idx_src + num : idx_src - num;

	log_debug("idx_dst: %d %d", idx_dst, field_src->id.idx);

	s_field *field_dst = &game->point[idx_dst];

	if (field_dst->owner != OWNER_NONE && field_dst->owner != field_src->owner && field_dst->num > 1) {
		log_debug_str("diff owner!");
		return NULL;
	}

	log_debug("Owner: %d, idx: %d", field_src->owner, field_src->num);

	return field_dst;
}


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

/******************************************************************************
 * The header source file implements functions that implement rules of the
 * game. They operate on the fieldset, which is a collection of fields (points,
 * bear off, bar) and the status of the game.
 *****************************************************************************/

#include "bg_defs.h"
#include "lib_logging.h"
#include "rules.h"

/******************************************************************************
 * The function is looking for the last checker of the player in turn on the
 * points. It is used to decide whether the player is in the E_PHASE_BEAR_OFF
 * phase. The requires that the last checker is in the last quarter of the
 * points.
 *
 * The function computes the minimal relative index for the player that is in
 * turn. The function assumes that at least one checker is on a point.
 *
 * (unit tested)
 *****************************************************************************/

int rules_min_rel_idx(const s_fieldset *fieldset, const s_status *status) {
	int idx_abs;
	const s_field *field;

	for (int idx_rel = 0; idx_rel < POINTS_NUM; idx_rel++) {

		//
		// The index in the direction of the player.
		//
		idx_abs = s_field_idx_rel(status->turn, idx_rel);

		field = &fieldset->point[idx_abs];

		//
		// We stop as we find the first checker.
		//
		if (field->num > 0 && field->owner == status->turn) {
			log_debug("Owner: %s relative: %d absolute: %d", e_owner_str(status->turn), idx_abs, idx_rel);
			return idx_rel;
		}
	}

	log_exit_str("No checker found on the points!");
}

/******************************************************************************
 * The function updates the player phase for that player that is in turn.
 *
 * (unit tested)
 *****************************************************************************/

void rules_update_phase(const s_fieldset *fieldset, s_status *status) {

	//
	// Check: E_PHASE_BEAR_OFF => E_PHASE_WIN
	//
	if (fieldset->bear_off[status->turn].num == CHECKER_NUM) {
		status->player_phase[status->turn] = E_PHASE_WIN;
	}

	//
	// Check: E_PHASE_NORMAL / E_PHASE_BEAR_OFF => E_PHASE_BAR
	//
	else if (fieldset->reenter[status->turn].num > 0) {
		status->player_phase[status->turn] = E_PHASE_BAR;
	}

	//
	// Check: E_PHASE_NORMAL / E_PHASE_BAR  => E_PHASE_BEAR_OFF
	//
	else if (rules_min_rel_idx(fieldset, status) >= 3 * POINTS_QUARTER) {
		status->player_phase[status->turn] = E_PHASE_BEAR_OFF;
	}

	//
	// Everything else is normal
	//
	else {
		status->player_phase[status->turn] = E_PHASE_NORMAL;
	}

	log_debug("Phase: %s owner: %s", e_player_phase_str(status->player_phase[status->turn]), e_owner_str(status->turn));
}

/******************************************************************************
 * The function is called with an id of a field. The id comes from an mouse
 * event. The function returns that field, if it is a valid source field for a
 * move or NULL if not.
 *
 * (unit tested)
 *****************************************************************************/

s_field* rules_get_field_src(s_fieldset *fieldset, const s_status *status, const s_field_id field_id) {

	//
	// If a checker reached the bear off area, it cannot moved again.
	//
	if (field_id.type == E_FIELD_BEAR_OFF) {
		return NULL;
	}

	//
	// Identify the source field
	//
	s_field *field_src = s_fieldset_get(fieldset, field_id);

#ifdef DEBUG
	s_field_log(field_src, "Source field");
#endif

	log_debug("Phase: %s", e_player_phase_str(status->player_phase[status->turn]));

	//
	// If there is no checker on the field, there is nothing to do.
	//
	if (field_src->num == 0) {
		return NULL;
	}

	//
	// If there is a checker we need to ensure that this is the checker of the
	// player in turn.
	//
	if (field_src->owner != status->turn) {
		return NULL;
	}

	//
	// If the player phase is E_PHASE_BAR, we need to ensure that the source is
	// E_FIELD_BAR. From the last check we know that the player in turn is the
	// owner of the bar.
	//
	if (status->player_phase[status->turn] == E_PHASE_BAR && field_src->id.type != E_FIELD_BAR) {
		return NULL;
	}

	//
	// E_FIELD_BAR is only correct with E_PHASE_BAR
	//
	if (status->player_phase[status->turn] != E_PHASE_BAR && field_src->id.type == E_FIELD_BAR) {
		return NULL;
	}

	return field_src;
}

/******************************************************************************
 * The function is called with a source field and the result from a dice. The
 * source field is identified by a mouse event, so everything is possible.
 *
 * If the field is valid and contains a checker, the function checks if it can
 * move the given number of fields. If so, it returns the destination field. If
 * not it returns NULL.
 *****************************************************************************/
// TODO: unit tests
s_field* rules_can_mv(s_fieldset *fieldset, const s_status *status, const s_field *field_src) {

	//
	// Get the value from the active dice.
	//
	const int dice = s_dices_get_value(&status->dices);

	//
	// Get the relative index of the destination.
	//
	const int idx_rel_dst = (field_src->id.type == E_FIELD_BAR ? -1 : s_field_idx_rel(field_src->owner, field_src->id.idx)) + dice;

	//
	// Check if the destination is outside the board.
	//
	if (idx_rel_dst > POINTS_NUM - 1) {

		//
		// If the destination is outside, check if we have a bear off phase.
		//
		if (s_status_is_phase(status, field_src->owner, E_PHASE_BEAR_OFF)) {

			//
			// Check if the destination is an exact move outside.
			//
			if (idx_rel_dst == POINTS_NUM) {
				log_debug("Bear off exact - dst-rel: %d", idx_rel_dst);
				return s_fieldset_get_bear_off(fieldset, field_src->owner);
			}

			//
			// If the minimum + dice is outside, then the destination is valid.
			//
			const int min_rel_idx = rules_min_rel_idx(fieldset, status);

			if (min_rel_idx + dice > POINTS_NUM - 1) {
				log_debug("Bear off min - dst-rel: %d min: %d", idx_rel_dst, min_rel_idx);
				return s_fieldset_get_bear_off(fieldset, field_src->owner);
			}
		}

		//
		// At this point the target is outside and there is not valid bear off
		// move.
		//
		log_debug_str("Outside board and no valid destination!");
		return NULL;
	}

	//
	// At this point our destination is a regular point.
	//
	s_field *field_dst = s_fieldset_get_point(fieldset, s_field_idx_rel(field_src->owner, idx_rel_dst));

	//
	// Ensure that the destination is not occupied by the opponent (num > 1 => hit).
	//
	if (field_dst->owner != E_OWNER_NONE && field_dst->owner != field_src->owner && field_dst->num > 1) {
		log_debug("Destination occupied by the opponent - dst-rel: %d", idx_rel_dst);
		return NULL;
	}

	log_debug("Valid regular point - owner: %d, idx: %d", field_src->owner, field_src->num);
	return field_dst;
}

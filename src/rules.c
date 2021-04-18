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

int rules_min_rel_idx(const s_status *status, const s_fieldset *fieldset) {
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

void rules_update_phase(s_status *status, const s_fieldset *fieldset) {

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
	else if (rules_min_rel_idx(status, fieldset) >= 3 * POINTS_QUARTER) {
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

s_field* rules_get_field_src(const s_status *status, s_fieldset *fieldset, const s_field_id field_id) {

	//
	// If a checker reached the bear off area, it cannot moved again.
	//
	if (field_id.type == E_FIELD_BEAR_OFF) {
		return NULL;
	}

	//
	// Identify the source field
	//
	s_field *field_src = s_fieldset_get_by_id(fieldset, field_id);

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
 * The function checks if a field is occupied by the opponent. This means there
 * are at least two checkers. The test makes only sense if the destination
 * field is a point.
 *****************************************************************************/

static bool rules_is_dest_occupied(const s_field *field_src, const s_field *field_dst) {

#ifdef DEBUG

	//
	// Ensure that the destination is a point.
	//
	if (field_dst->id.type != E_FIELD_POINTS) {
		log_exit_str("destination in not a point!");
	}

#endif

	//
	// The owner has to be the opponent and num has to be greater than 1.
	//
	if (field_dst->owner != E_OWNER_NONE && field_dst->owner != field_src->owner && field_dst->num > 1) {
		log_debug_str("Destination occupied by the opponent!");
		return true;
	}

	return false;
}

/******************************************************************************
 * The function is called with a source field and the result from a dice. The
 * source field is identified by a mouse event, so everything is possible.
 *
 * If the field is valid and contains a checker, the function checks if it can
 * move the given number of fields. If so, it returns the destination field. If
 * not it returns NULL.
 *
 * (Unit tested - 3 functions)
 *****************************************************************************/

s_field* rules_can_mv(const s_status *status, s_fieldset *fieldset, const s_field *field_src) {

#ifdef DEBUG

	s_field_log(field_src, "Source to move.");

	s_field_id_valid_id(field_src->id);

	if (field_src->owner == E_OWNER_NONE || field_src->num == 0) {
		s_field_log(field_src, "src - is empty");
		log_exit_str("Source is empty!");
	}
#endif

	//
	// Get the value from the active dice.
	//
	const int dice = s_dices_get_value(&status->dices);

	//
	// Get the relative index of the destination.
	//
	const int idx_rel_dst = (field_src->id.type == E_FIELD_BAR ? -1 : s_field_idx_rel(field_src->owner, field_src->id.idx)) + dice;

	//
	// CASE: destination is normal point
	//
	if (!s_fieldset_rel_is_out(idx_rel_dst)) {

		s_field *field_dst = s_fieldset_get_point(fieldset, s_field_idx_rel(field_src->owner, idx_rel_dst));

		//
		// Ensure that the destination is not occupied by the opponent (num > 1 => hit).
		//
		if (rules_is_dest_occupied(field_src, field_dst)) {
			return NULL;
		}

#ifdef DEBUG
		s_field_log(field_dst, "Destination is a regular point.");
#endif
		return field_dst;
	}

	//
	// CASE: destination is out but no bear off phase
	//
	if (!s_status_is_phase(status, field_src->owner, E_PHASE_BEAR_OFF)) {
		log_debug("Destination is outside: %d but phase: %s", idx_rel_dst, e_player_phase_str(status->player_phase[field_src->owner]));
		return NULL;
	}

	//
	// CASE: destination is exact outside and phase is bear off
	//
	if (s_fieldset_rel_is_out_ex(idx_rel_dst)) {
		s_field *field_dst = s_fieldset_get_bear_off(fieldset, field_src->owner);

#ifdef DEBUG
		s_field_log(field_dst, "Destination is exact outside.");
#endif
		return field_dst;
	}

	//
	// If the minimum + dice is outside, then the destination is valid.
	//
	const int min_rel_idx = rules_min_rel_idx(status, fieldset);

#ifdef DEBUG
	const s_field *tmp = s_fieldset_get_point(fieldset, min_rel_idx);
	s_field_log(tmp, "Minimal field of the player.");
#endif

	//
	// CASE: far outside and bear off (exact out was tested before)
	//
	if (s_fieldset_rel_is_out(min_rel_idx + dice)) {
		log_debug("Bear off min - dst-rel: %d min: %d", idx_rel_dst, min_rel_idx);
		return s_fieldset_get_bear_off(fieldset, field_src->owner);
	}

	//
	// CASE: far outside and no bear off
	//
	log_debug("Destination is far outside: %d", idx_rel_dst);
	return NULL;
}

/******************************************************************************
 *
 *****************************************************************************/

// TODO: first try / not used
bool rules_is_dice_valid(s_status *status, s_fieldset *fieldset) {
	s_field *field;

	const int dice = s_dices_get_value(&status->dices);

	if (s_status_is_phase(status, status->turn, E_PHASE_BAR)) {

		field = s_fieldset_get_point_rel(fieldset, status->turn, dice - 1);
		return !(field->owner == e_owner_other(status->turn) && field->num > 1);
	}

	bool is_last = true;

	for (int i = 0; i < POINTS_NUM; i++) {

		field = s_fieldset_get_point_rel(fieldset, status->turn, i);

		if (field->owner != status->turn) {
			continue;
		}

		if (i + dice == POINTS_NUM) {
			return s_status_is_phase(status, status->turn, E_PHASE_BEAR_OFF);
		}

		if (i + dice >= POINTS_NUM) {
			return is_last && s_status_is_phase(status, status->turn, E_PHASE_BEAR_OFF);
		}

		is_last = false;

		field = s_fieldset_get_point_rel(fieldset, status->turn, i + dice);
		return !(field->owner == e_owner_other(status->turn) && field->num > 1);
	}

	return false;
}

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

#include "bg_defs.h"
#include "lib_logging.h"
#include "rules.h"

/******************************************************************************
 * The function computes the minimal relative index for the player that is in
 * turn. The function assumes that at least one checker is on a point.
 *
 * (unit tested)
 *****************************************************************************/

int rules_min_rel_idx(const s_game *game, const s_status *status) {
	int idx_abs;
	const s_field *field;

	for (int idx_rel = 0; idx_rel < POINTS_NUM; idx_rel++) {

		idx_abs = s_field_idx_rel(status->turn, idx_rel);

		field = &game->point[idx_abs];

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

void rules_update_phase(const s_game *game, s_status *status) {

	//
	// Check: E_PHASE_BEAR_OFF => E_PHASE_WIN
	//
	if (game->bear_off[status->turn].num == CHECKER_NUM) {
		status->player_phase[status->turn] = E_PHASE_WIN;
	}

	//
	// Check: E_PHASE_NORMAL / E_PHASE_BEAR_OFF => E_PHASE_BAR
	//
	else if (game->reenter[status->turn].num > 0) {
		status->player_phase[status->turn] = E_PHASE_BAR;
	}

	//
	// Check: E_PHASE_NORMAL / E_PHASE_BAR  => E_PHASE_BEAR_OFF
	//
	else if (rules_min_rel_idx(game, status) >= 3 * POINTS_QUARTER) {
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

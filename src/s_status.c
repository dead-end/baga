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

#include <stdlib.h>

#include "lib_logging.h"
#include "s_status.h"
#include "dice.h"

/******************************************************************************
 * The function initializes the status struct, with the game configurations.
 * The values do not change after the start of the game.
 *****************************************************************************/

void s_status_init(s_status *status, const s_game_cfg *game_cfg) {

	status->owner_top_color = game_cfg->owner_top_color;

	status->owner_start = game_cfg->owner_start;
}

/******************************************************************************
 * The function is called on a new game. It sets the user, that has the first
 * turn.
 *****************************************************************************/

void s_status_start(s_status *status) {

	//
	// Set the player that starts: OWNER_TOP / OWNER_BOT
	//
	status->turn = status->owner_start;

	status->player_phase[0] = E_PHASE_NORMAL;
	status->player_phase[1] = E_PHASE_NORMAL;

	s_dices_toss(&status->dices);
}

/******************************************************************************
 *
 *****************************************************************************/
// TODO: comment / maybe name with dice
bool s_status_mv_done(s_status *status) {
	bool result;

	if (s_dices_processed(&status->dices)) {
		status->turn = e_owner_other(status->turn);
		s_dices_toss(&status->dices);
		result = true;
	} else {
		result = false;
	}

	s_dices_debug(&status->dices);

	//
	// TODO: is this correct? I added this drunk :o)
	//
	dice_print(status);

	return result;
}

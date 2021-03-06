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
#include <string.h>

#include "lib_logging.h"
#include "controls.h"
#include "s_status.h"

/******************************************************************************
 * Definition of structures to save the status to be able to do an undo.
 *****************************************************************************/

static s_status _status_undo;

static s_fieldset _fieldset_undo;

/******************************************************************************
 * The functions saves the status, so that we can do a undo.
 *****************************************************************************/

static void s_status_undo_save(const s_status *status, const s_fieldset *fieldset) {

	log_debug_str("Do save");

	memcpy(&_status_undo, status, sizeof(s_status));
	memcpy(&_fieldset_undo, fieldset, sizeof(s_fieldset));
}

/******************************************************************************
 * The function resets the status on an undo request.
 *****************************************************************************/

void s_status_undo_reset(s_status *status, s_fieldset *fieldset) {

	log_debug_str("Do reset");

	memcpy(status, &_status_undo, sizeof(s_status));
	memcpy(fieldset, &_fieldset_undo, sizeof(s_fieldset));
}

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

void s_status_start(s_status *status, const s_fieldset *fieldset) {

	//
	// Set the player that starts: OWNER_TOP / OWNER_BOT
	//
	status->turn = status->owner_start;

	status->player_phase[0] = E_PHASE_NORMAL;
	status->player_phase[1] = E_PHASE_NORMAL;

	//
	// Toss the dices and save the result for an undo request.
	//
	s_dices_toss(&status->dices);
	s_status_undo_save(status, fieldset);
}

/******************************************************************************
 * The function is called to confirm the current round. It changes the player
 * in turn and tosses the dices.
 *****************************************************************************/

void s_status_do_confirm(s_status *status, const s_fieldset *fieldset) {

#ifdef DEBUG

	//
	// Ensure that we are in a state to confirm the round. This means that we
	// have no active dice.
	//
	if (!s_dices_is_done(status->dices)) {
		log_exit_str("Wrong status - dices not processed!");
	}
#endif

	//
	// Change the player in turn.
	//
	status->turn = e_owner_other(status->turn);

	//
	// Toss the dices and save the result for an undo request.
	//
	s_dices_toss(&status->dices);
	s_status_undo_save(status, fieldset);

	// TODO: check E_DICE_NOT_POS for the active dice

	s_dices_debug(&status->dices);
}

/******************************************************************************
 * The function selects the next dice and prints the control window.
 *****************************************************************************/

void s_status_next_dice(s_status *status) {

	s_dices_next(&status->dices);

	// TODO: check E_DICE_NOT_POS for the active dice

	controls_print(status);
}

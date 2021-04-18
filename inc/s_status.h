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

#ifndef INC_S_STATUS_H_
#define INC_S_STATUS_H_

#include "bg_defs.h"
#include "s_dices.h"
#include "s_game_cfg.h"
#include "e_owner.h"
#include "e_player_phase.h"
#include "s_fieldset.h"

/******************************************************************************
 *
 *****************************************************************************/

typedef struct {

	e_owner turn;

	e_player_phase player_phase[2];

	s_dices dices;

	//
	// Define whether BLACK or WHITE is playing from top to bottom.
	//
	// TODO: use for logging
	e_player_color owner_top_color;

	//
	// Define the player that starts: TOP or BOTTOM
	//
	e_owner owner_start;

} s_status;

/******************************************************************************
 * Macro to check if the game ended.
 *****************************************************************************/

#define s_status_is_end(s) ((s)->player_phase[0] == E_PHASE_WIN || (s)->player_phase[1] == E_PHASE_WIN)

#define s_status_is_phase(s,o,p) ((s)->player_phase[o] == (p))

#define s_status_set_phase(s,o,p) (s)->player_phase[o] = (p)

/******************************************************************************
 * The macro checks if the current round has to be confirmed. The human player
 * confirms his own and the computer round.
 *****************************************************************************/

#define s_status_need_confirm(s) s_dices_is_done((s)->dices)

void s_status_do_confirm(s_status *status, const s_fieldset *fieldset);

/******************************************************************************
 * The function declarations.
 *****************************************************************************/

void s_status_init(s_status *status, const s_game_cfg *game_cfg);

void s_status_start(s_status *status, const s_fieldset *fieldset);

void s_status_next_dice(s_status *status);

/******************************************************************************
 * Declaration of functions for an undo request.
 *****************************************************************************/

void s_status_undo_reset(s_status *status, s_fieldset *fieldset);

#endif /* INC_S_STATUS_H_ */

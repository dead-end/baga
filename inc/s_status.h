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

/******************************************************************************
 * The enum defines the possible phases of a player.
 *****************************************************************************/

typedef enum {

	//
	// None of the other two phases.
	//
	E_PHASE_NORMAL = 0,

	//
	// At least one of the checkers is on the bar.
	//
	E_PHASE_BAR = 1,

	//
	// All checkers are in that player's home board.
	//
	E_PHASE_BEAR_OFF = 2,

	//
	// All checkers are in the bear off.
	//
	E_PHASE_WIN = 3,

} e_player_phase;

// TODO: new place
const char* e_player_phase_str(const e_player_phase player_phase);

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
 * The function declarations.
 *****************************************************************************/

void s_status_init(s_status *status, const s_game_cfg *game_cfg);

void s_status_start(s_status *status);

void s_status_next_turn(s_status *status);

void s_status_mv_done(s_status *status);

#endif /* INC_S_STATUS_H_ */

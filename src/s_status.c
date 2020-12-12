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
#include <time.h>

#include "lib_logging.h"
#include "s_status.h"
#include "dice.h"

/******************************************************************************
 * The macro is rolling the dice.
 *****************************************************************************/

#define rolling_dice() rand() % 6 + 1

/******************************************************************************
 * The function initializes the status struct.
 *****************************************************************************/

void s_status_init(s_status *status, const s_game_cfg *game_cfg) {
	time_t t;

	srand((unsigned) time(&t));

	status->up_2_down = game_cfg->up_2_down;
}

/******************************************************************************
 * The function sets new values for the dices. They are sorted to ensure that
 * the first dice has the higher value.
 *****************************************************************************/

static void s_status_dice(s_status *status) {

	status->dices[0].value = rolling_dice();
	status->dices[1].value = rolling_dice();

	status->dices[0].num_set = 0;
	status->dices[1].num_set = 0;

	if (status->dices[0].value == status->dices[1].value) {
		status->dices[0].num = 2;
		status->dices[1].num = 2;

	} else {
		status->dices[0].num = 1;
		status->dices[1].num = 1;
	}

	//
	// Set status
	//
	if (status->dices[0].value >= status->dices[1].value) {
		status->dices[0].status = E_DICE_ACTIVE;
		status->dices[1].status = E_DICE_INACTIVE;
	} else {
		status->dices[0].status = E_DICE_INACTIVE;
		status->dices[1].status = E_DICE_ACTIVE;
	}

	log_debug("dice-1 value %d num: %d set: %d", status->dices[0].value, status->dices[0].num, status->dices[0].num_set);
	log_debug("dice-2 value %d num: %d set: %d", status->dices[1].value, status->dices[1].num, status->dices[1].num_set);
}

/******************************************************************************
 * The function is called on a new game. It sets the user, that has the first
 * turn.
 *****************************************************************************/

void s_status_start(s_status *status) {

	//
	// The player that plays from top to bottom starts.
	//
	status->turn = status->up_2_down;

	s_status_dice(status);
}

/******************************************************************************
 * The function is called on the next turn.
 *****************************************************************************/

void s_status_next_turn(s_status *status) {
	status->turn = e_owner_other(status->turn);

	s_status_dice(status);
}

/******************************************************************************
 *
 *****************************************************************************/
// TODO:
int s_status_get_dice(const s_status *status) {

	if (status->dices[0].status == E_DICE_ACTIVE) {
		return status->dices[0].value;
	}

	if (status->dices[1].status == E_DICE_ACTIVE) {
		return status->dices[1].value;
	}

	log_exit_str("Active!");
}

/******************************************************************************
 *
 *****************************************************************************/
// TODO:
void s_status_mv_done(s_status *status) {

	if (status->dices[0].status == E_DICE_ACTIVE) {

		status->dices[0].num_set++;
		if (status->dices[0].num_set == status->dices[0].num) {
			status->dices[0].status = E_DICE_SET;

			if (status->dices[1].status == E_DICE_INACTIVE) {
				status->dices[1].status = E_DICE_ACTIVE;
			} else {
				s_status_next_turn(status);
			}
		}

	} else if (status->dices[1].status == E_DICE_ACTIVE) {

		status->dices[1].num_set++;
		if (status->dices[1].num_set == status->dices[1].num) {
			status->dices[1].status = E_DICE_SET;

			if (status->dices[0].status == E_DICE_INACTIVE) {
				status->dices[0].status = E_DICE_ACTIVE;
			} else {
				s_status_next_turn(status);
			}
		}

	}
}

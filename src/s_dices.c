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
 * The source file implements the functionality of a pair of dices. The two
 * dices always work together. Each dice has its own status, but the
 * functionality always requires two.
 *****************************************************************************/

#include <time.h>

#include "lib_logging.h"
#include "s_dices.h"

/******************************************************************************
 * Macro to select the index of the other dice.
 *****************************************************************************/

#define s_dice_other(i) ((i + 1) % 2)

/******************************************************************************
 * The macro is rolling the dice.
 *****************************************************************************/

#define rolling_dice() (rand() % 6 + 1)

/******************************************************************************
 * The function initializes the random numbers.
 *****************************************************************************/

void s_dices_init() {
	time_t t;

	srand((unsigned) time(&t));
}

/******************************************************************************
 * The function returns a string representation of the dice status.
 *****************************************************************************/

const char* s_dice_status_str(const e_dice_status dice_status) {

	switch (dice_status) {

	case E_DICE_ACTIVE:
		return "E_DICE_ACTIVE";

	case E_DICE_INACTIVE:
		return "E_DICE_INACTIVE";

	case E_DICE_SET:
		return "E_DICE_SET";

	case E_DICE_NOT_POS:
		return "E_DICE_NOT_POS";

	default:
		log_exit("Unknown dice status: %d", dice_status)
		;
	}
}

/******************************************************************************
 * The function prints debug informations. If DEBUG is not set, there is a
 * warning about unused parameter, so we define the function only in DEBUG
 * mode.
 *****************************************************************************/

#ifdef DEBUG

void s_dices_debug(const s_dices *dices) {

	for (int i = 0; i < 2; i++) {
		log_debug("dice: %d max: %d set: %d value: %d %s",

		i, dices->dice[i].max_set, dices->dice[i].num_set, dices->dice[i].value, s_dice_status_str(dices->dice[i].status));
	}
}

#endif

/******************************************************************************
 * The function sets new values for the dices. This is mainly used for testing.
 *****************************************************************************/

void s_dices_toss(s_dices *dices) {

	s_dices_set(dices, rolling_dice(), rolling_dice());
}

/******************************************************************************
 * The function sets new values for the dices. They are sorted to ensure that
 * the first dice has the higher value.
 *****************************************************************************/

void s_dices_set(s_dices *dices, const int dice1, const int dice2) {

	//
	// Ensure that the first dice has the higher value.
	//
	if (dice1 >= dice2) {
		dices->dice[0].value = dice1;
		dices->dice[1].value = dice2;
	} else {
		dices->dice[0].value = dice2;
		dices->dice[1].value = dice1;
	}

	dices->dice[0].num_set = 0;
	dices->dice[1].num_set = 0;

	dices->dice[0].status = E_DICE_ACTIVE;
	dices->dice[1].status = E_DICE_INACTIVE;

	//
	// Set the max number depending on whether we have doublets.
	//
	if (dices->dice[0].value == dices->dice[1].value) {
		dices->dice[0].max_set = 2;
		dices->dice[1].max_set = 2;

	} else {
		dices->dice[0].max_set = 1;
		dices->dice[1].max_set = 1;
	}

#ifdef DEBUG
	s_dices_debug(dices);
#endif

}

/******************************************************************************
 * The function returns the current value of the dices. This is the value of
 * the active dice.
 *
 * (Unit tested)
 *****************************************************************************/

int s_dices_get_value(const s_dices *dices) {

	if (dices->dice[0].status == E_DICE_ACTIVE) {
		return dices->dice[0].value;
	}

	if (dices->dice[1].status == E_DICE_ACTIVE) {
		return dices->dice[1].value;
	}

	log_exit_str("No active dice found!");
}

/******************************************************************************
 * The function is called if a dice value is processed. If we have doublets
 * then the current dice value can be processed. If the dices are not doublets
 * or both values are processed the status of the E_DICE_ACTIVE dice is set to
 * E_DICE_SET. If the other dice is E_DICE_INACTIVE, it is switched to
 * E_DICE_ACTIVE.
 *
 * (Unit tested)
 *****************************************************************************/

void s_dices_next(s_dices *dices) {
	s_dice *dice;

	for (int dice_idx = 0; dice_idx < 2; dice_idx++) {

		dice = &dices->dice[dice_idx];

		if (dice->status == E_DICE_ACTIVE) {

			//
			// If we have doublets each dice can be set twice.
			//
			dice->num_set++;
			if (dice->num_set == dice->max_set) {
				dice->status = E_DICE_SET;

				//
				// Get the other dice
				//
				s_dice *dice_other = &dices->dice[s_dice_other(dice_idx)];

				//
				// Switch the other dice to active.
				//
				if (dice_other->status == E_DICE_INACTIVE) {
					dice_other->status = E_DICE_ACTIVE;
					return;
				}

				//
				// If the other dice cannot be activated, we are done.
				//
				return;
			}

			return;
		}
	}

	log_exit_str("No active dice to set!");
}

/******************************************************************************
 * The function is called in reaction to a mouse event on one of the dices and
 * tries to toggle the active dice. To succeed this requires that the clicked
 * dice is inactive (in this case the other has to be active). All other dice
 * states are not valid for toggle.
 *
 * (Unit tested)
 *****************************************************************************/

bool s_dice_toggle_active(s_dices *dices, const int idx) {

	log_debug("Checking toogle: %d", idx);

	if (dices->dice[idx].status == E_DICE_INACTIVE && dices->dice[s_dice_other(idx)].status == E_DICE_ACTIVE) {
		dices->dice[idx].status = E_DICE_ACTIVE;
		dices->dice[s_dice_other(idx)].status = E_DICE_INACTIVE;

		return true;
	}

	return false;
}

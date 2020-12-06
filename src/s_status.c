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

// TODO: better name
#define next() rand() % 6 + 1

/******************************************************************************
 *
 *****************************************************************************/
// TODO:
void s_status_init() {
	time_t t;

	srand((unsigned) time(&t));
}

/******************************************************************************
 * The function sets new values for the dices. They are sorted to ensure that
 * the first dice has the higher value.
 *****************************************************************************/

static void s_status_dice(s_status *status) {

	status->dice_1 = next();
	status->dice_2 = next();

	//
	// Swap the values if necessary.
	//
	if (status->dice_1 < status->dice_2) {

		const int tmp = status->dice_1;
		status->dice_1 = status->dice_2;
		status->dice_2 = tmp;
	}

	status->active = 0;
}

/******************************************************************************
 * The function is called on a new game. It sets the user, that has the first
 * turn.
 *****************************************************************************/

void s_status_start(s_status *status, const e_owner owner) {
	status->turn = owner;

	s_status_dice(status);
}

/******************************************************************************
 * The function is called on the next turn.
 *****************************************************************************/

void s_status_next_turn(s_status *status) {
	status->turn = e_owner_other(status->turn);

	s_status_dice(status);
}

// TODO:
int s_status_get_dice(s_status *status) {

	if (status->active == 0) {
		return status->dice_1;
	}

	if (status->active == 1) {
		return status->dice_2;
	}

	log_exit_str("Active!");
}

// TODO:
void s_status_mv_done(s_status *status) {

	if (status->active == 1) {
		s_status_next_turn(status);

	} else {
		status->active++;
	}

	dice_print(status);
}

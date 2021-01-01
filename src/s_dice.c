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

#include "lib_logging.h"
#include "s_dice.h"

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
 * The function is called with an array of dices and prints debug informations.
 * If DEBUG is not set, there is a warning about unused parameter, so we define
 * the function only in DEBUG mode.
 *****************************************************************************/

#ifdef DEBUG

void s_dice_debug(const s_dice dice[]) {

	for (int i = 0; i < 2; i++) {
		log_debug("dice: %d num: %d set: %d value: %d %s", i, dice[i].num, dice[i].num_set, dice[i].value, s_dice_status_str(dice[i].status));
	}
}

#endif

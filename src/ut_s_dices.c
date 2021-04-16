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

#include "ut_utils.h"

#include "s_dices.h"

/******************************************************************************
 * The function checks the s_dices_processed() calls.
 *****************************************************************************/

static void test_s_dices_processed() {
	s_dices dices;
	bool result;

	//
	// Different dice values
	//
	dices.dice[0].max_set = 1;
	dices.dice[1].max_set = 1;

	dices.dice[0].num_set = 0;
	dices.dice[1].num_set = 0;

	dices.dice[0].status = E_DICE_ACTIVE;
	dices.dice[1].status = E_DICE_INACTIVE;

	s_dices_next(&dices);
	result = s_dices_is_done(dices);

	ut_check_bool(result, false, "diff 1-1");
	ut_check_bool(dices.dice[0].status == E_DICE_SET, true, "diff 1-2");
	ut_check_bool(dices.dice[1].status == E_DICE_ACTIVE, true, "diff 1-3");

	s_dices_next(&dices);
	result = s_dices_is_done(dices);

	ut_check_bool(result, true, "diff 2-1");
	ut_check_bool(dices.dice[0].status == E_DICE_SET, true, "diff 2-2");
	ut_check_bool(dices.dice[1].status == E_DICE_SET, true, "diff 2-3");

	//
	// Same dice values
	//
	dices.dice[0].max_set = 2;
	dices.dice[1].max_set = 2;

	dices.dice[0].num_set = 0;
	dices.dice[1].num_set = 0;

	dices.dice[0].status = E_DICE_ACTIVE;
	dices.dice[1].status = E_DICE_INACTIVE;

	s_dices_next(&dices);
	result = s_dices_is_done(dices);

	ut_check_bool(result, false, "same 1-1");
	ut_check_bool(dices.dice[0].status == E_DICE_ACTIVE, true, "same 1-2");
	ut_check_bool(dices.dice[1].status == E_DICE_INACTIVE, true, "same 1-3");

	s_dices_next(&dices);
	result = s_dices_is_done(dices);

	ut_check_bool(result, false, "same 2-1");
	ut_check_bool(dices.dice[0].status == E_DICE_SET, true, "same 2-2");
	ut_check_bool(dices.dice[1].status == E_DICE_ACTIVE, true, "same 2-3");

	s_dices_next(&dices);
	result = s_dices_is_done(dices);

	ut_check_bool(result, false, "same 3-1");
	ut_check_bool(dices.dice[0].status == E_DICE_SET, true, "same 3-2");
	ut_check_bool(dices.dice[1].status == E_DICE_ACTIVE, true, "same 3-3");

	s_dices_next(&dices);
	result = s_dices_is_done(dices);

	ut_check_bool(result, true, "same 4-1");
	ut_check_bool(dices.dice[0].status == E_DICE_SET, true, "same 4-2");
	ut_check_bool(dices.dice[1].status == E_DICE_SET, true, "same 4-3");
}

/******************************************************************************
 * The function checks the s_dice_toggle_active() calls.
 *****************************************************************************/

static void test_s_dice_toggle_active() {
	s_dices dices;
	bool result;

	dices.dice[0].status = E_DICE_ACTIVE;
	dices.dice[1].status = E_DICE_INACTIVE;

	result = s_dice_toggle_active(&dices, 0);
	ut_check_bool(result, false, "click active");

	dices.dice[0].status = E_DICE_SET;
	dices.dice[1].status = E_DICE_ACTIVE;

	result = s_dice_toggle_active(&dices, 0);
	ut_check_bool(result, false, "click set");

	dices.dice[0].status = E_DICE_NOT_POS;
	dices.dice[1].status = E_DICE_ACTIVE;

	result = s_dice_toggle_active(&dices, 0);
	ut_check_bool(result, false, "click not possible");

	dices.dice[0].status = E_DICE_INACTIVE;
	dices.dice[1].status = E_DICE_ACTIVE;

	result = s_dice_toggle_active(&dices, 0);
	ut_check_bool(result, true, "click inactive");
}

/******************************************************************************
 * The function checks the s_dices_get_value() calls.
 *****************************************************************************/

static void test_s_dices_get_value() {
	s_dices dices;
	int result;

	dices.dice[0].value = 1;
	dices.dice[1].value = 2;

	dices.dice[0].status = E_DICE_ACTIVE;
	dices.dice[1].status = E_DICE_INACTIVE;

	result = s_dices_get_value(&dices);
	ut_check_int(result, 1, "1");

	dices.dice[0].status = E_DICE_INACTIVE;
	dices.dice[1].status = E_DICE_ACTIVE;

	result = s_dices_get_value(&dices);
	ut_check_int(result, 2, "2");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_s_dices_exec() {

	test_s_dices_processed();

	test_s_dice_toggle_active();

	test_s_dices_get_value();

}

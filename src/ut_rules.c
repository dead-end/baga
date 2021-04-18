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

#include "s_fieldset.h"
#include "bg_defs.h"
#include "ut_utils.h"
#include "s_status.h"
#include "rules.h"

/******************************************************************************
 * The macro initializes the fieldset and the status.
 *****************************************************************************/

#define init_fieldset_status(f,s,o) s_fieldset_init(f); (s).turn = (o)

#define init_fielset_status_phase(f,s,o,p) s_fieldset_init(f); (s).turn = (o) ; (s).player_phase[o] = (p)

/******************************************************************************
 * The function is a helper function for the test_rules_update_phase() test.
 *****************************************************************************/

static void check_rules_update_phase(s_status *status, const s_fieldset *fieldset, e_player_phase player_phase_exp, const char *msg) {

	//
	// Update the phase in the status
	//
	rules_update_phase(status, fieldset);

	//
	// Check the result
	//
	ut_check_int(status->player_phase[status->turn], player_phase_exp, msg);
}

/******************************************************************************
 * The function tests the rules_update_phase() function.
 *****************************************************************************/

static void test_rules_update_phase() {
	s_fieldset fieldset;
	s_status status;

	//
	// Check: win
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_bear_off(&fieldset, status.turn, CHECKER_NUM);
	check_rules_update_phase(&status, &fieldset, E_PHASE_WIN, "win - top");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_bear_off(&fieldset, status.turn, CHECKER_NUM);
	check_rules_update_phase(&status, &fieldset, E_PHASE_WIN, "win - bottom");

	//
	// Check: bar
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_bear_off(&fieldset, status.turn, CHECKER_NUM - 1);
	s_fieldset_set_bar(&fieldset, status.turn, 1);
	check_rules_update_phase(&status, &fieldset, E_PHASE_BAR, "bar - top");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_bear_off(&fieldset, status.turn, CHECKER_NUM - 1);
	s_fieldset_set_bar(&fieldset, status.turn, 1);
	check_rules_update_phase(&status, &fieldset, E_PHASE_BAR, "bar - bottom");

	//
	// Check: bear-off
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, 20, CHECKER_NUM);
	check_rules_update_phase(&status, &fieldset, E_PHASE_BEAR_OFF, "bear-off - top");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_point_rel(&fieldset, status.turn, 20, CHECKER_NUM);
	check_rules_update_phase(&status, &fieldset, E_PHASE_BEAR_OFF, "bear-off - bottom");

	//
	// Check: normal
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, 20, CHECKER_NUM - 1);
	s_fieldset_set_point_rel(&fieldset, status.turn, 10, 1);
	check_rules_update_phase(&status, &fieldset, E_PHASE_NORMAL, "normal - top");

	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, 20, CHECKER_NUM - 1);
	s_fieldset_set_point_rel(&fieldset, status.turn, 10, 1);
	check_rules_update_phase(&status, &fieldset, E_PHASE_NORMAL, "normal - bottom");
}

/******************************************************************************
 * The function tests the rules_min_rel_idx() function.
 *****************************************************************************/

static void test_rules_min_rel_idx() {
	s_fieldset fieldset;
	s_status status;

	//
	// Check: 0
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, 0, 1);
	ut_check_int(rules_min_rel_idx(&status, &fieldset), 0, "top 0");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_point_rel(&fieldset, status.turn, 0, 1);
	ut_check_int(rules_min_rel_idx(&status, &fieldset), 0, "bottom 0");

	//
	// Check: 10
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, 10, 1);
	ut_check_int(rules_min_rel_idx(&status, &fieldset), 10, "top 10");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_point_rel(&fieldset, status.turn, 10, 1);
	ut_check_int(rules_min_rel_idx(&status, &fieldset), 10, "bottom 10");

	//
	// Check: 23
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, POINTS_NUM - 1, 1);
	ut_check_int(rules_min_rel_idx(&status, &fieldset), POINTS_NUM - 1, "top 23");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_point_rel(&fieldset, status.turn, POINTS_NUM - 1, 1);
	ut_check_int(rules_min_rel_idx(&status, &fieldset), POINTS_NUM - 1, "bottom 23");
}

/******************************************************************************
 * The function checks the members of a s_field.
 *****************************************************************************/

static void ut_check_field(const s_field *field, const e_field_type type, const int idx, const char *msg) {

	ut_check_bool(field != NULL, true, msg);
	ut_check_int(field->id.type, type, msg);
	ut_check_int(field->id.idx, idx, msg);
}

/******************************************************************************
 * The function tests the rules_get_field_src() function.
 *****************************************************************************/

static void test_rules_get_field_src() {
	s_fieldset fieldset;
	s_status status;
	s_field *field;

	//
	// Point ok
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set_point(&fieldset, E_OWNER_TOP, 5, 1);
	field = rules_get_field_src(&status, &fieldset, field->id);

	ut_check_field(field, E_FIELD_POINTS, 5, "Point OK");

	//
	// Point not set
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set_point(&fieldset, E_OWNER_TOP, 10, 0);
	field = rules_get_field_src(&status, &fieldset, field->id);

	ut_check_bool(field == NULL, true, "Not set");

	//
	// Point wrong owner
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set_point(&fieldset, E_OWNER_BOT, 15, 1);
	field = rules_get_field_src(&status, &fieldset, field->id);

	ut_check_bool(field == NULL, true, "Wrong owner");

	//
	// Bar ok
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_BAR);

	field = s_fieldset_set_bar(&fieldset, E_OWNER_TOP, 1);
	field = rules_get_field_src(&status, &fieldset, field->id);

	ut_check_field(field, E_FIELD_BAR, E_OWNER_TOP, "Bar");

	//
	// Bar wrong owner
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_BAR);

	field = s_fieldset_set_bar(&fieldset, E_OWNER_BOT, 1);
	field = rules_get_field_src(&status, &fieldset, field->id);

	ut_check_bool(field == NULL, true, "Bar wrong owner");

	//
	// Bar wrong phase
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set_bar(&fieldset, E_OWNER_TOP, 1);
	field = rules_get_field_src(&status, &fieldset, field->id);

	ut_check_bool(field == NULL, true, "Bar wrong phase");

	//
	// Bear-off own
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set_bear_off(&fieldset, E_OWNER_TOP, 1);
	field = rules_get_field_src(&status, &fieldset, field->id);

	ut_check_bool(field == NULL, true, "Bear-off own");

	//
	// Bear-off wrong
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set_bear_off(&fieldset, E_OWNER_BOT, 1);
	field = rules_get_field_src(&status, &fieldset, field->id);

	ut_check_bool(field == NULL, true, "Bear-off wrong");

	//
	// Bear-off phase
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_BEAR_OFF);

	field = s_fieldset_set_bear_off(&fieldset, E_OWNER_TOP, 1);
	field = rules_get_field_src(&status, &fieldset, field->id);

	ut_check_bool(field == NULL, true, "Bear-off phase");
}

/******************************************************************************
 * The function tests a case of the rules_can_mv() function.
 *
 * The source is the bar, so the destination cannot be outside. The main
 * question is whether the destination is occupied or not.
 *****************************************************************************/

static void test_rules_can_mv_bar() {
	s_fieldset fieldset;
	s_status status;
	s_field *field_dst;

	//
	// Set the bear off phase
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_BAR);

	//
	// Set the source
	//
	const s_field *field_src = s_fieldset_set_bar(&fieldset, E_OWNER_TOP, 1);

	//
	// Set the dice
	//
	s_dices_set(&status.dices, 4, 3);

	//
	// target: 0
	//
	s_fieldset_set_point_rel(&fieldset, E_OWNER_BOT, POINTS_NUM - 4, 0);
	field_dst = rules_can_mv(&status, &fieldset, field_src);
	ut_check_bool(field_dst != NULL, true, "From bar - 0");

	//
	// target: 1
	//
	s_fieldset_set_point_rel(&fieldset, E_OWNER_BOT, POINTS_NUM - 4, 1);
	field_dst = rules_can_mv(&status, &fieldset, field_src);
	ut_check_bool(field_dst != NULL, true, "From bar - 1");

	//
	// target: 2
	//
	s_fieldset_set_point_rel(&fieldset, E_OWNER_BOT, POINTS_NUM - 4, 2);
	field_dst = rules_can_mv(&status, &fieldset, field_src);
	ut_check_bool(field_dst != NULL, false, "From bar - 2");
}

/******************************************************************************
 * The function tests a case of the rules_can_mv() function.
 *
 * The destination is exact outside, but the phase is not bear off.
 *****************************************************************************/

static void test_rules_can_mv_not_bear_off() {
	s_fieldset fieldset;
	s_status status;
	s_field *field_dst;

	//
	// Set the bear off phase
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	//
	// Set the source
	//
	const s_field *field_src = s_fieldset_set_point_rel(&fieldset, E_OWNER_TOP, POINTS_NUM - 3, 2);

	//
	// CASE: Exact out but not bear off phase
	//
	s_dices_set(&status.dices, 3, 3);
	field_dst = rules_can_mv(&status, &fieldset, field_src);
	ut_check_bool(field_dst == NULL, true, "Exact out but not bear off phase");
}

/******************************************************************************
 * The function tests a case of the rules_can_mv() function.
 *
 * The function contains tests for the bear off phase.
 *****************************************************************************/

static void test_rules_can_mv_out() {
	s_fieldset fieldset;
	s_status status;
	s_field *field_dst;

	//
	// Set the bear off phase
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_BEAR_OFF);

	//
	// Set the last checker
	//
	s_fieldset_set_point_rel(&fieldset, E_OWNER_TOP, POINTS_NUM - 5, 2);

	//
	// Set the source after the last checker
	//
	const s_field *field_src = s_fieldset_set_point_rel(&fieldset, E_OWNER_TOP, POINTS_NUM - 3, 2);

	//
	// CASE: Exact Outside and bear off
	//
	s_dices_set(&status.dices, 3, 3);
	field_dst = rules_can_mv(&status, &fieldset, field_src);
	ut_check_bool(field_dst != NULL, true, "Exact Outside and bear off");

	//
	// CASE: Last is far out and bear off
	//
	s_dices_set(&status.dices, 6, 3);
	field_dst = rules_can_mv(&status, &fieldset, field_src);
	ut_check_bool(field_dst != NULL, true, "Far outside and bear off");

	//
	// CASE: Far outside and bear off but not last
	//
	s_dices_set(&status.dices, 4, 3);
	field_dst = rules_can_mv(&status, &fieldset, field_src);
	ut_check_bool(field_dst == NULL, true, "Far outside and bear off but not last");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_rules_exec() {

	test_rules_update_phase();

	test_rules_min_rel_idx();

	test_rules_get_field_src();

	test_rules_can_mv_bar();

	test_rules_can_mv_not_bear_off();

	test_rules_can_mv_out();
}

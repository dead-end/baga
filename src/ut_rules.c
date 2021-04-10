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

static void check_rules_update_phase(const s_fieldset *fieldset, s_status *status, e_player_phase player_phase_exp, const char *msg) {

	//
	// Update the phase in the status
	//
	rules_update_phase(fieldset, status);

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
	check_rules_update_phase(&fieldset, &status, E_PHASE_WIN, "win - top");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_bear_off(&fieldset, status.turn, CHECKER_NUM);
	check_rules_update_phase(&fieldset, &status, E_PHASE_WIN, "win - bottom");

	//
	// Check: bar
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_bear_off(&fieldset, status.turn, CHECKER_NUM - 1);
	s_fieldset_set_bar(&fieldset, status.turn, 1);
	check_rules_update_phase(&fieldset, &status, E_PHASE_BAR, "bar - top");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_bear_off(&fieldset, status.turn, CHECKER_NUM - 1);
	s_fieldset_set_bar(&fieldset, status.turn, 1);
	check_rules_update_phase(&fieldset, &status, E_PHASE_BAR, "bar - bottom");

	//
	// Check: bear-off
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, 20, CHECKER_NUM);
	check_rules_update_phase(&fieldset, &status, E_PHASE_BEAR_OFF, "bear-off - top");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_point_rel(&fieldset, status.turn, 20, CHECKER_NUM);
	check_rules_update_phase(&fieldset, &status, E_PHASE_BEAR_OFF, "bear-off - bottom");

	//
	// Check: normal
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, 20, CHECKER_NUM - 1);
	s_fieldset_set_point_rel(&fieldset, status.turn, 10, 1);
	check_rules_update_phase(&fieldset, &status, E_PHASE_NORMAL, "normal - top");

	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, 20, CHECKER_NUM - 1);
	s_fieldset_set_point_rel(&fieldset, status.turn, 10, 1);
	check_rules_update_phase(&fieldset, &status, E_PHASE_NORMAL, "normal - bottom");
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
	ut_check_int(rules_min_rel_idx(&fieldset, &status), 0, "top 0");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_point_rel(&fieldset, status.turn, 0, 1);
	ut_check_int(rules_min_rel_idx(&fieldset, &status), 0, "bottom 0");

	//
	// Check: 10
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, 10, 1);
	ut_check_int(rules_min_rel_idx(&fieldset, &status), 10, "top 10");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_point_rel(&fieldset, status.turn, 10, 1);
	ut_check_int(rules_min_rel_idx(&fieldset, &status), 10, "bottom 10");

	//
	// Check: 23
	//
	init_fieldset_status(&fieldset, status, E_OWNER_TOP);
	s_fieldset_set_point_rel(&fieldset, status.turn, POINTS_NUM - 1, 1);
	ut_check_int(rules_min_rel_idx(&fieldset, &status), POINTS_NUM - 1, "top 23");

	init_fieldset_status(&fieldset, status, E_OWNER_BOT);
	s_fieldset_set_point_rel(&fieldset, status.turn, POINTS_NUM - 1, 1);
	ut_check_int(rules_min_rel_idx(&fieldset, &status), POINTS_NUM - 1, "bottom 23");
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

	field = s_fieldset_set(&fieldset, E_FIELD_POINTS, 5, E_OWNER_TOP, 1);
	field = rules_get_field_src(&fieldset, &status, field->id);

	ut_check_field(field, E_FIELD_POINTS, 5, "Point OK");

	//
	// Point not set
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set(&fieldset, E_FIELD_POINTS, 10, E_OWNER_TOP, 0);
	field = rules_get_field_src(&fieldset, &status, field->id);

	ut_check_bool(field == NULL, true, "Not set");

	//
	// Point wrong owner
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set(&fieldset, E_FIELD_POINTS, 15, E_OWNER_BOT, 1);
	field = rules_get_field_src(&fieldset, &status, field->id);

	ut_check_bool(field == NULL, true, "Wrong owner");

	//
	// Bar ok
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_BAR);

	field = s_fieldset_set(&fieldset, E_FIELD_BAR, E_OWNER_TOP, E_OWNER_TOP, 1);
	field = rules_get_field_src(&fieldset, &status, field->id);

	ut_check_field(field, E_FIELD_BAR, E_OWNER_TOP, "Bar");

	//
	// Bar wrong owner
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_BAR);

	field = s_fieldset_set(&fieldset, E_FIELD_BAR, E_OWNER_BOT, E_OWNER_BOT, 1);
	field = rules_get_field_src(&fieldset, &status, field->id);

	ut_check_bool(field == NULL, true, "Bar wrong owner");

	//
	// Bar wrong phase
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set(&fieldset, E_FIELD_BAR, E_OWNER_TOP, E_OWNER_TOP, 1);
	field = rules_get_field_src(&fieldset, &status, field->id);

	ut_check_bool(field == NULL, true, "Bar wrong phase");

	//
	// Bear-off own
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set(&fieldset, E_PHASE_BEAR_OFF, E_OWNER_TOP, E_OWNER_TOP, 1);
	field = rules_get_field_src(&fieldset, &status, field->id);

	ut_check_bool(field == NULL, true, "Bear-off own");

	//
	// Bear-off wrong
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_NORMAL);

	field = s_fieldset_set(&fieldset, E_PHASE_BEAR_OFF, E_OWNER_BOT, E_OWNER_BOT, 1);
	field = rules_get_field_src(&fieldset, &status, field->id);

	ut_check_bool(field == NULL, true, "Bear-off wrong");

	//
	// Bear-off phase
	//
	init_fielset_status_phase(&fieldset, status, E_OWNER_TOP, E_PHASE_BEAR_OFF);

	field = s_fieldset_set(&fieldset, E_PHASE_BEAR_OFF, E_OWNER_TOP, E_OWNER_TOP, 1);
	field = rules_get_field_src(&fieldset, &status, field->id);

	ut_check_bool(field == NULL, true, "Bear-off own");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_rules_exec() {

	test_rules_update_phase();

	test_rules_min_rel_idx();

	test_rules_get_field_src();
}

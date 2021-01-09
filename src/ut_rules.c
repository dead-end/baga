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

#include "bg_defs.h"
#include "ut_utils.h"
#include "s_game.h"
#include "s_status.h"
#include "rules.h"

/******************************************************************************
 * The macro initializes the game and the status.
 *****************************************************************************/

#define init_game_status(g,s,o) s_game_init(g); (s).turn = (o)

/******************************************************************************
 * The function is a helper function for the test_rules_update_phase() test.
 *****************************************************************************/

static void check_rules_update_phase(const s_game *game, s_status *status, e_player_phase player_phase_exp, const char *msg) {

	//
	// Update the phase in the status
	//
	rules_update_phase(game, status);

	//
	// Check the result
	//
	ut_check_int(status->player_phase[status->turn], player_phase_exp, msg);
}

/******************************************************************************
 * The function tests the rules_update_phase() function.
 *****************************************************************************/

static void test_rules_update_phase() {
	s_game game;
	s_status status;

	//
	// Check: win
	//
	init_game_status(&game, status, E_OWNER_TOP);
	s_game_set_bear_off(&game, status.turn, CHECKER_NUM);
	check_rules_update_phase(&game, &status, E_PHASE_WIN, "win - top");

	init_game_status(&game, status, E_OWNER_BOT);
	s_game_set_bear_off(&game, status.turn, CHECKER_NUM);
	check_rules_update_phase(&game, &status, E_PHASE_WIN, "win - bottom");

	//
	// Check: bar
	//
	init_game_status(&game, status, E_OWNER_TOP);
	s_game_set_bear_off(&game, status.turn, CHECKER_NUM -1);
	s_game_set_bar(&game, status.turn, 1);
	check_rules_update_phase(&game, &status, E_PHASE_BAR, "bar - top");

	init_game_status(&game, status, E_OWNER_BOT);
	s_game_set_bear_off(&game, status.turn, CHECKER_NUM -1);
	s_game_set_bar(&game, status.turn, 1);
	check_rules_update_phase(&game, &status, E_PHASE_BAR, "bar - bottom");

	//
	// Check: bear-off
	//
	init_game_status(&game, status, E_OWNER_TOP);
	s_game_set_point_rel(&game, status.turn, 20, CHECKER_NUM);
	check_rules_update_phase(&game, &status, E_PHASE_BEAR_OFF, "bear-off - top");

	init_game_status(&game, status, E_OWNER_BOT);
	s_game_set_point_rel(&game, status.turn, 20, CHECKER_NUM);
	check_rules_update_phase(&game, &status, E_PHASE_BEAR_OFF, "bear-off - bottom");

	//
	// Check: normal
	//
	init_game_status(&game, status, E_OWNER_TOP);
	s_game_set_point_rel(&game, status.turn, 20, CHECKER_NUM - 1);
	s_game_set_point_rel(&game, status.turn, 10, 1);
	check_rules_update_phase(&game, &status, E_PHASE_NORMAL, "normal - top");

	init_game_status(&game, status, E_OWNER_TOP);
	s_game_set_point_rel(&game, status.turn, 20, CHECKER_NUM - 1);
	s_game_set_point_rel(&game, status.turn, 10, 1);
	check_rules_update_phase(&game, &status, E_PHASE_NORMAL, "normal - bottom");
}

/******************************************************************************
 * The function tests the rules_min_rel_idx() function.
 *****************************************************************************/

static void test_rules_min_rel_idx() {
	s_game game;
	s_status status;

	//
	// Check: 0
	//
	init_game_status(&game, status, E_OWNER_TOP);
	s_game_set_point_rel(&game, status.turn, 0, 1);
	ut_check_int(rules_min_rel_idx(&game, &status), 0, "top 0");

	init_game_status(&game, status, E_OWNER_BOT);
	s_game_set_point_rel(&game, status.turn, 0, 1);
	ut_check_int(rules_min_rel_idx(&game, &status), 0, "bottom 0");

	//
	// Check: 10
	//
	init_game_status(&game, status, E_OWNER_TOP);
	s_game_set_point_rel(&game, status.turn, 10, 1);
	ut_check_int(rules_min_rel_idx(&game, &status), 10, "top 10");

	init_game_status(&game, status, E_OWNER_BOT);
	s_game_set_point_rel(&game, status.turn, 10, 1);
	ut_check_int(rules_min_rel_idx(&game, &status), 10, "bottom 10");

	//
	// Check: 23
	//
	init_game_status(&game, status, E_OWNER_TOP);
	s_game_set_point_rel(&game, status.turn, POINTS_NUM - 1, 1);
	ut_check_int(rules_min_rel_idx(&game, &status), POINTS_NUM - 1, "top 23");

	init_game_status(&game, status, E_OWNER_BOT);
	s_game_set_point_rel(&game, status.turn, POINTS_NUM - 1, 1);
	ut_check_int(rules_min_rel_idx(&game, &status), POINTS_NUM - 1, "bottom 23");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_rules_exec() {

	test_rules_update_phase();

	test_rules_min_rel_idx();
}

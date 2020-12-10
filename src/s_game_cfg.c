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

#include "s_game_cfg.h"

/******************************************************************************
 * The function initializes the game configurations.
 *
 * TODO: read from file
 *****************************************************************************/

void s_game_cfg_init(s_game_cfg *game_cfg) {

	//
	// Color: board border and bear off reversed
	//
	game_cfg->clr_board_start = "#391313";

	game_cfg->clr_board_end = "#602020";

	//
	// Color: inner / outer board
	//
	game_cfg->clr_outer_inner_start = "#ffe0b3";

	game_cfg->clr_outer_inner_end = "#ffb84d";

	//
	// Color: points
	//
	game_cfg->clr_points_black_start = "#4d2600";

	game_cfg->clr_points_black_end = "#663300";

	game_cfg->clr_points_white_start = "#cc6600";

	game_cfg->clr_points_white_end = "#ff8c1a";

	//
	// Color: checker
	//
	game_cfg->clr_checker_black_start = "#1a1a1a";

	game_cfg->clr_checker_black_end = "#666666";

	game_cfg->clr_checker_white_start = "#f2f2f2";

	game_cfg->clr_checker_white_end = "#737373";

	//
	// Color: dices
	//
	game_cfg->clr_dice_black_active = "#0f0f0f";

	game_cfg->clr_dice_black_inactive = "#1f1f1f";

	game_cfg->clr_dice_white_active = "#e0e0e0";

	game_cfg->clr_dice_white_inactive = "#b0b0b0";

}

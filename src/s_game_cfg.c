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
	game_cfg->clr_board_start = "#22222f";

	game_cfg->clr_board_end = "#55555f";

	//
	// Color: inner / outer board
	//
	game_cfg->clr_outer_inner_start = "#88888f";

	game_cfg->clr_outer_inner_end = "#cccccf";

	//
	// Color: points
	//
	game_cfg->clr_points_black_start = "#ffffff";

	game_cfg->clr_points_black_end = "#cccccc";

	game_cfg->clr_points_white_start = "#555555";

	game_cfg->clr_points_white_end = "#222222";

	//
	// Color: checker
	//
	game_cfg->clr_checker_black_start = "#8a8a5c";

	game_cfg->clr_checker_black_end = "#0f0f0a";

	game_cfg->clr_checker_white_start = "#ac3939";

	game_cfg->clr_checker_white_end = "#130606";
}

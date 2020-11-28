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

#ifndef INC_S_GAME_CFG_H_
#define INC_S_GAME_CFG_H_

/******************************************************************************
 * The definition of various configurations of the game.
 *****************************************************************************/

typedef struct s_game_cfg {

	//
	// Color: board border and bear off reversed
	//
	char *clr_board_start;

	char *clr_board_end;

	//
	// Color: inner / outer board background
	//
	char *clr_outer_inner_start;

	char *clr_outer_inner_end;

	//
	// Color: points
	//
	char *clr_points_black_start;

	char *clr_points_black_end;

	char *clr_points_white_start;

	char *clr_points_white_end;

	//
	// Color: checker
	//
	char *clr_checker_black_start;

	char *clr_checker_black_end;

	char *clr_checker_white_start;

	char *clr_checker_white_end;

} s_game_cfg;

/******************************************************************************
 * Definition of functions and macros.
 *****************************************************************************/

void s_game_cfg_init(s_game_cfg *game_cfg);

#endif /* INC_S_GAME_CFG_H_ */

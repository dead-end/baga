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

#include "bg_board.h"
#include "nc_board.h"

#include "lib_logging.h"

/******************************************************************************
 * The definition of the index for black and white. White is playing from upper
 * right to lower right and black is playing in reverse direction.
 *****************************************************************************/

#define idx_white(i) (i)

#define idx_black(i) reverse_idx(POINTS_NUM,i)

/******************************************************************************
 * The function initializes the bg board struct. No checker are placed.
 *****************************************************************************/

void bg_board_init(s_bg_board *bg_board) {

	//
	// Initialize the points
	//
	for (int i = 0; i < POINTS_NUM; i++) {
		bg_board->bg_point[i].num = 0;
		bg_board->bg_point[i].owner = OWNER_NONE;
	}

	bg_board->bear_off[OWNER_BLACK] = 0;
	bg_board->bear_off[OWNER_WHITE] = 0;

	bg_board->reenter[OWNER_BLACK] = 0;
	bg_board->reenter[OWNER_WHITE] = 0;
}

/******************************************************************************
 * The function resets the bg board struct and sets the checker for the new
 * game.
 *****************************************************************************/

void bg_board_new_game(s_bg_board *bg_board) {

	bg_board_init(bg_board);

	//
	// Index 0
	//
	bg_board->bg_point[idx_white(0)].num = 2;
	bg_board->bg_point[idx_white(0)].owner = OWNER_WHITE;

	bg_board->bg_point[idx_black(0)].num = 2;
	bg_board->bg_point[idx_black(0)].owner = OWNER_BLACK;

	//
	// Index 5
	//
	bg_board->bg_point[idx_white(5)].num = 5;
	bg_board->bg_point[idx_white(5)].owner = OWNER_WHITE;

	bg_board->bg_point[idx_black(5)].num = 5;
	bg_board->bg_point[idx_black(5)].owner = OWNER_BLACK;

	//
	// Index 7
	//
	bg_board->bg_point[idx_white(7)].num = 5;
	bg_board->bg_point[idx_white(7)].owner = OWNER_WHITE;

	bg_board->bg_point[idx_black(7)].num = 5;
	bg_board->bg_point[idx_black(7)].owner = OWNER_BLACK;

	//
	// Index 11
	//
	bg_board->bg_point[idx_white(11)].num = 5;
	bg_board->bg_point[idx_white(11)].owner = OWNER_WHITE;

	bg_board->bg_point[idx_black(11)].num = 5;
	bg_board->bg_point[idx_black(11)].owner = OWNER_BLACK;
}

/******************************************************************************
 * The function prints the checkers to the board.
 *****************************************************************************/

void bg_board_print(s_bg_board *bg_board) {

	for (int i = 0; i < POINTS_NUM; i++) {

		if (bg_board->bg_point[i].num != 0) {

			s_board_points_add_checkers(i, bg_board->bg_point[i].owner, bg_board->bg_point[i].num, E_UNCOMP);
		}
	}
}

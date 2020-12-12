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

#include "lib_s_point.h"
#include "lib_curses.h"
#include "lib_utils.h"
#include "lib_logging.h"

/******************************************************************************
 * The layout has 3 windows. One window for the board and one window for the
 * dice. These are derived windows. The containing window is the game window.
 *****************************************************************************/

static WINDOW *_win_game = NULL;

static WINDOW *_win_board = NULL;

static WINDOW *_win_dice = NULL;

/******************************************************************************
 *
 *****************************************************************************/

#define W_BORDER_ROW 2

/******************************************************************************
 *
 *****************************************************************************/
// TODO:
void layout_init(const s_point dim_board, const s_point dim_dice) {

	const s_point total = { .row = dim_board.row + dim_dice.row + W_BORDER_ROW, .col = lu_max(dim_board.col, dim_dice.col) };

	const s_point pos = { .row = lu_center(getmaxy(stdscr), total.row), .col = lu_center(getmaxx(stdscr), total.col) };

	// TODO: ensure position is not negative
	if ((_win_game = newwin(total.row, total.col, pos.row, pos.col)) == NULL) {
		log_exit("Unable to create win dim: %d/%d pos: %d/%d", total.row, total.col, pos.row, pos.col);
	}

	if ((_win_board = derwin(_win_game, dim_board.row, dim_board.col, 0, 0)) == NULL) {
		log_exit_str("Unable to create window!");
	}

	log_debug("win board: %d/%d", dim_board.row, dim_board.col);

	if ((_win_dice = derwin(_win_game, dim_dice.row, dim_dice.col, dim_board.row + W_BORDER_ROW, lu_center(total.col, dim_dice.col))) == NULL) {
		log_exit_str("Unable to create window!");
	}

	log_debug("win dice: %d/%d", dim_dice.row, dim_dice.col);
}

/******************************************************************************
 * The function frees the allocated windows.
 *****************************************************************************/

void layout_free() {

	lc_win_del(_win_dice);

	lc_win_del(_win_board);

	lc_win_del(_win_game);
}

/******************************************************************************
 * Simple getter function.
 *****************************************************************************/

WINDOW* layout_win_board() {
	return _win_board;
}

/******************************************************************************
 * Simple getter function.
 *****************************************************************************/

WINDOW* layout_win_dice() {
	return _win_dice;
}

/******************************************************************************
 * Simple getter function.
 *****************************************************************************/
// TODO: necessary?
WINDOW* layout_win_game() {
	return _win_game;
}

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

#include <ncurses.h>

#include "lib_color_pair.h"

#include "nc_board.h"
#include "s_color_def.h"

/******************************************************************************
 *
 *****************************************************************************/

/******************************************************************************
 * The function sets the board with a default color and a foreground color and
 * a background color gradient.
 *****************************************************************************/

static void nc_board_init_gradient(s_nc_board *board, const wchar_t chr, const short fg_color, const short *bg_colors) {

	for (int row = 0; row < BOARD_ROW; row++) {
		for (int col = 0; col < BOARD_COL; col++) {

			board->arr[row][col].chr = chr;
			board->arr[row][col].fg = fg_color;
			board->arr[row][col].bg = bg_colors[row];
		}
	}
}

/******************************************************************************
 * The function sets the background color with a gradient for a given area.
 *****************************************************************************/

static void nc_board_set_area_bg(s_nc_board *board, const s_area *area_board, const short *bg_colors) {

	//
	// Store the position of the area.
	//
	const int pos_row = area_board->pos.row;
	const int pos_col = area_board->pos.col;

	for (int row = 0; row < area_board->dim.row; row++) {
		for (int col = 0; col < area_board->dim.col; col++) {

			board->arr[pos_row + row][pos_col + col].bg = bg_colors[row];
		}
	}
}

/******************************************************************************
 * The function sets the board with a default character and a foreground and
 * background color.
 *****************************************************************************/

void nc_board_set_tchar(s_nc_board *board, const s_tchar tchar) {

	for (int row = 0; row < BOARD_ROW; row++) {
		for (int col = 0; col < BOARD_COL; col++) {

			board->arr[row][col] = tchar;
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_init_bg(s_nc_board *board_bg, const s_area *area_board_outer, const s_area *area_board_inner) {

	//
	// Complete board
	//
	short color_bar_bg[BOARD_ROW];

	s_color_def_gradient(color_bar_bg, BOARD_ROW, "#1a0d00", "#663500");

	nc_board_init_gradient(board_bg, EMPTY, 0, color_bar_bg);

	//
	// Inner / outer board
	//
	short color_board_bg[BOARD_HALF_ROW];

	s_color_def_gradient(color_board_bg, BOARD_HALF_ROW, "#ffe6cc", "#ff9933");

	nc_board_set_area_bg(board_bg, area_board_outer, color_board_bg);

	nc_board_set_area_bg(board_bg, area_board_inner, color_board_bg);
}

/******************************************************************************
 *
 *****************************************************************************/

void nc_board_print(const s_nc_board *board_fg, const s_nc_board *board_bg) {
	short cp;
	wchar_t chr;

	for (int row = 0; row < BOARD_ROW; row++) {
		for (int col = 0; col < BOARD_COL; col++) {

			if (board_fg->arr[row][col].chr == EMPTY) {

				cp = cp_color_pair_get(board_bg->arr[row][col].fg, board_bg->arr[row][col].bg);
				chr = board_bg->arr[row][col].chr;

			} else {
				cp = cp_color_pair_get(board_fg->arr[row][col].fg, board_fg->arr[row][col].bg);
				chr = board_fg->arr[row][col].chr;
			}

			attrset(COLOR_PAIR(cp));

			mvprintw(row, col, "%lc", chr);
		}
	}
}

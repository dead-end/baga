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

#include "nc_board.h"

/******************************************************************************
 * The function sets the board with a default character and a foreground and
 * background color.
 *****************************************************************************/

void s_nc_board_init_empty(s_nc_board *board, const s_tchar tchar) {

	for (int row = 0; row < BOARD_ROW; row++) {
		for (int col = 0; col < BOARD_COL; col++) {

			board->arr[row][col] = tchar;
		}
	}
}

/******************************************************************************
 * The function sets the board with a default color and a foreground color and
 * a background color gradient.
 *****************************************************************************/

void s_nc_board_init_gradient(s_nc_board *board, const wchar_t chr, const short fg_color, const short *bg_colors) {

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

void s_nc_board_set_area_bg(s_nc_board *board, const s_area *area_board, const short *bg_colors) {

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

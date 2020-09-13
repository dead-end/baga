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

#include <locale.h>
#include <ncurses.h>
#include <math.h>

#include <string.h>

#include "lib_logging.h"
#include "lib_color.h"
#include "lib_color_pair.h"
#include "lib_s_tchar.h"
#include "lib_s_point.h"

#include "s_color_def.h"

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory.
 *****************************************************************************/

static void exit_callback() {

	endwin();
}

/******************************************************************************
 * The method initializes the application.
 *****************************************************************************/

static void init() {

	//
	// Set the locale to allow utf8.
	//
	if (setlocale(LC_CTYPE, "") == NULL) {
		log_exit_str("Unable to set the locale.");
	}

	//
	// Register callback for log_exit calls.
	//
	log_atexit(exit_callback);

	if (initscr() == NULL) {
		log_exit_str("Unable to init screen!");
	}

	if (start_color() == ERR) {
		log_exit_str("Unable to start color!");
	}

	curs_set(0);
}

// --------------------------------------------

/******************************************************************************
 * Definition of the characters used by the triangles.
 *****************************************************************************/

//
// Full block
//
#define T_II L'\u2588'

//
// Left half block
//
#define T_I_ L'\u258C'

//
// Right half block
//
#define T__I L'\u2590'

//
// Empty
//
#define T___ L' '

/******************************************************************************
 * Triangles
 *****************************************************************************/

#define TRIANGLE_HIGH 10

#define TRIANGLE_WIDTH 6

#define triangle_reverse(r) (TRIANGLE_HIGH - 1 - (r))

/******************************************************************************
 *
 *****************************************************************************/

typedef struct {

	wchar_t arr[TRIANGLE_HIGH][TRIANGLE_WIDTH];

	s_point dim;

} s_templ;

/******************************************************************************
 *
 *****************************************************************************/

s_templ _tri_tmpl = {

.dim = { .row = TRIANGLE_HIGH, .col = TRIANGLE_WIDTH },

.arr = {

{ T___, T___, T__I, T_I_, T___, T___ },

{ T___, T___, T__I, T_I_, T___, T___ },

{ T___, T___, T_II, T_II, T___, T___ },

{ T___, T___, T_II, T_II, T___, T___ },

{ T___, T__I, T_II, T_II, T_I_, T___ },

{ T___, T__I, T_II, T_II, T_I_, T___ },

{ T___, T_II, T_II, T_II, T_II, T___ },

{ T___, T_II, T_II, T_II, T_II, T___ },

{ T__I, T_II, T_II, T_II, T_II, T_I_ },

{ T__I, T_II, T_II, T_II, T_II, T_I_ },

}

};

/******************************************************************************
 *
 *****************************************************************************/

#define BORDER_ROW 1

#define BORDER_COL 2

#define PIECE_ROW 2

#define PIECE_COL 4

#define HALF_BOARD_WIDTH 36

#define HALF_BOARD_HEIGHT 24

//
// Total rows and columns
//
#define ROWS_TOTAL BORDER_ROW + HALF_BOARD_HEIGHT + BORDER_ROW

#define COLS_TOTAL BORDER_COL + HALF_BOARD_WIDTH + BORDER_COL + PIECE_COL + BORDER_COL + HALF_BOARD_WIDTH + BORDER_COL+ PIECE_COL + BORDER_COL

/******************************************************************************
 * The definition of an area which consists of a position and a dimension.
 *****************************************************************************/

typedef struct {

	s_point pos;

	s_point dim;

} s_area;

/******************************************************************************
 *
 *****************************************************************************/

//
// Outer board
//
s_area _area_board_outer = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL },

.dim = { .row = HALF_BOARD_HEIGHT, .col = HALF_BOARD_WIDTH }

};

//
// Inner bar
//
s_area _area_bar_inner = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL + HALF_BOARD_WIDTH + BORDER_COL },

.dim = { .row = HALF_BOARD_HEIGHT, .col = PIECE_COL }

};

//
// Inner board
//
s_area _area_board_inner = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL + HALF_BOARD_WIDTH + BORDER_COL + PIECE_COL + BORDER_COL },

.dim = { .row = HALF_BOARD_HEIGHT, .col = HALF_BOARD_WIDTH }

};

//
// bear off area (right bar)
//
s_area _area_bar_exit = {

.pos = { .row = BORDER_ROW, .col = BORDER_COL + HALF_BOARD_WIDTH + BORDER_COL + PIECE_COL + BORDER_COL + HALF_BOARD_WIDTH + BORDER_COL },

.dim = { .row = HALF_BOARD_HEIGHT, .col = PIECE_COL }

};

/******************************************************************************
 *
 *****************************************************************************/

#define TRI_NUM 24

#define TRI_NUM_QUARTER 6

s_point tri_pos[TRI_NUM];

void triangle_set_pos(const s_area *area_board_outer, const s_area *area_board_inner) {

	for (int i = 0; i < TRI_NUM_QUARTER; i++) {

		//
		// Upper right triangles
		//
		tri_pos[0 * TRI_NUM_QUARTER + i].row = area_board_inner->pos.row;
		tri_pos[0 * TRI_NUM_QUARTER + i].col = area_board_inner->pos.col + i * TRIANGLE_WIDTH;

		//
		// Upper left triangles
		//
		tri_pos[1 * TRI_NUM_QUARTER + i].row = area_board_outer->pos.row;
		tri_pos[1 * TRI_NUM_QUARTER + i].col = area_board_outer->pos.col + i * TRIANGLE_WIDTH;

		//
		// Lower left triangles
		//
		tri_pos[2 * TRI_NUM_QUARTER + i].row = area_board_outer->pos.row + area_board_outer->dim.row - TRIANGLE_HIGH;
		tri_pos[2 * TRI_NUM_QUARTER + i].col = area_board_outer->pos.col + i * TRIANGLE_WIDTH;

		//
		// Lower right triangles
		//
		tri_pos[3 * TRI_NUM_QUARTER + i].row = area_board_inner->pos.row + area_board_inner->dim.row - TRIANGLE_HIGH;
		tri_pos[3 * TRI_NUM_QUARTER + i].col = area_board_inner->pos.col + i * TRIANGLE_WIDTH;
	}
}

/******************************************************************************
 *
 *****************************************************************************/

typedef struct {

	s_tchar arr[ROWS_TOTAL][COLS_TOTAL];

	s_point dim;

} s_board;

static s_board _board = { .dim = { .row = ROWS_TOTAL, .col = COLS_TOTAL } };

/******************************************************************************
 *
 *****************************************************************************/

static void s_board_init(s_board *board, const wchar_t chr, const short fg_color, const short *bg_colors) {

	for (int row = 0; row < board->dim.row; row++) {
		for (int col = 0; col < board->dim.col; col++) {

			board->arr[row][col].fg = fg_color;
			board->arr[row][col].bg = bg_colors[row];
			board->arr[row][col].chr = chr;
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void s_board_set_area_bg(s_board *board, const s_area *area_board, const short *bg_colors) {

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
 *
 *****************************************************************************/

static void s_board_print(const s_board *board) {
	short cp;

	for (int row = 0; row < board->dim.row; row++) {
		for (int col = 0; col < board->dim.col; col++) {

			cp = cp_color_pair_get(board->arr[row][col].fg, board->arr[row][col].bg);
			attrset(COLOR_PAIR(cp));

			mvprintw(row, col, "%lc", board->arr[row][col].chr);
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void s_board_add_templ(s_board *board, const s_templ *tri_templ, const bool reverse, const s_point *pos, const short *fg_colors) {

	s_tchar *tchr;

	for (int row = 0; row < tri_templ->dim.row; row++) {
		for (int col = 0; col < tri_templ->dim.col; col++) {

			tchr = &board->arr[pos->row + row][pos->col + col];

			if (reverse) {
				tchr->chr = tri_templ->arr[triangle_reverse(row)][col];
			} else {
				tchr->chr = tri_templ->arr[row][col];
			}

			tchr->fg = fg_colors[row];
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void s_board_triangle_add(s_board *board, const s_templ *tri_templ, const s_point *tri_pos, const short *tri_light, const short *tri_dark) {

	for (int i = 0; i < TRI_NUM; i++) {

		s_board_add_templ(board, tri_templ, (i < 12), &tri_pos[i], (i % 2) ? tri_light : tri_dark);
	}
}

/******************************************************************************
 * The main function.
 *****************************************************************************/

int main() {
	log_debug_str("Starting baga...");

	init();

	short color_board_bg[HALF_BOARD_HEIGHT];
	short color_tri_light[TRIANGLE_HIGH];
	short color_tri_dark[TRIANGLE_HIGH];
	short color_bar_bg[ROWS_TOTAL];

	//
	// Board
	//
	s_color_def_gradient(color_board_bg, HALF_BOARD_HEIGHT, "#ffe6cc", "#ff9933");

	//
	// Light and dark triangle
	//
	s_color_def_gradient(color_tri_light, TRIANGLE_HIGH, "#ff8000", "#cc6600");

	s_color_def_gradient(color_tri_dark, TRIANGLE_HIGH, "#804000", "#4d2800");

	//
	// Bar
	//
	s_color_def_gradient(color_bar_bg, ROWS_TOTAL, "#1a0d00", "#663500");

	//
	// Initialize the board
	//
	s_board_init(&_board, T___, 0, color_bar_bg);

	s_board_set_area_bg(&_board, &_area_board_outer, color_board_bg);

	s_board_set_area_bg(&_board, &_area_board_inner, color_board_bg);

	triangle_set_pos(&_area_board_outer, &_area_board_inner);

	s_board_triangle_add(&_board, &_tri_tmpl, tri_pos, color_tri_light, color_tri_dark);

	//
	// Print the initialized board
	//
	s_board_print(&_board);

	getch();

	log_debug_str("Ending baga...");
}

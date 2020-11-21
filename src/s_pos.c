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

#include "lib_logging.h"
#include "bg_defs.h"
#include "s_pos.h"

/******************************************************************************
 * Definitions of the various areas of the board. An area has a position and a
 * dimension. The main board has the following four areas:
 *
 * outer | inner | inner | bear
 * board | bar   | board | off
 *****************************************************************************/

#define BORDER_ROW 1

#define BORDER_COL 2

s_board_areas _board_areas;

/******************************************************************************
 * The positions of the point, bar bear off areas. This is the upper left
 * corner for upper areas and the lower left corner of lower areas.
 *****************************************************************************/

static s_pos _pos_points[POINTS_NUM];

static s_pos _pos_bar[NUM_PLAYER];

static s_pos _pos_bear_off[NUM_PLAYER];

/******************************************************************************
 * The function sets the positions of the checkers on the bear off areas.
 *****************************************************************************/

static void s_pos_set_bear_off(s_pos *pos_bear_off, const s_area *area_bear_off) {

	pos_bear_off[0].pos.row = area_bear_off->pos.row;
	pos_bear_off[0].pos.col = area_bear_off->pos.col;
	pos_bear_off[0].is_upper = true;

	pos_bear_off[1].pos.row = area_bear_off->pos.row + area_bear_off->dim.row - 1;
	pos_bear_off[1].pos.col = area_bear_off->pos.col;
	pos_bear_off[1].is_upper = false;
}

/******************************************************************************
 * The function sets the positions of the checkers on the bar areas.
 *****************************************************************************/

static void s_pos_set_bar(s_pos *pos_bar, const s_area *area_bar) {

	pos_bar[0].pos.row = area_bar->pos.row;
	pos_bar[0].pos.col = area_bar->pos.col;
	pos_bar[0].is_upper = true;

	pos_bar[1].pos.row = area_bar->pos.row + area_bar->dim.row - 1;
	pos_bar[1].pos.col = area_bar->pos.col;
	pos_bar[1].is_upper = false;
}

/******************************************************************************
 * The function sets the positions of the checkers on the points.
 *****************************************************************************/

static void s_pos_set_points(s_pos *pos_points, const s_area *area_board_outer, const s_area *area_board_inner) {

	const int quarter = POINTS_NUM / 4;

	log_debug("inner board - pos: %d/%d dim: %d/%d", area_board_inner->pos.row, area_board_inner->pos.col, area_board_inner->dim.row, area_board_inner->dim.col);

	const int lower_inner_row = area_board_inner->pos.row + area_board_inner->dim.row - 1;
	const int lower_outer_row = area_board_outer->pos.row + area_board_outer->dim.row - 1;

	for (int i = 0; i < quarter; i++) {

		//
		// Upper right triangles
		//
		pos_points[0 * quarter + i].pos.row = area_board_inner->pos.row;
		pos_points[0 * quarter + i].pos.col = area_board_inner->pos.col + reverse_idx(quarter, i) * POINTS_COL;
		pos_points[0 * quarter + i].is_upper = true;

		//
		// Upper left triangles
		//
		pos_points[1 * quarter + i].pos.row = area_board_outer->pos.row;
		pos_points[1 * quarter + i].pos.col = area_board_outer->pos.col + reverse_idx(quarter, i) * POINTS_COL;
		pos_points[1 * quarter + i].is_upper = true;

		//
		// Lower left triangles
		//
		pos_points[2 * quarter + i].pos.row = lower_outer_row;
		pos_points[2 * quarter + i].pos.col = area_board_outer->pos.col + i * POINTS_COL;
		pos_points[2 * quarter + i].is_upper = false;

		//
		// Lower right triangles
		//
		pos_points[3 * quarter + i].pos.row = lower_inner_row;
		pos_points[3 * quarter + i].pos.col = area_board_inner->pos.col + i * POINTS_COL;
		pos_points[3 * quarter + i].is_upper = false;
	}
}

/******************************************************************************
 * The function initializes the different areas of the board.
 *****************************************************************************/

static void s_pos_areas_init(s_board_areas *board_areas) {

	const int board_half_row = (2 * POINTS_ROW + CHECKER_ROW + 2 * BORDER_ROW);
	const int board_half_col = (6 * POINTS_COL);

	//
	// area: outer board
	//
	board_areas->board_outer.dim.row = board_half_row;
	board_areas->board_outer.dim.col = board_half_col;

	board_areas->board_outer.pos.row = BORDER_ROW;
	board_areas->board_outer.pos.col = BORDER_COL;

	//
	// area: inner bar
	//
	board_areas->bar_inner.dim.row = board_half_row;
	board_areas->bar_inner.dim.col = POINTS_COL;

	board_areas->bar_inner.pos.row = BORDER_ROW;
	board_areas->bar_inner.pos.col = board_areas->board_outer.pos.col + board_areas->board_outer.dim.col + BORDER_COL;

	//
	// area: inner board
	//
	board_areas->board_inner.dim.row = board_half_row;
	board_areas->board_inner.dim.col = board_half_col;

	board_areas->board_inner.pos.row = BORDER_ROW;
	board_areas->board_inner.pos.col = board_areas->bar_inner.pos.col + board_areas->bar_inner.dim.col + BORDER_COL;

	//
	// area: bear off
	//
	board_areas->bear_off.dim.row = board_half_row;
	board_areas->bear_off.dim.col = POINTS_COL;

	board_areas->bear_off.pos.row = BORDER_ROW;
	board_areas->bear_off.pos.col = board_areas->board_inner.pos.col + board_areas->board_inner.dim.col + BORDER_COL;

	//
	// area: total board
	//
	board_areas->board_dim.row = 2 * BORDER_ROW + board_half_row;
	board_areas->board_dim.col = board_areas->bear_off.pos.col + board_areas->bear_off.dim.col + BORDER_COL;
}

/******************************************************************************
 * The function initializes the areas and positions.
 *****************************************************************************/

const s_board_areas* s_pos_init() {

	//
	// Areas
	//
	s_pos_areas_init(&_board_areas);

	//
	// Positions
	//
	s_pos_set_bear_off(_pos_bear_off, &_board_areas.bear_off);

	s_pos_set_bar(_pos_bar, &_board_areas.bar_inner);

	s_pos_set_points(_pos_points, &_board_areas.board_outer, &_board_areas.board_inner);

	return &_board_areas;
}

/******************************************************************************
 * TODO: not clear if this is the desired approach.
 *****************************************************************************/

const s_pos* s_pos_get_points() {
	return _pos_points;
}

/******************************************************************************
 * The function returns the position of a checker on a point / bar / bear off
 * area. The checker may be smaller than the containing area.
 *****************************************************************************/

s_pos s_pos_get_checker(const e_pos type, const int idx) {
	s_pos result;

	switch (type) {

	case E_POS_BAR:

		result.pos.row = _pos_bar[idx].pos.row;
		result.pos.col = _pos_bar[idx].pos.col + CHECKER_OFFSET_COL;
		result.is_upper = _pos_bar[idx].is_upper;

		return result;

	case E_POS_BEAR_OFF:

		result.pos.row = _pos_bear_off[idx].pos.row;
		result.pos.col = _pos_bear_off[idx].pos.col + CHECKER_OFFSET_COL;
		result.is_upper = _pos_bear_off[idx].is_upper;

		return result;

	case E_POS_POINTS:

		result.pos.row = _pos_points[idx].pos.row;
		result.pos.col = _pos_points[idx].pos.col + CHECKER_OFFSET_COL;
		result.is_upper = _pos_points[idx].is_upper;

		return result;

	default:
		log_exit("Unknown type: %d", type)
		;
	}
}

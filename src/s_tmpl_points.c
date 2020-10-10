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

#include <stdbool.h>

#include "lib_logging.h"

#include "s_tmpl_points.h"
#include "s_color_def.h"
#include "bg_defs.h"

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
 * Definition of the characters for the points.
 *****************************************************************************/

static const wchar_t _tchar_points[POINTS_ROW][POINTS_COL] = {

{ T__I, T_II, T_II, T_II, T_II, T_I_ },

{ T__I, T_II, T_II, T_II, T_II, T_I_ },

{ T___, T_II, T_II, T_II, T_II, T___ },

{ T___, T_II, T_II, T_II, T_II, T___ },

{ T___, T__I, T_II, T_II, T_I_, T___ },

{ T___, T__I, T_II, T_II, T_I_, T___ },

{ T___, T___, T_II, T_II, T___, T___ },

{ T___, T___, T_II, T_II, T___, T___ },

{ T___, T___, T__I, T_I_, T___, T___ },

{ T___, T___, T__I, T_I_, T___, T___ },

};

/******************************************************************************
 * The array with the four templates for the points:
 *
 * black / white - top / bottom.
 *****************************************************************************/

static s_tarr *_tmpls[NUM_PLAYER][NUM_ORIENT];

/******************************************************************************
 * The function copies the character template to the template structure.
 *****************************************************************************/

static void s_tmpl_point_cp(s_tarr *tmpl, const wchar_t chr_tmpl[POINTS_ROW][POINTS_COL], const short *fg, const bool reverse) {

	s_tchar *tchr;
	int row_reverse;

	for (int row = 0; row < POINTS_ROW; row++) {
		for (int col = 0; col < POINTS_COL; col++) {

			//
			// The s_tmpl structure is not an 2 dim array, so we have to call
			// the function to get the element.
			//
			tchr = &s_tarr_get(tmpl, row, col);

			//
			// If required, we copy the characters with reversed rows.
			//
			row_reverse = reverse ? reverse_idx(POINTS_ROW, row) : row;

			tchr->chr = chr_tmpl[row_reverse][col];
			tchr->fg = fg[row];
			tchr->bg = -1;
		}
	}
}

/******************************************************************************
 * The function initializes the four templates for the points.
 *****************************************************************************/

static void s_tmpl_point_create() {

	//
	// An array for the color gradient.
	//
	short colors[POINTS_ROW];

	//
	// Black points
	//
	s_color_def_gradient(colors, POINTS_ROW, "#ff8000", "#cc6600");

	_tmpls[OWNER_BLACK][ORIENT_TOP] = s_tarr_new(POINTS_ROW, POINTS_COL);

	s_tmpl_point_cp(_tmpls[OWNER_BLACK][ORIENT_TOP], _tchar_points, colors, false);

	_tmpls[OWNER_BLACK][ORIENT_BOT] = s_tarr_new(POINTS_ROW, POINTS_COL);

	s_tmpl_point_cp(_tmpls[OWNER_BLACK][ORIENT_BOT], _tchar_points, colors, true);

	//
	// White points
	//
	s_color_def_gradient(colors, POINTS_ROW, "#804000", "#4d2800");

	_tmpls[OWNER_WHITE][ORIENT_TOP] = s_tarr_new(POINTS_ROW, POINTS_COL);

	s_tmpl_point_cp(_tmpls[OWNER_WHITE][ORIENT_TOP], _tchar_points, colors, false);

	_tmpls[OWNER_WHITE][ORIENT_BOT] = s_tarr_new(POINTS_ROW, POINTS_COL);

	s_tmpl_point_cp(_tmpls[OWNER_WHITE][ORIENT_BOT], _tchar_points, colors, true);

}

/******************************************************************************
 * The function frees the four templates for the points.
 *****************************************************************************/

static void s_tmpl_point_free() {

	s_tarr_free(&_tmpls[OWNER_BLACK][ORIENT_TOP]);

	s_tarr_free(&_tmpls[OWNER_BLACK][ORIENT_BOT]);

	s_tarr_free(&_tmpls[OWNER_WHITE][ORIENT_TOP]);

	s_tarr_free(&_tmpls[OWNER_WHITE][ORIENT_BOT]);
}

/******************************************************************************
 * The function returns the template for the points index. The color toggles
 * and the orientation is reversed on the second half.
 *****************************************************************************/

static s_tarr* s_tmpl_point_get_tmpl(const int point_idx) {

	return _tmpls[point_idx % NUM_PLAYER][point_idx < POINTS_HALF ? 0 : 1];
}

/******************************************************************************
 * The functions computes the positions of the points.
 *****************************************************************************/

void s_tmpl_points_set_pos(s_point *point_pos, const s_area *area_board_outer, const s_area *area_board_inner) {

	const int quarter = POINTS_NUM / 4;

	log_debug("inner board - pos: %d/%d dim: %d/%d", area_board_inner->pos.row, area_board_inner->pos.col, area_board_inner->dim.row, area_board_inner->dim.col);

	const int lower_inner_row = area_board_inner->pos.row + area_board_inner->dim.row - 1;
	const int lower_outer_row = area_board_outer->pos.row + area_board_outer->dim.row - 1;

	for (int i = 0; i < quarter; i++) {

		//
		// Upper right triangles
		//
		point_pos[0 * quarter + i].row = area_board_inner->pos.row;
		point_pos[0 * quarter + i].col = area_board_inner->pos.col + reverse_idx(quarter, i) * POINTS_COL;

		//
		// Upper left triangles
		//
		point_pos[1 * quarter + i].row = area_board_outer->pos.row;
		point_pos[1 * quarter + i].col = area_board_outer->pos.col + reverse_idx(quarter, i) * POINTS_COL;

		//
		// Lower left triangles
		//
		point_pos[2 * quarter + i].row = lower_outer_row;
		point_pos[2 * quarter + i].col = area_board_outer->pos.col + i * POINTS_COL;

		//
		// Lower right triangles
		//
		point_pos[3 * quarter + i].row = lower_inner_row;
		point_pos[3 * quarter + i].col = area_board_inner->pos.col + i * POINTS_COL;

	}
}

/******************************************************************************
 * The function adds the points to the board, which is a s_tarr structure. The
 * templates for the points are identified by the point index. The odd and even
 * points have different colors and the last 12 points are upside down.
 *****************************************************************************/

void s_tmpl_point_add_2_tarr(s_tarr *tarr, const s_point *points_pos) {

	//
	// Allocate and initialize the point templates.
	//
	s_tmpl_point_create();

	s_tarr *tmpl;

	s_point pos;

	for (int i = 0; i < POINTS_NUM; i++) {

		log_debug("Adding point: %d", i);

		tmpl = s_tmpl_point_get_tmpl(i);

		pos = s_tarr_ul_pos_get(tmpl, points_pos[i], (i >= 12));

		s_tarr_cp_fg(tarr, tmpl, pos);
	}

	//
	// Do not forget to free the point templates.
	//
	s_tmpl_point_free();
}

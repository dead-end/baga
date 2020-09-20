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

#include "s_tmpl_points.h"
#include "s_color_def.h"

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

static const wchar_t template[POINTS_ROW][POINTS_COL] = {

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

};

/******************************************************************************
 * The function copies the template character and the foreground color to the
 * struct.
 *****************************************************************************/

static void points_init_tmpl(s_points_tchar *tmpl_points, const wchar_t tmpl[POINTS_ROW][POINTS_COL], const short *fg) {

	for (int row = 0; row < POINTS_ROW; row++) {
		for (int col = 0; col < POINTS_COL; col++) {

			tmpl_points->tchar[row][col].chr = tmpl[row][col];
			tmpl_points->tchar[row][col].fg = fg[row];
			tmpl_points->tchar[row][col].bg = -1;
		}
	}
}

/******************************************************************************
 * The functions computes the positions of the points.
 *****************************************************************************/

void s_tmpl_points_set_pos(s_point *point_pos, const s_area *area_board_outer, const s_area *area_board_inner) {

	const int quarter = POINTS_NUM / 4;

	const int lower_inner_row = area_board_inner->pos.row + area_board_inner->dim.row - POINTS_ROW;
	const int lower_outer_row = area_board_outer->pos.row + area_board_outer->dim.row - POINTS_ROW;

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
 * The function initializes the templates for the points.
 *****************************************************************************/

void s_tmpl_points_init(s_tmpl_points *tmpl_point) {

	//
	// An array for the color gradient.
	//
	short colors[POINTS_ROW];

	//
	// Black points
	//
	s_color_def_gradient(colors, POINTS_ROW, "#ff8000", "#cc6600");

	points_init_tmpl(&tmpl_point->points[OWNER_BLACK], template, colors);

	//
	// White points
	//
	s_color_def_gradient(colors, POINTS_ROW, "#804000", "#4d2800");

	points_init_tmpl(&tmpl_point->points[OWNER_WHITE], template, colors);
}


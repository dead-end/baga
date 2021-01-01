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

#include "lib_utils.h"
#include "lib_logging.h"
#include "s_point_layout.h"

/******************************************************************************
 * The array is a definition of the checkers on a point. The values depend only
 * on the total number of checker. Each checker can be displayed fully, half or
 * not at all. The "num_half" member contains the number of checkers that are
 * displayed half. The "label_idx" contains the index of the label. If the
 * value is -1, then there is no value. The label index is the index of the
 * last visible checker. Checkers with a higher index are not displayed.
 *
 * The index of the array is the total number of checker on the point, so the
 * index 0 is ignored. The checkers can be compressed or not. This is the
 * second dimension.
 *****************************************************************************/

s_point_layout _point_layout[CHECKER_NUM + 1][2] = {

//
// total 0
//
		{ { .total = 0, .num_half = -1, -1 }, { .total = 0, .num_half = -1, -1 } },

		//
		// total 1 - 5 all are displayed fully.
		//
		{ { .total = 1, .num_half = 0, .label_idx = -1 }, { .total = 1, .num_half = 0, .label_idx = -1 } },

		{ { .total = 2, .num_half = 0, .label_idx = -1 }, { .total = 2, .num_half = 0, .label_idx = -1 } },

		{ { .total = 3, .num_half = 0, .label_idx = -1 }, { .total = 3, .num_half = 0, .label_idx = -1 } },

		{ { .total = 4, .num_half = 0, .label_idx = -1 }, { .total = 4, .num_half = 0, .label_idx = -1 } },

		{ { .total = 5, .num_half = 0, .label_idx = -1 }, { .total = 5, .num_half = 1, .label_idx = -1 } }, // The number 5 is compressed.

		//
		// total 6 - 9
		//
		{ { .total = 6, .num_half = 2, .label_idx = 5 }, { .total = 6, .num_half = 3, .label_idx = 5 } },

		{ { .total = 7, .num_half = 4, .label_idx = 6 }, { .total = 7, .num_half = 5, .label_idx = 6 } },

		{ { .total = 8, .num_half = 6, .label_idx = 7 }, { .total = 8, .num_half = 7, .label_idx = 7 } },

		{ { .total = 9, .num_half = 8, .label_idx = 8 }, { .total = 9, .num_half = 7, .label_idx = 7 } },

		//
		// Total: 10 - 15 none is displayed fully.
		//
		{ { .total = 10, .num_half = 8, .label_idx = 8 }, { .total = 10, .num_half = 7, .label_idx = 7 } },

		{ { .total = 11, .num_half = 8, .label_idx = 8 }, { .total = 11, .num_half = 7, .label_idx = 7 } },

		{ { .total = 12, .num_half = 8, .label_idx = 8 }, { .total = 12, .num_half = 7, .label_idx = 7 } },

		{ { .total = 13, .num_half = 8, .label_idx = 8 }, { .total = 13, .num_half = 7, .label_idx = 7 } },

		{ { .total = 14, .num_half = 8, .label_idx = 8 }, { .total = 14, .num_half = 7, .label_idx = 7 } },

		{ { .total = 15, .num_half = 8, .label_idx = 8 }, { .total = 15, .num_half = 7, .label_idx = 7 } }

};

/******************************************************************************
 * The function returns the color index for a checker on a point. The color
 * index 0 is reserved for the traveler color. The index of the last visible
 * checker is always 1.
 *
 * total: 4 (un-)compressed
 * idx:   0 1 2 3
 * color: 4 3 2 1 <= last visible
 *
 * total: 16 uncompressed
 * idx:   0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
 * color: 9 8 7 6 5 4 3 2 1 <= last visible
 *
 * total: 16 compressed
 * idx:   0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
 * color: 8 7 6 5 4 3 2 1 <= last visible
 *
 * (Unit tested)
 *****************************************************************************/

int s_point_layout_color_idx(const s_point_layout layout, const int idx) {

	//
	// Get the number of visible checkers for the point.
	//
	const int num_vis = s_point_layout_num_vis(layout);

#ifdef DEBUG

	//
	// Ensure that the index is valid.
	//
	if (idx >= num_vis) {
		log_exit("Invalid index: %d with visible: %d", idx, num_vis);
	}
#endif

	//
	// The colors are assigned in the reverse order.
	//
	const int col_idx = lu_reverse_idx(num_vis, idx);

	//
	// Adding an offset, because the index 0 is used for the traveler.
	//
	return col_idx + 1;
}

/******************************************************************************
 * The function returns an area for a given position and dimension with respect
 * to the upper / lower location. In the upper case the position is the upper
 * left corner of the area. In the lower case it is the lower left position,
 * which has to be changed to the upper left position.
 *
 * An example with row = 3 and pos = 5:
 *
 * 0
 * 1
 * 2 <-pos - row
 * --------------------------
 * 3 <-pos - row + 1 = result
 * 4
 * 5 <-pos (input param)
 * --------------------------
 * 6
 *****************************************************************************/

s_area s_point_layout_get_area(const s_pos *pos, const int dim_row, const int dim_col) {

	const s_area result = {

	.dim.row = dim_row,

	.dim.col = dim_col,

	.pos.row = (pos->is_upper) ? pos->pos.row : pos->pos.row - dim_row + 1,

	.pos.col = pos->pos.col

	};

	log_debug("area - pos: %d/%d dim: %d/%d", result.pos.row, result.pos.col, result.dim.row, result.dim.col);

	return result;
}

/******************************************************************************
 * The function computes the position of a checker. It is called with the
 * position of a point and computes the position of the n-th full checker on
 * that point. The point can be compressed, which means that one checker is
 * reduced.
 *
 * Important:
 * The compressed flag is not trivial. If we have num full: 6, compressed this
 * means 5 full plus one.
 *****************************************************************************/

s_point s_point_layout_pos_full(const s_pos *pos, const e_compressed compressed, const int num_full) {

#ifdef DEBUG

	//
	// The maximum number of full checkers is the maximum visible checkers + 1
	//
	if (num_full > CHECK_DIS_FULL + 1) {
		log_exit("Too many full checkers: %d", num_full);
	}

	//
	// Ensure that num_full is valid.
	//
	if (compressed && num_full < CHECK_DIS_FULL) {
		log_exit("Compressed: %d", num_full);
	}
#endif

	s_point result;
	result.col = pos->pos.col;

	if (pos->is_upper) {

		//
		// 0  <= Index
		// 1  11 <= pos
		// 2  11
		// 3  22
		// 4  22
		// 5  33
		// 6  33
		// 7  44 <= result
		// 8  44
		//
		result.row = pos->pos.row + CHECKER_ROW * (num_full - 1);

		if (compressed == E_COMP) {
			result.row--;
		}

	} else {

		//
		// 0  <= Index
		// 1  11 <= result
		// 2  11
		// 3  22
		// 4  22
		// 5  33
		// 6  33
		// 7  44
		// 8  44 <= pos
		//
		result.row = pos->pos.row - CHECKER_ROW * (num_full - 1) - CHECKER_ROW + 1;

		if (compressed == E_COMP) {
			result.row++;
		}
	}

	log_debug("pos: %d/%d result: %d/%d full: %d", pos->pos.row, pos->pos.col, result.row, result.col, num_full);

	return result;
}

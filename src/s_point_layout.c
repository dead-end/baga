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
		// Total: 10 - 16 none is displayed fully.
		//
		{ { .total = 10, .num_half = 8, .label_idx = 8 }, { .total = 10, .num_half = 7, .label_idx = 7 } },

		{ { .total = 11, .num_half = 8, .label_idx = 8 }, { .total = 11, .num_half = 7, .label_idx = 7 } },

		{ { .total = 12, .num_half = 8, .label_idx = 8 }, { .total = 12, .num_half = 7, .label_idx = 7 } },

		{ { .total = 13, .num_half = 8, .label_idx = 8 }, { .total = 13, .num_half = 7, .label_idx = 7 } },

		{ { .total = 14, .num_half = 8, .label_idx = 8 }, { .total = 14, .num_half = 7, .label_idx = 7 } },

		{ { .total = 15, .num_half = 8, .label_idx = 8 }, { .total = 15, .num_half = 7, .label_idx = 7 } },

		{ { .total = 16, .num_half = 8, .label_idx = 8 }, { .total = 16, .num_half = 7, .label_idx = 7 } }

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
	const int col_idx = reverse_idx(num_vis, idx);

	//
	// Adding an offset, because the index 0 is used for the traveler.
	//
	return col_idx + 1;
}

/******************************************************************************
 * The function returns the area of a point. This is the area of the full point
 * plus an additional checker. It defines the area which should be updated if
 * a traveler checker starts or arrives.
 *
 * (unit tested)
 *****************************************************************************/

#define POINT_AREA_ROW (POINTS_ROW + CHECKER_ROW)

s_area s_point_layout_get_area(const s_point point_pos, const bool is_upper) {

	const s_area result = {

	//
	// The extended area of the point
	//
			.dim.row = POINT_AREA_ROW,

			.dim.col = CHECKER_COL,

			//
			// This uses the result.dim.row which is POINT_AREA_ROW
			//
			.pos.row = (is_upper) ? point_pos.row : point_pos.row - POINT_AREA_ROW + 1,

			.pos.col = point_pos.col

	};

	log_debug("pos: %d/%d area - pos: %d/%d dim: %d/%d",point_pos.row, point_pos.col, result.pos.row, result.pos.col,result.dim.row, result.dim.col);

	return result;
}

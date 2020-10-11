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
#include "lib_color_pair.h"

#include "s_tarr.h"

/******************************************************************************
 * The function allocates a s_tarr structure with a given dimension.
 *****************************************************************************/

s_tarr* s_tarr_new(const int row, const int col) {

	log_debug("New s_tarr with: %d/%d", row, col);

	//
	// Allocate the structure
	//
	s_tarr *tarr = malloc(sizeof(s_tarr));
	if (tarr == NULL) {
		log_exit_str("Unable to allocate memory!");
	}

	//
	// Allocate the array (1-dimensional)
	//
	tarr->arr = malloc(row * col * sizeof(s_tchar));
	if (tarr->arr == NULL) {
		log_exit_str("Unable to allocate memory!");
	}

	//
	// Set the dimensions.
	//
	tarr->dim.row = row;
	tarr->dim.col = col;

	return tarr;
}

/******************************************************************************
 * The function frees the s_tarr structure.
 *****************************************************************************/

void s_tarr_free(s_tarr **tarr) {

	//
	// Ensure that there is something to free
	//
	if (*tarr == NULL) {
		log_exit_str("s_tarr already freed!");
	}

	//
	// Free the array
	//
	free((*tarr)->arr);

	//
	// Free the structure.
	//
	free(*tarr);

	//
	// Set the pointer to NULL, to mark it as freed.
	//
	*tarr = NULL;
}

/******************************************************************************
 * The function initializes the array with a s_tchar. All elements are treated
 * the same, so do not need to care about rows and cols and treat the structure
 * as a one dimensional array.
 *****************************************************************************/

void s_tarr_set(s_tarr *tarr, const s_tchar tchar) {

	const int end = tarr->dim.row * tarr->dim.col;

	for (int i = 0; i < end; i++) {
		tarr->arr[i] = tchar;
	}
}

/******************************************************************************
 * The function initializes the array with a wchar_t character, a foreground
 * color and an array with a background gradient.
 *****************************************************************************/

void s_tarr_set_gradient(s_tarr *tarr, const wchar_t chr, const short fg_color, const short *bg_colors) {

	s_tchar *tchr;

	for (int row = 0; row < tarr->dim.row; row++) {
		for (int col = 0; col < tarr->dim.col; col++) {

			tchr = &s_tarr_get(tarr, row, col);

			tchr->chr = chr;
			tchr->fg = fg_color;
			tchr->bg = bg_colors[row];
		}
	}
}

/******************************************************************************
 * The function sets the background color with a gradient for a given area
 * inside a s_tarr structure.
 *****************************************************************************/

void s_tarr_set_bg(s_tarr *tarr, const s_area *area, const short *bg_colors) {

	//
	// Store the position of the area.
	//
	const int pos_row = area->pos.row;
	const int pos_col = area->pos.col;

	for (int row = 0; row < area->dim.row; row++) {
		for (int col = 0; col < area->dim.col; col++) {

			s_tarr_get(tarr, pos_row + row, pos_col + col).bg = bg_colors[row];
		}
	}
}

/******************************************************************************
 * The function copies the foreground (character and color) from a template to
 * a s_tarr. The background of the target is untouched.
 *****************************************************************************/

void s_tarr_cp_fg(s_tarr *to_arr, const s_tarr *from_arr, const s_point pos) {

	const s_tchar *from;
	s_tchar *to;

	for (int row = 0; row < from_arr->dim.row; row++) {
		for (int col = 0; col < from_arr->dim.col; col++) {

			to = &s_tarr_get(to_arr, pos.row + row, pos.col + col);
			from = &s_tarr_get(from_arr, row, col);

			to->chr = from->chr;
			to->fg = from->fg;
		}
	}
}

/******************************************************************************
 * The function copies the from_array to the to_array at a given position.
 *****************************************************************************/

void s_tarr_cp(s_tarr *to_arr, const s_tarr *from_arr, const s_point pos) {

	for (int row = 0; row < from_arr->dim.row; row++) {
		for (int col = 0; col < from_arr->dim.col; col++) {

			s_tarr_get(to_arr, pos.row + row, pos.col + col) = s_tarr_get(from_arr, row, col);
		}
	}
}

/******************************************************************************
 * The function computes the upper left corner of the array with a given
 * position. The position is either the upper or the lower left corner.
 *
 * +--------+
 * | <- upper left
 * |        |
 * |        |
 * | <- lower left
 * +--------+
 *****************************************************************************/

s_point s_tarr_ul_pos_get(const s_tarr *tarr, s_point cur_pos, const bool reverse) {

	if (reverse) {
		cur_pos.row = cur_pos.row - tarr->dim.row + 1;
	}

	return cur_pos;
}

/******************************************************************************
 * The function print the complete foreground / background.
 *****************************************************************************/

void s_tarr_print(WINDOW *win, const s_tarr *ta_fg, const s_tarr *ta_bg) {

#ifdef DEBUG

	//
	// Ensure that both have the same dimension.
	//
	if (ta_fg->dim.row != ta_bg->dim.row || ta_fg->dim.col != ta_bg->dim.col) {
		log_exit("Wrong dim - fg: %d/%d bg: %d/%d", ta_fg->dim.row, ta_fg->dim.col, ta_bg->dim.row, ta_bg->dim.col);
	}
#endif

	short cp;

	const s_tchar *tchar;

	for (int row = 0; row < ta_bg->dim.row; row++) {
		for (int col = 0; col < ta_bg->dim.col; col++) {

			//
			// We first try the foreground
			//
			tchar = &s_tarr_get(ta_fg, row, col);

			//
			// If it is not defined we use the background.
			//
			if (!s_tchar_is_defined(tchar)) {
				tchar = &s_tarr_get(ta_bg, row, col);
			}

			//
			// Set the color pair with the t_char
			//
			cp = cp_color_pair_get(tchar->fg, tchar->bg);
			attrset(COLOR_PAIR(cp));

			mvwprintw(win, row, col, "%lc", tchar->chr);
		}
	}
}

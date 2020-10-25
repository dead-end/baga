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
#include "s_color_def.h"
#include "s_tmpl_checker.h"

/******************************************************************************
 * The definition of the template structures. To print the checkers on a point
 * we need a full and a half structure. For the traveling checker we need an
 * other, independent template. We do not overwrite the traveler template when
 * we update a point.
 *****************************************************************************/

typedef enum {

	TS_FULL = 0, TS_HALF = 1, TS_TRAVELER = 2

} e_tmpl_size;

#define _NUM_SIZES 3

static s_tarr *_tmpls[_NUM_SIZES];

/******************************************************************************
 * The definition of colors for the checkers on a point.
 *****************************************************************************/

#define COLOR_IDX_TRAV 0

#define COLOR_IDX_FG 0

#define _COLOR_NUM CHECK_DIS_MAX + 1

static short _colors[NUM_PLAYER][_COLOR_NUM];

/******************************************************************************
 * The function adds the number of checker to the checker template. The number
 * of checker for each player is 16, so this is the max value.
 *****************************************************************************/

static wchar_t wchar_t_map[] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9' };

static void s_tmpl_checker_set_label(s_tarr *tmpl, const int total, const bool reverse) {
	s_tchar *tchar;

#ifdef DEBUG

	//
	// Ensure that the number of checker is valid
	//
	if (total < 1 || total > CHECKER_NUM) {
		log_exit("Invalid checker num: %d", total);
	}
#endif

	//
	// Add 1 or leading 0
	//
	tchar = &s_tarr_get(tmpl, reverse ? 0 : 1, 1);
	tchar->chr = total < 10 ? L'0' : L'1';

	//
	// Add the second digit.
	//
	tchar = &s_tarr_get(tmpl, reverse ? 0 : 1, 2);
	tchar->chr = total < 10 ? wchar_t_map[total] : wchar_t_map[total - 10];

}

// ----------------------- INTERFACE ------------------------------------------
/******************************************************************************
 * The function initializes the template structures and the color array.
 *****************************************************************************/

void s_tmpl_checker_create() {

	//
	// Create color
	//
	s_color_def_gradient(_colors[OWNER_BLACK], _COLOR_NUM, "#777777", "#222222");

	s_color_def_gradient(_colors[OWNER_WHITE], _COLOR_NUM, "#ffffff", "#aaaaaa");

	//
	// Create templates
	//
	_tmpls[TS_FULL] = s_tarr_new(CHECKER_ROW, CHECKER_COL);

	_tmpls[TS_HALF] = s_tarr_new(CHECKER_ROW / 2, CHECKER_COL);

	_tmpls[TS_TRAVELER] = s_tarr_new(CHECKER_ROW, CHECKER_COL);
}

/******************************************************************************
 * The function frees the template structures.
 *****************************************************************************/

void s_tmpl_checker_free() {

	log_debug_str("Freeing checker!");

	s_tarr_free(&_tmpls[TS_FULL]);

	s_tarr_free(&_tmpls[TS_HALF]);

	s_tarr_free(&_tmpls[TS_TRAVELER]);
}

/******************************************************************************
 * The function returns a template for a traveling checker. The template is
 * full. It has no label, so the foreground is not used.
 *****************************************************************************/

const s_tarr* s_tmpl_checker_get_travler(const e_owner owner) {

	s_tarr *tmpl = _tmpls[TS_TRAVELER];

	s_tarr_set(_tmpls[TS_TRAVELER], (s_tchar ) {

			.chr = EMPTY,

			.fg = _colors[e_owner_other(owner)][COLOR_IDX_FG],

			.bg = _colors[owner][COLOR_IDX_TRAV] });

	return tmpl;
}

/******************************************************************************
 * The function returns the template for the checker. The checker can be
 * displayed fully, half or not at all. The color depends on the owner, and the
 * display index.
 *****************************************************************************/

const s_tarr* s_tmpl_checker_get_tmpl(const e_owner owner, const s_point_layout point_layout, const int idx, const bool reverse) {

	//
	// We do not display checkers behind the label.
	//
	if (s_point_layout_not_visible(point_layout, idx)) {
		return NULL;
	}

	//
	// We get the color index for the current checker.
	//
	const int color_idx = s_point_layout_color_idx(point_layout.total, idx);

	//
	// Select the template, which can be full or half.
	//
	s_tarr *tmpl = s_point_layout_is_half(point_layout, idx) ? _tmpls[TS_HALF] : _tmpls[TS_FULL];

	s_tarr_set(tmpl, (s_tchar ) {

			.chr = EMPTY,

			.fg = _colors[e_owner_other(owner)][COLOR_IDX_FG],

			.bg = _colors[owner][color_idx] });

	//
	// Add the label if necessary
	//
	if (s_point_layout_has_label(point_layout, idx)) {
		s_tmpl_checker_set_label(tmpl, point_layout.total, reverse);
	}

	return tmpl;
}

/******************************************************************************
 *
 *****************************************************************************/
// todo:unused
s_point s_tmpl_checker_pos(const int point_idx, s_point point_pos, const int total, const int num) {

	const s_point_layout *layout = &s_point_layout_get(total, E_UNCOMP);

	if (s_point_layout_not_visible(*layout, num)) {
		log_exit("Not visible: %d", num);
	}

	const int sign = (point_idx < 12) ? 1 : -1;

	s_point result;
	result.col = point_pos.col;

	if (layout->num_half == 0) {
		result.row = point_pos.row + sign * (CHECKER_ROW * num);

	} else if (layout->num_half >= num) {
		result.row = point_pos.row + sign * (CHECKER_ROW / 2 * num);

	} else {
		result.row = point_pos.row + sign * (CHECKER_ROW / 2 * layout->num_half + CHECKER_ROW * (num - layout->num_half));
	}

	return result;
}

// -----------------------------

/******************************************************************************
 *
 *****************************************************************************/

// todo: ensure it works with half (if half > 0 => last index always the same!!)
// todo: unit tests
// todo: compressed
s_point s_tmpl_checker_last_pos(const s_point point_pos, const int point_idx, const int total) {

	const s_point_layout *layout = &s_point_layout_get(total, E_UNCOMP);

	//
	// If the number of half displayed checkers is not null, the position of
	// the last is fixed and the position is the position of the last fully
	// visible checker with CHECK_DIS_FULL checkers.
	//
	const int num_full = min(layout->total, CHECK_DIS_FULL);

	s_point result;
	result.col = point_pos.col;

	if (point_idx < 12) {

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
		result.row = point_pos.row + CHECKER_ROW * (num_full - 1);

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
		result.row = point_pos.row - CHECKER_ROW * num_full + 1;
	}

	log_debug("result: %d/%d total: %d half: %d full: %d", result.row, result.col, total, layout->num_half, num_full);

	return result;
}

// todo: unit tests
s_area s_tmpl_checker_point_area(const s_point point_pos, const int point_idx, const int total) {

	const s_point_layout *layout = &s_point_layout_get(total, E_UNCOMP);

	s_area result;

	const int num_full = max(layout->total, layout->label_idx + 1) - layout->num_half;

	result.dim.row = CHECKER_ROW / 2 * layout->num_half + CHECKER_ROW * num_full;
	result.dim.col = CHECKER_COL;

	if (point_idx < 12) {
		result.pos.row = point_pos.row;
	} else {
		result.pos.row = point_pos.row - result.dim.row;
	}
	result.pos.col = point_pos.col;

	return result;
}

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

s_checker_layout _checker_layout[CHECKER_NUM + 1][2] = {

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

//
// The macro checks if the checker with the given index is not visible.
//
#define s_checker_layout_not_visible(cl,i) ((cl).label_idx >= 0) && ((i) > (cl).label_idx)

//
// The macro checks if the checker with the given index has a label.
//
#define s_checker_layout_has_label(cl,i) ((cl).label_idx == (i))

//
// The macro checks if the checker with the given index is displayed half.
//
#define s_checker_layout_is_half(cl,i) ((i) < (cl).num_half)

//
// The macro returns the color index of a given checker.
//
#define s_checker_layout_color_idx(t,i) (reverse_idx(max(CHECK_DIS_MAX, (t)), (i)) + 1)

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

const s_tarr* s_tmpl_checker_get_tmpl(const e_owner owner, const s_checker_layout checker_layout, const int idx, const bool reverse) {

	//
	// We do not display checkers behind the label.
	//
	if (s_checker_layout_not_visible(checker_layout, idx)) {
		return NULL;
	}

	//
	// We get the color index for the current checker.
	//
	const int color_idx = s_checker_layout_color_idx(checker_layout.total, idx);

	//
	// Select the template, which can be full or half.
	//
	s_tarr *tmpl = s_checker_layout_is_half(checker_layout, idx) ? _tmpls[TS_HALF] : _tmpls[TS_FULL];

	s_tarr_set(tmpl, (s_tchar ) {

			.chr = EMPTY,

			.fg = _colors[e_owner_other(owner)][COLOR_IDX_FG],

			.bg = _colors[owner][color_idx] });

	//
	// Add the label if necessary
	//
	if (s_checker_layout_has_label(checker_layout, idx)) {
		s_tmpl_checker_set_label(tmpl, checker_layout.total, reverse);
	}

	return tmpl;
}

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
 * The definition of the template structure, we have one structure for the full
 * and one for the half template.
 *****************************************************************************/

typedef enum {

	TS_FULL = 0, TS_HALF = 1

} e_tmpl_size;

#define _NUM_SIZES 2

static s_tarr *_tmpls[_NUM_SIZES];

/******************************************************************************
 * The definition of the checkers on a point.
 *****************************************************************************/

//
// No more than CHECK_DIS_MAX checkers are displayed on a point.
//
#define CHECK_DIS_MAX 9

//
// The maximum number of checker that are displayed completely on a point.
//
#define CHECK_DIS_FULL 5

/******************************************************************************
 * The definition of colors for the checkers on a point.
 *****************************************************************************/

#define COLOR_IDX_TRAV 0

#define COLOR_IDX_FG 0

#define _COLOR_NUM 10

static short _colors[NUM_PLAYER][_COLOR_NUM];

/******************************************************************************
 * The macro returns the number of checkers that will be displayed as half for
 * a given number of checkers on a point.
 *****************************************************************************/

static int _num_half_map[] = {

//
// Index 0 unused
//
		-1,

		//
		// 1 - 5 => all checkers are displayed completely
		//
		0, 0, 0, 0, 0,

		//
		// 6 - 9 => all checkers are displayed (full or half)
		//
		2, 4, 6, 8,

		//
		// 10 - 16 => 9 checkers are displayed (full or half) and the rest is
		// missing.
		//
		8, 8, 8, 8, 8, 8, 8

};

#define s_tmpl_checker_num_half(t) _num_half_map[t]

/******************************************************************************
 * The array contains the index for the label with a given total number of
 * checker. If the value is negative, there is no label.
 *****************************************************************************/

static int _total_label_idx[] = {

//
// Index 0 unused
//
		-1,

		//
		// 1 - 5 => all checkers are displayed completely, so there is no
		// label.
		//
		-1, -1, -1, -1, -1,

		//
		// 6 - 9 => all checkers are displayed (full or half). The last has the
		// label.
		//
		5, 6, 7, 8,

		//
		// 10 - 16 => 9 checkers are displayed (full or half) and the rest is
		// missing. The last visible has the label.
		//
		8, 8, 8, 8, 8, 8, 8

};

#define s_tmpl_checker_has_label(t,i) (_total_label_idx[(t)] == (i))

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

/******************************************************************************
 *
 *****************************************************************************/

#define s_tmpl_checker_color_idx(t,i) (reverse_idx(max(CHECK_DIS_MAX, (t)), (i)) + 1)

// ----------------------- INTERFACE ------------------------------------------

/******************************************************************************
 * The function initializes the template structures and the color array.
 *****************************************************************************/

void s_tmpl_checker_create() {

#ifdef DEBUG

	//
	// Ensure that the _num_half_map has the correct size.
	//
	if (sizeof(_num_half_map) / sizeof(_num_half_map[0]) != CHECKER_NUM + 1) {
		log_exit_str("Invalid number of elements for _num_half_map");
	}
#endif

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
}

/******************************************************************************
 * The function frees the template structures.
 *****************************************************************************/

void s_tmpl_checker_free() {

	log_debug_str("Freeing checker!");

	s_tarr_free(&_tmpls[TS_FULL]);

	s_tarr_free(&_tmpls[TS_HALF]);
}

/******************************************************************************
 * The function returns a template for a traveling checker. The template is
 * full. It has no label, so the foreground is not used.
 *****************************************************************************/

const s_tarr* s_tmpl_checker_get_travler(const e_owner owner) {

	s_tarr *tmpl = _tmpls[TS_FULL];

	s_tarr_set(_tmpls[TS_FULL], (s_tchar ) {

			.chr = EMPTY,

			.fg = _colors[e_owner_other(owner)][COLOR_IDX_FG],

			.bg = _colors[owner][COLOR_IDX_TRAV] });

	return tmpl;
}

/******************************************************************************
 * The function returns a template for a checker on a point. The template can
 * be displayed fully, half or not at all. The template can contains a label.
 *****************************************************************************/

const s_tarr* s_tmpl_checker_get_tmpl(const e_owner owner, const int total, const int idx, const bool reverse) {

	//
	// Only the first 9 checkers are displayed.
	//
	if (idx >= CHECK_DIS_MAX) {
		return NULL;
	}

	//
	// We get the color index for the current checker
	//
	const int color_idx = s_tmpl_checker_color_idx(total, idx);

	//
	// Select the template, which can be full or half.
	//
	s_tarr *tmpl = (idx < s_tmpl_checker_num_half(total)) ? _tmpls[TS_HALF] : _tmpls[TS_FULL];

	s_tarr_set(tmpl, (s_tchar ) {

			.chr = EMPTY,

			.fg = _colors[e_owner_other(owner)][COLOR_IDX_FG],

			.bg = _colors[owner][color_idx] });

	//
	// Add label if necessary
	//
	if (s_tmpl_checker_has_label(total, idx)) {
		s_tmpl_checker_set_label(tmpl, total, reverse);
	}

	return tmpl;
}

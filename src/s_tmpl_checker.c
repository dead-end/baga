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

#define NUM_SIZES 2

static s_tmpl _tmpls[NUM_SIZES];

/******************************************************************************
 * The array with the color definition.
 *****************************************************************************/

#define _COLOR_NUM 9

short _colors[NUM_PLAYER][_COLOR_NUM];

/******************************************************************************
 * The function returns the number of checkers that will be displayed as half
 * for a given number of checkers on a point.
 *****************************************************************************/

static int s_tmpl_checker_num_half(const int total) {

	//
	// 5 checker can be displayed on a point completely.
	//
	if (total < 6) {
		return 0;
	}

	if (total < 10) {
		return (total - 5) * 2;
	}

	//
	// 8 half and one complete checker.
	//
	return 8;
}

/******************************************************************************
 *
 *****************************************************************************/

static wchar_t wchar_t_map[] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9' };

static void s_tmpl_checker_set_label(s_tmpl *tmpl, const int total, const bool reverse) {
	short tmp;
	s_tchar *tchar;

	tchar = s_tmpl_get_ptr(tmpl, reverse ? 0 : 1, 1);
	tchar->chr = total < 10 ? L'0' : L'1';

	tmp = tchar->fg;
	tchar->fg = tchar->bg;
	tchar->bg = tmp;

	tchar = s_tmpl_get_ptr(tmpl, reverse ? 0 : 1, 2);
	tchar->chr = total < 10 ? wchar_t_map[total] : wchar_t_map[total - 10];

	tmp = tchar->fg;
	tchar->fg = tchar->bg;
	tchar->bg = tmp;
}

/******************************************************************************
 * The function returns the color index of a checker. If the index is not
 * visible the function returns -1.
 *
 * Example: total 12 => 1-8 and 12 are visible, the others not.
 *****************************************************************************/

static int s_tmpl_checker_color_idx(const int total, const int idx) {

#ifdef DEBUG

	//
	// Ensure that the total number of checkers is valid.
	//
	if (total < 0 || total > CHECKER_NUM) {
		log_exit("Invalid number of checkers on the point: %d", total);
	}

	//
	// Ensure that the index is valid.
	//
	if (idx < 0 || idx > total) {
		log_exit("Invalid index: %d", total);
	}
#endif

	//
	// The last checker on the point is always visible.
	//
	if (idx == total - 1) {
		return 0;
	}

	//
	// The first 8 checker are visible.
	//
	if (idx < 8) {
		return reverse_idx(max(_COLOR_NUM, total), idx);
	}

	//
	// All other checkers are not visible.
	//
	return -1;
}

// ----------------------- INTERFACE ------------------------------------------

/******************************************************************************
 *
 *****************************************************************************/

const s_tmpl* s_tmpl_checker_get_tmpl(const e_owner owner, const int total, const int idx, const bool reverse) {

	//
	// We get the color index for the current checker
	//
	const int color_idx = s_tmpl_checker_color_idx(total, idx);

	//
	// If the index is negative, the the checker will not be displayed.
	//
	if (color_idx < 0) {
		return NULL;
	}

	const bool half = idx < s_tmpl_checker_num_half(total);

	s_tmpl *tmpl = half ? &_tmpls[TS_HALF] : &_tmpls[TS_FULL];

	s_tmpl_set(tmpl, (s_tchar ) { .chr = FULL, .bg = 0, .fg = _colors[owner][color_idx] });

	if (idx == total - 1 && total > 5) {
		s_tmpl_checker_set_label(tmpl, total, reverse);
	}

	return tmpl;
}

/******************************************************************************
 * The function initializes the template structures and the color array.
 *****************************************************************************/

void s_tmpl_checker_create() {

	//
	// Create color
	//
	s_color_def_gradient(_colors[OWNER_BLACK], _COLOR_NUM, "#777777", "#111111");

	s_color_def_gradient(_colors[OWNER_WHITE], _COLOR_NUM, "#ffffff", "#999999");

	//
	// Create templates
	//
	s_tmpl_create(&_tmpls[TS_FULL], CHECKER_ROW, CHECKER_COL);

	s_tmpl_create(&_tmpls[TS_HALF], CHECKER_ROW / 2, CHECKER_COL);
}

/******************************************************************************
 * The function frees the template structures.
 *****************************************************************************/

void s_tmpl_checker_free() {

	s_tmpl_free(&_tmpls[TS_FULL]);

	s_tmpl_free(&_tmpls[TS_HALF]);
}


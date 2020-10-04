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
 * Definition of the characters used by the checker.
 *****************************************************************************/

//
// Full block
//
#define T_FU L'\u2588'

//
// Upper half block
//
#define T_UP L'\u2580'

//
// Lower half block
//
#define T_LO L'\u2584'

/******************************************************************************
 * Definition of the characters for the checker.
 *****************************************************************************/

static const wchar_t _tchar_checker[CHECKER_ROW][CHECKER_COL] = {

{ T_FU, T_UP, T_UP, T_FU },

{ T_FU, T_LO, T_LO, T_FU },

};

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

#define BG_OFFSET 2

#define _COLOR_NUM CHECKER_ROW + CHECKER_ROW

short _colors[NUM_PLAYER][_COLOR_NUM];

/******************************************************************************
 * The function initializes the template structures and the color array.
 *****************************************************************************/

void s_tmpl_checker_create() {

	//
	// Create color
	//
	s_color_def_gradient(_colors[OWNER_BLACK], _COLOR_NUM, "#222222", "#555555");

	s_color_def_gradient(_colors[OWNER_WHITE], _COLOR_NUM, "#bbbbbb", "#eeeeee");

	//
	// Create templates
	//
	s_tmpl_create(&_tmpls[TS_FULL], CHECKER_ROW, CHECKER_COL);

	s_tmpl_create(&_tmpls[TS_HALF], CHECKER_ROW / 2, CHECKER_COL);
}

/******************************************************************************
 *
 *****************************************************************************/

void s_tmpl_checker_free() {

	s_tmpl_free(&_tmpls[TS_FULL]);

	s_tmpl_free(&_tmpls[TS_HALF]);
}

/******************************************************************************
 *
 *****************************************************************************/

static int s_tmpl_checker_num_half(const int total) {

	//
	//
	//
	if (total < 6) {
		return 0;
	}

	if (total < 10) {
		return (total - 5) * 2;
	}

	return 8;
}

static void s_tmpl_checker_labeled(s_tmpl *tmpl, const int total, const bool reverse, const short fg, const short bg) {

	s_tchar *tchar;

	for (int row = 0; row < tmpl->dim.row; row++) {
		for (int col = 0; col < tmpl->dim.col; col++) {

			tchar = s_tmpl_get_ptr(tmpl, row, col);

			tchar->chr = T_FU;
			tchar->fg = fg;
			tchar->bg = bg;
		}
	}

	tchar = s_tmpl_get_ptr(tmpl, reverse ? 0 : 1, 1);
	tchar->chr = L'1';

	tchar = s_tmpl_get_ptr(tmpl, reverse ? 0 : 1, 2);

	switch (total - 10) {
	case 0:
		tchar->chr = L'0';
		break;
	case 1:
		tchar->chr = L'1';
		break;
	case 2:
		tchar->chr = L'2';
		break;
	case 3:
		tchar->chr = L'3';
		break;
	case 4:
		tchar->chr = L'4';
		break;
	case 5:
		tchar->chr = L'5';
		break;
	case 60:
		tchar->chr = L'6';
		break;

	default:
		log_exit("Invalid number: %d", total)
		;
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void s_tmpl_checker_cp(s_tmpl *tmpl, const int row_start, const wchar_t char_tmpl[CHECKER_ROW][CHECKER_COL], const short fg, const short bg) {

	s_tchar *tchar;

	for (int row = 0; row < tmpl->dim.row; row++) {
		for (int col = 0; col < tmpl->dim.col; col++) {

			tchar = s_tmpl_get_ptr(tmpl, row, col);

			tchar->chr = char_tmpl[row_start + row][col];
			tchar->fg = fg;
			tchar->bg = bg;
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void s_tmpl_checker_colors(const e_owner owner, const int idx, short *fg, short *bg) {

	//
	// Only up to 9 checkers are printed in points, so the last valid index is
	// 8.
	//
	const int i = idx < 9 ? idx : 8;

	const int color = (i % 2 ? 1 : 0);
	*fg = _colors[owner][color];
	*bg = _colors[owner][color + BG_OFFSET];
}

/******************************************************************************
 * The function returns a template for the checker. Depending on the input
 * data, a full or half template is chosen and filled.
 *****************************************************************************/

const s_tmpl* s_tmpl_checker_get_tmpl(const e_owner owner, const int total, const int idx, const bool reverse) {

	short fg, bg;
	s_tmpl_checker_colors(owner, idx, &fg, &bg);

	const bool half = idx < s_tmpl_checker_num_half(total);

	const int row_start = half && reverse ? 1 : 0;

	//
	// Get the template, that we will update and return. Here only the size
	// matters.
	//
	s_tmpl *tmpl = NULL;

	if (total < 10 || idx < 8) {

		tmpl = half ? &_tmpls[TS_HALF] : &_tmpls[TS_FULL];
		s_tmpl_checker_cp(tmpl, row_start, _tchar_checker, fg, bg);

	} else if (idx == total - 1) {
		tmpl = &_tmpls[TS_FULL];
		s_tmpl_checker_labeled(tmpl, total, reverse, fg, bg);
	}

	return tmpl;
}

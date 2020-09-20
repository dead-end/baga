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

#include <wchar.h>

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

static const wchar_t template[CHECKER_ROW][CHECKER_COL] = {

{ T_FU, T_UP, T_UP, T_FU },

{ T_FU, T_LO, T_LO, T_FU },

};

/******************************************************************************
 * The definition of the foreground and background color numbers. There is one
 * array with gradients that are used for the foreground and background.
 *
 * fg: 0 1 2 3
 * bg: 2 3 4 5
 *****************************************************************************/

#define _COLOR_SHIFT 2

#define _COLOR_NUM CHECKER_COL + _COLOR_SHIFT

/******************************************************************************
 * The function sets the foreground and background colors of the checker
 * struct.
 *****************************************************************************/

static void copy_colors(s_checker_tchar *tmpl_checker, const short *colors) {

	for (int row = 0; row < CHECKER_ROW; row++) {
		for (int col = 0; col < CHECKER_COL; col++) {

			tmpl_checker->tchar[row][col].fg = colors[col];
			tmpl_checker->tchar[row][col].bg = colors[col + _COLOR_SHIFT];
		}
	}
}
/******************************************************************************
 * The function copies the wchar_t characters from the template to the checker
 * struct.
 *****************************************************************************/

static void copy_tmpl(s_checker_tchar *tmpl_checker, const wchar_t tmpl[][CHECKER_COL]) {

	for (int row = 0; row < CHECKER_ROW; row++) {
		for (int col = 0; col < CHECKER_COL; col++) {
			tmpl_checker->tchar[row][col].chr = tmpl[row][col];
		}
	}
}

/******************************************************************************
 * The function initializes the checker struct.
 *****************************************************************************/

void s_tmpl_checker_init(s_tmpl_checker *templ_checker) {

	//
	// An array for the color gradient.
	//
	short colors[_COLOR_NUM];

	//
	// Black checker
	//
	s_color_def_gradient(colors, _COLOR_NUM, "#111111", "#555555");

	copy_colors(&templ_checker->checker[OWNER_BLACK], colors);

	copy_tmpl(&templ_checker->checker[OWNER_BLACK], template);

	//
	// White checker
	//
	s_color_def_gradient(colors, _COLOR_NUM, "#aaaaaa", "#eeeeee");

	copy_colors(&templ_checker->checker[OWNER_WHITE], colors);

	copy_tmpl(&templ_checker->checker[OWNER_WHITE], template);
}

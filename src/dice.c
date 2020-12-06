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

// TODO: check includes
#include "bg_defs.h"
#include "s_color_def.h"
#include "lib_color.h"
#include "lib_color_pair.h"
#include "lib_logging.h"
#include "s_tarr.h"
#include "s_status.h"

/******************************************************************************
 *
 *****************************************************************************/

//
// Full block
//
#define FU L'\u2588'

//
// Upper half
//
#define UP L'\u2584'

// TODO: not necessary
//
// Lower half
//
#define LO L'\u2580'

/******************************************************************************
 *
 *****************************************************************************/

// TODO: get size for layout
#define D_ROWS 4

#define D_COLS 7

#define PADDING 2

/******************************************************************************
 * The elements have fixed positions inside the window.
 *****************************************************************************/

static const s_point _pos_dice_1 = { .row = 0, .col = 0 };

static const s_point _pos_dice_2 = { .row = 0, .col = D_COLS + PADDING };

/******************************************************************************
 *
 *****************************************************************************/

static const wchar_t _tmpl_wchr[6][D_ROWS][D_COLS] = {

{

{ UP, UP, UP, UP, UP, UP, UP },

{ FU, FU, FU, FU, FU, FU, FU },

{ FU, FU, FU, UP, FU, FU, FU },

{ FU, FU, FU, FU, FU, FU, FU },

},

{

{ UP, UP, UP, UP, UP, UP, UP },

{ FU, UP, FU, FU, FU, FU, FU },

{ FU, FU, FU, FU, FU, FU, FU },

{ FU, FU, FU, FU, FU, UP, FU },

},

{

{ UP, UP, UP, UP, UP, UP, UP },

{ FU, FU, FU, FU, FU, UP, FU },

{ FU, FU, FU, UP, FU, FU, FU },

{ FU, UP, FU, FU, FU, FU, FU },

},

{

{ UP, UP, UP, UP, UP, UP, UP },

{ FU, UP, FU, FU, FU, UP, FU },

{ FU, FU, FU, FU, FU, FU, FU },

{ FU, UP, FU, FU, FU, UP, FU },

},

{

{ UP, UP, UP, UP, UP, UP, UP },

{ FU, UP, FU, FU, FU, UP, FU },

{ FU, FU, FU, UP, FU, FU, FU },

{ FU, UP, FU, FU, FU, UP, FU },

},

{

{ UP, UP, UP, UP, UP, UP, UP },

{ FU, UP, FU, FU, FU, UP, FU },

{ FU, UP, FU, FU, FU, UP, FU },

{ FU, UP, FU, FU, FU, UP, FU },

}

};

/******************************************************************************
 *
 *****************************************************************************/

static WINDOW *_win;

static s_tarr *_tmpl;

/******************************************************************************
 *
 *****************************************************************************/
// TODO: e_dice_state => s_dice
#define D_STATUS_NUM 2

#define D_STAT_ACTIVE 0

#define D_STAT_INACTIVE 1

// TODO: colorpairs instead of colors
static short _colors[NUM_PLAYER][D_STATUS_NUM];

/******************************************************************************
 *
 *****************************************************************************/

void dice_init(WINDOW *win) {

	// TODO: DEBUG
	if (win == NULL) {
		log_exit_str("No window!");
	}

	_win = win;
	_tmpl = s_tarr_new(D_ROWS, D_COLS);

	_colors[OWNER_BLACK][D_STAT_ACTIVE] = s_color_def_hex_create("#0f0f0f");
	_colors[OWNER_BLACK][D_STAT_INACTIVE] = s_color_def_hex_create("#1f1f1f");

	_colors[OWNER_WHITE][D_STAT_ACTIVE] = s_color_def_hex_create("#e0e0e0");
	_colors[OWNER_WHITE][D_STAT_INACTIVE] = s_color_def_hex_create("#b0b0b0");
}

/******************************************************************************
 *
 *****************************************************************************/

void dice_free() {
	s_tarr_free(&_tmpl);
}

/******************************************************************************
 *
 *****************************************************************************/

static s_tarr* dice_get_tmpl(const int num, const short fg, const short bg) {
	s_tchar *tchar;

	for (int row = 0; row < D_ROWS; row++) {
		for (int col = 0; col < D_COLS; col++) {

			tchar = &s_tarr_get(_tmpl, row, col);
			tchar->chr = _tmpl_wchr[num - 1][row][col];
			tchar->fg = fg;
			tchar->bg = (row == 0) ? 0 : bg;
		}
	}

	return _tmpl;
}

// TODO: better solution
static void get_color(const s_status *status, const bool active, short *fg, short *bg) {

	const int idx = active ? D_STAT_ACTIVE : D_STAT_INACTIVE;

	if (status->turn == OWNER_BLACK) {
		*fg = _colors[OWNER_BLACK][idx];
		*bg = _colors[OWNER_WHITE][idx];
	} else {
		*fg = _colors[OWNER_WHITE][idx];
		*bg = _colors[OWNER_BLACK][idx];
	}
}

/******************************************************************************
 *
 *****************************************************************************/

void dice_print(const s_status *status) {
	s_tarr *tmpl;

	short fg, bg;

	get_color(status, status->active == 0, &fg, &bg);
	tmpl = dice_get_tmpl(status->dice_1, fg, bg);
	s_tarr_print(_win, tmpl, _pos_dice_1);

	get_color(status, status->active == 1, &fg, &bg);
	tmpl = dice_get_tmpl(status->dice_2, fg, bg);
	s_tarr_print(_win, tmpl, _pos_dice_2);

	wrefresh(_win);
}

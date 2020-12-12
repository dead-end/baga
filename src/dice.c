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
#include "dice.h"

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

//
// Lower half
//
#define LO L'\u2580'

/******************************************************************************
 * The elements have fixed positions inside the window.
 *****************************************************************************/

static const s_point _pos_dice_1 = { .row = 0, .col = 0 };

static const s_point _pos_dice_2 = { .row = 0, .col = D_COLS + D_PAD };

static const s_point _pos_undo = { .row = 0, .col = 2 * (D_COLS + D_PAD) };

static const s_point _pos_confim = { .row = 0, .col = 3 * (D_COLS + D_PAD) };

/******************************************************************************
 *
 *****************************************************************************/

static const wchar_t _tmpl_dices[6][D_ROWS][D_COLS] = {

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

static const wchar_t _tmpl_undo[D_ROWS][D_COLS] =

{

{ UP, UP, UP, UP, UP, UP, UP },

{ FU, UP, LO, FU, LO, UP, FU },

{ FU, FU, LO, UP, LO, FU, FU },

{ FU, UP, FU, FU, FU, UP, FU },

};

/******************************************************************************
 *
 *****************************************************************************/

static const wchar_t _tmpl_confirm[D_ROWS][D_COLS] =

{

{ UP, UP, UP, UP, UP, UP, UP },

{ FU, FU, FU, FU, FU, LO, FU },

{ FU, LO, FU, LO, UP, FU, FU },

{ FU, FU, UP, FU, FU, FU, FU },

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
static short _color_dices[NUM_PLAYER][D_STATUS_NUM];

static short _color_ctrl_undo[D_STATUS_NUM];

static short _color_ctrl_confirm[D_STATUS_NUM];

static short _color_ctrl_bg[D_STATUS_NUM];

/******************************************************************************
 *
 *****************************************************************************/

void dice_init(const s_game_cfg *game_cfg, WINDOW *win) {

#ifdef DEBUG

	//
	// Ensure that we initialized all in the correct order.
	//
	if (win == NULL) {
		log_exit_str("No window!");
	}
#endif

	_win = win;
	_tmpl = s_tarr_new(D_ROWS, D_COLS);

	_color_dices[OWNER_BLACK][D_STAT_ACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_black_active);
	_color_dices[OWNER_BLACK][D_STAT_INACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_black_inactive);

	_color_dices[OWNER_WHITE][D_STAT_ACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_white_active);
	_color_dices[OWNER_WHITE][D_STAT_INACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_white_inactive);

	_color_ctrl_confirm[D_STAT_ACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_confirm_active);
	_color_ctrl_confirm[D_STAT_INACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_confirm_inactive);

	_color_ctrl_undo[D_STAT_ACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_undo_active);
	_color_ctrl_undo[D_STAT_INACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_undo_inactive);

	_color_ctrl_bg[D_STAT_ACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_bg_active);
	_color_ctrl_bg[D_STAT_INACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_bg_inactive);
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
			tchar->chr = _tmpl_dices[num - 1][row][col];
			tchar->fg = fg;
			tchar->bg = (row == 0) ? 0 : bg;
		}
	}

	return _tmpl;
}

/******************************************************************************
 *
 *****************************************************************************/

static s_tarr* ctl_get_tmpl(const wchar_t _tmpl_ctl[D_ROWS][D_COLS], const short fg, const short bg) {
	s_tchar *tchar;

	for (int row = 0; row < D_ROWS; row++) {
		for (int col = 0; col < D_COLS; col++) {

			tchar = &s_tarr_get(_tmpl, row, col);
			tchar->chr = _tmpl_ctl[row][col];
			tchar->fg = fg;
			tchar->bg = (row == 0) ? 0 : bg;
		}
	}

	return _tmpl;
}

/******************************************************************************
 *
 *****************************************************************************/

static void get_color(const s_status *status, const bool active, short *fg, short *bg) {

	const int idx = active ? D_STAT_ACTIVE : D_STAT_INACTIVE;

	*fg = _color_dices[status->turn][idx];
	*bg = _color_dices[e_owner_other(status->turn)][idx];
}

/******************************************************************************
 *
 *****************************************************************************/

void dice_print(const s_status *status) {
	s_tarr *tmpl;

	short fg, bg;

	//
	// Dice: 0
	//
	get_color(status, status->dices[0].status == E_DICE_ACTIVE, &fg, &bg);
	tmpl = dice_get_tmpl(status->dices[0].value, fg, bg);
	s_tarr_print(_win, tmpl, _pos_dice_1);

	//
	// Dice: 1
	//
	get_color(status, status->dices[1].status == E_DICE_ACTIVE, &fg, &bg);
	tmpl = dice_get_tmpl(status->dices[1].value, fg, bg);
	s_tarr_print(_win, tmpl, _pos_dice_2);

	//
	// Control: undo
	//
	if (status->dices[0].status == E_DICE_SET || status->dices[1].status == E_DICE_SET) {
		bg = _color_ctrl_undo[D_STAT_ACTIVE];
		fg = _color_ctrl_bg[D_STAT_ACTIVE];

		tmpl = ctl_get_tmpl(_tmpl_undo, fg, bg);
		s_tarr_print(_win, tmpl, _pos_undo);

	} else {
		s_tarr_print_empty(_win, (s_point ) { D_ROWS, D_COLS }, _pos_undo);
	}

	//
	// Control: confirm
	//
	if (status->dices[0].status != E_DICE_ACTIVE && status->dices[1].status != E_DICE_ACTIVE) {
		bg = _color_ctrl_confirm[D_STAT_ACTIVE];
		fg = _color_ctrl_bg[D_STAT_ACTIVE];

		tmpl = ctl_get_tmpl(_tmpl_confirm, fg, bg);
		s_tarr_print(_win, tmpl, _pos_confim);

	} else {
		s_tarr_print_empty(_win, (s_point ) { D_ROWS, D_COLS }, _pos_confim);
	}

	wrefresh(_win);
}

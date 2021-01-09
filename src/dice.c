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
 * Definition of the characters to build the templates.
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

static const s_point _pos_dice_0 = { .row = 0, .col = 0 };

static const s_point _pos_dice_1 = { .row = 0, .col = D_COLS + D_PAD };

static const s_point _pos_undo = { .row = 0, .col = 2 * (D_COLS + D_PAD) };

static const s_point _pos_confim = { .row = 0, .col = 3 * (D_COLS + D_PAD) };

/******************************************************************************
 * The constant dimension of the templates.
 *****************************************************************************/

static const s_point _tmp_dim = { .row = D_ROWS, .col = D_COLS };

/******************************************************************************
 * The definition of the characters for the dices.
 *****************************************************************************/

static const wchar_t _tmpl_dices[6][D_ROWS][D_COLS] = {

{

// Dice: 1

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, FU, FU, FU, FU, FU, FU },

		{ FU, FU, FU, UP, FU, FU, FU },

		{ FU, FU, FU, FU, FU, FU, FU },

},

{

// Dice: 2

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, UP, FU, FU, FU, FU, FU },

		{ FU, FU, FU, FU, FU, FU, FU },

		{ FU, FU, FU, FU, FU, UP, FU },

},

{

// Dice: 3

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, FU, FU, FU, FU, UP, FU },

		{ FU, FU, FU, UP, FU, FU, FU },

		{ FU, UP, FU, FU, FU, FU, FU },

},

{

// Dice: 4

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, UP, FU, FU, FU, UP, FU },

		{ FU, FU, FU, FU, FU, FU, FU },

		{ FU, UP, FU, FU, FU, UP, FU },

},

{

// Dice: 5

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, UP, FU, FU, FU, UP, FU },

		{ FU, FU, FU, UP, FU, FU, FU },

		{ FU, UP, FU, FU, FU, UP, FU },

},

{

// Dice: 6

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, UP, FU, FU, FU, UP, FU },

		{ FU, UP, FU, FU, FU, UP, FU },

		{ FU, UP, FU, FU, FU, UP, FU },

}

};

/******************************************************************************
 * Definition of the characters of the undo button.
 *****************************************************************************/

static const wchar_t _tmpl_undo[D_ROWS][D_COLS] =

{

// Button: undo

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, UP, LO, FU, LO, UP, FU },

		{ FU, FU, LO, UP, LO, FU, FU },

		{ FU, UP, FU, FU, FU, UP, FU },

};

/******************************************************************************
 * Definition of the characters of the confirm button.
 *****************************************************************************/

static const wchar_t _tmpl_confirm[D_ROWS][D_COLS] =

{

// Button: confirm

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, FU, FU, FU, FU, LO, FU },

		{ FU, LO, FU, LO, UP, FU, FU },

		{ FU, FU, UP, FU, FU, FU, FU },

};

/******************************************************************************
 * The window for the dices and buttons.
 *****************************************************************************/

static WINDOW *_win;

/******************************************************************************
 * The template that is used for the dices and the buttons.
 *****************************************************************************/

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

	//
	// Create color: black
	//
	const int owner_black = e_player_color_2_owner(game_cfg->owner_top_color, PLAYER_COLOR_BLACK);
	_color_dices[owner_black][D_STAT_ACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_black_active);
	_color_dices[owner_black][D_STAT_INACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_black_inactive);

	//
	// Create color: white
	//
	const int owner_white = e_player_color_2_owner(game_cfg->owner_top_color, PLAYER_COLOR_WHITE);
	_color_dices[owner_white][D_STAT_ACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_white_active);
	_color_dices[owner_white][D_STAT_INACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_white_inactive);

	//
	// Create color: undo
	//
	_color_ctrl_undo[D_STAT_ACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_undo_active);
	_color_ctrl_undo[D_STAT_INACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_undo_inactive);

	//
	// Create color: confirm
	//
	_color_ctrl_confirm[D_STAT_ACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_confirm_active);
	_color_ctrl_confirm[D_STAT_INACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_confirm_inactive);

	//
	// Create color: background
	//
	_color_ctrl_bg[D_STAT_ACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_bg_active);
	_color_ctrl_bg[D_STAT_INACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_bg_inactive);
}

/******************************************************************************
 * The function does the freeing of the resources.
 *****************************************************************************/

void dice_free() {

	s_tarr_free(&_tmpl);
}

/******************************************************************************
 * The function is called with template of character, the forground and the
 * background colors.
 *****************************************************************************/

static s_tarr* dice_update_tmpl(const wchar_t _tmpl_chr[D_ROWS][D_COLS], const short fg, const short bg) {
	s_tchar *tchar;

	for (int row = 0; row < D_ROWS; row++) {
		for (int col = 0; col < D_COLS; col++) {

			tchar = &s_tarr_get(_tmpl, row, col);
			tchar->chr = _tmpl_chr[row][col];
			tchar->fg = fg;
			tchar->bg = (row == 0) ? 0 : bg;
		}
	}

	return _tmpl;
}

/******************************************************************************
 * The function sets the colors for the dices.
 *****************************************************************************/

static void dice_get_color(const s_status *status, const bool active, short *fg, short *bg) {

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
	dice_get_color(status, s_dices_has_status(status->dices, 0, E_DICE_ACTIVE), &fg, &bg);
	tmpl = dice_update_tmpl(_tmpl_dices[status->dices.dice[0].value - 1], fg, bg);
	s_tarr_print(_win, tmpl, _pos_dice_0);

	//
	// Dice: 1
	//
	dice_get_color(status, s_dices_has_status(status->dices, 1, E_DICE_ACTIVE), &fg, &bg);
	tmpl = dice_update_tmpl(_tmpl_dices[status->dices.dice[1].value - 1], fg, bg);
	s_tarr_print(_win, tmpl, _pos_dice_1);

	//
	// Control: undo
	//
	if (s_dices_can_undo(status->dices)) {
		bg = _color_ctrl_undo[D_STAT_ACTIVE];
		fg = _color_ctrl_bg[D_STAT_ACTIVE];
		tmpl = dice_update_tmpl(_tmpl_undo, fg, bg);
		s_tarr_print(_win, tmpl, _pos_undo);

	} else {
		s_tarr_print_empty(_win, _tmp_dim, _pos_undo);
	}

	//
	// Control: confirm
	//
	if (s_dices_is_done(status->dices)) {
		bg = _color_ctrl_confirm[D_STAT_ACTIVE];
		fg = _color_ctrl_bg[D_STAT_ACTIVE];
		tmpl = dice_update_tmpl(_tmpl_confirm, fg, bg);
		s_tarr_print(_win, tmpl, _pos_confim);

	} else {
		s_tarr_print_empty(_win, _tmp_dim, _pos_confim);
	}

	wrefresh(_win);
}

/******************************************************************************
 * The function processes mouse events. The events are relative to the window.
 *****************************************************************************/

void dice_process_event(s_status *status, const s_point *event) {

	//
	// Target: dice action: toogle
	//
	if (s_point_is_inside(&_pos_dice_0, &_tmp_dim, event)) {

		if (s_dice_toogle_active(&status->dices, 0)) {
			dice_print(status);
		}
	}

	//
	// Target: dice action: toogle
	//
	else if (s_point_is_inside(&_pos_dice_1, &_tmp_dim, event)) {

		if (s_dice_toogle_active(&status->dices, 1)) {
			dice_print(status);
		}
	}

	else if (s_point_is_inside(&_pos_undo, &_tmp_dim, event)) {
		log_debug_str("on undo");
	}

	else if (s_point_is_inside(&_pos_confim, &_tmp_dim, event)) {
		log_debug_str("on confirm");
	}

	else {
		log_debug("outside: %d/%d", event->row, event->col);
	}
}

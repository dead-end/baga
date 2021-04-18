/*
 * MIT License
 *
 * Copyright (c) 2021 dead-end
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

/******************************************************************************
 * The source file implements the window with the 4 buttons (2 dices, undo and
 * confirm). All 4 buttons have the same size and are build from a template
 * with characters and a foreground and background color, which reflect the
 * status of the button.
 *
 * TODO: undo button does not do anything.
 * TODO: confirm button missing
 * TODO: rename to controls
 *****************************************************************************/

#include <wchar.h>

#include "../inc/controls.h"
#include "s_color_def.h"
#include "lib_logging.h"
#include "s_tarr.h"

/******************************************************************************
 * The buttons are build with the following characters.
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
 * The buttons have fixed positions inside the window.
 *****************************************************************************/

static const s_point _pos_dice_0 = { .row = 0, .col = 0 };

static const s_point _pos_dice_1 = { .row = 0, .col = D_COLS + D_PAD };

static const s_point _pos_undo = { .row = 0, .col = 2 * (D_COLS + D_PAD) };

static const s_point _pos_confim = { .row = 0, .col = 3 * (D_COLS + D_PAD) };

/******************************************************************************
 * The constant dimension of the buttons (and their templates).
 *****************************************************************************/

static const s_point _tmp_dim = { .row = D_ROWS, .col = D_COLS };

/******************************************************************************
 * The definition of the 6 templates of the dices, which show the values from
 * 1 to 6.
 *****************************************************************************/

static const wchar_t _tmpl_dices[6][D_ROWS][D_COLS] = {

{

// Dice value: 1

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, FU, FU, FU, FU, FU, FU },

		{ FU, FU, FU, UP, FU, FU, FU },

		{ FU, FU, FU, FU, FU, FU, FU },

},

{

// Dice value: 2

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, UP, FU, FU, FU, FU, FU },

		{ FU, FU, FU, FU, FU, FU, FU },

		{ FU, FU, FU, FU, FU, UP, FU },

},

{

// Dice value: 3

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, FU, FU, FU, FU, UP, FU },

		{ FU, FU, FU, UP, FU, FU, FU },

		{ FU, UP, FU, FU, FU, FU, FU },

},

{

// Dice value: 4

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, UP, FU, FU, FU, UP, FU },

		{ FU, FU, FU, FU, FU, FU, FU },

		{ FU, UP, FU, FU, FU, UP, FU },

},

{

// Dice value: 5

		{ UP, UP, UP, UP, UP, UP, UP },

		{ FU, UP, FU, FU, FU, UP, FU },

		{ FU, FU, FU, UP, FU, FU, FU },

		{ FU, UP, FU, FU, FU, UP, FU },

},

{

// Dice value: 6

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
 * The template that is used for the dices and the buttons. (The dices are
 * buttons).
 *****************************************************************************/

static s_tarr *_button_tmpl;

/******************************************************************************
 * The enum defines the status of a button (dice, confirm, undo).
 *
 * TODO: maybe it makes sense to add a E_BUTTON_DISABLED status, for dices that
 * are set, or dices that are impossible.
 *****************************************************************************/

typedef enum {

	//
	// The status of an active, usable button.
	//
	E_BUTTON_ACTIVE = 0,

	//
	// The status of an inactive button.
	//
	E_BUTTON_INACTIVE = 1,

} e_button_status;

#define BUTTON_STATUS_MAX 2

/******************************************************************************
 * The definition of the foreground and background colors of the buttons. Each
 * button has a pair for each status.
 *
 * TODO: colorpairs instead of colors
 *****************************************************************************/

//
// The definition of the foreground colors for the dices for each player (for
// the stati)
//
static short _color_dices[NUM_PLAYER][BUTTON_STATUS_MAX];

//
// The definition of the foreground color of the undo button (for the stati).
//
static short _color_ctrl_undo[BUTTON_STATUS_MAX];

//
// The definition of the foreground color of the confirm button (for the
// stati).
//
static short _color_ctrl_confirm[BUTTON_STATUS_MAX];

//
// The definition of the background color for all buttons (for the stati).
//
static short _color_ctrl_bg[BUTTON_STATUS_MAX];

/******************************************************************************
 * The initialize function allocates the memory for the template and gets the
 * colors for the buttons.
 *****************************************************************************/

void controls_init(const s_game_cfg *game_cfg, WINDOW *win) {

#ifdef DEBUG

	//
	// Ensure that we initialized all in the correct order.
	//
	if (win == NULL) {
		log_exit_str("No window!");
	}
#endif

	_win = win;

	_button_tmpl = s_tarr_new(D_ROWS, D_COLS);

	//
	// Create color: black
	//
	const int owner_black = e_player_color_2_owner(game_cfg->owner_top_color, PLAYER_COLOR_BLACK);
	_color_dices[owner_black][E_BUTTON_ACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_black_active);
	_color_dices[owner_black][E_BUTTON_INACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_black_inactive);

	//
	// Create color: white
	//
	const int owner_white = e_player_color_2_owner(game_cfg->owner_top_color, PLAYER_COLOR_WHITE);
	_color_dices[owner_white][E_BUTTON_ACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_white_active);
	_color_dices[owner_white][E_BUTTON_INACTIVE] = s_color_def_hex_create(game_cfg->clr_dice_white_inactive);

	//
	// Create color: undo
	//
	_color_ctrl_undo[E_BUTTON_ACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_undo_active);
	_color_ctrl_undo[E_BUTTON_INACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_undo_inactive);

	//
	// Create color: confirm
	//
	_color_ctrl_confirm[E_BUTTON_ACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_confirm_active);
	_color_ctrl_confirm[E_BUTTON_INACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_confirm_inactive);

	//
	// Create color: background
	//
	_color_ctrl_bg[E_BUTTON_ACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_bg_active);
	_color_ctrl_bg[E_BUTTON_INACTIVE] = s_color_def_hex_create(game_cfg->clr_ctrl_bg_inactive);
}

/******************************************************************************
 * The function does the freeing of the resources.
 *****************************************************************************/

void controls_free() {

	s_tarr_free(&_button_tmpl);
}

/******************************************************************************
 * We have 8 character templates. 6 templates for the dice numbers and one for
 * the undo and one for the confirm button. The 4 buttons (2 dices, undo,
 * confirm) consists of one of the character templates with a foreground and
 * background color, reflecting the state of the button.
 *
 * The function copies the characters from the template with a foreground and
 * background color to the button template.
 *****************************************************************************/

static void update_button_tmpl(s_tarr *tmpl_button, const wchar_t tmpl_chars[D_ROWS][D_COLS], const short fg, const short bg) {
	s_tchar *tchar;

	for (int row = 0; row < D_ROWS; row++) {
		for (int col = 0; col < D_COLS; col++) {

			tchar = &s_tarr_get(tmpl_button, row, col);

			tchar->chr = tmpl_chars[row][col];
			tchar->fg = fg;
			tchar->bg = (row == 0) ? 0 : bg;
		}
	}
}

/******************************************************************************
 * The function sets the colors for the dices. The colors depend on the player
 * (black / white) and the status (active / inactive).
 *
 * TODO: maybe we need a color for not possible.
 *****************************************************************************/

static void controls_get_color(const s_status *status, const bool active, short *fg, short *bg) {

	const int idx = active ? E_BUTTON_ACTIVE : E_BUTTON_INACTIVE;

	*fg = _color_dices[status->turn][idx];
	*bg = _color_dices[e_owner_other(status->turn)][idx];
}

/******************************************************************************
 * The function is called with the status struct and prints the window with the
 * 4 buttons (2 dices, undo and confirm).
 *****************************************************************************/

void controls_print(const s_status *status) {

	short fg, bg;

	//
	// Dice: 0
	//
	controls_get_color(status, s_dices_has_status(status->dices, 0, E_DICE_ACTIVE), &fg, &bg);
	update_button_tmpl(_button_tmpl, _tmpl_dices[status->dices.dice[0].value - 1], fg, bg);
	s_tarr_print(_win, _button_tmpl, _pos_dice_0);

	//
	// Dice: 1
	//
	controls_get_color(status, s_dices_has_status(status->dices, 1, E_DICE_ACTIVE), &fg, &bg);
	update_button_tmpl(_button_tmpl, _tmpl_dices[status->dices.dice[1].value - 1], fg, bg);
	s_tarr_print(_win, _button_tmpl, _pos_dice_1);

	//
	// Control: undo
	//
	if (s_dices_can_undo(status->dices)) {
		bg = _color_ctrl_undo[E_BUTTON_ACTIVE];
		fg = _color_ctrl_bg[E_BUTTON_ACTIVE];
		update_button_tmpl(_button_tmpl, _tmpl_undo, fg, bg);
		s_tarr_print(_win, _button_tmpl, _pos_undo);

	} else {
		s_tarr_print_empty(_win, _tmp_dim, _pos_undo);
	}

	//
	// Control: confirm
	//
	if (s_dices_is_done(status->dices)) {
		bg = _color_ctrl_confirm[E_BUTTON_ACTIVE];
		fg = _color_ctrl_bg[E_BUTTON_ACTIVE];
		update_button_tmpl(_button_tmpl, _tmpl_confirm, fg, bg);
		s_tarr_print(_win, _button_tmpl, _pos_confim);

	} else {
		s_tarr_print_empty(_win, _tmp_dim, _pos_confim);
	}

	wrefresh(_win);
}

/******************************************************************************
 * The function processes mouse events. The events are relative to the window,
 * which contains only buttons, so we need to check which button is clicked.
 *
 * The function returns true, if the board needs to be updated (due to reset
 * request).
 *****************************************************************************/

bool controls_process_event(s_fieldset *fieldset, s_status *status, const s_point *event) {

	//
	// Target: dice action: toogle
	//
	if (s_point_is_inside(&_pos_dice_0, &_tmp_dim, event)) {

		if (s_dice_toggle_active(&status->dices, 0)) {
			controls_print(status);
		}
	}

	//
	// Target: dice action: toogle
	//
	else if (s_point_is_inside(&_pos_dice_1, &_tmp_dim, event)) {

		if (s_dice_toggle_active(&status->dices, 1)) {
			controls_print(status);
		}
	}

	else if (s_point_is_inside(&_pos_undo, &_tmp_dim, event)) {
		// TODO: no undo button
		log_debug_str("on undo");

		if (s_dices_can_undo(status->dices)) {
			s_status_undo_reset(fieldset, status);
			controls_print(status);
			return true;
		}
	}

	//
	// Target: confirm button (position)
	//
	else if (s_point_is_inside(&_pos_confim, &_tmp_dim, event)) {

		if (!s_status_need_confirm(status)) {
			return false;
		}

		s_status_do_confirm(status);

		s_status_undo_save(fieldset, status);

		controls_print(status);
	}

	else {
		log_debug("outside: %d/%d", event->row, event->col);
	}

	return false;
}

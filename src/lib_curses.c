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

#include <ncurses.h>
#include <menu.h>

#include "lib_logging.h"
#include "lib_string.h"
#include "lib_utils.h"

/******************************************************************************
 * The function initializes the main features of ncurses. It does not start
 * mouse support.
 *****************************************************************************/

void lc_curses_init() {

	//
	// Initialize screen.
	//
	if (initscr() == NULL) {
		log_exit_str("Unable to initialize the screen.");
	}

	//
	// Disable line buffering.
	//
	if (raw() == ERR) {
		log_exit_str("Unable to set raw mode.");
	}

	//
	// Disable echoing.
	//
	if (noecho() == ERR) {
		log_exit_str("Unable to switch off echoing.");
	}

	//
	// Enable keypad for the terminal.
	//
	if (keypad(stdscr, TRUE) == ERR) {
		log_exit_str("Unable to enable the keypad of the terminal.");
	}

	//
	// Switch off the cursor.
	//
	if (curs_set(0) == ERR) {
		log_exit_str("Unable to set cursor visibility.");
	}

	//
	// Disable ESC delay. (The man page says: "These functions all return TRUE
	// or FALSE, except as noted." which seems not to be correct.
	//
	set_escdelay(0);

	//
	// Start color support
	//
	if (start_color() == ERR) {
		log_exit_str("Unable to start color!");
	}
}

/******************************************************************************
 * The function finishes the main features of ncurses .
 *****************************************************************************/

void lc_curses_finish() {

	//
	// Reset the terminal.
	//
	if (!isendwin()) {

		log_debug_str("Calling: endwin()");

		endwin();
	}
}

/******************************************************************************
 * The function initializes the ncurses mouse support.
 *****************************************************************************/

void lc_mouse_init(const mmask_t mask, const bool moving) {

	if (has_mouse()) {
		log_exit_str("Terminal does not support a mouse!");
	}

	//
	// The maximum time that can elapse between press and release events for
	// them to be recognized  as  a click. (no error handling)
	//
	mouseinterval(0);

	//
	// Register mouse events (which do not have a propper error handling)
	//
	mousemask(mask, NULL);

	if (moving) {
		printf("\033[?1003h\n");
	}

}

/******************************************************************************
 * The function finishes the ncurses mouse support.
 *****************************************************************************/

void lc_mouse_finish(const bool moving) {

	//
	// Disable mouse movement events, as l = low
	//
	if (moving) {
		printf("\033[?1003l\n");
	}
}

/******************************************************************************
 * The function creates a window with the size of the stdscr.
 *
 * (Not used)
 *****************************************************************************/

WINDOW* lc_win_create_full() {

	WINDOW *win = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);

	if (win == NULL) {
		log_exit_str("Unable to create win!");
	}

	log_debug("new win: %d/%d", getmaxy(win), getmaxx(win));

	return win;
}

/******************************************************************************
 * The function frees a window. It is a simple wrapper with error handling.
 *****************************************************************************/

void lc_win_del(WINDOW *win) {

	log_debug("Delete window (is null: %s)", ls_bool_str(win == NULL));

	//
	// Ensure that the window is initialized.
	//
	if (win == NULL) {
		return;
	}

	//
	// Free the windows.
	//
	if (delwin(win) != OK) {
		log_exit_str("Unable to delete the window!");
	}
}

/******************************************************************************
 * The function refreshes a window. It is a simple wrapper with error handling.
 *****************************************************************************/

void lc_win_refresh(WINDOW *win) {

	//
	// Ensure that the window is initialized.
	//
	if (win == NULL) {
		return;
	}

	if (wrefresh(win) == ERR) {
		log_exit_str("Unable to refresh window!");
	}
}

/******************************************************************************
 * The function checks whether a row / column is inside a window or not.
 *****************************************************************************/

bool lc_win_is_inside(WINDOW *win, const int row, const int col) {

	const int start_row = getbegy(win);

	if (row < start_row || row >= start_row + getmaxy(win)) {
		return false;
	}

	const int start_col = getbegx(win);

	if (col < start_col || col >= start_col + getmaxx(win)) {
		return false;
	}

	return true;
}

/******************************************************************************
 * The function centers an area inside a window.
 *****************************************************************************/

s_point lc_win_pos_center(WINDOW *win_parent, const s_point *dim) {

	const s_point result = {

	.row = lu_center(getmaxy(win_parent), dim->row),

	.col = lu_center(getmaxx(win_parent), dim->col)

	};

	log_debug("Win: %d/%d centered: %d/%d", getmaxy(win_parent), getmaxx(win_parent), result.row, result.col);

	return result;
}

/******************************************************************************
 * The function centers an area inside a window at its bottom.
 *****************************************************************************/

s_point lc_win_pos_center_bottom(WINDOW *win_parent, const s_point *dim) {

	const s_point result = {

	.row = getmaxy(win_parent) - dim->row,

	.col = lu_center(getmaxx(win_parent), dim->col)

	};

	log_debug("Win: %d/%d centered: %d/%d", getmaxy(win_parent), getmaxx(win_parent), result.row, result.col);

	return result;

}

/******************************************************************************
 * The function returns the index of the current item of a menu.
 *****************************************************************************/

int lc_menu_cur_item_idx(MENU *menu) {

	//
	// Get the currently selected item of the menu
	//
	const ITEM *item = current_item(menu);
	if (item == NULL) {
		log_exit_str("Unable to get current item!");
	}

	//
	// Get the index of the item
	//
	const int idx = item_index(item);
	if (idx == ERR) {
		log_exit_str("Unable to get current item index!");
	}

	log_debug("Current item: %s with index: %d", item_name(item), idx);

	return idx;
}

/******************************************************************************
 * The function sets the index of the currently selected item in a menu. It is
 * ensured that the index is valid.
 *****************************************************************************/

void lc_menu_set_cur_item_idx(MENU *menu, const int idx) {

	//
	// Get the number of items in the menu
	//
	const int count = item_count(menu);
	if (count == ERR) {
		log_exit_str("Unable to count items!");
	}

	//
	// Ensure that the index is valid
	//
	if (idx < 0 || count - 1 < idx) {
		log_exit("Item index: %d out of range - count: %d", idx, count);
	}

	//
	// Get the array of the items
	//
	ITEM **items = menu_items(menu);
	if (items == NULL) {
		log_exit_str("Unable to get items from the menu!");
	}

	//
	// Set the item index
	//
	if (set_current_item(menu, items[idx]) != E_OK) {
		log_exit("Unable to set the item index: %d", idx);
	}
}

/******************************************************************************
 * The function maps the event coordinates from the stdscr to a given window.
 * The function returns false, if the event is not inside the window.
 *****************************************************************************/

bool lc_event_stdscr_to_win(WINDOW *win, const int e_row, const int e_col, s_point *rel) {

	s_point_set_ptr(rel, e_row, e_col);

	//
	// Transform from stdscr to win, if the event is inside the window.
	//
	if (!wmouse_trafo(win, &rel->row, &rel->col, false)) {
		log_debug("Event: %d/%d not inside the window", e_row, e_col);
		return false;
	}

	log_debug("before: %d/%d after: %d/%d", e_row, e_col, rel->row, rel->col);

	return true;
}


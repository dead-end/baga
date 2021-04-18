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

#include <locale.h>
#include <s_board_areas.h>

#include "lib_logging.h"
#include "lib_curses.h"
#include "lib_string.h"
#include "lib_popup.h"
#include "s_fieldset.h"
#include "nc_board.h"
#include "layout.h"
#include "dice.h"

static const char *headers[] = {

" _______            __                                              ",

"|   _   \\---.-.----|  |--.-----.---.-.--------.--------.-----.-----.",

"|.  1   |  _  |  __|    <|  _  |  _  |        |        |  _  |     |",

"|.  _   |___._|____|__|__|___  |___._|__|__|__|__|__|__|_____|__|__|",

"|:  1    \\               |_____|",

"|::.. .  /",

"`-------'",

"",

NULL };

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory.
 *****************************************************************************/

static void exit_callback() {

	layout_free();

	dice_free();

	nc_board_free();

	//
	// Finish ncurses stuff
	//
	lc_mouse_finish(true);

	lc_curses_finish();
}

/******************************************************************************
 * The method initializes the application.
 *****************************************************************************/

static void init() {

	//
	// Set the locale to allow utf8.
	//
	if (setlocale(LC_CTYPE, "") == NULL) {
		log_exit_str("Unable to set the locale.");
	}

	//
	// Register callback for log_exit calls.
	//
	log_atexit(exit_callback);

	//
	// Initialize ncurses stuff
	//
	lc_curses_init();

	lc_mouse_init(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, true);
}

/******************************************************************************
 *
 *****************************************************************************/

void show_menu() {
	log_debug_str("Showing start menu");

	//
	// Initialize the choices array.
	//
	const char *choices[3] = { "New Game", "Exit" };

	//
	// Process the menu (the second parameter is a flag to ignore ESC)
	//
	const int idx = lp_process_menu(headers, choices, 0, true);

	if (idx == 1) {
		exit(EXIT_SUCCESS);

	} else if (idx == 0) {
		log_debug_str("New game!");

	} else {
		log_exit("Unknown index: %d", idx);
	}
}

/******************************************************************************
 * The main function.
 *****************************************************************************/

int main() {
	s_status status;
	s_point m_event;
	s_game_cfg game_cfg;

	MEVENT event;
	s_field_id field_id;

	log_debug_str("Starting baga...");

	init();

	// TODO: sort init functions
	s_game_cfg_init(&game_cfg);

	s_dices_init();

	s_status_init(&status, &game_cfg);

	const s_board_areas *board_areas = s_board_areas_init();

	layout_init(board_areas->board_dim, (s_point ) { .row = D_ROWS, .col = D_COLS * 4 + D_PAD * 3 });

	dice_init(&game_cfg, layout_win_dice());

	//
	// Initialize the ncurses board function
	//
	nc_board_init(layout_win_board(), &game_cfg, board_areas);

	//
	// Write the whole windows.
	//
	wrefresh(stdscr);

	show_menu();

	//
	// Show dice window
	// TODO: do more than a colored window
	//
//	wrefresh(layout_win_dice());

	//
	// Initialize the game board function
	//
	s_fieldset fieldset;

	s_fieldset_new_game(&fieldset);

	//
	// Reset the status for a new game.
	//
	s_status_start(&status);
	// TODO: part of setting the dice, so that it is called implicitly.
	s_status_undo_save(&fieldset, &status);

	//
	// Print
	//
	nc_board_points_add_checker(&fieldset);

	nc_board_print_win();

	dice_print(&status);

	log_debug_str("Ending baga...");

	for (;;) {
		int c = wgetch(stdscr);

		//
		// Exit with 'q'
		//
		if (c == 'q') {
			break;
		}

		if (c == ERR) {
			log_debug_str("Nothing happened.");
			continue;
		}

		if (c == KEY_RESIZE) {

			log_debug_str("reseize");

			nc_board_print_win();

		} else if (c == 27) {

			log_debug_str("esc");

		} else {

			switch (c) {

			case KEY_MOUSE:

				if (getmouse(&event) != OK) {

					//
					// Report the details of the mouse event.
					//
					log_exit("Unable to get mouse event! mask: %s coords: %d/%d/%d", ls_bool_str(event.bstate == 0), event.x, event.y, event.z);
				}

				if (event.bstate & BUTTON1_PRESSED) {

					if (lc_event_stdscr_to_win(layout_win_board(), event.y, event.x, &m_event)) {

						// TODO: check if a dice is active (this is done but not obvious from the macro name).
						if (s_status_need_confirm(&status)) {
							continue;
						}

						s_board_areas_mouse_target(m_event, &field_id);

						if (field_id.type != E_FIELD_NONE) {
							nc_board_process(&fieldset, &status, field_id);
						}

					} else if (lc_event_stdscr_to_win(layout_win_dice(), event.y, event.x, &m_event)) {

						if (dice_process_event(&fieldset, &status, &m_event)) {
							nc_board_reset(&fieldset);
						}
					}

					continue;
				} else {
					continue;
				}

				break;

			case KEY_UP:
				log_debug_str("KEY_UP");
				break;

			case KEY_DOWN:
				log_debug_str("KEY_DOWN");
				break;

			case KEY_LEFT:
				log_debug_str("KEY_LEFT");
				break;

			case KEY_RIGHT:
				log_debug_str("KEY_RIGHT");
				break;

			case '\t':
				log_debug_str("tab");
				break;

			case 10:
				log_debug_str("enter");

				break;

			default:
				log_debug("Pressed key %d (%s)", c, keyname(c));
				continue;
			}
		}

		//
		// Do a refresh
		//
		// TODO
	}

	exit(EXIT_SUCCESS);
}

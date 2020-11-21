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

#include <bg_board.h>
#include <locale.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>

#include "nc_board.h"

#include "lib_string.h"
#include "lib_logging.h"
#include "s_pos.h"

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory.
 *****************************************************************************/

static void exit_callback() {

	nc_board_free();

	endwin();
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

	if (initscr() == NULL) {
		log_exit_str("Unable to init screen!");
	}

	if (start_color() == ERR) {
		log_exit_str("Unable to start color!");
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

	if (has_mouse()) {
		log_exit_str("Terminal does not support a mouse!");
	}

	//
	// Register mouse events (which do not have a propper error handling)
	//
	mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);

	mouseinterval(0);
}

/******************************************************************************
 * The main function.
 *****************************************************************************/

int main() {
	MEVENT event;
	s_pos_idx pos_idx;

	log_debug_str("Starting baga...");

	init();

	const s_board_areas *board_areas = s_pos_init();

	//
	// Initialize the ncurses board function
	//
	nc_board_init(board_areas);

	//
	// Initialize the game board function
	//
	s_bg_board bg_board;

	bg_board_new_game(&bg_board);

	bg_board_print(&bg_board);

	//
	// Print the initialized board
	//
	nc_board_print();

	getch();

	nc_board_test();

	getch();

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

			nc_board_print();

		} else if (c == 27) {

			log_debug_str("esc");

		} else {

			switch (c) {

			case KEY_MOUSE:
				log_debug_str("mouse");

				if (getmouse(&event) != OK) {

					//
					// Report the details of the mouse event.
					//
					log_exit("Unable to get mouse event! mask: %s coords: %d/%d/%d", bool_str(event.bstate == 0), event.x, event.y, event.z);
				}

				if (event.bstate & BUTTON1_PRESSED) {
					//const s_point event_point = { event.y, event.x };

					s_pos_mouse_target((s_point ) { event.y, event.x }, &pos_idx);

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

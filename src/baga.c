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

#include "lib_logging.h"

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory.
 *****************************************************************************/

static void exit_callback() {

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

	curs_set(0);
}

/******************************************************************************
 * The main function.
 *****************************************************************************/

int main() {
	log_debug_str("Starting baga...");

	init();

	//
	// Initialize the ncurses board function
	//
	nc_board_init();

	//
	// Initialize the game board function
	//
	s_bg_board bg_board;
	bg_board_new_game(&bg_board);

	s_board_points_add_checkers(0, OWNER_BLACK, 1);
	s_board_points_add_checkers(1, OWNER_BLACK, 2);
	s_board_points_add_checkers(2, OWNER_BLACK, 3);
	s_board_points_add_checkers(3, OWNER_BLACK, 4);
	s_board_points_add_checkers(4, OWNER_BLACK, 5);
	s_board_points_add_checkers(5, OWNER_BLACK, 6);
	s_board_points_add_checkers(6, OWNER_BLACK, 7);
	s_board_points_add_checkers(7, OWNER_BLACK, 8);
	s_board_points_add_checkers(8, OWNER_BLACK, 9);
	s_board_points_add_checkers(9, OWNER_BLACK, 10);
	s_board_points_add_checkers(10, OWNER_BLACK, 11);
	s_board_points_add_checkers(11, OWNER_BLACK, 12);

//

	s_board_points_add_checkers(12, OWNER_WHITE, 1);
	s_board_points_add_checkers(13, OWNER_WHITE, 2);
	s_board_points_add_checkers(14, OWNER_WHITE, 3);
	s_board_points_add_checkers(15, OWNER_WHITE, 4);
	s_board_points_add_checkers(16, OWNER_WHITE, 5);
	s_board_points_add_checkers(17, OWNER_WHITE, 6);

	//s_board_points_add_checkers(1, OWNER_WHITE, 7);

	s_board_points_add_checkers(18, OWNER_WHITE, 7);
	s_board_points_add_checkers(19, OWNER_WHITE, 8);
	s_board_points_add_checkers(20, OWNER_WHITE, 9);
	s_board_points_add_checkers(21, OWNER_WHITE, 10);
	s_board_points_add_checkers(22, OWNER_WHITE, 11);
	s_board_points_add_checkers(23, OWNER_WHITE, 12);

	//
	// Print the initialized board
	//
	nc_board_print();

	getch();

	log_debug_str("Ending baga...");
}

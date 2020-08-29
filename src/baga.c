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

#include "lib_logging.h"

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory.
 *****************************************************************************/

static void exit_callback() {

	log_debug_str("Dummy callback");
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
}

/******************************************************************************
 * The main function.
 *****************************************************************************/

int main() {
	log_debug_str("Starting baga...");

	init();

	log_debug_str("Ending baga...");
}

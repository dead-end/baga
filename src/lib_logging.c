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

#include <stdarg.h>

#include "lib_logging.h"

/******************************************************************************
 * Error handling with ncurses has special challenges. If ncurses is active
 * printing to stdout or stderr is not visible. To do a proper error handling
 * we have to stop ncurses before we can write a log message. Stopping ncurses
 * can be implemented in callback function. If an error occurs inside the
 * callback function, we end in an infinite loop. To face this challenge, we
 * store the callback function and register a wrapper function as an exit
 * function. The wrapper function ensures that the callback function is called
 * only once.
 *****************************************************************************/

static void (*_callback_fct_ptr)() = NULL;

/******************************************************************************
 * The wrapper function ensures that the callback function is called only once.
 *****************************************************************************/

static void log_atexit_wrapper() {

	//
	// If a callback function is stored, we call it.
	//
	if (_callback_fct_ptr != NULL) {

		//
		// Set the stored function pointer to NULL before we call the function.
		//
		void (*tmp)() = _callback_fct_ptr;

		_callback_fct_ptr = NULL;

		//
		// Call the callback function.
		//
		(*tmp)();
	}
}

/******************************************************************************
 * The function stores the pointer to the callback function and registers the
 * wrapper function that will be called on exit.
 *****************************************************************************/

void log_atexit(void (*atexit_ptr)()) {

	//
	// Register the wrapper function.
	//
	if (atexit(log_atexit_wrapper) != 0) {
		log_exit_str("Unable to register exit function!");
	}

	//
	// Store the callback function.
	//
	_callback_fct_ptr = atexit_ptr;
}

/******************************************************************************
 * In case of an error, we call the wrapper function fist. The wrapper function
 * calls the callback function if the callback function has not been called. If
 * the callback function has been called before, the callback function does
 * nothing.
 *
 * The callback function stops ncurses. Then writing the log message is
 * possible.
 *****************************************************************************/

void log_fatal(FILE *stream, const char *fmt, ...) {

	//
	// Call the wrapper function
	//
	log_atexit_wrapper();

	//
	// Print the error message.
	//
	va_list argp;
	va_start(argp, fmt);

	vfprintf(stream, fmt, argp);

	va_end(argp);
}

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

#ifndef INC_LIB_LOGGING_H_
#define INC_LIB_LOGGING_H_

#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * Definition of the debug logging macro. It is only defined with DEBUG
 * defined. It prints to stderr not to restrain curses.
 *****************************************************************************/

#ifdef DEBUG

#define log_debug(fmt, ...) fprintf(stderr, "DEBUG %s:%d:%s() " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define log_debug_str(fmt)  fprintf(stderr, "DEBUG %s:%d:%s() " fmt "\n", __FILE__, __LINE__, __func__)

#define DEBUG_USED

#else

#define log_debug(fmt, ...)
#define log_debug_str(fmt)

//
// Can be used for function parameters, that are only used in debug mode.
//
#define DEBUG_USED __attribute__((unused))

#endif

/******************************************************************************
 * Definition of the error logging macro. The macro terminates the program
 * after printing the message.
 *****************************************************************************/

#define log_exit(fmt, ...) log_fatal(stderr, "FATAL %s:%d:%s() " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); exit(EXIT_FAILURE)
#define log_exit_str(fmt)  log_fatal(stderr, "FATAL %s:%d:%s() " fmt "\n", __FILE__, __LINE__, __func__); exit(EXIT_FAILURE)

/******************************************************************************
 * The definition of the functions.
 *****************************************************************************/

void log_fatal(FILE *stream, const char *fmt, ...);

void log_atexit(void (*exit_callback_ptr)());

#endif /* INC_LIB_LOGGING_H_ */

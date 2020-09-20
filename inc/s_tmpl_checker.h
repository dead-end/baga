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

#ifndef INC_S_TMPL_CHECKER_H_
#define INC_S_TMPL_CHECKER_H_

#include "bg_defs.h"
#include "lib_s_tchar.h"

/******************************************************************************
 * The struct contains the character and color definition of a checker.
 *****************************************************************************/

typedef struct {

	s_tchar tchar[CHECKER_ROW][CHECKER_COL];

} s_checker_tchar;

/******************************************************************************
 * The struct contains the definition of the checkers for the two players.
 *****************************************************************************/

typedef struct {

	s_checker_tchar checker[NUM_PLAYER];

} s_tmpl_checker;

/******************************************************************************
 * Declaration of the functions.
 *****************************************************************************/

void s_tmpl_checker_init(s_tmpl_checker *templ_checker);

#endif /* INC_S_TMPL_CHECKER_H_ */

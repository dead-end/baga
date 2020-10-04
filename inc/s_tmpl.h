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

#ifndef INC_S_TMPL_H_
#define INC_S_TMPL_H_

#include "lib_s_point.h"
#include "lib_s_tchar.h"

/******************************************************************************
 * The structure represents a two dimensional array. The data is stored in a
 * one dimensional array. We have a macro to access an element.
 *****************************************************************************/

typedef struct {

	s_point dim;

	s_tchar *arr;

} s_tmpl;

/******************************************************************************
 * The function declarations.
 *****************************************************************************/

void s_tmpl_create(s_tmpl *tmpl, const int row, const int col);

void s_tmpl_free(s_tmpl *tmpl);

void s_tmpl_set(s_tmpl *tmpl, const s_tchar tchar);

/******************************************************************************
 * The macro to access the element of the array.
 *****************************************************************************/

#define s_tmpl_get_ptr(t,r,c) (&((t)->arr[ (r) * (t)->dim.col + (c)]))

#define s_tmpl_get(t,r,c) ((t)->arr[ (r) * (t)->dim.col + (c)])

#endif /* INC_S_TMPL_H_ */

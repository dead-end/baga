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

#ifndef INC_LIB_S_TCHAR_H_
#define INC_LIB_S_TCHAR_H_

#include <wchar.h>

/******************************************************************************
 * The s_tchar struct represents a terminal character, which consists of a
 * foreground and a background color and a wide character.
 *****************************************************************************/

typedef struct {

	wchar_t chr;

	short fg;

	short bg;

} s_tchar;

/******************************************************************************
 * Definition of the macros.
 *****************************************************************************/

#define TCHAR_CHR_UNUSED L'\0'

#define S_TCHAR_UNUSED (s_tchar ) { TCHAR_CHR_UNUSED, -1, -1 }

#define s_tchar_set(t,c,f,b) (t)->chr = (c) ; (t)->fg = (f) ; (t)->bg = (b)

#define s_tchar_is_defined(t) ((t)->chr != TCHAR_CHR_UNUSED)

#endif /* INC_LIB_S_TCHAR_H_ */

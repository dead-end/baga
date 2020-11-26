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

#ifndef INC_LIB_UTILS_H_
#define INC_LIB_UTILS_H_

/******************************************************************************
 * Definition of common macros.
 *****************************************************************************/

//
// The macro returns the reverse index given a total number and an index.
//
#define lu_reverse_idx(t,i) ((t) - 1 - (i))

//
// The macro returns the maximum of two value.
//
#define lu_max(t,i) ((i) > (t) ? (i) : (t))

//
// The macro returns the maximum of two value.
//
#define lu_min(t,i) ((i) > (t) ? (t) : (i))

//
// The macro centers a width w in a total t
//
#define lu_center(t,w) (((t) - (w)) / 2)

#endif /* INC_LIB_UTILS_H_ */

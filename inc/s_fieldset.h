/*
 * MIT License
 *
 * Copyright (c) 2021 dead-end
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

/******************************************************************************
 * The header file provides an interface for the s_fieldset struct, which is a
 * collection of fields: points, bear off, bar
 *
 * It is a pure data structure and knows nothing about ncurses, status, phases.
 *****************************************************************************/

#ifndef INC_S_FIELDSET_H_
#define INC_S_FIELDSET_H_

#include "bg_defs.h"
#include "s_field_id.h"
#include "s_field.h"

/******************************************************************************
 * The struct contains the fields of the game board.
 *****************************************************************************/

typedef struct {

	//
	// The fields of type point.
	//
	s_field point[POINTS_NUM];

	//
	// The fields of type bear off. The owner (OWNER_BLACK / OWNER_WHITE) is
	// the index.
	//
	s_field bear_off[NUM_PLAYER];

	//
	// The fields of type bar / reenter. The owner (OWNER_BLACK / OWNER_WHITE)
	// is the index.
	//
	s_field reenter[NUM_PLAYER];

} s_fieldset;

/******************************************************************************
 * Function declarations.
 *****************************************************************************/

void s_fieldset_init(s_fieldset *fieldset);

void s_fieldset_new_game(s_fieldset *fieldset);

/******************************************************************************
 * Functions and macros are used to get a field. There is one function and all
 * the macros used that function.
 *
 * For the bar and bear off, the owner and the index are the same. A relative
 * index is only valid for a point.
 *****************************************************************************/

s_field* s_fieldset_get(s_fieldset *fieldset, const e_field_type type, const int idx);

#define s_fieldset_get_by_id(f,i) s_fieldset_get(f, i.type, i.idx)

#define s_fieldset_get_point(f,i) s_fieldset_get(f, E_FIELD_POINTS, i)

#define s_fieldset_get_point_rel(f,o,i) s_fieldset_get(f, E_FIELD_POINTS, s_field_idx_rel(o, i))

#define s_fieldset_get_bear_off(f,o) s_fieldset_get(f, E_FIELD_BEAR_OFF, o)

#define s_fieldset_get_bar(f,o) s_fieldset_get(f, E_FIELD_BAR, o)

/******************************************************************************
 * Functions and macros to set a field. There is one function and all the
 * macros used that function.
 *
 * For the bar and bear off, the owner and the index are the same. A relative
 * index is only valid for a point.
 *****************************************************************************/

s_field* s_fieldset_set(s_fieldset *fieldset, const e_field_type type, const e_owner owner, const int idx, const int num);

#define s_fieldset_set_point_rel(f,o,i,n) s_fieldset_set(f, E_FIELD_POINTS, o, s_field_idx_rel(o, i), n)

#define s_fieldset_set_point(f,o,i,n) s_fieldset_set(f, E_FIELD_POINTS, o, i, n)

#define s_fieldset_set_bear_off(f,o,n) s_fieldset_set(f, E_FIELD_BEAR_OFF, o, o, n)

#define s_fieldset_set_bar(f,o,n) s_fieldset_set(f, E_FIELD_BAR, o, o, n)

#endif /* INC_S_FIELDSET_H_ */

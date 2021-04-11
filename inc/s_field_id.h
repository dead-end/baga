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

/******************************************************************************
 * The header file provides an interface for a field id. The consists of a type
 * and an index. For bars and bear off, the index is the owner of the field.
 *****************************************************************************/

#ifndef INC_S_FIELD_ID_H_
#define INC_S_FIELD_ID_H_

/******************************************************************************
 * An enumeration for the three types of fields, which can contain checkers.
 *****************************************************************************/

typedef enum {

	E_FIELD_POINTS = 0, E_FIELD_BAR = 1, E_FIELD_BEAR_OFF = 2, E_FIELD_NONE = -1,

} e_field_type;

/******************************************************************************
 * The struct defines a field in the game, which is one of the points or one of
 * the bar or bear off areas.
 *****************************************************************************/

typedef struct {

	//
	// The type of the field.
	//
	e_field_type type;

	//
	// The index of a point (starting upper right with 0) or black / white for
	// the bar or the bear off area.
	//
	int idx;

} s_field_id;

/******************************************************************************
 * Macro definitions.
 *****************************************************************************/

#define FIELD_NONE_IDX -1

#define s_field_id_set(f,t,i) (f).type = (t) ; (f).idx = (i)

#define s_field_id_set_ptr(f,t,i) (f)->type = (t) ; (f)->idx = (i)

#define s_field_id_set_none_ptr(f) s_field_id_set_ptr(f, E_FIELD_NONE, FIELD_NONE_IDX)

/******************************************************************************
 * The function declarations.
 *****************************************************************************/

char* e_field_type_str(const e_field_type type);

void s_field_id_valid_values(const e_field_type type, const int idx);

#define s_field_id_valid_id(i) s_field_id_valid_values(i.tpye, i.idx)

#endif /* INC_S_FIELD_ID_H_ */

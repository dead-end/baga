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

#ifndef INC_S_FIELD_H_
#define INC_S_FIELD_H_

#include "bg_defs.h"
#include "s_field_id.h"

/******************************************************************************
 * The struct defines a field on a the game board. A field has an id, which is
 * the type of the field (point, bar, bear off) and an index. Each field can
 * have checkers. The owner and the number are part of the file.
 *****************************************************************************/

typedef struct {

	//
	// The id of the checker (type and index)
	//
	s_field_id id;

	//
	// If the field holds checker, this is the owner.
	//
	e_owner owner;

	//
	// If the field holds checker, this is the number.
	//
	int num;

} s_field;

/******************************************************************************
 * Function declarations.
 *****************************************************************************/

char* s_field_type_str(const s_field *field);

char* s_field_owner_str(const s_field *field);

#endif /* INC_S_FIELD_H_ */

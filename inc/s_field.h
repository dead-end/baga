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

#include <stdbool.h>

#include "bg_defs.h"
#include "lib_utils.h"
#include "s_field_id.h"
#include "s_status.h"

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
 * Two macros to set fields.
 *****************************************************************************/

#define s_field_set_full(f,t,i,n,o) \
		(f).id.type = (t); \
		(f).id.idx = (i); \
		(f).num = (n); \
		(f).owner = (o)

#define s_field_set(f,n,o) \
		(f).num = (n); \
		(f).owner = (o)

/******************************************************************************
 * The macro is called with the owner and an index. The index is is relative
 * from start to end for the owner. It computes the absolute index.
 *
 * Example:
 *
 * white: from top    -> bottom => relative: 0 absolute 0
 * black: from bottom -> top    => relative: 0 absolute 23
 *****************************************************************************/

#define s_field_idx_rel(o,i) ((OWNER_TOP == (o)) ? (i) : lu_reverse_idx(POINTS_NUM,i))

/******************************************************************************
 * The macro adds a value to an absolute index. This means adding or
 * subtracting depending on
 *****************************************************************************/

#define s_field_idx_add_abs(o,i,a) ((OWNER_TOP == (o)) ? ((i) + (a)) : ((i) - (a)))

/******************************************************************************
 * The macro checks if the index is above the max value. We assume that the
 * value is absolute. We get the relative index and this index has to be over
 * 23.
 *****************************************************************************/

#define s_field_idx_is_out(o,i) (s_field_idx_rel(o,i) > POINTS_NUM - 1)

/******************************************************************************
 * The macro checks if the index is the max value. We assume that the value is
 * absolute. We get the relative index and this index has to be 23.
 *****************************************************************************/

#define s_field_idx_is_ex_out(o,i) (s_field_idx_rel(o,i) == POINTS_NUM)

/******************************************************************************
 * Function declarations.
 *****************************************************************************/

char* s_field_type_str(const e_field_type type);

char* s_field_owner_str(const e_owner owner);

bool s_field_is_valid_src(const s_field *field, const s_status *status);

void s_field_mv(s_field *field_src, s_field *field_dst);

int s_field_get_src_idx(const s_field *field_src);

#endif /* INC_S_FIELD_H_ */

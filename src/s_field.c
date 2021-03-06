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

#include "lib_logging.h"
#include "s_field.h"

/******************************************************************************
 * The function prints the field. If DEBUG is not defined, then there is no
 * logging and you have 2 unused parameter.
 *****************************************************************************/

#ifdef DEBUG

void s_field_log(const s_field *field, const char *msg) {

	//
	// For bars and bear off the index is the owner.
	//
	if (field->id.type == E_FIELD_BAR || field->id.type == E_FIELD_BEAR_OFF) {
		log_debug("[idx: %s type: %s num: %d owner: %s] - %s",

		e_owner_str(field->id.idx), e_field_type_str(field->id.type), field->num, e_owner_str(field->owner), msg);

	} else {
		log_debug("[idx: %d type: %s num: %d owner: %s] - %s",

		field->id.idx, e_field_type_str(field->id.type), field->num, e_owner_str(field->owner), msg);
	}
}

#endif

/******************************************************************************
 * The function moves a checker from a source to a destination field.
 *****************************************************************************/

void s_field_mv(s_field *field_src, s_field *field_dst) {

#ifdef DEBUG
	s_field_log(field_src, "src - before");
	s_field_log(field_dst, "dst - before");
#endif

	//
	// If there is not a checker on the destination we have to set the new
	// owner.
	//
	if (field_dst->id.type == E_FIELD_POINTS && field_dst->num == 0) {
		field_dst->owner = field_src->owner;
	}

	field_dst->num++;
	field_src->num--;

	//
	// If no checker is left on the source field, we delete the owner.
	//
	if (field_src->id.type == E_FIELD_POINTS && field_src->num == 0) {
		field_src->owner = E_OWNER_NONE;
	}

#ifdef DEBUG
	s_field_log(field_src, "src - after");
	s_field_log(field_dst, "dst - after");
#endif

}

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
 * The function returns a string representation of the owner of a field.
 *****************************************************************************/

char* s_field_owner_str(const s_field *field) {

	switch (field->owner) {

	case OWNER_BLACK:
		return "BLACK";

	case OWNER_WHITE:
		return "WHITE";

	case OWNER_NONE:
		return "EMPTY";

	default:
		log_exit("Unknown owner: %d", field->owner)
		;
	}
}

/******************************************************************************
 * The function returns a string representation of the type of a field.
 *****************************************************************************/

char* s_field_type_str(const s_field *field) {

	switch (field->id.type) {

	case E_FIELD_POINTS:
		return "POINT";

	case E_FIELD_BAR:
		return "BAR";

	case E_FIELD_BEAR_OFF:
		return "BEAR-OFF";

	case E_FIELD_NONE:
		return "NONE";

	default:
		log_exit("Unknown type: %d", field->id.type)
		;
	}
}

/******************************************************************************
 * The function prints the field. If DEBUG is not defined, then there is no
 * logging and you have 2 unused parameter.
 *****************************************************************************/

#ifdef DEBUG

static void s_field_log(const s_field *field, const char *msg) {
	log_debug("idx: %d type: %s num: %d owner: %s - %s", field->id.idx, s_field_type_str(field), field->num, s_field_owner_str(field), msg);
}

#endif

/******************************************************************************
 * The function checks if the field is a valid source for a move of the owner
 * (turn).
 *****************************************************************************/

bool s_field_is_valid_src(const s_field *field, const s_status *status) {

	//
	// If a checker reached the bear off area, it cannot moved again.
	//
	if (field->id.type == E_FIELD_BEAR_OFF) {
		return false;
	}

	//
	// If there is no checker on the field, there is nothing to do.
	//
	if (field->num == 0 || field->owner != status->turn) {
		return false;
	}

	return true;
}

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
		field_src->owner = OWNER_NONE;
	}

#ifdef DEBUG
	s_field_log(field_src, "src - after");
	s_field_log(field_dst, "dst - after");
#endif

}
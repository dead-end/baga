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

char* s_field_owner_str(const e_owner owner) {

	switch (owner) {

	case OWNER_BLACK:
		return "BLACK";

	case OWNER_WHITE:
		return "WHITE";

	case OWNER_NONE:
		return "EMPTY";

	default:
		log_exit("Unknown owner: %d", owner)
		;
	}
}

/******************************************************************************
 * The function returns a string representation of the type of a field.
 *****************************************************************************/

char* s_field_type_str(const e_field_type type) {

	switch (type) {

	case E_FIELD_POINTS:
		return "POINT";

	case E_FIELD_BAR:
		return "BAR";

	case E_FIELD_BEAR_OFF:
		return "BEAR-OFF";

	case E_FIELD_NONE:
		return "NONE";

	default:
		log_exit("Unknown type: %d", type)
		;
	}
}

/******************************************************************************
 * The function prints the field. If DEBUG is not defined, then there is no
 * logging and you have 2 unused parameter.
 *****************************************************************************/

#ifdef DEBUG

static void s_field_log(const s_field *field, const char *msg) {

	log_debug("[idx: %d type: %s num: %d owner: %s] - %s",

	field->id.idx, s_field_type_str(field->id.type), field->num, s_field_owner_str(field->owner), msg);
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

/******************************************************************************
 * The function returns the index of the source field. If the source field is a
 * point the index does not change. But if the source is the bar, we need a
 * source index to add our dice to.
 *****************************************************************************/

static int s_field_get_src_idx(const s_field *field_src, const s_status *status) {

#ifdef DEBUG

	//
	// Ensure that the source field type is the expected.
	//
	if (field_src->id.type != E_FIELD_BAR && field_src->id.type != E_FIELD_POINTS) {
		log_exit("invalid type: %s", s_field_type_str(field_src->id.type));
	}
#endif

	if (field_src->id.type == E_FIELD_BAR) {
		return (status->up_2_down == field_src->owner) ? -1 : POINTS_NUM;
	}

	return field_src->id.idx;
}

/******************************************************************************
 * The function is called with a source field and the status and it computes
 * the index of the destination field.
 *****************************************************************************/
// TODO: missing: bear-off phase
s_field_id s_field_get_dst_id(const s_field *field_src, const s_status *status) {
	s_field_id id_dst;

	//
	// Get the value from the active dice.
	//
	const int dice = s_status_get_dice(status);

	//
	// Get the source index of the field with respect to bars.
	//
	const int idx_src = s_field_get_src_idx(field_src, status);

	//
	// Compute the destination index.
	//
	id_dst.idx = (status->up_2_down == field_src->owner) ? idx_src + dice : idx_src - dice;
	id_dst.type = E_FIELD_POINTS;

	log_debug("field index src: %d dst: %d dice: %d", idx_src, id_dst.idx, dice);

#ifdef DEBUG

	//
	// TODO: only possible if last phase of the game => to bear off
	//
	if (id_dst.idx < 0 || id_dst.idx >= POINTS_NUM) {
		log_exit("out of range dst: %d src: %d", id_dst.idx, idx_src);
	}
#endif

	return id_dst;
}

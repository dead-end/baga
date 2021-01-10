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

#include "bg_defs.h"
#include "lib_logging.h"
#include "s_field_id.h"

/******************************************************************************
 * The function returns a string representation of the type of a field.
 *****************************************************************************/

char* e_field_type_str(const e_field_type type) {

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
 * The function ensures that the field id has valid values.
 *****************************************************************************/

void s_field_id_ensure_valid(const s_field_id id) {

	switch (id.type) {

	case E_FIELD_BAR:
		if (id.idx < 0 || id.idx > NUM_PLAYER) {
			log_exit("Field: E_FIELD_BAR index out of range: %d", id.idx);
		}
		break;

	case E_FIELD_BEAR_OFF:
		if (id.idx < 0 || id.idx > NUM_PLAYER) {
			log_exit("Field: E_FIELD_BEAR_OFF index out of range: %d", id.idx);
		}
		break;

	case E_FIELD_POINTS:
		if (id.idx < 0 || id.idx > POINTS_NUM) {
			log_exit("Field: E_FIELD_POINTS index out of range: %d", id.idx);
		}
		break;

	case E_FIELD_NONE:
		if (id.idx != FIELD_NONE_IDX) {
			log_exit("Field: E_FIELD_NONE index out of range: %d", id.idx);
		}
		break;
	default:
		log_exit("Unknown type: %d", id.type)
		;
	}
}


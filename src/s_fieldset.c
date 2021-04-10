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
 * The source file implements function to access the s_fieldset, which is a
 * collection of fields: points, bear off, bar
 *****************************************************************************/

#include "s_fieldset.h"
#include "lib_logging.h"

/******************************************************************************
 * The function initializes the s_fieldset struct. It sets the owner of the
 * fields. The points have no owner, the bar and the bear off area have a fixed
 * owner. The number of checkers on the fields are set to 0 on all fields.
 *****************************************************************************/

void s_fieldset_init(s_fieldset *fieldset) {

	//
	// The points have no owner (all fields have 0 checker)
	//
	for (int i = 0; i < POINTS_NUM; i++) {

		s_field_set_full(fieldset->point[i], E_FIELD_POINTS, i, 0, E_OWNER_NONE);
	}

	//
	// Each player has his bear off area (all fields have 0 checker)
	//
	s_field_set_full(fieldset->bear_off[E_OWNER_TOP], E_FIELD_BEAR_OFF, E_OWNER_TOP, 0, E_OWNER_TOP);

	s_field_set_full(fieldset->bear_off[E_OWNER_BOT], E_FIELD_BEAR_OFF, E_OWNER_BOT, 0, E_OWNER_BOT);

	//
	// Each player has his bar (all fields have 0 checker)
	//
	s_field_set_full(fieldset->reenter[E_OWNER_TOP], E_FIELD_BAR, E_OWNER_TOP, 0, E_OWNER_TOP);

	s_field_set_full(fieldset->reenter[E_OWNER_BOT], E_FIELD_BAR, E_OWNER_BOT, 0, E_OWNER_BOT);
}

/******************************************************************************
 * The function resets the s_fieldset struct and sets the checker for the new
 * game.
 *****************************************************************************/

void s_fieldset_new_game(s_fieldset *fieldset) {

	//
	// Set all to 0
	//
	s_fieldset_init(fieldset);

	//
	// Index 0
	//
	s_field_set(fieldset->point[s_field_idx_rel( E_OWNER_BOT, 0)], 2, E_OWNER_BOT);
	s_field_set(fieldset->point[s_field_idx_rel( E_OWNER_TOP, 0)], 2, E_OWNER_TOP);

	//
	// Index 11
	//
	s_field_set(fieldset->point[s_field_idx_rel( E_OWNER_BOT, 11)], 5, E_OWNER_BOT);
	s_field_set(fieldset->point[s_field_idx_rel( E_OWNER_TOP, 11)], 5, E_OWNER_TOP);

	//
	// Index 16
	//
	s_field_set(fieldset->point[s_field_idx_rel( E_OWNER_BOT, 16)], 3, E_OWNER_BOT);
	s_field_set(fieldset->point[s_field_idx_rel( E_OWNER_TOP, 16)], 3, E_OWNER_TOP);

	//
	// Index 11
	//
	s_field_set(fieldset->point[s_field_idx_rel( E_OWNER_BOT, 18)], 5, E_OWNER_BOT);
	s_field_set(fieldset->point[s_field_idx_rel(E_OWNER_TOP, 18)], 5, E_OWNER_TOP);
}

/******************************************************************************
 * The function returns a pointer to the field with the given id.
 *****************************************************************************/

s_field* s_fieldset_get(s_fieldset *fieldset, const s_field_id id) {

#ifdef DEBUG

	//
	// Ensure that the field id is valid
	//
	s_field_id_ensure_valid(id);
#endif

	switch (id.type) {

	case E_FIELD_BAR:
		return &fieldset->reenter[id.idx];

	case E_FIELD_BEAR_OFF:
		return &fieldset->bear_off[id.idx];

	case E_FIELD_POINTS:
		return &fieldset->point[id.idx];

	default:
		log_exit("Unknown type: %d", id.type)
		;
	}
}

/******************************************************************************
 * The function sets a field and returns a pointer to that field.
 *****************************************************************************/

s_field* s_fieldset_set(s_fieldset *fieldset, const e_field_type type, const int idx, const e_owner owner, const int num) {

	s_field *field;

	switch (type) {

	case E_FIELD_BAR:
		field = &fieldset->reenter[idx];
		if (idx != owner) {
			log_exit("idx: %d owner: %d", idx, owner);
		}
		break;

	case E_FIELD_BEAR_OFF:
		field = &fieldset->bear_off[idx];
		if (idx != owner) {
			log_exit("idx: %d owner: %d", idx, owner);
		}
		break;

	case E_FIELD_POINTS:
		field = &fieldset->point[idx];
		field->owner = owner;
		break;

	default:
		log_exit("Unknown type: %d", type)
		;
	}

	field->num = num;

	return field;
}

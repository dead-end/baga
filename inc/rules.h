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
 * The header field provides an interface for functions that implement rules of
 * the game. They operate on the fieldset, which is a collection of fields
 * (points, bear off, bar) and the status of the game.
 *****************************************************************************/

#ifndef INC_RULES_H_
#define INC_RULES_H_

#include "s_fieldset.h"
#include "s_status.h"

/******************************************************************************
 * Definition of functions and macros.
 *****************************************************************************/

void rules_update_phase(s_status *status, const s_fieldset *fieldset);

int rules_min_rel_idx(const s_status *status, const s_fieldset *fieldset);

s_field* rules_get_field_src(const s_status *status, s_fieldset *fieldset, const s_field_id id);

s_field* rules_can_mv(const s_status *status, s_fieldset *fieldset, const s_field *field_src);

#endif /* INC_RULES_H_ */

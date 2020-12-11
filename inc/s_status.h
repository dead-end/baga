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

#ifndef INC_S_STATUS_H_
#define INC_S_STATUS_H_

#include "bg_defs.h"

/******************************************************************************
 *
 *****************************************************************************/

typedef enum {

	//
	// The dice can be set and is selected to be set.
	//
	E_DICE_ACTIVE = 0,

	//
	// The dice can be set and is not selected to be set.
	//
	E_DICE_INACTIVE = 1,

	//
	// The dice has been set.
	//
	E_DICE_SET = 2,

	//
	// The dice is not possible to be set.
	//
	E_DICE_NOT_POS = 3

} e_dice_status;

/******************************************************************************
 *
 *****************************************************************************/

typedef struct {

	e_dice_status status;

	int value;

	//
	// The values are 1 or 2 if the dices have the same value.
	//
	// TODO: maybe bool
	int num;

	//
	//
	//
	int num_set;

} s_dice;

/******************************************************************************
 *
 *****************************************************************************/

typedef struct {

	e_owner turn;

	s_dice dices[2];

	e_owner up_2_down;

} s_status;

/******************************************************************************
 * The function declarations.
 *****************************************************************************/

void s_status_init();

void s_status_start(s_status *status, const e_owner owner);

void s_status_next_turn(s_status *status);

int s_status_get_dice(s_status *status);

void s_status_mv_done(s_status *status);

#endif /* INC_S_STATUS_H_ */

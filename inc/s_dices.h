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

#ifndef INC_S_DICES_H_
#define INC_S_DICES_H_

#include <stdbool.h>

/******************************************************************************
 * The enum contains the status of a dice.
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
 * The structure defines a dice.
 *****************************************************************************/

typedef struct {

	//
	// The status of the dice.
	//
	e_dice_status status;

	//
	// The value of the dice: 1 - 6
	//
	int value;

	//
	// If both dices have the same value, num is 2 otherwise 1.
	//
	int num;

	//
	// The number of values set. If num == num_set => E_DICE_SET
	//
	int num_set;

} s_dice;

/******************************************************************************
 * The structure represents the two dices. Currently they are implemented as an
 * array. Maybe the status can be combined in the future.
 *
 * Use macros or functions to access the internal data.
 *****************************************************************************/

typedef struct {

	s_dice dice[2];

} s_dices;

/******************************************************************************
 * The macro checks if a dice has the expected value.
 *****************************************************************************/

#define s_dices_has_status(d,i,s)  ((d).dice[i].status == (s))

/******************************************************************************
 * The macro checks if a dice is not active.
 *
 * TODO: is the name correct?
 *****************************************************************************/

#define s_dices_is_done(s) ((s).dice[0].status != E_DICE_ACTIVE && (s).dice[1].status != E_DICE_ACTIVE)

/******************************************************************************
 * The macro checks if a dice can be undone. The status E_DICE_SET cannot be
 * used, because if both dices have the same value. The num_set parameter is
 * set every time a checker moves, so this is appropriate.
 *****************************************************************************/

#define s_dices_can_undo(s) ((s).dice[0].num_set > 0 || (s).dice[1].num_set > 0)

/******************************************************************************
 * The function declarations.
 *****************************************************************************/

void s_dices_init();

void s_dices_toss(s_dices *dices);

int s_dices_get_value(const s_dices *dices);

const char* s_dice_status_str(const e_dice_status dice_status);

void s_dices_debug(const s_dices *dices);

bool s_dices_set(s_dices *dices);

bool s_dice_toogle_active(s_dices *dices, const int idx);

#endif /* INC_S_DICES_H_ */

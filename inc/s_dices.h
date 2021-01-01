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

#ifndef INC_S_DICE_H_
#define INC_S_DICE_H_

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
	// The values are 1 or 2 if the dices have the same value.
	//
	int num;

	//
	// The number of values set. If num == num_set => E_DICE_SET
	//
	int num_set;

} s_dice;

/******************************************************************************
 * The function declarations.
 *****************************************************************************/

const char* s_dice_status_str(const e_dice_status dice_status);

void s_dice_debug(const s_dice dice[]);

#endif /* INC_S_DICE_H_ */

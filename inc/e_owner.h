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

#ifndef INC_E_OWNER_H_
#define INC_E_OWNER_H_

/******************************************************************************
 * The enum defines an owner of a checker. The owner can be the player from top
 * to bottom or from bottom to top or none at all. The owner can be used as an
 * index for some arrays.
 *****************************************************************************/

typedef enum {

	//
	// Player from top to bottom
	//
	E_OWNER_TOP = 0,

	//
	// Player from bottom to top
	//
	E_OWNER_BOT = 1,

	//
	// Not used
	//
	E_OWNER_NONE = -1

} e_owner;

#define e_owner_has_owner(o) ((o) != -1)

#define e_owner_other(o) (((o) + 1) % 2)

/******************************************************************************
 * Function declarations.
 *****************************************************************************/

const char* e_owner_str(const e_owner owner);

#endif /* INC_E_OWNER_H_ */

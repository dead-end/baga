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

#include "s_tmpl.h"

/******************************************************************************
 * The function creates the array for the structure with the given dimension
 * and stores the dimension.
 *****************************************************************************/

void s_tmpl_create(s_tmpl *tmpl, const int row, const int col) {

	log_debug("Creating s_tmpl: %d/%d", row, col);

	tmpl->arr = malloc(row * col * sizeof(s_tchar));
	if (tmpl->arr == NULL) {
		log_exit_str("Unable to allocate memory!");
	}

	tmpl->dim.row = row;
	tmpl->dim.col = col;
}

/******************************************************************************
 * The function frees the array of the structure.
 *****************************************************************************/

void s_tmpl_free(s_tmpl *tmpl) {

#ifdef DEBUG
	if (tmpl->arr == NULL) {
		log_exit("Template already freed: %d/%d", tmpl->dim.row, tmpl->dim.col);
	}
#endif

	log_debug("Freeing s_tmpl: %d/%d", tmpl->dim.row, tmpl->dim.col);

	if (tmpl->arr != NULL) {
		free(tmpl->arr);
		tmpl->arr = NULL;
	}
}

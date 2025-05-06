#pragma once
#include "matrix.h"
#include "utils.h"
#include <stdio.h>

void matrix_print_rowcol(double* arr, int size,int flag);
void matrix_print(const matrix_t *m);
void matrix_set_linear_range(matrix_t* m);
void matrix_set_rand_val(matrix_t* m);

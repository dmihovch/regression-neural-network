#pragma once
#include "matrix.h"
#include <time.h>
void shuffle_rows(matrix_t* a, matrix_t* b);
void assert_matrix_shape(matrix_t* a, matrix_t* b);
double randf(); //returns a random double [0,1)
double derivative(double (*p_act_func)(double), double x, double h_step);

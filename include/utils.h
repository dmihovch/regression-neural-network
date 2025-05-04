#pragma once
#include "matrix.h"
void shuffle_rows(matrix_t* a, matrix_t* b);
void assert_matrix_shape(matrix_t* a, matrix_t* b);
float randf(); //returns a random float [0,1)
float derivative(float (*p_act_func)(float), float x, float h_step);

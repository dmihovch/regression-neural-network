#pragma once
#include "matrix.h"
#include <time.h>

extern uint32_t lcgstate;
#define A 1103515245
#define C 12345

void shuffle_rows(matrix_t* a, matrix_t* b);
void assert_matrix_shape(matrix_t* a, matrix_t* b);
double derivative(double (*p_act_func)(double), double x, double h_step);
double lcgrandf();
void seedlcgrandf(uint32_t seed);

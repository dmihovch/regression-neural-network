#pragma once
#include <math.h>
#include "../include/utils.h"
#include "layer.h"
#include "matrix.h"
#define HSTEP 1e-5
double relu(double x);
double drelu(double x);
double sigmoid(double x);
double dsigmoid(double x);
matrix_t* activation_backwards_relu(matrix_t* z, matrix_t* dA);
matrix_t* activation_backwards_sigmoid(matrix_t* z, matrix_t* dA);
matrix_t* activation_backwards(matrix_t* z, matrix_t* dA, activation_type act);

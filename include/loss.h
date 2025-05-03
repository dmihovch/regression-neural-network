#pragma once
#include "matrix.h"
float mean_squared_error_loss(matrix_t* y_true, matrix_t* y_pred);
void dmean_squared_error(matrix_t* y_true, matrix_t* y_pred, matrix_t* grad_out);

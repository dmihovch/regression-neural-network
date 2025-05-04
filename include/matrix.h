#pragma once
#include <stdlib.h>
#include <stdio.h>


typedef struct matrix_t {
    int rows;
    int cols;
    float* data; //index a value at i,j => data[i * cols + j]
} matrix_t;



//if a function in here returns void, it is assumed that the values have been sanitized, and they will not alloc or free any memory




matrix_t* matrix_alloc(int rows, int cols);
void matrix_free(matrix_t* m);
matrix_t* matrix_mult(matrix_t* a, matrix_t* b);
void matrix_print(const matrix_t* m);
float* matrix_index(matrix_t* m,int r, int c);
void matrix_set_linear_range(matrix_t* m);
float* matrix_get_row(matrix_t* m, int row);
float* matrix_get_col(matrix_t* m, int col);
void matrix_print_rowcol(float* arr, int size, int flag);//flag 1 = row 0 = col
void matrix_write_to_index(matrix_t* m, float val, int row, int col);
void matrix_mult_loop_handler(matrix_t* c, matrix_t* a, matrix_t* b, const int shared_dimension_size_ab);
void matrix_add(matrix_t* a, matrix_t* b, matrix_t* out);
void matrix_sub(matrix_t* a, matrix_t* b, matrix_t* out);
void matrix_scalar_mult(matrix_t* a, float scalar);
void matrix_hadamard(matrix_t* a, matrix_t*b, matrix_t* out);


/*

functions not exposed, they do not need to null check inputs

*/

matrix_t* matrix_transpose(matrix_t* m);
void matrix_copy(matrix_t* dest, matrix_t* src);

void matrix_apply_activation_ip(matrix_t* m, float(*p_act_func)(float));
void matrix_apply_activation_new(matrix_t* m, float(*p_act_func)(float), matrix_t* out);

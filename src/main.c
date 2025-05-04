#include "../include/matrix.h"
#include <stdio.h>

int main(int argc, char* argv[]){
    matrix_t* mat = matrix_alloc(3, 4);
    if(mat == NULL){
        return 1;
    }
    matrix_t* mat2 = matrix_alloc(4,3);
    if(mat2 == NULL){
        matrix_free(mat);
        return 1;
    }
    matrix_set_linear_range(mat);
    matrix_set_linear_range(mat2);
    matrix_print(mat);
    matrix_print(mat2);


    matrix_t* mat3 = matrix_mult(mat, mat2);
    if(mat3 != NULL){
        matrix_print(mat3);
    }

    matrix_t* mat4 = matrix_transpose(mat);
    if(mat4 != NULL){
        matrix_print(mat4);
    }
    matrix_free(mat4);
    matrix_free(mat3);
    matrix_free(mat2);
    matrix_free(mat);
    return 0;
}

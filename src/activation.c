#include "../include/activation.h"

double relu(double x){
    return x > 0 ? x : 0.;
}

double drelu(double x){
    return (x>0) ? 1.0 : 0.0;
}

double sigmoid(double x){
    //not good for larger values
    return 1. / (1. + exp(-x));
}

double dsigmoid(double x){
    return derivative(sigmoid, x, HSTEP);
}

matrix_t* activation_backwards_relu(matrix_t* z, matrix_t* dA){
    const int rows = z->rows;
    const int cols = z->cols;
    matrix_t* dZ = matrix_alloc(rows,cols);
    if(dZ == NULL){
        return NULL;
    }
    matrix_t* z_deriv_relu = matrix_copy_alloc_new(z);
    if(z_deriv_relu == NULL){
        matrix_free(dZ);
        return NULL;
    }
    const int size = rows*cols;
    double* z_deriv_relu_arr = z_deriv_relu->data;
    int i = 0;
    for(;i<size;++i){
        z_deriv_relu_arr[i] = drelu(z_deriv_relu_arr[i]);
    }
    matrix_hadamard(z_deriv_relu, dA, dZ);
    matrix_free(z_deriv_relu);
    return dZ;
}

matrix_t* activation_backwards(matrix_t* z, matrix_t* dA, activation_type act){
    if(act == A_NONE){
        return matrix_copy_alloc_new(dA);
    }
    if(act == A_RELU){
        return activation_backwards_relu(z,dA);
    }
    /*
    if(act == A_SIGMOID){
        return activation_backwards_sigmoid(z, dA); //todo
    }
    */
    return NULL;
}

//todo: tanh

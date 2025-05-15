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
    const int size = rows*cols;
    double* z_arr = z->data;
    int i = 0;
    for(;i<size;++i){
        z_arr[i] = drelu(z_arr[i]);
    }
    matrix_hadamard(z, dA, dZ);
    return dZ;
}

matrix_t* activation_backwards(matrix_t* z, matrix_t* dA, activation_type act){
    if(act == A_RELU){
        return activation_backwards_relu(z,dA);
    }
    /*
    if(act == A_SIGMOID){
        return activation_backwards_sigmoid(z, dA); //todo
    }
    */
    printf("act type not relu\n");
    return NULL;
}

//todo: tanh

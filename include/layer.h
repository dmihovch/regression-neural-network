#pragma once
#include "matrix.h"
#include "activation.h"
#include "utils.h"

typedef enum activation_type {
  A_NONE,
  A_RELU,
  A_SIGMOID
} activation_type;

typedef struct layer_t {
    matrix_t* weights;
    matrix_t* biases;
    matrix_t* input;
    matrix_t* output;
    matrix_t* dweights;
    matrix_t* dbiases;
    matrix_t* dinputs;
    activation_type act;
} layer_t;

layer_t* layer_init(int input_size, int output_size, activation_type act);
void layer_forward(layer_t* layer, matrix_t* input);
void layer_backwards(layer_t* layer, matrix_t* grad_output, float learning_rate);
void layer_free(layer_t* layer);
void init_randf_vals(matrix_t* m);

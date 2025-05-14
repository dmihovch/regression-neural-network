#pragma once
#include "matrix.h"

#include "utils.h"

typedef enum activation_type {
  A_NONE,
  A_RELU,
  A_SIGMOID
} activation_type;

#include "activation.h"

typedef struct layer_t {
    matrix_t* weights;
    matrix_t* biases;
    matrix_t* input;
    matrix_t* output;
    matrix_t* dweights;
    matrix_t* dbiases;
    matrix_t* dinputs;
    matrix_t* pre_act_z;
    activation_type act;
} layer_t;

layer_t* layer_init(int input_size, int output_size, activation_type act);
void layer_forward(layer_t* layer, matrix_t* input);
void layer_backwards(layer_t* l, matrix_t* dA, double learning_rate);
void layer_free(layer_t* layer);

void init_bias(matrix_t* m, activation_type act);

void init_weights(matrix_t* m, activation_type act, int inputs, int outputs);
void init_weights_relu(matrix_t* m, double mu, double sigma);
void init_weights_sigmoid(matrix_t* m, int inputs, int outputs);

//returns 0 if success, and mallocs layer->pre_act_z
int save_pre_activation_z(layer_t* layer, matrix_t* z);


void init_randf_vals(matrix_t* m);

double box_muller_tran(double mu, double sigma);

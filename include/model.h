#pragma once
#include "layer.h"
#include "matrix.h"
#include "loss.h"
#define MAX_LAYERS 10

typedef struct model_t{
    layer_t** layers;
    int num_layers;
    double learning_rate;
}model_t;

model_t* model_init(int input_size, int* layer_sizes, activation_type act, int num_layers, double learning_rate);
matrix_t* model_predict(model_t* model, matrix_t* input);
void model_train(model_t* model, matrix_t* x, matrix_t* y, int epochs);
void model_free(model_t* model);

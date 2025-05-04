#include "../include/layer.h"
#include <math.h>

//mallocs a layer
layer_t* layer_init(int input_size, int output_size, activation_type act){

    layer_t* l = calloc(1,sizeof(layer_t));
    if(l == NULL){
        return NULL;
    }
    l->weights = matrix_alloc(input_size, output_size);
    if(l->weights == NULL){
        free(l);
        return NULL;
    }
    l->biases = matrix_alloc(1,output_size);
    if(l->biases == NULL){
        matrix_free(l->weights); //technically only need to call free here, but I'll just do it for kicks
        free(l);
        return NULL;
    }
    l->act = act;

    init_weights(l->weights, l->act, input_size, output_size);
    init_bias(l->biases, l->act);


    return l;
}

void layer_forward(layer_t* layer, matrix_t* input){

    layer->input = input;
    matrix_t* in = layer->input;
    matrix_t* w = layer->weights;
    matrix_t* b = layer->biases;
    activation_type act = layer->act;

    matrix_t* w_in = matrix_mult(in,w);
    if(w_in == NULL){
        layer->output = NULL;
        return;
    }
    matrix_add_bias(w_in, b);
    if(act == A_RELU){
        matrix_apply_activation_ip(w_in, relu);
    }
    if(act == A_SIGMOID){
        matrix_apply_activation_ip(w_in,sigmoid);
    }

    layer->output = w_in;
}


void init_randf_vals(matrix_t* m){
    const int size = m->rows * m->cols;
    float* arr = m->data;
    int i = 0;
    for(;i<size;++i){
        arr[i] = randf();
    }
}


void init_bias(matrix_t* m, activation_type act){
    if(act == A_RELU){
        float* arr = m->data;
        const int size = m->rows*m->cols;
        int i = 0;
        for(;i<size;++i){
            arr[i] = 0.01;
        }
    }
}


void init_weights(matrix_t* m, activation_type act, int inputs, int outputs){
    if(act == A_RELU){
        init_weights_relu(m, inputs);
    }
    if(act == A_SIGMOID){
        init_weights_sigmoid(m, inputs, outputs);
    }
    if(act == A_NONE){
        init_randf_vals(m);
    }
}
void init_weights_relu(matrix_t* m, int inputs){
    float* arr = m->data;
    //todo

}
void init_weights_sigmoid(matrix_t* m, int inputs, int outputs){
    float* arr = m->data;
    const float min = -sqrtf(6./(inputs+outputs));
    const float max = sqrtf(6./(inputs+outputs));
    const int size = m->rows*m->cols;
    const float mult = min + (max - min);
    int i = 0;
    for(;i<size;++i){
        arr[i] = mult * randf();
    }
}

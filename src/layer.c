#include "../include/layer.h"

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
    init_randf_vals(l->weights);
    init_randf_vals(l->biases);


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

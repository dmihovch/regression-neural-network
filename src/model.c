#include "../include/model.h"


model_t* model_init(int input_size, int* layer_sizes, activation_type act, int num_layers, double learning_rate){
    if(layer_sizes == NULL || layer_sizes[num_layers-1] != 1){
        printf("layer_sizes issue\n");
        return NULL;
    }

    model_t* model = (model_t*)calloc(1,sizeof(model_t));
    if(model == NULL){
        return NULL;
    }
    model->layers = (layer_t**)calloc(num_layers,sizeof(layer_t*));
    if(model->layers == NULL){
        free(model);
        return NULL;
    }


    model->learning_rate = learning_rate;
    model->num_layers = num_layers;
    int i = 0;

    for(;i<num_layers;++i){
        if(i == 0){
            model->layers[i] = layer_init(input_size, layer_sizes[i],act);
        }
        else{
            model->layers[i] = layer_init(layer_sizes[i-1], layer_sizes[i], act);
        }

        if(model->layers[i] == NULL){
            model_free(model);
            return NULL;
        }
    }
    return model;
}


void model_free(model_t *model){
    int i = 0;
    if(model == NULL){
        return;
    }
    if(model->layers == NULL){
        free(model);
        return;
    }
    const int num_layers = model->num_layers;

    for(;i<num_layers;++i){
        if(model->layers[i] != NULL){
            layer_free(model->layers[i]);
        }
    }
    free(model->layers);
    free(model);
    return;
}

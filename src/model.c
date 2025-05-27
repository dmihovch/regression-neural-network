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

    if(model == NULL){
        return;
    }
    if(model->layers == NULL){
        free(model);
        return;
    }
    const int num_layers = model->num_layers;
    int i = 0;
    for(;i<num_layers;++i){
        if(model->layers[i] != NULL){
            layer_free(model->layers[i]);
        }
    }
    free(model->layers);
    free(model);
    return;
}

void model_train(model_t* model, matrix_t* x, matrix_t* y, int epochs){

    if(model == NULL || x == NULL || y == NULL){
        return;
    }

    const int y_true_rows = y->rows;
    const int y_true_cols = y->cols;

    double loss;
    const int num_layers = model->num_layers;
    const double learn_rate = model->learning_rate;
    int ep = 0;
    for(;ep<epochs;++ep){


        for(int i = 0;i<num_layers;++i){
            if(!i){
                layer_forward(model->layers[i], x);
            }
            else{
                layer_forward(model->layers[i], model->layers[i-1]->output);
            }
        }

        printf("last layer output after forward prop\n");
        matrix_print(model->layers[num_layers-1]->output);
        printf("\n");
        loss = mean_squared_error_loss(y, model->layers[num_layers-1]->output);

        matrix_t* dA = matrix_alloc(y_true_rows,y_true_cols);
        if(dA == NULL){
            return;
        }
        dmean_squared_error(y, model->layers[num_layers-1]->output, dA);

        matrix_t* dA_current = dA;
        for(int i = num_layers-1;i>=0;--i){
            layer_backwards(model->layers[i], dA_current, learn_rate);

            printf("da: %dx%d || din: %dx%d\n",dA_current->rows,dA_current->cols, model->layers[i]->dinputs->rows,model->layers[i]->dinputs->cols);
            dA_current = model->layers[i]->dinputs;
        }
        matrix_free(dA);


        printf("Epoch %d, Loss: %f\n",ep,loss);
    }
}

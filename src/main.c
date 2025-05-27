#include "../include/matrix.h"
#include "../include/utils.h"
#include "../include/layer.h"
#include "../include/model.h"
#include "../include/debug.h"
//#include "../include/data.h"
//#include "../include/loss.h"



int main(/*int argc, char* argv[]*/){
    seedlcgrandf(time(NULL));

    matrix_t* init = matrix_alloc(10, 10);
    matrix_set_rand_val(init);

    matrix_print(init);

    int layer_sizes[5] = {10,7,15,10,1};
    model_t* model = model_init(10, layer_sizes,A_RELU, 5, 0.1);
    matrix_t* y_true = matrix_alloc(10, 1);
    if(y_true == NULL){
        model_free(model);
        return 1;
    }
    matrix_set_const_val(y_true, 1.0);

    matrix_print(y_true);

    model_train(model, init, y_true, 2);
    matrix_free(init);
    matrix_free(y_true);
    model_free(model);

    printf("End of program...\n\n");

    return 0;
}

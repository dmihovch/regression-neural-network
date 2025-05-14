#include "../include/loss.h"
#include <math.h>

double mean_squared_error_loss(matrix_t *y_true, matrix_t *y_pred){
    //maybe null checking

    const int rows = y_pred->rows;
    const int cols = y_pred->cols;

    if(rows != y_true->rows || cols != y_true->cols){
        return -1.;
    }

    double* pred_arr = y_pred->data;
    double* true_arr = y_true->data;
    double mse = 0.0;
    const double n = rows*cols;
    int i = 0;
    for(;i<rows;++i){
        int j = 0;
        for(;j<cols;++j){
            /*

            const int index = i*cols+j;
            const double y_true = true_arr[index] * 10000;
            const double y_pred = pred_arr[index] * 10000;
            printf("y_true[%d]*10k: %0.10f\ny_pred[%d]*10k: %0.10f\n",index,y_true,index,y_pred);
            */

            mse += pow(true_arr[i*cols+j]-pred_arr[i*cols+j],2);
        }
    }

    return mse/n;
}

void dmean_squared_error(matrix_t *y_true, matrix_t *y_pred, matrix_t *grad_out){
    const int size = grad_out->rows*grad_out->cols;
    const double twoon = 2./size;
    double* t_arr = y_true->data;
    double* p_arr = y_pred->data;
    double* o_arr = grad_out->data;
    int i = 0;
    for(;i<size;++i){
        o_arr[i] = twoon*(p_arr[i]-t_arr[i]);
    }
}

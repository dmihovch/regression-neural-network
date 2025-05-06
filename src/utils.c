#include "../include/utils.h"
#include <stdlib.h>

double derivative(double (*p_act_func)(double), double x, double h_step){
    return (p_act_func(x+h_step)-p_act_func(x-h_step)) / (2*h_step);
}

double randf(){
    return (double)rand() / ((double)RAND_MAX+1.);
}

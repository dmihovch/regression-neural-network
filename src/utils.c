#include "../include/utils.h"
#include <stdlib.h>

float derivative(float (*p_act_func)(float), float x, float h_step){
    return (p_act_func(x+h_step)-p_act_func(x-h_step)) / (2*h_step);
}

float randf(){
    srand(time(NULL));
    return (float)rand() / ((float)RAND_MAX+1.);
}

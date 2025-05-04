#include "../include/utils.h"

float derivative(float (*p_act_func)(float), float x, float h_step){
    return (p_act_func(x+h_step)-p_act_func(x-h_step)) / (2*h_step);
}

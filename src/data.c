#include "../include/data.h"

#include <stdlib.h>


dataset_t* dataset_alloc(int samples, int features){
    dataset_t* d = calloc(1, sizeof(dataset_t));
    if(d == NULL){
        return NULL;
    }
    d->features = features;
    d->samples = samples;
    d->input = matrix_alloc(samples, features);
    if(d->input == NULL){
        free(d);
        return NULL;
    }
    d->output = matrix_alloc(samples, 1);
    if(d->output == NULL){
        dataset_free(d);
        return NULL;
    }



    return d;
}

void dataset_free(dataset_t* d){
    if(d == NULL){
        return;
    }
    if(d->input != NULL){
        matrix_free(d->input);
    }
    if(d->output != NULL){
        matrix_free(d->output);
    }
    free(d);
    return;
}

dataset_t* csv_parser(char* filepath, int mode){
    FILE* data_file = fopen(filepath, "r");
    if(data_file == NULL){
        printf("Failed to open file: %s\n", filepath);
        return NULL;
    }

    dataset_t* data = NULL;
    if(mode == TRAINING){
        data = parse_training(data_file);
    }
    if(mode == TESTING){
        data = parse_testing(data_file);
    }
    free(data_file);
    return data;

}

dataset_t* parse_training(FILE* data_file){
    //assuming csv format for now

    int features = count_features(data_file);
    int samples = count_samples(data_file);
    int max_line_chars = count_max_line_chars(data_file);

    dataset_t* d = dataset_alloc(samples, features);
    if(d == NULL){
        return NULL;
    }

    d->features = features;
    d->samples = samples;

    char line[max_line_chars];
    int input_index = 0;
    int output_index = 0;
    double* input_arr = d->input->data;
    double* output_arr = d->output->data;
    char* end;
    while( fgets(line,max_line_chars,data_file) ){
        char* tok = strtok(line, ",");
        for(int i = 0; line[i] != '\n' && line[i] != '\0' && tok != NULL; i++){
            double tok_double = strtod(tok, &end);
            if(end == tok || *end != '\0'){
                printf("Parsing error: tok: |%s| i: %d\n",tok,i);
                dataset_free(d);
                return NULL;
            }
            if(i < features){
                input_arr[input_index] = tok_double;
                input_index++;
            }
            if(i == features){
                output_arr[output_index] = tok_double;
                output_index++;
            }
        }

    }
    return d;

}




dataset_t* parse_testing(FILE* data_file){
    int features = count_features(data_file) + 1; //since there is nfeatures - 1 commas
    int samples = count_samples(data_file);
    int chars = count_max_line_chars(data_file);
}

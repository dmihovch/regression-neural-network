# spotify-neural-network
regression based neural network written in C, to predict a Spotify track's popularity score



design choices:


memory:
    1. caller of function needs to check if matrix_t* is valid
    2. user of matrix_t->data needs to make sure data is valid

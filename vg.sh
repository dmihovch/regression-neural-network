gcc -o main src/main.c src/matrix.c src/debug.c
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main

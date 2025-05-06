all:
	gcc -o main src/main.c src/matrix.c src/debug.c src/activation.c src/layer.c src/model.c src/utils.c src/loss.c src/data.c -lm
clean:
	rm *.o main

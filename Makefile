CC = gcc
CFLAGS = -Wall -Wextra -I include
LDFLAGS = -lm

SRC = \
	src/main.c \
	src/model.c \
	src/layer.c \
	src/matrix.c \
	src/activation.c \
	src/loss.c \
	src/utils.c \
	src/debug.c \
	src/data.c \
	src/threading.c

BIN = model

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

clean:
	rm -f $(BIN)

valgrind: $(BIN)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN) >> perf-bench.txt

run: $(BIN)
	./$(BIN) >> perf-bench.txt

.PHONY: all clean valgrind run

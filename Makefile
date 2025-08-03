# BSD Makefile
CC = clang
CFLAGS := -Wall -Wextra -O0 -g


SOURCE!= ls src/*.c

all: build

build:
		$(CC) $(CFLAGS) $(SOURCE) -o arcpack

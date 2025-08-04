# BSD Makefile
CC = clang
CFLAGS := -Wall -Wextra -O0 -g -DVBS -Wno-unused-parameter


SOURCE!= ls src/*.c

all: build

build:
		$(CC) $(CFLAGS) $(SOURCE) -o arcpack

clean:
		rm -f arcpack *.arc

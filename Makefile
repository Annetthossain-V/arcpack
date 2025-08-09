# BSD Makefile

# target linux 
# -target x86_64-pc-linux-gnu

CC = clang
CFLAGS := -Wall -Wextra -O0 -g -Wno-unused-parameter

SOURCE != ls src/*.c
CODE_ASM != ls src/*.S

all: build

build:
		$(CC) $(CFLAGS) $(SOURCE) $(CODE_ASM) -o arcpack

clean:
		rm -f arcpack *.arc

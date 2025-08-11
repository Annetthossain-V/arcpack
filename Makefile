# BSD Makefile

# target linux 
# -target x86_64-pc-linux-gnu

CC = clang
CFLAGS := -Wall -Wextra -O2 -march=native 

SOURCE != ls src/*.c

all: build

build:
		$(CC) $(CFLAGS) $(SOURCE) -o arcpack

clean:
		rm -f arcpack *.arc

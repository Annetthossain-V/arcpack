# BSD Makefile

# target linux 
# -target x86_64-pc-linux-gnu

CC = cc
CFLAGS := -Wall -Wextra -O2

SOURCE != ls src/*.c

all: build

build:
		$(CC) $(CFLAGS) $(SOURCE) -o arcpack

clean:
		rm -f arcpack *.arc

name = erg2
src = $(wildcard src/*.cpp)
obj = $(src:/c=.o)

CC = g++
CFLAGS = -std=c++0x -O3

BOOSTDIR = '/usr/include'

all: $(name)
$(name): $(obj)
	$(CC) $(CFLAGS) -o $@ $^ -I$(BOOSTDIR)

run:
	./$(name)

clean:
	rm -f $(name)

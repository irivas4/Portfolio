# Midterm Project
GCC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

puzzle: ppm_io.o puzzle_funcs.o puzzle.o
	$(GCC) -o puzzle puzzle.o puzzle_funcs.o ppm_io.o

puzzle.o: puzzle.c ppm_io.h puzzle.h
	$(GCC) -c puzzle.c $(CFLAGS)

puzzle_funcs.o: puzzle_funcs.c puzzle.h ppm_io.h
	$(GCC) -c puzzle_funcs.c $(CFLAGS)

ppm_io.o: ppm_io.c ppm_io.h
	$(GCC) -c ppm_io.c $(CFLAGS)

clean:
	rm -f *.o puzzle *~

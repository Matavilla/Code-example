CC=g++
CFLAGS=-O2 -Wall -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy -Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Weffc++ -std=gnu++0x -c

all: alg_MorkvinAA_201 gen_MorkvinAA_201

alg_MorkvinAA_201: alg.o functional.h
	$(CC) alg.o functional.h -o alg_MorkvinAA_201

gen_MorkvinAA_201: gen.o
	$(CC) gen.o -o gen_MorkvinAA_201

alg.o: alg.cpp
	$(CC) $(CFLAGS) alg.cpp

gen.o: gen.cpp
	$(CC) $(CFLAGS) gen.cpp

clean:
	rm -rf *.o alg_MorkvinAA_201 gen_MorkvinAA_201


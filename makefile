
CC = g++
CFLAGS = -I.
OUTPUT = frontEnd
CLEANCOMM = rm -f

%.o: %.cpp
	$(CC) -std=c++11 -c -o $@ $< $(CFLAGS)

$(OUTPUT): main.o parser.o scanner.o
	$(CC) -std=c++11 -Wall -o $(OUTPUT) main.o parser.o scanner.o
	make clean

clean:
	$(CLEANCOMM) *.o

run:
	$(OUTPUT) input.sp21

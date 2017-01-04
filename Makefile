all: labwork4a labwork4b

labwork4a: labwork4a.o labwork4.o
	gcc labwork4a.o labwork4.o -o labwork4a

labwork4b: labwork4b.o labwork4.o
	gcc labwork4b.o labwork4.o -o labwork4b

%.o: %.c labwork4.h
	gcc -c -g $< -o $@

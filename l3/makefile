all: calc

calc: calc.o
	ld -o calc calc.o -m elf_i386
calc.o: calc.s
	as --32 -o calc.o calc.s -gstabs

clean:
	rm -rf *o calc
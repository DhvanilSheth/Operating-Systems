make:
	yasm -f elf64 -g dwarf2 Asgn.asm
	gcc -m64 -no-pie Asgn.o -o Asgn
run:
	./Asgn

clean:
	rm Asgn.o Asgn


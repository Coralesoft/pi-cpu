# build an executable named pi-cpu from main.c
  all: main.c 
	gcc -O2  -c ./main.c -o pi-cpu.o
	gcc  -o ./pi-cpu ./pi-cpu.o  -static-libstdc++ -static-libgcc -static -s 

  clean: 
	  $(RM) ./pi-cpu
	  $(RM) ./pi-cpu.o

install: 
	pi-cpu

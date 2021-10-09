# build an executable named pi-cpu from main.c
  all: main.c 
	gcc -O2  -c ./main.c -o picpu.o
	gcc  -o ./picpu ./picpu.o  -static-libstdc++ -static-libgcc -static -s 

  clean: 
	  $(RM) ./picpu
	  $(RM) ./picpu.o

install: 
	cp ./picpu ./bin/
	cp ./picpu /usr/local/bin/picpu

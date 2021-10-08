# build an executable named pi-cpu from main.c
  all: main.c 
	gcc -O2  -c ./main.c -o obj/Release/pi-cpu.o
	gcc  -o bin/Release/pi-cpu obj/Release/pi-cpu.o  -static-libstdc++ -static-libgcc -static -s 

  clean: 
	  $(RM) ./bin/Release/pi-cpu
	  $(RM) ./obj/Release/pi-cpu.o
install: 
	mv ./bin/Release/pi-cpu /usr/local/bin/

all : server

driver.o : driver.c driver.h types.h
	gcc -c driver.c -lpthread

util.o : util.c util.h
	gcc -c util.c -lpq

main.o : main.c util.h driver.h
	gcc -c main.c -lpthread

server : driver.o util.o main.o
	gcc driver.o util.o main.o -o server -lpthread -lpq
    
clean :
	rm -f server *.o
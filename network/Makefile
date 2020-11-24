all : museen_tcp_server

driver.o : driver.c driver.h types.h
	gcc -c driver.c -lpthread

util.o : util.c util.h
	gcc -c util.c -lpq

server.o : server.c server.h driver.h util.h
	gcc -c server.c -lpthread

main.o : main.c server.h
	gcc -c main.c

museen_tcp_server : driver.o util.o server.o main.o
	gcc driver.o util.o server.o main.o -o museen_tcp_server -lpthread -lpq
    
clean :
	rm -f server *.o
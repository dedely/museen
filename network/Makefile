all : museen_tcp_server

util.o : util.c util.h
	gcc -c util.c -lpq

query.o : query.c query.h types.h util.h
	gcc -c query.c -lpq

driver.o : driver.c driver.h server.h query.h types.h util.h
	gcc -c driver.c -lpthread

server.o : server.c server.h driver.h util.h query.h
	gcc -c server.c -lpthread

main.o : main.c server.h types.h
	gcc -c main.c

museen_tcp_server : driver.o main.o server.o util.o query.o
	gcc driver.o main.o util.o server.o query.o -o museen_tcp_server -lpthread -lpq
    
clean :
	rm -f museen_tcp_server *.o
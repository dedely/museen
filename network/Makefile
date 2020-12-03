PG_PATH = $(shell pg_config --includedir)
PG_LIBS = $(shell pg_config --libdir)
CPPFLAGS += -I$(PG_PATH)
LDFLAGS += -L$(PG_LIBS)

all : museen_tcp_server

types.o : types.c types.h
	gcc -c types.c

util.o : util.c util.h types.h
	gcc -c util.c

query.o : query.c query.h types.h util.h
	gcc -c -I$(PG_PATH) query.c

driver.o : driver.c driver.h server.h query.h types.h util.h
	gcc -c -I$(PG_PATH) driver.c

server.o : server.c server.h driver.h util.h query.h types.h
	gcc -c -I$(PG_PATH) server.c 

main.o : main.c server.h types.h
	gcc -c -I$(PG_PATH) main.c

museen_tcp_server : driver.o main.o server.o util.o query.o types.o
	gcc driver.o main.o util.o server.o query.o types.o -o museen_tcp_server -lpthread -L$(PG_LIBS) -lpq
    
clean :
	rm -f museen_tcp_server *.o
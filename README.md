# Museen

## Overview
Management of a classical museum: inventory management, referencing of exhibited works and room organization; visitor management (single entry/subscription rates).

Smart guided tours: visitors have an application that locates them inside the museum and makes suggestions according to attendance and/or user preferences.

## Contributors
  - Aëlien Moubêche
  - Adel Abbas

## Running
Here's how you should run museen network server for the first time :
```bash
make && ./museen_tcp_server
```
You can specify the ip address and the port, if you don't the server will use default settings (i.e. listen on the port 42424 of all avaible addresses on your machine).
```bash
./museen_tcp_server -i ip_adress -p port
```
Enter debug mode and directly test the database connection with:
```bash
./museen_tcp_server -d
```

Finally, you can list command-line options with
```bash
./museen_tcp_server -h
```

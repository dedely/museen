# Museen

## Overview
Management of a classical museum: inventory management, referencing of exhibited works and room organization; visitor management (single entry/subscription rates).

Smart guided tours: visitors have an application that locates them inside the museum and makes suggestions according to attendance and/or user preferences.

## Contributors
  - Aëlien Moubêche
  - Adel Abbas

## Running
The museen network server has 3 command-line options at the moment.

Here's how you should run it for the first time :
```bash
make && ./museen_tcp_server
```

Enter debug mode and directly test the database connection with:
```bash
./museen_tcp_server -d
```

Finally, you can list command-line options with
```bash
./museen_tcp_server -h
```
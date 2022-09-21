#!/usr/bin/sh
gcc -Wall -pedantic -o test.exe resource.h lab.h infected.h cell.h board.h resource.c lab.c infected.c cell.c board.c main.c -lpthread
./test.exe
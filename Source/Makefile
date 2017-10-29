#
# IFJ 2017 project
#	File			Makefile
#	Description		Makefile for gcc
#	Author			Michal Zilvar (xzilva02)
#	Last update		12:15, 29-10-2017

CFLAGS=-std=c99 -Wall -pedantic -g
NAME=IFJ17
CC=gcc
OBJ=main.o scanner.o string.o error.o

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	rm IFJ17
	rm main.o
	rm scanner.o
	rm string.o
	rm error.o
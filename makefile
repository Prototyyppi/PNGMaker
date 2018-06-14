#This is a simple makefile for building, cleaning and running Subscription.

FLAGS = -Wall -o maker.o
CC = gcc
FILE = pngmaker.c

.PHONY: run clean

pngmaker: $(FILE)
	@$(CC) $(FLAGS) $(FILE)
	@echo "--> Compiled OK <--"


clean:
	@$(RM) *.pngtest
	@echo "--> Cleaned OK <--"

# Run program
run:
	@echo
	@./maker.o


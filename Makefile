#Author : Ali Mohammad
#File: Makefile

CC = gcc

#sharedProcessMain.c sharedProcessFunctions.c, and sharedProcessHeader.h
#should be in the same directory. 
FILES = sharedProcessMain.c sharedProcessFunctions.c

OUT_EXE = sharedProcess.out

build: $(FILES)
	$(CC) $(FILES) -o $(OUT_EXE)
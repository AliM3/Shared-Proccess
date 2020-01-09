/*=============================================================================
|  Source Code:  sharedProcessHeader.h
|       Author:  Ali Mohammad
|
|     Language:  C
|	 File Type:  .h
|
|  +---------------------------------------------------------------------------
|  
|  Description: Header file for the shared process program. It includes the 
|				headers used in the programs, defined constants, and the 
|				function prototypes.
|                  
|  
*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>	//Used to check if memory attachment was successful

//Used for shared memory and fork processes
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MIN_ARGS 2		//Command line needs at least one integer input
#define MAX_ARGS 8		//Number of memory space and function call
#define MIN_INPUT_VAL 0	//Minimum integer input
#define MAX_INPUT_VAL 9	//Maximum integer input
#define TRUE 1
#define FALSE 0
#define FAILURE 1
#define PARENT 0		//ID for Parent
#define END_FUNCTION 0

//Prototypes
int validateInput(int argumentCount, char *arguments[]);

int getSharedMemory(int argumentCount);

int *attachMemory(int mem_ID);

void detachMemory(int *mem_ptr);

void removeMemory(int mem_ID);

void fillMemory(int *mem_ptr, int argumentCount, char* arguments[]);

void displayMemory(int role, int *mem_ptr, int argumentCount);

void executeChildProcess(int *mem_ptr, int argumentCount);

void editMemory(int currentChild, int *mem_ptr, int argumentCount);

void waitForOthers(int numOfChildren);

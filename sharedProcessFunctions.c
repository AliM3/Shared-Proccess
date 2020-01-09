/*=============================================================================
|   Source code:  sharedProccessFunctions.c
|        Author:  Ali Mohammad
|    Student ID:  5914999  
|    Assignment:  Program #6 Shared Process
|  
|        Course:  COP 4338 (Programming III)   
|       Section:  U03
|    Instructor:  William Feild  
|      Due Date:  November 30, 2018, at the beginning of class
|
|	I hereby certify that this collective work is my own
|	and none of it is the work of any other person or entity.
|
|
|	______________________________________
|  
|        Language:  C
|		File Type:  .c
|
|  +---------------------------------------------------------------------------  
|  
|  Description: Functions used to run the Shared Process program
|                  
*============================================================================*/

#include "sharedProcessHeader.h"

/*---------------------------- validateInput ----------------------------------
|  Function: validateInput(argumentCount, arguments)
|
|  Purpose : Checks if the input is valid. 
|			 There must be one to seven integer values given and if each
|			 integer is between zero and nine.
|
|  @param    Number of arguments passed
|  @param    Array of the arguments
|
|  @return   True or false value
*----------------------------------------------------------------------------*/
int validateInput(int argumentCount, char *arguments[])
{	
	printf("%s", "Parent: Validate Command Line\n");
	
	int arg = 0;
	
	double input = 0;

	if(MIN_ARGS <= argumentCount && argumentCount <= MAX_ARGS)
	{
		for(arg = 1; arg < argumentCount; arg++)
		{
			input = strtod(arguments[arg], NULL);
			
			if(input == 0)
			{
				if(strcmp(arguments[arg], "0") != 0)
				{
					printf("%s","The input must be an integer\n");
					
					return FALSE;
				}
			}
			if(input < MIN_INPUT_VAL || input > MAX_INPUT_VAL)
			{
				printf("%s", "Input value was not in range\n");
				
				return FALSE;
			}
		}
		
		return TRUE;
	}
	
	printf("%s","Invalid number of arguments\n");
	
	return FALSE;
}

/*---------------------------- getShareMemory ---------------------------------
|  Function: getSharedMemory(argumentCount)
|
|  Purpose : Requests shared memory
|
|  @param    Number of arguments that are to be placed in the memory
|
|  @return   the location of the memory
*----------------------------------------------------------------------------*/
int getSharedMemory(int argumentCount)
{
	printf("%s", "Parent: Requests shared memory\n");
	
	int spaces = argumentCount - 1;
	
	//Gets the shared memory
	int memory = shmget(IPC_PRIVATE, spaces * sizeof(int), IPC_CREAT | 0666);
	
	if(memory < 0)
	{
		printf("%s","Requested memory could not be given\n");
		
		exit(FAILURE);
	}
	else
	{
		printf("%s", "Parent: Receives shared memory\n");
		
		return memory;
	}
}


/*---------------------------- attachMemory -----------------------------------
|  Function: attachMemory(mem_ID)
|
|  Purpose : Attaches the memory to the parent
|
|  @param    The ID of the memory to be attached
|
|  @return   The pointer to the memory
*----------------------------------------------------------------------------*/
int *attachMemory(int mem_ID)
{	
	printf("%s","Parent: Attaches shared memory\n");
	
	//Pointer to the attached memory
	int *shm_ptr = (int *) shmat(mem_ID, NULL, 0);
	
	if((intptr_t) shm_ptr == -1)
	{
		printf("%s", "Memory could not be attached");
		
		exit(FAILURE);
	}
	else
	{
		return shm_ptr;
	}
}

/*----------------------------  detachMemory ----------------------------------
|  Function: detachMemory(mem_ptr)
|
|  Purpose : Detaches the memory from the parent
|
|  @param    The pointer of the memory to be detached
*----------------------------------------------------------------------------*/
void detachMemory(int *mem_ptr)
{
	printf("%s", "Parent: Detaches shared memory\n");
	
	shmdt(mem_ptr);
}

/*---------------------------- removeMemory -----------------------------------
|  Function: removeMemory(mem_ID)
|
|  Purpose : Removes the memory
|
|  @param    The ID of the memory to be removed
*----------------------------------------------------------------------------*/
void removeMemory(int mem_ID)
{
	printf("%s", "Parent: Removes shared memory\n");
	
	shmctl(mem_ID, IPC_RMID, NULL);
}

/*---------------------------- fillMemory -------------------------------------
|  Function: fillMemory(mem_ptr, argumentCount, arguments[])
|
|  Purpose : Fills the memory with an array of values given in the command
|			 line.
|
|  @param    The location of the memory to be filled
|  @param	 The number of items to be placed in the memory
|  @param 	 The array of values to be converted and copied into the memory
*----------------------------------------------------------------------------*/
void fillMemory(int *mem_ptr, int argumentCount, char* arguments[])
{
	printf("%s","Parent: Fills shared memory\n");
	
	int arg = 0;
	
	for(arg = 0; arg < argumentCount - 1; arg++)
	{
		mem_ptr[arg] = atoi(arguments[arg + 1]);
	}
}

/*---------------------------- displayMemory ----------------------------------
|  Function: displayMemory(role, mem_ptr, argumentCount)
|
|  Purpose : Prints the values int the memory location
|
|  @param    Checks if the child or parent is requesting the function
|  @param	 The location of the memory
|  @param	 The number of values to be printed from the memory
*----------------------------------------------------------------------------*/
void displayMemory(int role, int *mem_ptr, int argumentCount)
{
	int args = 0;
	
	if(role == PARENT)
	{
		printf("%s", "Parent: Displays shared memory\n");
	
		for(args = 0; args < argumentCount - 1; args++)
		{
			printf("%d%s", mem_ptr[args], " ");
		}
	}
	else
	{
		printf("%s%d", "	Child ID: ", role);
		printf("%s", " Displays shared memory \n	");
		
		for(args = 0; args < argumentCount - 1; args++)
		{
			printf("%d%s", mem_ptr[args], " ");
		}
	}
	
	printf("%s", "\n");
}

/*------------------------- executeChildProcess -------------------------------
|  Function: executeChildProcess(mem_ptr, argumentCount)
|
|  Purpose : Forks off other processes to edit an element of the memory
|			 they are assigned to and prints the result.
|
|  @param    The memory to be used
|  @param	 The number of elements in the memory
*----------------------------------------------------------------------------*/
void executeChildProcess(int *mem_ptr, int argumentCount)
{
	int numOfChildren = argumentCount - 1;
	
	int currentChild = 0;
	
	int status;
	
	pid_t pid[numOfChildren];
	
	for(currentChild = 0; currentChild < numOfChildren; ++currentChild)
	{	
		pid[currentChild] = fork();
		
		if(pid[currentChild] != 0)
		{
			printf("%s%d%s", "Parent: Forks child ", currentChild + 1, " process\n");
		}
		if(pid[currentChild] == -1)
		{
			perror("fork");
		}
		if(pid[currentChild] == 0)
		{	
			editMemory(currentChild, mem_ptr, argumentCount);
			
			exit(0);
		}
	}
	
	waitForOthers(numOfChildren);
	
	printf("%s", "All child IDs and exit codes: \n");
	
	for(currentChild = 0; currentChild < numOfChildren; currentChild++)
	{
		printf("%s%d	", "PID: ", pid[currentChild]);
		
		printf("%s%d\n", "Exit Code: ", WEXITSTATUS(status));
	}
}

/*---------------------------- editMemory -------------------------------------
|  Function: editMemory(int currentChild, int *mem_ptr, int argumentCount)
|
|  Purpose : Edits a particular element of the memory
|
|  @param	 The child that will be editing the memory
|  @param    The memory to be edited
|  @param	 The number of elements in the memory
*----------------------------------------------------------------------------*/
void editMemory(int currentChild, int *mem_ptr, int argumentCount)
{
	printf("%s%d%s", "	Child ID: ", currentChild + 1, " Starts\n");

	displayMemory(currentChild + 1, mem_ptr, argumentCount);

	printf("%s%d%s%d%s", "	Child ", currentChild + 1, " Unique PID : ", getpid(), "\n");
	
	printf("%s%d%s", "	Child ID: ", currentChild + 1, " Updates shared memory\n");

	//Edits the element the child is assigned to
	mem_ptr[currentChild] = mem_ptr[currentChild] * (currentChild + 1);

	displayMemory(currentChild + 1, mem_ptr, argumentCount);

	printf("%s%d%s", "	Child ID: ", currentChild + 1, " Exits \n");
}

/*---------------------------- waitForOthers ----------------------------------
|  Function: waitForOthers(int numOfChildren)
|
|  Purpose : Waits for all other child processes to finish
|
|  @param	 The number of children that the function will wait for
*----------------------------------------------------------------------------*/
void waitForOthers(int numOfChildren)
{
	printf("%s", "Parent: Waits for (each) child\n");
	
	int currentChild = 0;
	
	pid_t pid = 0;
	
	int status;
	
	for(currentChild = 0; currentChild < numOfChildren; ++currentChild)
	{
		pid = wait(&status);
		
		printf("	Child Exit Code: %d\n", WEXITSTATUS(status));
		
		printf("%s%d%s", "Parent: Detects child ", pid, " completion\n");
	}
	
	printf("%s", "Parent: Detects all children completion\n");
}
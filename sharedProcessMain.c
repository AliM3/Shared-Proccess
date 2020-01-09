/*=============================================================================
|  Source code:  sharedProcessMain.c
|       Author:  Ali Mohammad
|
|	I hereby certify that this collective work is my own
|	and none of it is the work of any other person or entity.
|
|
|	______________________________________
|  
|     Language:  C
|	 File Type:  .c
|
|	Before Running: Be sure to have sharedProcessMain.c, 
|					sharedProcessFunctions.c, SharedProcessHeader.h,
|					and Makefile all in the same directory.
|
|     Compile/Run:
| 		gcc sharedProcessMain.c sharedProcessFunctions.c –o sharedProcess.out
|		(Enter)
|		./sharedProcess.out ! @ # $ % ^ & 
|
|		OR
|
|		make
|		(Enter)
|		./sharedProcess.out ! @ # $ % ^ & 
|
|		Each symbol (! @ # $ % ^ &) represents an integer that must in the 
|		range zero to nine. The number of inputs must be between one to seven.
|
|  +---------------------------------------------------------------------------
|  
|  Description: This program will receive input from the user in the command
|				line. A number of child processes equal to the number of inputs
|				will be created. The parent will create a shared memory for
|				each child to alter. A child will only alter its own
|				element in the array. The array will displayed be and after
|				the alteration. A trace of the program will also be printed.
|
|		 Input: The input will only be digits and the number of inputs
|				will indicate the number of child processes to be made.
|				The user must have one to seven inputs and each input must
|				be within the range of zero to nine.
|
|		Output: A trace will be shown for each step of the process. An array
|				will also be printed. The array will initially hold the values
|				that were given in the command line. Then the program will
|				alter the array and print it again. All memory created by
|				the program will be deleted once the program has been 
|				completed.
|
|	   Process: The parent starts once the program starts. Each step is printed
|				as the program runs. The user input is validated before the 
|				entire program is executed. If the input is valid, a piece
|				of shared memory is created and attached. The memory is then 
|				filled with an array that has the values given in the command
|				line. The memory is displayed and then the child processes are
|				executed. Each step of the child process is printed. Each child
|				will go to the element of the array they are assigned to and
|				multiply their ID with that value and store it again. The 
|				final result of the memory will be printed again. The memory
|				is then detached and removed. The parent then states that it
|				has finished.
|  
*============================================================================*/

#include "sharedProcessHeader.h"

int main(int argc, char *argv[])
{
	printf("%s", "Parent: Starts\n");

	if(validateInput(argc, argv))
	{
		int sharedMemory = getSharedMemory(argc);
		
		int *mem_ptr = attachMemory(sharedMemory);
		
		fillMemory(mem_ptr, argc, argv);
		
		displayMemory(PARENT, mem_ptr, argc);
		
		executeChildProcess(mem_ptr, argc);

		displayMemory(PARENT, mem_ptr, argc);
		
		detachMemory(mem_ptr);
		
		removeMemory(sharedMemory);
	}
	
	printf("%s", "Parent: Finished\n");
	
	return END_FUNCTION;
}

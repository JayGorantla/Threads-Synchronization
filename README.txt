# Compilation 
This program can be compiled with the given Makefile using the "make" command. In the Makefile, 
there is a -g flag that allows for using the GDB Debugger. After using the "make" command, run 
"final" with "./final" followed by only one number that defines the number of COVID callers 
(threads) you want to create.

# Implementation
--> Firstly, to implement this program, I started off by importing numerous header files, most 
notably <semaphore.h>, <pthread.h>, and <unistd.h>. Then I started off by declaring the global 
variables that we were told to have. I also added a global variable of next_id that is initialized
to 1. 

--> The main method allows for input to be taken from the command prompt line. However, there is error 
checking done in the main method that ensures only 1 integer is being taken by the program. The 
program follows by initializing the pthread_t array of length input and also the two semaphores we 
will be using in our program. One of them is a binary semaphore named connected_lock and the other 
is a 4ary semaphore called operators (for the 4 operators we have). The main method then creates 
and joins the pthreads we have created using two separate for loops of length call. Finally, the 
main method ends in the two semaphores being destroyed to follow professional programming constraints. 

--> The method phonecall is the primary control for the threads and the COVID calls simulation. This method 
is in charge of taking care of connecting the callers to the lines, assigning operators to those lines,
and printing statements that show the progress that is being made. This method uses critical sections of 
code that are locked and unlocked by semaphores. Assigning the id from thread to thread without repeats
is done here inside a binary semaphore. The bulk of the method is done using an if/else statement that 
connects the caller to an available line if there is one open. If not, it is put on hold using a while
loop and then connected to a line when open. It stays in the while loop until it enters an if statement 
only when there is an open line. Then, connected is incremented in a critical section and it breaks from
the while loop. After this, the operators semaphore creates a critical section where print statements are 
executed along with a sleep call to simulate an actual operator. Then, the connected_lock semaphore is 
used again to decrement connected as one more thread has been finished and another thread can be connected 
to a line. Program finishes with a pthread exit call. 

#Project Requirements
A - Binary semaphore connected_lock is used repetitively only to protect critical regions of code such
as incrementing and decrementing connected and next id.
B - Counting semaphore is used and explained on line 100 where it ensures only 4 operators are working
at a time
C - Semaphores are properly initialized and destroyed in the main method
D - Thread function named phonecall is created and implemented to satisfy the needs of the program
E - Threads are created and joined properly in the main method with two separate for loops that allow
for proper implementation
F - Int variable of next_id exists and is incremented with connected_lock in the phonecall function
G - Variable connected is properly incremented in the phonecall function to represent the number of 
connected callers. This is done with the connected_lock semaphore. 
H - All the print statements needed are printed with the corresponding id as it given as a variable
I - Static modifier is used in the program for all variables
J - Irregular and no user input is managed properly using the error handling code presented at the top
of the main function
K - The program correctly works and suits all the necessary requirements
L - Program works correctly with any number of phone calls
M - No segmentation faults are found
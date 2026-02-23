# CMPT 360 - ASO1 | X02L
-----------------------------------------------
Name: Viraj Randeria
ID: 3143233
Date: 21/02/2026
-----------------------------------------------

1. academic Integrity

"I certify that this submission represents entirely my own work."

2. Overview

This program takes in the policy type, burst time (quantum) if RR and the file with workloads
as command line arguments.
Checks if the arguments are correct,
and implements the correct scheduling policy.

3. Build

Compilation:

    command "make": compiles sched.c, queue.c and sched.h and creates an executable called "sched"
    to run it type "./sched" in the command line

    command "make sched": does the same thing as make.

    command "make clean": clears all the executables.


Valgrind:
    use command - make valgrind ARGS="(enter the argument here)" for valgrind 
    eg: 
        make valgrind ARGS="./sched --policy=RR --quantum=1 --in=Workload_Samples/W1.txt"

    The program has zero leaks and no errors.

4. Run

    to run it type "./sched (arguments)" in the command line
          Enter a arguments here ^
    
    if the format of argument is incorrect, it will print out the correct usage
        "Usage: ./sched --policy=FCFS|RR [--quantum=N] --in=FILE"


5. Logic Summary
    
    The program first takes in the arguments from the command line and checks if the
    format is right. 

    Then checks for the policy.

    if policy = FCFS,
        it dynamically allocates the time, run and policy array that store the string required to be printed.
        
        it calls getline() and receives pid, arrival and cpu_time from it.

        for each line, 
            it calculates the TAT, RESP, and completion time
            it adds the number of time ticks to time array
            keeps adding the pid to run array while it is running
            adds it TAT and RESP to the total TAT and total RESP
    
    if policy == RR,
        it reads through the file
        initiates the process instance
            adds the data received from the file to it
        
        then initialize the timeline instance,
        which creates a queue containing the pid in it.

        then start the round robin logic 
            - initialize the ready queue
            - add the processes to it
            while all processes are not done
            - pop a process from the queue
            - run it for the burst time(quantum) 
            - if the process is completed, dequeue it and move to the next process
            - else, reset the quantum and enqueue it again.
            continue until all processes are completed 
            after completion, compute the metrics 
        
        then print the timeline, processes and the metrics



6. Status
    The program works perfectly
    



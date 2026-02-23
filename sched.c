// CMPT 360 - ASO1 | X02L
// -----------------------------------------------
// Name: Viraj Randeria
// ID: 3143233
// Date: 22/02/2026
// -----------------------------------------------

#include "sched.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// Initialize the used functions
static process_t *read_workload_infile(const char *infile, int *final_n);
void timeline_init(timeline_t *timeline, int cap);
void timeline_free(timeline_t *timeline);
void timeline_add(timeline_t *timeline, int pid);
int count_context_switches(const timeline_t *timeline);
void print_timeline(const timeline_t *tl);
void print_metrics(process_t *process, int n, const timeline_t *tl);
void RR(process_t *p, int n, int quantum, timeline_t *tl);



int main(int argc, char *argv[]){
  char *policy = NULL;
  char *infile = NULL;
  int quantum = -1;
  int valid = 1;

  if (strncmp(argv[1],"--policy=",9) == 0){
      policy = argv[1] + 9;
    if (strcmp(policy,"FCFS")!=0 && strcmp(policy,"RR") !=0){
      valid = 0;
    }
  }
  else{
    valid = 0;
  }

  if ((valid == 1) && strcmp(policy,"RR") == 0){
    if (argc != 4 ){
      valid = 0;
    }

    if (strncmp(argv[2], "--quantum=",10)==0){
      quantum = atoi(argv[2] + 10);
      if (quantum < 0){
        valid = 0;
      }
    } 
    else{
      valid = 0;
    }

    if (strncmp(argv[3],"--in=",5) == 0){
      infile = argv[3] + 5;
    }
    else{
      valid = 0;
    }
  }

  if ((valid == 1) && strcmp(policy,"FCFS") == 0){
    if (argc != 3){
      valid = 0;
    }
    if (strncmp(argv[2],"--in=",5) == 0){
      infile = argv[2] + 5;
    }
    else{
      valid = 0;
    }
  }

  if (valid == 0){
    printf("Usage: ./sched --policy=FCFS|RR [--quantum=N] --in=FILE\n");
    return 0;
  }

  // implement the FCFS scheduling algorithm
  else if(strcmp(policy, "FCFS") == 0) {
    FILE *file = fopen(infile,"r");
    if (file != NULL){
        char *line= NULL; // initialiez the holder of each line for getline()
        size_t size = 0; // size for getline
        ssize_t nread; // = -1 if no more output from getline()

        int count_line = 0;
        int total_time = 0; // this is needed to compute TAT

        int pid, arrival, cpu_time, first_run, completion, tat, response;

        // this contains the string of time:
        char* time = malloc(10* sizeof(char));
        size_t time_cap = 10;
        size_t time_len = 0;

        // this contains the string of run
        char* run = malloc(10* sizeof(char));
        size_t run_cap = 10;
        size_t run_len = 0;

        // run += "run: "
        snprintf(run, run_cap, "run:  ");
        run_len = strlen(run);

        // this contains the processes string
        char* processes = malloc(10* sizeof(char));
        int process_len = 0;
        int process_cap = 10;

        int rows=0; 
        int total_tat=0;
        int total_response=0;       

        // loops through the file once to implement FCFS
        while((nread = getline(&line, &size, file)) != -1){
          // do not read the first two lines 
          if (count_line > 1){
            sscanf(line, "%d %d %d", &pid, &arrival, &cpu_time);
            
            // add "- " to run if no processes are running
            while(total_time < arrival){
              total_time += 1;
              int needed = snprintf(NULL, 0, "- ");
              while(run_len + needed +1 > run_cap){
                run_cap *= 2;
              }
              run = realloc(run, run_cap);

              snprintf(run + run_len, run_cap - run_len, "- ");
              run_len += needed;
            }
            
            // for a new process
            first_run = total_time; // set first_run as current time
            total_time += cpu_time; // add cpu time to total time
            completion = total_time; // set the completion time of the previous process
            tat = completion - arrival;
            response = first_run - arrival;

            total_response += response;
            total_tat += tat;
            rows += 1;

            // PRINT PROCESSES
            int needed = snprintf(NULL, 0, "P%i: first run=%i completion=%i TAT=%i RESP=%i\n",pid, first_run, completion, tat, response);

            while (process_len + needed + 1 > process_cap){
              process_cap *= 2;
            }
            processes = realloc(processes, process_cap);

            snprintf(processes + process_len, process_cap - process_len, "P%i: first run=%i completion=%i TAT=%i RESP=%i\n",pid, first_run, completion, tat, response);
            process_len += needed;

            // PRINTING RUN
            for (int i=0; i< cpu_time; i++){
              int needed = snprintf(NULL, 0, "%d ", pid);
              
              while(run_len + needed + 1> run_cap){
                run_cap *= 2;
              }
              run = realloc(run, run_cap);

              snprintf(run + run_len, run_cap - run_len, "%d ", pid);
              run_len += needed;
            }

          }

          count_line += 1;
        }

        free(line);
        fclose(file);

        // PRINTING TIME SLICES
        snprintf(time, time_cap, "time: ");
        time_len = strlen(time);
        for (int i=0; i< total_time; i++){
          int needed = snprintf(NULL, 0, "%d ", i);

          while (time_len + needed + 1> time_cap){
            time_cap *= 2;
          }
          time = realloc(time, time_cap);
          
          snprintf(time+time_len, time_cap - time_len, "%d ", i);
          time_len += needed;
        }

        
        // CALCULATE SYSTEM AVERAGES
        int ctx_switches = rows - 1;
        float avg_tat = (float) total_tat/rows;
        float avg_response = (float) total_response/rows;

        processes[strlen(processes) -1] = '\0';

        printf("%s\n",time);
        printf("%s\n", run);
        printf("%s\n",processes);

        // PRINT SYSTEM METRICS
        
        printf("System: ctx_switches=%i, avgTAT=%.3f, avgRESP=%.3f \n", ctx_switches, avg_tat, avg_response);

        free(run);
        free(time);
        free(processes);

    }
    else{
      printf("file does not exist\n");
    }
  }

  // implement round robin
  else if (strcmp(policy,"RR")==0){
    int n = 0; // variable for total number of processes

    // read the file and initialize the process_t instance
    process_t *p = read_workload_infile(infile,&n); 

    if (p == NULL){
      printf("file does not exist.\n");
      return 0;
    }

    timeline_t tl; // declare tl as an instance of timeline_t
    timeline_init(&tl,64); // initialize the timeline

    RR(p,n,quantum,&tl); // implement Round robin scheduling

    print_timeline(&tl); // print timeline
    print_metrics(p,n,&tl); // print metrics

    timeline_free(&tl); // free the timeline
    free(p); // free the process's data array
    
  }
}

/*
Purpose: reads the file, retrieves data from it, and initializes the process_t structure
params: infile - the file containing workload
        final_n - the number of processes
return: the process_t structure which contains the processes 
        and set the number of processes to final_n.
*/
static process_t *read_workload_infile(const char *infile, int *final_n){
  FILE *file = fopen(infile, "r");
  if (!file){
    return NULL;
  }

  int cap = 16;
  int n = 0;

  // initialize the array that will hold the pids in the process_t struct
  process_t *processes = (process_t*) malloc(cap * sizeof(process_t));
  
  char *line = NULL; // the pointer to each line
  size_t size= 0;

  while ((getline(&line, &size, file)) != -1){
    int pid, arrival, cpu;
    if (sscanf(line, "%d %d %d", &pid,&arrival,&cpu) != 3){
      continue;
    }

    // reallocate the processes array if the size exceeds.
    if (n == cap){
      cap *= 2;
      processes = realloc(processes, cap * sizeof(process_t));
    }

    processes[n].pid = pid;
    processes[n].arrival = arrival;
    processes[n].cpu_time = cpu;
    processes[n].remaining = cpu;
    processes[n].first_run = -1; // remains -1 until first run or scheduled time tick arrives
    processes[n].completion =-1;

    n++;
  }
  free(line);
  fclose(file);

  *final_n = n;
  return processes;
}

/*
Purpose: initialize the timeline structure
params: timeline - the instance of timeline_t structure
        cap - the capacity of structure
return: none 
*/
void timeline_init(timeline_t *timeline, int cap){
  timeline->cap = cap;
  timeline->run = (int*)malloc(timeline->cap * sizeof(int));
  timeline->len = 0;
}

/*
Purpose: frees the run array of the timeline struct and sets it to NULL.
params: timeline - the instance of timeline_t struct
return: none
*/
void timeline_free(timeline_t *timeline){
  free(timeline->run);
  timeline->run = NULL;
  timeline->cap = 0;
  timeline->len = 0;
}

/*
Purpose: adds a pid to the run array of timeline instance. 
         Resizes the timeline run array if needed.
params: timeline - the instance of timeline_t struct
        pid - the pid that needs to be added to run 
return: none 
*/
void timeline_add(timeline_t *timeline, int pid){
  if (timeline->len >= timeline->cap){
    timeline->cap *= 2;
    timeline->run = realloc(timeline->run, timeline->cap * sizeof(int));
  }
  timeline->run[timeline->len] = pid;
  timeline->len++;
}

/*
Purpose: counts the number of context switches in the run array of instance of timeline
params: timeline - the instance of timeline_t struct
return: The number of context switches
*/
int count_context_switches(const timeline_t *timeline){
  int ctx = 0;
  int prev = -1; // prev is -1 for the first pid.
  
  // loops through the run array and checks if the current pid is 
  // equal to the previous pid. if not then increment the ctx variable.
  for (int t=0; t < timeline->len; t++){
    int current = timeline->run[t];
    if (current >=0 && prev>=0 && current != prev){
      ctx++;
    }
    if (current >=0){
      prev = current;
    }
  }
  return ctx;
}

/*
Purpose: prints the timeline in the required format.
params: tl - the instance of timeline_t struct
return: none 
*/
void print_timeline(const timeline_t *tl){
  printf("time:");
  for (int i=0; i<tl->len; i++){
    printf(" %d",i);
  }
  printf("\n");
  printf("run:");
  for (int i=0; i<tl->len; i++){

    if (tl->run[i] <0){
      printf(" -");
    }
    else{
      printf(" %d",tl->run[i]);
    } 
  }
  printf("\n");
}

/*
Purpose: calculates the average turnaround time and response time.
params: process - instance of the process_t struct
        n - the length of the data array of queue instance
        tl - instance of timeline struct
return: none 
*/
void print_metrics(process_t *process, int n, const timeline_t *tl){

  double sumTAT = 0.0, sumRESP = 0.0;

  for (int i=0; i<n; i++){
    int tat = process[i].completion - process[i].arrival;
    int resp = process[i].first_run - process[i].arrival;

    sumTAT += tat;
    sumRESP += resp;

    printf("P%d: first run=%d completion=%d TAT=%d RESP=%d\n",
      process[i].pid, process[i].first_run, process[i].completion, tat, resp);
  }

  printf("System: ctx_switches=%d, avgTAT=%.3f, avgRESP=%.3f\n",count_context_switches(tl),
          sumTAT/n, sumRESP/n);
}

/*
Purpose: implements the round robin logic
params: p - instance of the process_t struct
        n - the number of processes in the data array of queue instance
        quantum - the burst time.
        tl - instance of timeline struct
return: none 
*/
void RR(process_t *p, int n, int quantum, timeline_t *tl){
  queue_t ready_queue; // declare the ready queue
  queue_init(&ready_queue,n); // initialize the ready queue

  int t=0, next=0; 
  int done=0;
  int cur = -1; // when cur is -1, the cpu is idle
  int quantum_left = 0;

  // Loop until all the processes are completed
  while (done<n){
    // when a new process enters, add it to the ready queue
    while(next<n && p[next].arrival == t){
      enqueue(&ready_queue,next);
      next++;
    }

    // if no process is running, take one from ready queue and set its time to quantum
    if (cur == -1){
      cur = queue_pop(&ready_queue);
      if (cur != -1){
        quantum_left = quantum;
      }
    }

    // if no processes are available, add value of -1 to timeline's run array and move to 
    // the next process
    if (cur == -1){
      timeline_add(tl,-1);
      t++;
      continue;
    }

    // for a new process, record its first run time.
    if (p[cur].first_run == -1){
      p[cur].first_run = t;
    }

    // after one unit of cpu time, add pid back to timeline and decrement the remaining cpu time
    // decrement the left quantum time
    timeline_add(tl, p[cur].pid);
    p[cur].remaining--;
    quantum_left--;

    // if a process finishes, increment done and set its completion time.
    if (p[cur].remaining == 0){
      p[cur].completion = t+1;
      done++;
      cur = -1;
    }

    // else if the process is not done but the burst time is over then add it back 
    // to the end of the queue
    else if(quantum_left == 0){
      enqueue(&ready_queue,cur);
      cur = -1;
    }

    t++; // increment time after each run
  }
  queue_free(&ready_queue); // free the ready queue after all processes are done
}
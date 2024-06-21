#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>


//Declaring Global Constants 
#define EXIT 0
#define INVALID -1
#define CPU 1
#define PID 5 

//Assignment 4
#define RACE 2

//Assignment 5
#define BANKER 3
#define PROCESS 5
#define RESOURCE 3
#define TRUE 1
#define FALSE 0

//Assignment 6 
#define MEMORY 4
#define FIRST 0
#define BEST 1
#define WORST 2
#define NEXT 3

//Assignment 7
#define PAGE 5
#define FIFO 0
#define LRU 1
#define FRAMES 4

//Assignment 8
#define DISK 6
#define FCFS 0
#define SSTF 1
#define REQUEST 8
  
//Function Prototypes
int displayMenu();                                                              //Displaying The Menu
void cpuScheduling();                                                           //CPU Scheduling
void fcfs(int process[], int at[], int bt[]);                                   //First Come First Serve Scheduling Algorithm
void sjf(int process [], int at[], int bt[]);                                   //Shortest Job First Scheduling Algorithm
void displaySchedule(int process[], int at[], int bt[], int wt[], int tat[]);   //Displaying The Schedule

//Assignment 4 Function Prototypes
void raceCondition();                                                           //Race Condition
void *threadFuncOne();                                                          //Thread Function One
void *threadFuncTwo();                                                          //Thread Function Two

//Assignment 5 Function Prototypes
void bankersAlgorithm();                                                        //Bankers Algorithm
void systemState(int feasible[], int safe[]);                                   //System State

//Assignment 6 Function Prototypes
void memoryManagement();                                                        //Memory Management
void displayProcess(int allocation[], int processes, int processSize[]);        //Displaying The Process
void firstFit(int blockSize[], int blocks, int processSize[], int processes);   //First Fit Algorithm
void worstFit(int blockSize[], int blocks, int processSize[], int processes);   //Worst Fit Algorithm
void bestFit(int blockSize[], int blocks, int processSize[], int processes);    //Best Fit Algorithm
void nextFit(int blockSize[], int blocks, int processSize[], int processes);    //Next Fit Algorithm

//Assignment 7 Function Prototypes
void pageReplacement();                                                         //Page Replacement
void lru();                                                                     //Least Recently Used Page Replacement Algorithm
int findLRU(int time[]);                                                        //Finding The Least Recently Used Page
void fifo();                                                                    //First In First Out Algorithm
void displayPages(int pages, int allocation[]);                                 //Displaying The Pages

//Assignment 8 Function Prototypes
void diskScheduling();                                                          //Disk Scheduling
void diskFcfs(int request[], int head);                                         //Disk Scheduling First Come First Serve Algorithm
void diskSstf(int request[], int head);                                         //Disk Scheduling Shortest Seek Time First Algorithm

//Shared Resource
int resource = 5;

//Main Function
int main() {
  int choice = INVALID;
  
  while(choice != EXIT) {
    choice = displayMenu();
    switch (choice) {
    case CPU:
      cpuScheduling();
      break;
    case RACE:
      raceCondition();
      break;
    case BANKER:
      bankersAlgorithm();
      break;
    case MEMORY:
      memoryManagement();
      break;
    case PAGE:
      pageReplacement();
      break;
    case DISK:
      diskScheduling();
      break;
    case EXIT:
      exit(EXIT);
    }
  }
  return EXIT;
}

//Displaying The Menu
int displayMenu() {
  int choice = INVALID;

  //A While Loop For The Display Menu
  while (choice == INVALID) {
    printf("\n\t\t\t\t\t********** Operating System Management Menu ********** \n\n");
    printf("Select the OS program to run, enter the number of your selection.\n");
    printf("1. CPU Scheduling\n");
    printf("2. Race Condition\n");
    printf("3. Banker's Algorithm\n");
    printf("4. Memory Management\n");
    printf("5. Page Replacement\n");
    printf("6. Disk Scheduling\n");
    printf("0. Exit\n");

    //User's Input For The Display Menu
    printf("\nEnter your selection: ");
    scanf("%d", &choice);

    if (choice <= 0 && choice >= 6)
      choice = INVALID;
  }
  return choice;
}
//Scheduling The CPU 
void cpuScheduling() {
  int process[] = { 1, 2, 3, 4, 5 };
  int arrvTime[] = { 0, 2, 4, 6, 7 };
  int burstTime[] = { 8, 5, 10, 2, 3 };

  //Display Heading For First Come First Serve
  printf("\n\n");
  printf("*************** FCFS ***************\n\n");
  printf("PID\tAT\tBT\tWT\tTAT\n");
  printf("---\t---\t---\t---\t---\n");

  fcfs(process, arrvTime, burstTime);

  //Display Heading for Shortest Job First
  printf("\n\n");
  printf("*************** SJF ***************\n\n");
  printf("PID\tAT\tBT\tWT\tTAT\n");
  printf("---\t---\t---\t---\t---\n");

  sjf(process, arrvTime, burstTime);
}

//First Come First Serve Function
void fcfs(int process[], int at[], int bt[]) {
  int wt[PID];
  int tat[PID];

  wt[0] = 0;
  //Calculate The Wait Times For Each Process
  for (int i = 1; i < PID; i++) {
    wt[i] = bt[i - 1] + wt[i - 1];
  }
  //Calculate The Turnaround Times For Each Process
    for (int i = 0; i < PID; i++) {
      tat[i] = bt[i] + wt[i];
  }
    displaySchedule(process, at, bt, wt, tat);
}

//Shortest Job First Function
void sjf(int process[], int at[], int bt[]) {
  int wt[PID];
  int tat[PID];
  int idx;
  int temp;
  //Sort The Process Data Based On Burst Time Using Nested Loops
  for (int i = 0; i < PID; i++) {
    idx = i;

    for (int j = i + 1; j < PID; j++) {
      if (bt[j] < bt[idx]) {
        idx = j;
      }
    }
    //Swap The Burst Times
    temp = bt[i];
    bt[i] = bt[idx];
    bt[idx] = temp;

    //Swap The Process IDs
    temp = process[i];
    process[i] = process[idx];
    process[idx] = temp;

    //Swap The Arrival Times
    temp = at[i];
    at[i] = at[idx];
    at[idx] = temp;
  }

  wt[0] = 0; 
  //Calculate The Wait Times For Each Process
  for (int i = 1; i < PID; i++) {
    wt[i] = bt[i - 1] + wt[i - 1];
  }
  for (int i = 0; i < PID; i++) {
    tat[i] = bt[i] + wt[i];
  }
  displaySchedule(process, at, bt, wt, tat);
}

//Displaying The Schedule 
void displaySchedule(int process[], int at[], int bt[], int wt[], int tat[]) {
  int totWt = 0;
  int totTat = 0;
  float avgWt = 0;
  float avgTat = 0;

  for (int i = 0; i < PID; i++) {
    totWt = totWt + wt[i];
    totTat = totTat + tat[i];
    printf("%d\t%d\t%d\t%d\t%d\n", process[i], at[i], bt[i], wt[i], tat[i]);
  }

  //Calculate The Average Wait Time and Average Turnaround Time
  avgWt = (float)totWt / PID;
  printf("Average waiting time = %.2f\n", avgWt);

  avgTat = (float)totTat / PID;
  printf("Average turn around time = %.2f\n", avgTat);
}
//Race Condition Function
  void raceCondition() {
    pthread_t threadOne;
    pthread_t threadTwo;
    
    //Create The Threads 
    pthread_create(&threadOne, NULL, threadFuncOne, NULL);
    pthread_create(&threadTwo, NULL, threadFuncTwo, NULL);
    pthread_join(threadOne, NULL);
    pthread_join(threadTwo, NULL);
    printf("Value of shared resource is %d\n", resource);
  }
//Thread Function One
  void *threadFuncOne() {
    int threadId = 1;
    int fOne = resource;
    
    printf("Thread 1 reads the value of shared resource as %d\n", fOne);
    fOne++;              //Increment The Shared Resource
    printf("Local update by Thread 1: %d\n", fOne);
    
    //Sleep For One Second
    sleep(1);
    resource = fOne;     //Update The Shared Resource
    printf("Value of shared resource updated by Thread 1 is: %d\n", resource);
  }
//Thread Function Two
  void *threadFuncTwo() {
    int threadId = 2;
    int fTwo = resource;

    printf("Thread 2 reads resource as %d\n", fTwo);
    fTwo--;              //Decrement The Shared Resource
    printf("Local update by Thread 2: %d\n", fTwo);

    //Sleep For One Second
    sleep(1);
    resource = fTwo;     //Update The Shared Resource
    printf("Value of shared resource updated by Thread 2 is: %d\n", resource);
  }

//Banker's Algorithm Function
  void bankersAlgorithm(){
    //Allocation Array
    int allocation[PROCESS][RESOURCE] = {{0, 0, 2},    //Row 0
                                         {3, 0, 2},    //Row 1
                                         {0, 1, 0},    //Row 2
                                         {2, 1, 1},    //Row 3
                                         {2, 0, 0}};   //Row 4
    //Maxdemand Array
    int maxDemand[PROCESS][RESOURCE] = {{4, 3, 3},     //Row 0
                                        {9, 0, 2},     //Row 1
                                        {7, 5, 3},     //Row 2
                                        {2, 2, 2},     //Row 3
                                        {3, 2, 2}};    //Row 4
    //Avaliable Array
    int available[RESOURCE] = {2, 4, 6};               //Row 0
    //Need Array
    int need[PROCESS][RESOURCE];
    //Feasible Array
    int feasible[PROCESS];
    //Safe Allocation Sequence
    int safe[PROCESS];
    int safeIdx = 0;
    
    //Initializing Feasible Array
    for(int p = 0; p < PROCESS; p++){
      feasible[p] = FALSE;
    }
    
    //Calculate The Needed Resources
    for(int p = 0; p < PROCESS; p++){
      for(int r = 0; r < RESOURCE; r++){
        need[p][r] = maxDemand[p][r] - allocation[p][r];
      }
    }
    
    //Iterate Through The Number Of Processes To Determine Resource Allocation
    for(int i = 0; i < PROCESS; i++){
      for(int p = 0; p < PROCESS; p++){
        if(feasible[p] == FALSE){
          int isUnsafe = FALSE;
          for(int r = 0; r < RESOURCE; r++){
            if(need[p][r] > available[r]){
              isUnsafe = TRUE;
              break;
            }
          }
          //If It Is A Safe Condition, Update The Available Resources
          if(isUnsafe == FALSE){
            safe[safeIdx++] = p;
            for(int r = 0; r < RESOURCE; r++){
              available[r] = available[r] + allocation[p][r];
            }
            feasible[p] = TRUE;
        }
      }
    }
    }
    //Call Function systemState
    systemState(feasible, safe);
  }
    void systemState(int feasible[], int safe[]){
      int isSafe = TRUE;
      //Check For An Unsafe State
      for(int p = 0; p < PROCESS; p++){
        if(feasible[p] == FALSE){
          isSafe = FALSE;
          printf("The operating system is not safe\n");
          break;
        }
      }
    //If The Operating System Is In A Safe State, Display The Safe Sequence
    if (isSafe == TRUE){
      printf("\nSafe resource allocation to process sequence\n");
      for(int p = 0; p < PROCESS - 1; p++){
        printf(" P%d ->", safe[p]);
      }
      printf(" P%d", safe[PROCESS - 1]);
      printf("\n");
    }
 }

//Memory Management Function
void memoryManagement(){

  printf("\n  ********** Memory Management **********\n\n");
  
  for(int algorithm = FIRST; algorithm <= NEXT; algorithm++){
    int blockSize[] = {70, 20, 45, 65, 40, 80};
    int processSize[] = {15, 35, 25, 45, 60, 20};
    int blocks = sizeof(blockSize) / sizeof(blockSize[0]);
    int processes = sizeof(processSize) / sizeof(processSize[0]);

    //Switch Statement For blockSize, blocks, processSize, and processes
    switch(algorithm){
      case FIRST:
           firstFit(blockSize, blocks, processSize, processes);
           break;
      case BEST:
           bestFit(blockSize, blocks, processSize, processes);
           break;
      case WORST:
           worstFit(blockSize, blocks, processSize, processes);
           break;
      case NEXT:
           nextFit(blockSize, blocks, processSize, processes);
           break;
    }
  }
}

//Next Fit Function
void nextFit(int blockSize[], int blocks, int processSize[], int processes){

  printf("\n  ********** Next Fit **********\n\n");

  int allocation[processes];
  int id = 0;
  memset(allocation, INVALID, sizeof(allocation));
  
//Loop Through The Number Of Processes
  for(int i = 0; i < processes; i++){
    while(id < blocks){
      if(blockSize[id] >= processSize[i]){
        allocation[i] = id;                                           //Update The Allocation Array
        blockSize[id] = blockSize[id] - processSize[i];              //Update The Block Size
        break;
      }
      id = (id + 1) % blocks;                                        //Update The Id
    }
  }
  displayProcess(allocation, processes, processSize);
}

//First Fit Function
void firstFit(int blockSize[], int blocks, int processSize[], int processes){
  
  printf("\n  ********** First Fit **********\n\n");

  int allocation[processes];
  memset(allocation, INVALID, sizeof(allocation));
  
//Loop Through The Number Of Processes
  for(int i = 0; i < processes; i++){
    for(int j = 0; j < blocks; j++){                                //Loop Through The Number Of Blocks
      if(blockSize[j] >= processSize[i]){                           
        allocation[i] = j;                                          //Update The Allocation Array
        blockSize[j] = blockSize[j] - processSize[i];               //Update The Block Size
        break;
      }
    }
  }
  displayProcess(allocation, processes, processSize);
}

//Best Fit Function
void bestFit(int blockSize[], int blocks, int processSize[], int processes){
  
  printf("\n  ********** Best Fit **********\n\n");

  int allocation[processes];
  memset(allocation, INVALID, sizeof(allocation));

//Loop Through The Number Of Processes  
  for(int i = 0; i < processes; i++){
    int bestIdx = INVALID;
    for(int j = 0; j < blocks; j++){                                //Loop Through The Number Of Blocks
      if(blockSize[j] >= processSize[i]){
        if(bestIdx == INVALID){                                     //If The Best Index Is Invalid, Update The Best Index
          bestIdx = j;
        }
        else if(blockSize[bestIdx] >= blockSize[j]){                
          bestIdx = j;
          }
      }
    }
    if(bestIdx != INVALID){                                         //If The Best Index Is Not Invalid, Update The Allocation Array and Block Size
      allocation[i] = bestIdx;
      blockSize[bestIdx] = blockSize[bestIdx] - processSize[i];
    }
  }
  displayProcess(allocation, processes, processSize);
}

//Worst Fit Function
void worstFit(int blockSize[], int blocks, int processSize[], int processes){
  
  printf("\n  ********** Worst Fit **********\n\n");

  int allocation[processes];
  memset(allocation, INVALID, sizeof(allocation));
  
//Loop Through The Number Of Processes
  for(int i = 0; i < processes; i++){
    int wstIdx = INVALID;
    for(int j = 0; j < blocks; j++){                                //Loop Through The Number Of Blocks
      if(blockSize[j] >= processSize[i]){
        if(wstIdx == INVALID){                                      //If The Worst Index Is Invalid, Update The Worst Index
          wstIdx = j;
        }
        else if(blockSize[wstIdx] < blockSize[j]){                  
          wstIdx = j;
        }
      }
    }
    if(wstIdx != INVALID){                                          //If The Worst Index Is Not Invalid, Update The Allocation Array and Block Size
      allocation[i] = wstIdx;
      blockSize[wstIdx] = blockSize[wstIdx] - processSize[i];
    }
  }
  displayProcess(allocation, processes, processSize);
}

//Displaying The Process
void displayProcess(int allocation[], int processes, int processSize[]){
  
  printf("\nProcess No. \tProcess Size\tBlock No.\n");
  
//Loop Through The Number Of Processes
  for(int i = 0; i < processes; i++){
    printf("%d\t\t%d\t\t", i + 1, processSize[i]);
    if(allocation[i] == INVALID){                                   //If The Allocation Array Is Invalid, Display Not Allocated
      printf("Not Allocated\n");
    }
    else{                                                           //If The Allocation Array Is Not Invalid, Display The Cuurent Allocation
      printf("%d\t\n", allocation[i] + 1);
    }
  }
}
//Displaying the Page Replacement
void pageReplacement(){
  printf("\n  ********** Page Replacement **********\n\n");

  for(int algorithm = FIFO; algorithm <= LRU; algorithm++){
    switch(algorithm){
      case FIFO:
        fifo();
        break;
      case LRU:
        lru();
        break;
    }
  }
}
//First In First Out Function
void fifo(){
  printf("\n  ********** First In First Out **********\n\n");
  printf("\tPage   Frame 1   Frame 2   Frame 3   Frame 4\n\n");
  //Declare The Variables
  int pageRequests[] = {2, 3, 8, 4, 5, 6, 5, 7, 1, 8, 3, 1, 4, 2, 6};
  int pageFaults = 0;
  int allocation[FRAMES];
  int present;
  int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);
  //Initialize Allocation Array
  memset(allocation, INVALID, sizeof(allocation));

  //Iterate Through The Number Of Pages
  for(int i = 0; i < pages; i++){
    present = 0;
    //Iterate Through The Number Of Frames
    for(int j = 0; j < FRAMES; j++){
      if(pageRequests[i] == allocation[j]){
        present++;
        pageFaults--;
      }
    }
    pageFaults++;
      //Update The Allocation Array
      if(pageFaults <= FRAMES && present == 0){
        allocation[i] = pageRequests[i];
      }
      else if(present == 0){
        allocation[(pageFaults - 1) % FRAMES] = pageRequests[i];
      }
    displayPages(pageRequests[i], allocation);
  }
  printf("\nTotal Page Faults: %d\n", pageFaults);
}

//Least Recently Used Function
void lru(){
  printf("\n  ********** Least Recently Used **********\n\n");
  printf("\tPage   Frame 1   Frame 2   Frame 3   Frame 4\n\n");
  int pageRequests[] = {2, 3, 8, 4, 5, 6, 5, 7, 1, 8, 3, 1, 4, 2, 6};
  int pageFaults = 0;
  int allocation[FRAMES];
  int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);
  int counter = 0;
  int time[10];
  int flag1, flag2; 
  int position = 0;

  //Initialize Allocation Array
  memset(allocation, INVALID, sizeof(allocation));
  //Iterate Through The Number Of Pages
  for(int i = 0; i < pages; i++){
    flag1 = 0;
    flag2 = 0;
    //Iterate Through The Number Of Frames
    for(int j = 0; j < FRAMES; j++){
      if(pageRequests[i] == allocation[j]){
        counter++;
        time[j] = counter;
        flag1 = 1;
        flag2 = 1;
        break;
      }
    }
    if(flag1 == 0){
      //Iterate Through The Number Of Frames
      for(int j = 0; j < FRAMES; j++){
        if(allocation[j] == INVALID){
          counter++;
          pageFaults++;
          allocation[j] = pageRequests[i];
          time[j] = counter;
          flag2 = 1;
          break;
        }
      }
    }
    if(flag2 == 0){
     position = findLRU(time);
      counter++;
      pageFaults++;
      allocation[position] = pageRequests[i];
      time[position] = counter;
    }
    displayPages(pageRequests[i], allocation);
  }
  printf("\nTotal Page Faults: %d\n", pageFaults);
}
//Finding The Least Recently Used Function
int findLRU(int time[]){
  int position = 0;
  int minimum = time[0];
  //Iterate Through The Number Of Frames
  for(int i = 0; i < FRAMES; i++){
    if(time[i] < minimum){
      minimum = time[i];
      position = i;
    }
  }
  return position;
}
//Displaying The Pages
void displayPages(int page, int allocation[]){
  printf("\t%d\t", page);
  //Iterate Through The Number Of Frames
  for(int i = 0; i < FRAMES; i++){
    if(allocation[i] == INVALID){
      printf("\t   -\t");
    }
    else{
      printf("\t   %d\t", allocation[i]);
    }
  }
  printf("\n");
}

//Disk Scheduling Function
void diskScheduling(){
  printf("\n\n  ********** Disk Scheduling **********\n\n");

  //Declare A One-Dimensional Array To Store The Disk Requests
  int requests[] = {146, 89, 24, 70, 102, 13, 51, 134};
  int head = 50;

  for(int algorith = FCFS; algorith <= SSTF; algorith++){
    if(algorith == FCFS){
      diskFcfs(requests, head);
    }
    else if(algorith == SSTF){
      diskSstf(requests, head);
    }
  }
}

//Disk Scheduling First Come First Serve Function
void diskFcfs(int requests[], int head){
  printf("\n  ********** FCFS **********\n\n");
  int seek = 0;
  int track = 0;
  int distance = 0;
  int start = head;

  printf("Seek Sequence: %d",head);

  //Iterate Through The Number Of Requests
  for(int i = 0; i < REQUEST; i++){
    track = requests[i];
    distance = abs(head - track);
    seek += distance;
    head = track;
  }
  
  //Display The Total Seek Sequence
  for(int i = 0; i < REQUEST; i++){
    printf(" -> %d", requests[i]);
  }  
  printf("\nTotal seek operations: %d\n", seek);
}

//Disk Scheduling Shortest Seek Time First Function
void diskSstf(int requests[], int head){
  printf("\n  ********** SSTF **********\n\n");

  int sequence[REQUEST];
  int distance[REQUEST];
  int seek = 0;
  int start = head;
  int minVal = 0;
  int minValIdx = 0;
  int seqIdx = 0;
    
//Iterate Through The Number Of Requests
  for (int i = 0; i < REQUEST; i++){
    for(int j = 0; j < REQUEST; j++){
    distance[j] = abs(head - requests[j]);                          //Update The Distance Array
    }
    
  //Update The Minimum Value And The Minimum Value Index
  minVal = distance[0];
  minValIdx = 0;

  //Iterate Through The Number Of Requests To Find The Minimum Value
  for(int j = 0; j < REQUEST; j++){
    if(minVal > distance[j]){
      minVal = distance[j];
      minValIdx = j;
    }
  }

  //Update The Sequence Array 
  sequence[seqIdx++] = requests[minValIdx];
  head = requests[minValIdx];
  //Update The Requests Array
  requests[minValIdx] = 999;
  }
  
  printf("Seek Sequence: %d", start);
  seek += abs(start - sequence[0]);
  printf(" -> %d", sequence[0]);

  //Iterate Through The Number Of Requests
  for(int i = 1; i < REQUEST; i++){
    seek += abs(sequence[i] - sequence[i -1]);
    printf(" -> %d", sequence[i]);
  }
    printf("\nTotal seek operations: %d\n\n", seek);
}
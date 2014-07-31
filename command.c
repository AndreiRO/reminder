#include<time.h>
#include<stdio.h>
#include"list.h"
#include"reminder.h"
#include"command.h"


void alarm(struct error* err) {
    /*
 *  Searches for the tasks for today(that either begin or end)
 *
 * */
    time_t clock;
    struct tm* current_time;
    time(&clock);
    current_time = localtime(&clock);   

    // we are not interested in the task for the exact hh:mm:ss but for those on that day
    current_time->tm_sec = -1;
    current_time->tm_min = -1;
    current_time->tm_hour= -1;
    struct error e;

    list tasks_starting_today   = getByStartDate(*current_time, &e);
    if(e.error != NO_ERROR) {
        fprintf(stderr, "Error while retrieving tasks: %s\n", e.description);
        return ;
    }

    list tasks_ending_today     = getByEndDate(*current_time, &e);
    if(e.error != NO_ERROR) {
        fprintf(stderr, "Error while retrieving tasks: %s\n", e.description);
        return ;
    }
   
    iterator i = l_iterator(tasks_ending_today);
    printf("\n");
    
    

 

}

void loop(void* (*callback)(void* , struct error*), void(*err_callback(struct error*))) {

}





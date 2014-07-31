#include<time.h>
#include<malloc.h>
#include<stdio.h>
#include"list.h"
#include"reminder.h"
#include"command.h"

static void task_destructor(void* t) {
    Task task = (Task)t;
    free(task->title);
    free(task->description);
    free(task->repeatPattern);
    free(task);
    task = NULL;
    t = NULL;
}

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
    printf("Tasks ending today: %d\n", l_size(tasks_ending_today));

    while(!l_iterator_at_end(i)) {
        Task t = (Task)l_value(i);
        printTask(t);
    }

    i = l_iterator(tasks_starting_today);
    printf("\n\nTasks starting today: %d\n", l_size(tasks_starting_today));
    while(!l_iterator_at_end(i)) {
        Task t = (Task)l_value(i);
        printTask(t);
    }
    
    l_delete(tasks_starting_today, task_destructor);
    l_delete(tasks_ending_today, task_destructor);
}

void loop(void* (*callback)(void* , struct error*), void(*err_callback(struct error*))) {

}

void initializeCommands() {
     
    commands[0] = (struct command) {.title="CREATE_TASK", .shortOption='c', .longOption="create"};
    commands[1] = (struct command) {.title="EDIT_TASK", .shortOption='e', .longOption="edit"};
    commands[2] = (struct command) {.title="DELETE_TASK", .shortOption='d', .longOption="delete"};
    commands[3] = (struct command) {.title="LIST_ALL_TASKS", .shortOption='l', .longOption="list"};
    commands[4] = (struct command) {.title="LIST_ALL_TASKS_BY_DATE", .shortOption='L', .longOption="listd"};
    commands[5] = (struct command) {.title="CHECK_TASKS_FOR_TODAY", .shortOption='t', .longOption="today"};

}

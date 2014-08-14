#include<time.h>
#include<malloc.h>
#include<string.h>
#include<stdio.h>
#include<math.h>
#include"list.h"
#include"reminder.h"
#include"command.h"

struct command commands[6] = {
    {.title="CREATE_TASK", .shortOption='c', .longOption="create"},
    {.title="EDIT_TASK", .shortOption='e', .longOption="edit"},
    {.title="DELETE_TASK", .shortOption='d', .longOption="delete"},
    {.title="LIST_ALL_TASKS", .shortOption='l', .longOption="list"},
    {.title="LIST_ALL_TASKS_BY_DATE", .shortOption='L', .longOption="listd"},
    {.title="CHECK_TASKS_FOR_TODAY", .shortOption='t', .longOption="today"},
};


static void task_destructor(void* t) {
    if(!t) return;
    Task task = (Task)t;
    free(task->title);
    free(task->description);
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


    list tasks_starting_today   = getByStartDate(*current_time, err);
    if(err->error != NO_ERROR) {
        fprintf(stderr, "Error while retrieving tasks: %s\n", err->description);
        return ;
    }

    list tasks_ending_today     = getByEndDate(*current_time, err);
    if(err->error != NO_ERROR) {
        fprintf(stderr, "Error while retrieving tasks: %s\n", err->description);
        return ;
    }
   
    iterator i = l_iterator(tasks_ending_today);
    printf("\n");
    printf("Tasks ending today: %ld\n", l_size(tasks_ending_today));

    while(!l_iterator_at_end(i)) {
        Task t = (Task)l_value(i);
        printTask(t);
        l_next(&i);
    }

    i = l_iterator(tasks_starting_today);
    printf("\n\nTasks starting today: %ld\n", l_size(tasks_starting_today));

    while(!l_iterator_at_end(i)) {
        Task t = (Task)l_value(i);
        printTask(t);
        l_next(&i);
    }
  
    l_delete(tasks_starting_today, task_destructor);
    l_delete(tasks_ending_today, task_destructor);
}

void loop(void* (*callback)(void* , struct error*), void(*err_callback(struct error*))) {

}

size_t get_buffer_size(FILE* f) {
    if(!f) {
        return 0;
    }

    char buffer[16];
    size_t sz = 0;
    
    while(fgets(buffer, 16, f)) {
        sz += strlen(buffer);
    }
    ++ sz;

    return sz;
}

bool starts_with(char* source, char* pattern) {
    int i = 0;

    if(strlen(source) < strlen(pattern)) {
        return false;
    }

    for( ; i < strlen(pattern); ++ i) {
        if(source[i] != pattern[i]) {
            return false;
        }
    }

    return true;
}

void parse_command(char** argv, int argc, struct command* commands, struct error* err) {
    err->error = NO_ERROR;

    if(argc <2) {
        err->error          = ARGUEMENTS_ERROR;
        err->description    = "Not enough arguements";
    }

    bool flag = false;
    if(starts_with(argv[1], "-c") || starts_with(argv[1], "-create")) {
        printf("\nCreating task:\n");
        printf("Task title: ");

        char* title = (char*)malloc(sizeof(char) * get_buffer_size(stdin));
        if(!fgets(title, get_buffer_size(stdin), stdin)) {
            free(title);
            err->error       = PARAMETER_ERROR;
            err->description = "Problem getting title for new task";
            return ;
        }

        char* decsription = (char*)malloc(sizeof(char) * get_buffer_size(stdin));

        printf("\nEnter start date('hh:mm:ss dd:mm:yyyy', blank for current time) :");
        char start_date[20];

        if(!fgets(start_date, 20, stdin)) {
            free(title);
            err->error       = PARAMETER_ERROR;
            err->description = "Problem getting start date for new task";
            return ;
        }
     
        printf("\nEnter due date('hh:mm:ss dd:mm:yyyy', blank for current time) :");
        char due_date[20];

        if(!fgets(due_date, 20, stdin)) {
            free(title);
            err->error       = PARAMETER_ERROR;
            err->description = "Problem getting due date for new task";
            return ;
        }


    } else if(starts_with(argv[1], "-e") || starts_with(argv[1], "-edit")){

    } else if(starts_with(argv[1], "-d") || starts_with(argv[1], "-delete")){

    } else if(starts_with(argv[1], "-l") || starts_with(argv[1], "-list")){

    } else if(starts_with(argv[1], "-L") || starts_with(argv[1], "-listd")){

    } else if(starts_with(argv[1], "-t") || starts_with(argv[1], "-today")){

    } 

}

/*
 *    {.title="CREATE_TASK", .shortOption='c', .longOption="create"},
    {.title="EDIT_TASK", .shortOption='e', .longOption="edit"},
    {.title="DELETE_TASK", .shortOption='d', .longOption="delete"},
    {.title="LIST_ALL_TASKS", .shortOption='l', .longOption="list"},
    {.title="LIST_ALL_TASKS_BY_DATE", .shortOption='L', .longOption="listd"},
    {.title="CHECK_TASKS_FOR_TODAY", .shortOption='t', .longOption="today"},
}
 * */



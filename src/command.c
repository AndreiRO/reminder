#define _XOPEN_SOURCE
#include<time.h>
#include<malloc.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"str.h"
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

static char* get_string(FILE* f) {
    if(!f) {
        return NULL;
    }

    struct string s = string_new();
    char buffer[1024];
    
    while(fgets(buffer, 1024, f)) {
        string_append(&s, buffer);    
    }

    return string_char(s);
    
}

static struct tm get_date(FILE* in) {
    struct tm time_;
    if(!in) {
        return time_;
    }

    printf("\nEnter date(HH:MM:SS DD:MM:YYYY) or enter for current date: ");
    struct string s = string_new();
    char buffer[512];

    while(fgets(buffer, 512, in)) {
        string_append(&s, buffer);
    }

    if(strcmp(string_char(s), "\n") == 0) {
        time_t t;
        time(&t);
        time_ = *(struct tm*)localtime(&t);
    } else {
        strptime(string_char(s), "%H:%M:%S %d:%m:%Y",&time_);
    }

    return time_;
}

static void handle_error(struct error* err) {
    if(err->error != NO_ERROR) {
        fprintf(stderr, "\nError:%d : %s\n", err->error, err->description);
        exit(err->error);
    }
}

void parse_command(char** argv, int argc, struct error* err) {
    err->error = NO_ERROR;

    if(argc <2) {
        err->error          = ARGUEMENTS_ERROR;
        err->description    = "Not enough arguements";
        handle_error(err);
    }

    bool flag = false;
    if(starts_with(argv[1], "-c") || starts_with(argv[1], "-create")) {
        flag = true;
        printf("\nCreating task:\n");
        printf("Task title: ");

        char* title = get_string(stdin);
        if(!title) {
            free(title);
            err->error       = PARAMETER_ERROR;
            err->description = "Problem getting title for new task";
            handle_error(err);
        }

        char* description = get_string(stdin);
        if(!description) {
            free(title);
            free(description);
            err->error       = PARAMETER_ERROR;
            err->description = "Problem getting description for task";
            handle_error(err);
        }

        struct tm start_date, end_date;
        start_date  = get_date(stdin);
        end_date    = get_date(stdin);

        handle_error(err);
        createTask(title, description, start_date, end_date, err);

    } else if(starts_with(argv[1], "-e") || starts_with(argv[1], "-edit")){
        flag = true;
        char* title = get_string(stdin);
        struct error e;

        printf("\nEnter new title(blank for unchanged):");
        char* new_title       = get_string(stdin);
        printf("\nEnter new description(blank for unchanged):");
        char* new_description = get_string(stdin);
        printf("\nEnter new start date(blank for unchanged):");
        struct tm start = get_date(stdin);
        printf("\nEnter new due date(blank for unchanged):"); 
        struct tm end   = get_date(stdin);
        Task old_task = getTask(title, &e);
        if(e.error != NO_ERROR) {
            fprintf(stderr, "\nError: %d:%s\n", e.error, e.description);
            free(title);
            free(new_title);
            free(new_description);
            handle_error(err);
        }

        Task t = (Task)malloc(sizeof(struct task));
        if(!t) {
            free(title);
            free(new_title);
            free(new_description);
            handle_error(err);
        }        

        if(strcmp(new_title, "\n") == 0) {
            t->title = title;
        }
        if(strcmp(new_description, "\n") == 0) {
            t->description = old_task->description;
        }
        t->startDay = start;
        t->dueDay    = end;
        
        editTask(
            /* old title */ title,
            /* new task object */t,
            /* error object */&e 
        ); 


        free(title);
        free(new_title);
        free(new_description);
        free(t);
        free(old_task);
    } else if(starts_with(argv[1], "-d") || starts_with(argv[1], "-delete")){
        flag = true;
        char* title = get_string(stdin);
        struct error e;
        deleteTask(title, &e);
        free(title);
        handle_error(err);
        
    } else if(starts_with(argv[1], "-l") || starts_with(argv[1], "-list")){
        flag = true;
        struct error e;
        list l = getTasks(&e); 

       handle_error(err); 

        iterator i = l_iterator(l);

        while(!l_iterator_at_end(i)) {
            Task t = l_value(i);
            printTask(t);
            l_next(&i);
        }
        
        l_delete(l, task_destructor);

    } else if(starts_with(argv[1], "-L") || starts_with(argv[1], "-listd")){
        flag = true;
        struct error e;
        struct tm start = get_date(stdin);
        struct tm end   = get_date(stdin);
        list l          = getBetweenDate(start, end, &e);
        handle_error(err);
        
        iterator i = l_iterator(l);
        while(!l_iterator_at_end(i)) {
            Task t = l_value(i);
            printTask(t);
            l_next(&i);
        }

        l_delete(l, task_destructor);
    } else if(starts_with(argv[1], "-t") || starts_with(argv[1], "-today")){
        flag = true;
        struct error e;
        alarm(&e);
        handle_error(err); 

    }

    if(!flag) {
        fprintf(stdout, "\nUsage:\n %s -c or -create for creating\n\
                                        \t\t\t-e or -edit to edit a task\n\
                                        \t\t\t-d or -delete to delete a task\n\
                                        \t\t\t-l or -list to list all tasks\n\
                                        \t\t\t-L or -listd to list by date\n\
                                        \t\t\t-t or -today to list all tasks that start/end today", argv[0]);
    } 

}


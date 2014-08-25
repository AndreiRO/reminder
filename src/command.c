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

    if(strlen(source) != strlen(pattern)) {
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
    char buffer[256];
    
    while(fgets(buffer, 256, f)) {
        int i = 0;
        while(i < strlen(buffer)) { 
            if(buffer[i] != '\n') {
                string_append3(&s, buffer[i]);
                ++ i;   
            } else {
                return string_char(s);
            }
        }
    }

    return string_char(s);
    
}

static int get_date(FILE* in, struct tm* t) {
  
    if(!in) {
        return -1;
    }

    fprintf(stdout, "\nEnter date(HH:MM:SS DD:MM:YYYY or enter for current date): ");
    
    char buffer[21];
    fgets(buffer, 21, in);

    if(buffer[0] == '\n') {
        time_t t2;
        time(&t2);
        *t = *((struct tm*)localtime(&t2));
        return 1;
    } else {
        char* s = strptime(buffer, "%H:%M:%S %d:%m:%Y", t);
        if(!s) {
            fprintf(stdout, "\nWrong date. Please try again(HH:MM:SS DD:MM:YY): ");
            get_date(in, t);
        }

        return 2;
    }

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
        fprintf(stdout, "\nUsage:\n %s -c or -create for creating\n\
                                        -e or -edit to edit a task\n\
                                        -d or -delete to delete a task\n\
                                        -l or -list to list all tasks\n\
                                        -L or -listd to list by date\n\
                                        -t or -today to list all tasks that start/end today\n", argv[0]); 
        fflush(stdout);
        handle_error(err);
    }

    bool flag = false;
    if(starts_with(argv[1], "-create") || starts_with(argv[1], "-c")) {
        flag = true;
        fprintf(stdout, "\nCreating task:\n");
        fprintf(stdout, "Task title: ");

        char* title = get_string(stdin);
        if(!title) {
            free(title);
            err->error       = PARAMETER_ERROR;
            err->description = "Problem getting title for new task";
            handle_error(err);
        }

        fprintf(stdout, "Description: ");
        char* description = get_string(stdin);
        if(!description) {
            free(title);
            free(description);
            err->error       = PARAMETER_ERROR;
            err->description = "Problem getting description for task";
            handle_error(err);
        }

        struct tm start_date, end_date;
       
        fprintf(stdout, "Start date: ");
        get_date(stdin, &start_date);
        fprintf(stdout, "End date: ");
        get_date(stdin, &end_date);

        handle_error(err);
        createTask(title, description, start_date, end_date, err);
    } else if(starts_with(argv[1], "-edit") || starts_with(argv[1], "-e") ){
        flag = true;
        fprintf(stdout, "\nEnter task title: ");
        char* title = get_string(stdin);
        struct error e;
        struct tm start, end;

        fprintf(stdout, "\nEnter new title(blank for unchanged):");
        char* new_title       = get_string(stdin);
        fprintf(stdout, "\nEnter new description(blank for unchanged):");
        char* new_description = get_string(stdin);
        fprintf(stdout, "\nEnter new start date(blank for unchanged):");
        int s1 = get_date(stdin, &start);
        fprintf(stdout, "\nEnter new due date(blank for unchanged):"); 
        int s2 = get_date(stdin, &end);

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
            free(old_task->title);
            free(old_task->description);
            handle_error(err); 
        }        

        if(strcmp(new_title, "") == 0) {
            t->title = (char*)malloc(sizeof(char) * (strlen(title)+1));
            strcpy(t->title, title);
        } else {
            t->title = (char*)malloc(sizeof(char) * (strlen(new_title)+1));
            strcpy(t->title, new_title);
        }
        
        if(strcmp(new_description, "") == 0) {
            t->description = (char*)malloc(sizeof(char) * (strlen(old_task->description) + 1));
            strcpy(t->description, old_task->description);
        } else {
            t->description = (char*)malloc(sizeof(char) * (strlen(new_description) + 1));
            strcpy(t->description, new_description);
        }
       
        if(s1 != 1) {
            t->startDay = start;
        } else {
            t->startDay = old_task->startDay;
        }

        if(s2 != 1) {
            t->dueDay = end;
        } else {
            t->dueDay   = old_task->dueDay;
        }
        
        editTask(
              /* old title */ title,
              /* new task object */t,
              /* error object */&e 
        ); 

        free(title);
        free(new_title);
        free(new_description);
        free(t->title);
        free(t->description);
        free(t);
        free(old_task->title);
        free(old_task->description);
        free(old_task);
    } else if(starts_with(argv[1], "-delete") || starts_with(argv[1], "-d")){
        flag = true;

        fprintf(stdout, "Enter task title: ");
        char* title = get_string(stdin);
        struct error e;
        deleteTask(title, &e);
        free(title);
        handle_error(err);
        
    } else if(starts_with(argv[1], "-list") || starts_with(argv[1], "-l")){
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

    } else if(starts_with(argv[1], "-listd") || starts_with(argv[1], "-L")){
        flag = true;
        struct error e;
        struct tm start;
        fprintf(stdout, "\nEnter start date: ");
        get_date(stdin, &start);
        struct tm end;
        fprintf(stdout, "\nEnter end date: ");
        get_date(stdin, &end);
        list l          = getBetweenDate(start, end, &e);
        handle_error(err);
        
        iterator i = l_iterator(l);
        while(!l_iterator_at_end(i)) {
            Task t = l_value(i);
            printTask(t);
            l_next(&i);
        }

        l_delete(l, task_destructor);
    } else if(starts_with(argv[1], "-today") || starts_with(argv[1], "-t")){
        flag = true;
        struct error e;
        alarm(&e);
        handle_error(err); 
    }

    if(!flag) {
        fprintf(stdout, "\nUsage:\n %s -c or -create for creating\n\
                                        -e or -edit to edit a task\n\
                                        -d or -delete to delete a task\n\
                                        -l or -list to list all tasks\n\
                                        -L or -listd to list by date\n\
                                        -t or -today to list all tasks that start/end today\n", argv[0]);
    } 

}


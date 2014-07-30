#include<stdio.h>
#include<malloc.h>
#include"reminder.h"

int main(int argc, char** argv) {
    time_t now;
    struct tm *t;
    time(&now);
    enum repeat_pattern pa[] = {MONDAY};
    
    t = localtime(&now);
    struct error err;
    Task task = (Task)malloc(sizeof(struct task));
    task->title = "NewTitle";
    task->description = "Fuck";
    task->startDay = *t;
    task->dueDay = *t;
    task->repeatPattern = pa;

    editTask("a",task, &err);

    close_db();
    if(err.error != NO_ERROR) {
        printf("Error! %s ", err.description);
    }  
    free(task);
    printf("Done.");

    return 0;
}

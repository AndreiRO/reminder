#include<stdio.h>
#include<time.h>
#include<malloc.h>
#include<stdlib.h>
#include"reminder.h"

int main(int argc, char** argv) {
    Task t = (Task)malloc(sizeof(struct task));
    struct tm* _t;
    time_t _time;
    
    time(&_time);
    _t = localtime(&_time);

    t->title = "salut";
    t->description = "cmf?";
    t->startDay = *_t;
    t->dueDay = *_t;
    enum repeat_pattern pat[] = {MONDAY};
    t->repeatPattern = pat;    

    struct error e;
    FILE* p = fopen("out.out", "w");
    serialize(t, p, &e);
    if(e.error != NO_ERROR) {
        fprintf(stderr, "Fuck...\n");
    }
    fclose(p);
    p = fopen("out.out", "r");
    Task b = deserialize(p,&e);
    printTask(b); 

    fclose(p);
    free(t);
    return 0;
}

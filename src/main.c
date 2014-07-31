#include<time.h>
#include<stdio.h>
#include<malloc.h>
#include"reminder.h"
#include"command.h"

int main(int argc, char** argv) {
    initializeCommands();
    time_t clock;
    struct tm* t;

    time(&clock);
    t = localtime(&clock);
    struct error e;
    enum repeat_pattern p[] = {MONDAY};


    createTask("Un task", "Ceva pe acolo", *t, *t, p, &e); 
    createTask("Un alt task", "Altceva pe acolo", *t, *t, p, &e); 
      
    alarm(&e);
 
    return 0;
}

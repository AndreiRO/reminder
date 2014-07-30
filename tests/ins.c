#include<stdio.h>
#include"reminder.h"

int main(int argc, char** argv) {
    time_t now;
    struct tm *t;
    time(&now);
    printf("fff");
    t = localtime(&now);
    struct error err;
    enum repeat_pattern p[] = {MONDAY};

    printf("Cr");
    createTask("titlu", "descr", *t, *t, p, &err);  
    close_db();
    if(err.error != NO_ERROR) {
        printf("Error! %s ", err.description);
    }  
    printf("Done.");

  
    return 0;
}

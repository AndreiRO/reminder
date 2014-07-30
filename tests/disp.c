#include<stdio.h>
#include<malloc.h>
#include"reminder.h"

int main(int argc, char** argv) {
    struct error err;
    Task t = getTask("NewTitle", &err);
    if(err.error != NO_ERROR) {
        fprintf(stderr, "Error: %s", err.description);
    }
    printTask(t);
    destructTask(t);

    printf("Done.");

    return 0;
}

#include<time.h>
#include<stdio.h>
#include<malloc.h>
#include"reminder.h"
#include"command.h"

int main(int argc, char** argv) {
    struct error err;
    parse_command(argv, argc, &err);
 
    return 0;
}

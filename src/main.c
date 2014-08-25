#include<time.h>
#include<string.h>
#include<stdio.h>
#include<malloc.h>
#include"reminder.h"
#include"command.h"

int main(int argc, char** argv) {
    struct error err;
    char** m_argv = NULL;
    if(argc == 2) {
        m_argv = (char**)malloc(3*sizeof(char*));
        m_argv[0] = (char*)malloc(sizeof(char) * (strlen(argv[0]) + 1));
        m_argv[1] = (char*)malloc(sizeof(char) * (strlen("tasks.db") + 1));
        m_argv[2] = (char*)malloc(sizeof(char) * (strlen(argv[1]) + 1));

        strcpy(m_argv[0], argv[0]);
        strcpy(m_argv[1], "tasks.db");
        strcpy(m_argv[2], argv[1]);

        ++ argc;
    }
    else {
        initializeConnection(argv[1]);
    } 
 
    parse_command(m_argv == NULL ? argv : m_argv, argc, &err);
 
    if(m_argv) {
        int i = 0;
        for(; i < 3; ++ i) {
            free(m_argv[i]);
        }
        free(m_argv);
    }

    return 0;
}

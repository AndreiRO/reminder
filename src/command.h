#ifndef _COMMAND_H_
#define _COMMAND_H_

/**
 * POSSIBLE COMMANDS
 *
 *   CREATE_TASK,
 *   EDIT_TASK,
 *   DELETE_TASK,
 *   LIST_ALL_TASKS,
 *   LIST_TASKS_BY_DATE,
 *   CHECK_TASKS_FOR_TODAY
 *   
 *
 * */
struct command {
    char title[25];
    char shortOption;
    char longOption[10];  
};

struct command commands[6];

void initializeCommands();
void alarm(struct error* err);
void loop(void* (*callback)(void* , struct error*), void(*err_callback(struct error*)));



#endif //_COMMAND_H_ defined

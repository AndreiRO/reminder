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

extern struct command commands[6];

void alarm(struct error* err);
void loop(void* (*callback)(void* , struct error*), void(*err_callback(struct error*)));
void clear_old_tasks(struct error* err, sqlite3* db);
void parse_command(char** argv, int argc, struct command* commands ,struct error* err);



#endif //_COMMAND_H_ defined

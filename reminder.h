#ifndef _REMINDER_H
#define _REMINDER_H

#include<stdbool.h>
#include<sqlite3.h>
#include<time.h>
#include"list.h"

enum repeat_pattern {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
    DAILY,
    WEEKLY,
    MONTHYL,
    YEARLY,
    DATE
};

enum error_code {
    NO_ERROR,
    DATABASE_CONNECTION,
    DATABASE_INSERT,
    DATABASE_UPDATE,
    DATABASE_DELETE,
    DATABASE_SELECT,
    DATABASE_OTHER,
    FILE_ERROR,
    PARAMETER_ERROR
};

struct error {
    const char* description;
    enum error_code error;
};

struct task {
    long long id;
    char* title;
    char* description;
    struct tm startDay;
    struct tm dueDay;
    enum repeat_pattern* repeatPattern;
};

typedef struct task* Task;

void close_db();
/* constructor destructor */
void createTask(char* title, char* description, struct tm, struct tm,
                    enum repeat_pattern* pattern, struct error* err);
void deleteTask(char* name, struct error* err);
void editTask(char* title, Task newValue, struct error* err);
Task getTask(char* title, struct error* err);

void serialize(Task task, FILE* f, struct error* err);
Task deserialize(FILE* f, struct error* err);


void printTask(Task t);

void destructTask(Task t);

list getByStartDate(struct tm t, struct error* err);
list getByEndDate(struct tm t, struct error* err);

#endif // _REMINDER_H is defined


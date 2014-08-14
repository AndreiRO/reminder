/* @author AndreiRo
 *
 * */
#define _XOPEN_SOURCE
#include<time.h>
#include<stdio.h>
#include<string.h>

#include<stdlib.h>
#include"reminder.h"
#include"list.h"


static sqlite3* db = NULL;

bool initializeConnection() {
    int rc;
    
    rc = sqlite3_open("tasks.db", &db);

    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }

    return true;
}

void close_db() {
    if(db) {
        sqlite3_close(db);
    }
}

    
void createTask(char* title, char* description, struct tm startDate, struct tm endDate,
                    struct error* err) {
    
    err->error = NO_ERROR;    
    /*
    *   1. Establish connection with db
    *   2. Execute insert statement
    *   Always check for errors
    **/
    if(!db) {
        if(!initializeConnection()) {
            exit(1);
        }
    }
    if(!title || !description) {
        err->error = DATABASE_INSERT;
        err->description= sqlite3_errmsg(db);
        return ;
    }

    const char* unused;
    sqlite3_stmt* statement;
    char* sql = "INSERT INTO TASKS(TITLE, DESCRIPTION, START_DATE, END_DATE) VALUES(?,?,?,?)";

    int rc = sqlite3_prepare(db, sql, -1, &statement, &unused);

    if(!rc == SQLITE_OK) {
        err->error = DATABASE_INSERT;
        err->description= sqlite3_errmsg(db);
        return ;
    }

    sqlite3_bind_text(statement, 1, title, strlen(title), SQLITE_STATIC); 
    sqlite3_bind_text(statement, 2, description, strlen(description), SQLITE_STATIC);
    char* st_date_str = asctime(&startDate);
    sqlite3_bind_text(statement, 3, st_date_str, strlen(st_date_str)-1, SQLITE_STATIC);
    char* en_date_str = asctime(&endDate);
    sqlite3_bind_text(statement, 4, en_date_str, strlen(en_date_str)-1, SQLITE_STATIC);
    
    sqlite3_step(statement);
    rc = sqlite3_finalize(statement);

    if(rc != SQLITE_OK) {
        err->error = DATABASE_INSERT;
        err->description = sqlite3_errmsg(db); 
    }
}

void deleteTask(char* name, struct error* err) {
    err->error = NO_ERROR;
    
    if(!db) {
        if(!initializeConnection()) {
            exit(1);
        }
    }

    const char* unused;
    sqlite3_stmt* statement;
    char* sql   = "DELETE FROM TASKS WHERE TITLE = ?";
    int rc      = sqlite3_prepare(db, sql, -1, &statement, &unused);

    if(rc != SQLITE_OK) {
        err->error = DATABASE_DELETE;
        err->description = sqlite3_errmsg(db);
        return ;
    }

    sqlite3_bind_text(statement, 1, name, strlen(name), SQLITE_STATIC);
    sqlite3_step(statement);
    rc = sqlite3_finalize(statement);

    if(rc != SQLITE_OK) {
        err->error = DATABASE_DELETE;
        err->description = sqlite3_errmsg(db);
    }
}

void editTask(char* title, Task newValue, struct error* err) {
    err->error = NO_ERROR;
    
    if(!db) {
        if(!initializeConnection()) {
            exit(1);
        }
    }

    sqlite3_stmt* statement;
    const char* unused;
    char* sql = "update tasks set title = ?, description = ?, start_date = ?, end_date = ?,\
                where title = ? ";
    int rc = sqlite3_prepare(db, sql, -1, &statement, &unused);
    if(rc != SQLITE_OK) {
        err->error          = DATABASE_UPDATE;
        err->description    = sqlite3_errmsg(db);
        return ;
    }

    sqlite3_bind_text(statement, 1, newValue->title, strlen(newValue->title), SQLITE_STATIC); 
    sqlite3_bind_text(statement, 2, newValue->description, strlen(newValue->description), SQLITE_STATIC);
    char* startDay_str  = asctime(&newValue->startDay);
    sqlite3_bind_text(statement, 3, startDay_str, strlen(startDay_str)-1, SQLITE_STATIC);
    char* dueDay_str    = asctime(&newValue->dueDay);
    sqlite3_bind_text(statement, 4, dueDay_str, strlen(dueDay_str)-1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 6, title, strlen(title), SQLITE_STATIC);

    sqlite3_step(statement);
    rc = sqlite3_finalize(statement);

    if(rc != SQLITE_OK) {
        err->error      = DATABASE_UPDATE;
        err->description= sqlite3_errmsg(db);
    }
}


Task getTask(char* title, struct error* err) {
    err->error = NO_ERROR;
    sqlite3_stmt* statement;

    if(!db) {
        if(!initializeConnection()) {
            exit(1);
        }
    }

    char* sql = "select * from tasks where title = ?";
    const char* unused;
    int rc = sqlite3_prepare(db, sql, -1, &statement, &unused);

    if(rc != SQLITE_OK) {
        err->error = DATABASE_SELECT;
        err->description = sqlite3_errmsg(db);
        return NULL;
    }

    sqlite3_bind_text(statement, 1, title, strlen(title), SQLITE_STATIC);
     
    if(sqlite3_step(statement) != SQLITE_ROW) {
        err->error = DATABASE_SELECT;
        err->description = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        return NULL; 
    }
    
    char* task_title = (char*)malloc(sizeof(char) * (strlen((char*)sqlite3_column_text(statement, 0)) +1));
    strcpy(task_title, (char*)sqlite3_column_text(statement, 0));    

    char* task_description = (char*)malloc(sizeof(char) * (strlen((char*)sqlite3_column_text(statement, 1))+1));
    strcpy(task_description, (char*)sqlite3_column_text(statement, 1));    

    char* task_start = (char*)malloc(sizeof(char) * (strlen((char*)sqlite3_column_text(statement, 2))+1));
    strcpy(task_start, (char*)sqlite3_column_text(statement, 2)); 

    char* task_end = (char*)malloc(sizeof(char) * (strlen((char*)sqlite3_column_text(statement, 3))+1));
    strcpy(task_end, (char*)sqlite3_column_text(statement, 3));    

     
    Task t = (Task)malloc(sizeof(struct task));
    t->title        = task_title;
    t->description  = task_description;

    strptime(task_start, "%a %b %d %H:%M:%S %Y%n", &(t->startDay));
    strptime(task_end, "%a %b %d %H:%M:%S %Y%n", &(t->dueDay));
   
    free(task_start);
    free(task_end);
 
    sqlite3_finalize(statement);

    return t;
}


void destructTask(Task t) {
    free(t->title);
    free(t->description);
    free(t);
}

void printTask(Task t) {
    if(!t) return;
    printf("\n>---------------------TASK-------------------------------\n");
    printf("Title: %s\n", t->title);
    printf("Description: %s\n", t->description);
    printf("Start date: %s", asctime(&(t->startDay)));
    printf("End date: %s", asctime(&(t->dueDay)));
    printf("---------------------------------------------------------\n\n");
}

static void populateList(list l, sqlite3_stmt* statement) {
    char* title;
    char* description;
    struct tm start;
    struct tm end;

    int title_length = strlen((char*)sqlite3_column_text(statement, 0)) + 1;
    int descr_length = strlen((char*)sqlite3_column_text(statement, 1)) + 1;

    title       = (char*)malloc(title_length*sizeof(char));
    description = (char*)malloc(descr_length*sizeof(char));

    strptime((char*)sqlite3_column_text(statement, 2), "%a %b %d %H:%M:%S %Y%n", &start);
    strptime((char*)sqlite3_column_text(statement, 3), "%a %b %d %H:%M:%S %Y%n", &end);

    strcpy(title,(char*)sqlite3_column_text(statement, 0));
    strcpy(description, (char*)sqlite3_column_text(statement, 1));

    Task t          = (Task)malloc(sizeof(struct task));
    t->title        = title;
    t->description  = description;
    t->startDay     = start;
    t->dueDay       = end;

    l_push(l, t);

}

list getByDate(struct tm t, struct error* err, const char* sql) {
    err->error = NO_ERROR;

    int seconds = t.tm_sec;
    int minutes = t.tm_min;
    int day     = t.tm_wday;
    int hours   = t.tm_hour;
    int month   = t.tm_mon;
    int year    = t.tm_year;
    year        += 1900;
    
    char* month_str;
    char* day_str;

    switch(month) {
        case 0:
            month_str = "Jan";
            break;
        case 1:
            month_str = "Feb";
            break;
        case 2:
            month_str = "Mar";
            break;
        case 3:
            month_str = "Apr";
            break;
        case 4:
            month_str = "May";
           break;
        case 5:
            month_str = "Jun";
            break;
        case 6:
            month_str = "Jul";
            break;
        case 7:
            month_str = "Aug";
            break;
        case 8:
            month_str = "Sep";
            break;
        case 9:
            month_str = "Oct";
            break;
        case 10:
            month_str = "Nov";
            break;
        case 11:
            month_str = "Dec";
            break;
        default:
            month_str = "%";
            break; 
    }
    
    switch(day) {
        case 0:
            day_str = "Sun";
            break;
        case 1:
            day_str = "Mon";
            break;
        case 2:
            day_str = "Tue";
            break;
        case 3:
            day_str = "Wed";
            break;
        case 4:
            day_str = "Thu";
            break;
        case 5:
            day_str = "Fri";
            break;
        case 6:
            day_str = "Sat";
            break;
        default:
            day_str = "%";
            break;
    }
    
    char seconds_str[3];
    char minutes_str[3];
    char hours_str[3];   
    char year_str[5];
    
    sqlite3_stmt* statement;
    const char* unused;
    
    if(seconds < 0 || seconds > 60) {
        strcpy(seconds_str,"%");
    } else {
        sprintf(seconds_str, "%d", seconds);
    }
    
    if(minutes < 0 || minutes > 59) {
        strcpy(minutes_str, "%");
    } else {
        sprintf(minutes_str, "%d", minutes);
    }

    if(hours < 0 || hours > 23) {
        strcpy(hours_str, "%");    
    } else {
        sprintf(hours_str, "%d", hours);
    }
   
    if(year < 1900) {
        strcpy(year_str, "%");
    } else {
        sprintf(year_str, "%d", year);
    }

    int rc = sqlite3_prepare(db, sql, -1, &statement, &unused);
    if(rc != SQLITE_OK) {
        err->error = DATABASE_SELECT;
        err->description = sqlite3_errmsg(db);
        return NULL;
    }

    int total_length = strlen(day_str) + 1 /*space*/ + 
        strlen(month_str)+ 1 /*space*/ + 8 /*hh:mm:ss*/+ 1/*space*/ + 4 /*year*/
        + 1 /*\0*/;
    char* value = (char*)malloc(total_length * sizeof(char));

    strcpy(value, day_str);
    strcat(value, " ");
    strcat(value, month_str);
    strcat(value, " ");
    strcat(value, hours_str);
    strcat(value, ":");
    strcat(value, minutes_str);
    strcat(value, ":");
    strcat(value, seconds_str);
    strcat(value, " ");
    strcat(value, year_str);

    sqlite3_bind_text(statement, 1, value, strlen(value), SQLITE_STATIC);
    list l = l_new();
    
    if(sqlite3_step(statement) != SQLITE_ROW) {
        err->error = DATABASE_SELECT;
        err->description = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        return NULL;
    }
    
    populateList(l, statement);

    while(sqlite3_step(statement) == SQLITE_ROW) {
        populateList(l, statement);
    }

    free(value);
    return l;
}




list getByStartDate(struct tm t, struct error* e) {
    return getByDate(t, e, "select * from tasks where start_date like ?");
}

list getByEndDate(struct tm t, struct error* e) {
    return getByDate(t, e, "select * from tasks where end_date like ?");
}

void serialize(Task t, FILE* f, struct error* e) {
    e->error = NO_ERROR;
    if(!f) {
        e->error        = FILE_ERROR;
        e->description  = "File given for serialize target is NULL";   
        return ;
    }
    if(!t) {
        e->error        = PARAMETER_ERROR;
        e->description  = "Task variable given as parameter was null"; 
        return;
    }

    fwrite(t, sizeof(struct task), 1, f); 
}

Task deserialize(FILE* f, struct error* e) {
    e->error = NO_ERROR;
    Task t = (Task)malloc(sizeof(struct task));
    if(!f) {
        e->error        = FILE_ERROR;
        e->description  = "File given for serialize target is NULL";   
        return NULL;
    }
    if(!t) {
        e->error        = FILE_ERROR;
        e->description  = "Task variable given as parameter was null"; 
    }

    fread(t, sizeof(struct task), 1, f);
    return t;
}

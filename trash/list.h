/*
 * list.h
 *
 *  Created on: May 13, 2014
 *      Author: andrei
 */

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>

#define DEBUG_FLAG 1
#define DEBUG(str) if(DEBUG_FLAG) fprintf(stdout, "%s\n", str)
#define INFO(str) fprintf(stdout, "%s\n", str)
#define ERROR(str) fprintf(stderr, "%s\n", str)


typedef enum {
FALSE,
TRUE
} BOOL;

struct node {
    void* value;
    struct node* next;
};

typedef struct node* Node;

struct list {
    Node start;
    size_t size;
};

typedef struct list* List;

/* constructor and destructor like*/
List L_NEW();
void L_DELETE(List list);

/* utility methods */
BOOL add(List list, void* value);
void map(List list, void* (*function)(void*));
void* get(List list, size_t index);
size_t size(List list);



#endif // LIST_H_INCLUDED

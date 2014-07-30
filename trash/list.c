/*
 * list.c
 *
 *  Created on: May 13, 2014
 *      Author: andrei
 */
#include"list.h"
BOOL error = FALSE;
char* errorMessage;

static void* xmalloc(size_t size) {
    void* ptr;

    ptr = malloc(size);
    if(ptr == NULL) {
        ERROR("Not enough memory for allocating memory");
        error = 1;
        errorMessage = "Not enough memory";
    } else {
        INFO("Memory allocated");
    }

    return ptr;
}

List L_NEW() {
    List list = xmalloc(sizeof(struct list));
    list->size = (size_t)0;
    list->start= NULL;

    return list;
}

void L_DELETE(List list) {
    Node node = list->start;

    while(node != NULL) {
        Node temp = node;
        node = node->next;
        free(temp);
        INFO("Deleting node");
    }

    INFO("Memory deallocated");
    free(list);
}


BOOL add(List list, void* value) {
    BOOL ret = TRUE;
    Node node = (Node)xmalloc(sizeof(struct node));
    node->value = value;
    node->next = list->start;
    list->start = node;
    ++ list->size;
    if(error) {
    	ret = FALSE;
    	error = FALSE;
    	errorMessage = "";
    }

    return ret;
}

void map(List list, void* (*function)(void*)) {
    Node node = list->start;

    while(node != NULL) {
        function(node);
        node = node->next;
    }
}

void* get(List list, size_t index) {
    size_t idx = (size_t)0;
    Node node = list->start;

    while(node != NULL && idx < index) {
        node = node->next;
    }

    return node;
}

size_t size(List list) {
    return list ? list->size : 0;
}


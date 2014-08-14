#include<malloc.h>
#include<string.h>
#include"str.h"

#define DEFAULT_SIZE 8*2


struct string {
    size_t max_size;
    size_t curr_size;
    char* str;
};

static void string_realloc(struct string* s, size_t new_size) {

}

struct string  string_new() {
    struct string s;
    s.max_size  = DEFAULT_SIZE;
    s.curr_size = 1;
    s.str       = (char*)malloc(sizeof(char) * DEFAULT_SIZE);
    s.str[0]    = '\0';


    return s;
}

struct string  string_new2(char* s) {
    size_t sz       = strlen(s);
    ++ sz; // + '\0'
    struct string str;
    str.max_size    = sz * 2;
    str.curr_size   = sz;
    str.str         = (char*) malloc(sizeof(char) * str.max_size);
    strcpy(str.str, s);

    return str; 
}

struct string string_new3(size_t sz) {
    struct string str;
    str.max_size    = sz * 2;
    str.curr_size   = sz;
    str.str         = (char*) malloc(sizeof(char) * str.max_size);
    str.str[0]      = '\0';

    return str;
}

void string_free(struct string* s) {
    free((*s).str);
}

size_t string_len(struct string s) {
    return s.curr_size-1;
}

void string_append(struct string* s, char* c) {
    size_t len = strlen(c);

    if(len > (*s).max_size - (*s).curr_size) {
        string_realloc(s, 2*len+1);
    }

    strcat(s->str, c);
    s->curr_size += len;
}

void string_append2(struct string* s1, struct string s2) {
    if(s1->max_size - s1->curr_size < s2.curr_size - 1) {
        string_realloc(s1, 2* s2.curr_size);
    }

    strcpy(s1->str, s2.str);
}

struct string mix(char* s1, char* s2) {
    if(!s1)
        s1 = "";
    if(!s2)
        s2 = "";
 
    size_t sz1 = strlen(s1), sz2 = strlen(s2);
    size_t sz = sz1 + sz2 + 1 /*'\0'*/;
    struct string s = string_new();

}



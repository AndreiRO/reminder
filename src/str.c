#include<malloc.h>
#include<string.h>
#include"str.h"

#define DEFAULT_SIZE 8*2


static void string_realloc(struct string* s, size_t new_size) {
    
    if(new_size < strlen(s->str)) {
        return ;
    }

    char* new_str = (char*)malloc(sizeof(char) * new_size * 2);
    strcpy(new_str, s->str);
    free(s->str);
    s->str        = new_str;
    s->curr_size  = new_size;
    s->max_size   = new_size * 2;
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
        string_realloc(s1, 2* s2.curr_size+1);
    }

    strcpy(s1->str, s2.str);
}

struct string string_concat(char* s1, char* s2) {
    if(!s1)
        s1 = "";
    if(!s2)
        s2 = "";
 
    size_t sz1 = strlen(s1), sz2 = strlen(s2);
    size_t sz = sz1 + sz2 + 1 /*'\0'*/;
    struct string s = string_new();
    free(s.str);
    s.str = (char*)malloc(sz*sizeof(char));
    strcpy(s.str, s1);
    strcat(s.str, s2);

    return s;
}

char*   string_char(struct string s) {
    return s.str;
}

void    string_debug(struct string s) {
    printf("\n-------------------------------------\n");
    printf("String: %s \nSize: %ld\nMax size: %ld", s.str, s.curr_size, s.max_size);
    printf("\n-------------------------------------\n");
}

struct string string_mix(char* s1, char* s2) {
    size_t  l1 = strlen(s1),
            l2 = strlen(s2) + 1 ;
    struct string s = string_new3(l1+l2);
    string_append(&s, s1);
    string_append(&s, s2);
    
    return s;
}

void string_append3(struct string* s, char c) {
    char txt[2] = {c,'\0'};
    string_append(s, txt);
}

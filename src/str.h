#ifndef _STR_H_
#define _STR_H_
#include<stddef.h>

struct string {
    size_t max_size;
    size_t curr_size;
    char* str;
};



struct  string  string_new();
struct  string  string_new2(char* s);
struct  string  string_new3(size_t len);
void            string_free(struct string* s);

size_t          string_len(struct string s);

void            string_append(struct string* s, char* c);
void            string_append2(struct string* s1, struct string s2);
void            string_append3(struct string* s, char c);

char*           string_char(struct string s);

struct string   string_mix(char* s1, char* s2);

void            string_debug(struct string s);

#endif

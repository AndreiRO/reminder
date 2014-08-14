#ifndef _STR_H_
#define _STR_H_
#include<stddef.h>

struct string;


struct  string  string_new();
struct  string  string_new2(char* s);
void            string_free(struct string* s);

size_t          string_len(struct string s);

void            string_append(struct string* s, char* c);
void            string_append2(struct string* s1, struct string s2);

struct string   mix(char* s1, char* s2);

#endif

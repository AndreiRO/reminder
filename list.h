#ifndef _LIST_H_
#define _LIST_H_
#include<stdbool.h>
#include<stddef.h>

struct l;
struct n;
typedef void* value;

typedef struct l* list;
typedef struct n* node;
typedef node      iterator;

list l_new();
void l_delete(list l, void(*el_destructor)(void*));

size_t l_size(list l);
bool   l_empty(list l);

void l_push(list l, value v);
value l_pop(list l);

iterator l_iterator(list l);
void     l_reset_iterator(list l, iterator i);
value    l_value(iterator i);
void     l_next(iterator* i);
bool     l_iterator_at_end(iterator i);

void test(int cond, char* err_message);
void no_destructor(void* v);
void free_destructor(void* v);

#endif // _LIST_H defined

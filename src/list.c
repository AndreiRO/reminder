#include<malloc.h>
#include<stdlib.h>
#include"list.h"

void test(int cond, char* err_message) {
    if(cond == false || cond == 0) {
        fprintf(stderr, "Error: %s\n", err_message);
    }
}

void no_destructor(void* v) {
    // do nothing
}

void free_destructor(void* v) {
    free(v);
}

struct l {
    size_t size;
    node start;
};

struct n {
    value v;
    struct n* next;
};

list l_new() {
    list l = (list)malloc(sizeof(struct l));
    l->start = NULL;
    l->size  = 0;

    return l;
}

void l_delete(list l, void(*el_destructor)(void*)) {
    if(l) {
        node n = l->start;
        node t;
        while(n) {
            t = n->next;
            el_destructor(n->v);
            free(n);
            n = t;
        }

        free(l);
    }

}

size_t l_size(list l) {
    return (l) ? l->size : 0;
}

bool l_empty(list l) {
    return l_size(l) == 0;
}

void l_push(list l, value v) {
    if(l) {
        
        if(!l->start && l->size == 0) {
            l->start    = (node)malloc(sizeof(struct n));
            l->start->v = v;
            l->size     = 1;
        } else if(l->start && l->size > 0) {
            node n  = (node)malloc(sizeof(struct n));
            n->v    = v;
            n->next = l->start;
            l->start= n;
            ++ l->size;
        }
    }
}

value l_pop(list l) {
    void* t = NULL;
    if(l) {
        if(l->start && l->size == 1) {
            t = l->start->v;
            free(l->start);
            l->start = NULL;
            l->size = 0;
        } else if(l->start && l->size > 1) {
            test(l->start->next != NULL, "WEIRD NULL POINTER IN LIST");
            t = l->start->next->v;
            node f = l->start;
            l->start = f->next;
            free(f);
            -- l->size;
        }
    }

    return t;
}

iterator l_iterator(list l) {
    if(l) {
        return l->start; 
    }

    return NULL;
}

void l_reset_iterator(list l, iterator i) {
    if(l) {
        i = l_iterator(l);
    }
}

value l_value(iterator i) {
    value v = NULL;

    if(i) {
        v = i->v;
    }

    return v;
}

void l_next(iterator* i) {
    if(*i) {
        *i = (*i)->next;
    }
}

bool l_iterator_at_end(iterator i) {
    return i == NULL;
}



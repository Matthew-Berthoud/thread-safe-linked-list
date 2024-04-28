#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>


struct list_item {
    int value;
    struct list_item *next;
};


struct linked_list {
    struct list_item *head;
    int size;
};


static inline struct linked_list *
ll_create(void)
{
	struct linked_list *ll = malloc(sizeof(struct linked_list));
    if (ll == NULL)
        return NULL;
    ll->size = 0;
    return ll;
}


static inline int
ll_destroy(struct linked_list *ll)
{
    if (ll->size != 0)
        return 0;
    free(ll);
    return 1;
}


static inline void
ll_add(struct linked_list *ll, int value)
{
    struct list_item *old_head = ll->head;
    struct list_item *new_item = malloc(sizeof(struct list_item));

    new_item->value = value;
    new_item->next = old_head;

    ll->head = new_item;
    ll->size++;
}


static inline int
ll_length(struct linked_list *ll)
{
	return ll->size;
}


static inline bool
ll_remove_first(struct linked_list *ll)
{
    if (ll->size == 0)
        return false;
    ll->head = ll->head->next;
    ll->size--;
	return true;
}


static inline int
ll_contains(struct linked_list *ll, int value)
{
    int i = 1;
    struct list_item *cur = ll->head;
    while (cur != NULL) {
        if (cur->value == value)
            return i;
        cur = cur->next;
        i++;
    }
	return 0;
}


static inline void
ll_print(struct linked_list *ll)
{
    printf("\nLINKED LIST: size = %d\n", ll_length(ll));
    printf("HEAD\n");

    int i = 1;
    struct list_item *cur = ll->head;
    while (cur != NULL) {
        printf("%3d: %d\n", i, cur->value);
        cur = cur->next;
        i++;
    }

    printf("TAIL\n\n");
}


#endif

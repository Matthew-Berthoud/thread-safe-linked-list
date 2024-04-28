#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


struct list_item {
    int value;
    struct list_item *next;
};


struct linked_list {
    struct list_item *head;
    pthread_mutex_t lock;
    int size;
};


static inline struct linked_list *
ll_create(void)
{
	struct linked_list *ll = malloc(sizeof(struct linked_list));
    if (ll == NULL)
        exit(1);

    ll->size = 0;

    int rc = pthread_mutex_init(&ll->lock, NULL);
    if (rc != 0)
        exit(rc);

    return ll;
}


// Returns 1 if list destroyed, 0 if not
static inline int
ll_destroy(struct linked_list *ll)
{
    if (ll->size != 0)
        return 0;

    int rc = pthread_mutex_destroy(&ll->lock);
    if (rc != 0)
        exit(rc);

    free(ll);
    return 1;
}


static inline void
ll_add(struct linked_list *ll, int value)
{
    struct list_item *new_item = malloc(sizeof(struct list_item));
    new_item->value = value;

    pthread_mutex_lock(&ll->lock);
    printf("MUTEX_LOCK\n");
    // critical section
    new_item->next = ll->head;
    ll->head = new_item;
    ll->size++;
    // end critical section
    printf("MUTEX_UNLOCK\n");
    pthread_mutex_unlock(&ll->lock);
}


static inline int
ll_length(struct linked_list *ll)
{
    // no lock required
	return ll->size;
}


static inline bool
ll_remove_first(struct linked_list *ll)
{
    bool removed;
    pthread_mutex_lock(&ll->lock);
    printf("MUTEX_LOCK\n");
    // critical section
    if (ll->size == 0)
        removed = false;
    else {
        ll->head = ll->head->next;
        ll->size--;
        removed = true;
    }
    // end critical section
    printf("MUTEX_UNLOCK\n");
    pthread_mutex_unlock(&ll->lock);

	return removed;
}


static inline int
ll_contains(struct linked_list *ll, int value)
{
    int i = 1;

    pthread_mutex_lock(&ll->lock);
    printf("MUTEX_LOCK\n");
    // critical section

    struct list_item *cur = ll->head;
    while (cur != NULL) {
        if (cur->value == value){
            break;
        }
        cur = cur->next;
        i++;
    }
    // return 0 if node not found
    if (cur == NULL)
        i = 0;
    // end critical section
    printf("MUTEX_UNLOCK\n");
    pthread_mutex_unlock(&ll->lock);

	return i;
}


static inline void
ll_print(struct linked_list *ll)
{
    pthread_mutex_lock(&ll->lock);
    printf("MUTEX_LOCK\n");
    // critical section
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
    // end critical section
    printf("MUTEX_UNLOCK\n");
    pthread_mutex_unlock(&ll->lock);
}


#endif


#ifndef LIST_H
#define LIST_H

#include <assert.h>
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
};


static inline struct linked_list *
ll_create(void) {
	struct linked_list *ll;
    int rc;

    printf("ll_create\n");

    ll = malloc(sizeof(struct linked_list));
    assert(ll != NULL);

    rc = pthread_mutex_init(&ll->lock, NULL);
    assert(rc == 0);

    return ll;
}


// Returns 1 if list destroyed, 0 if not
static inline int
ll_destroy(struct linked_list *ll) {
    int rc, empty;

    printf("ll_destroy\n");

    empty = 1;

    pthread_mutex_lock(&ll->lock);
    if (ll->head != NULL) {
        empty = 0;
        printf("List not empty, destroy failed.");
    }
    pthread_mutex_unlock(&ll->lock);

    if (empty) {
        rc = pthread_mutex_destroy(&ll->lock);
        assert(rc == 0);
        free(ll);
    }

    return empty;
}


static inline void
ll_add(struct linked_list *ll, int value) {
    struct list_item *new_item;

    printf("ll_add\n");

    new_item = malloc(sizeof(struct list_item));
    assert(new_item != NULL);
    
    new_item->value = value;

    pthread_mutex_lock(&ll->lock);
    new_item->next = ll->head;
    ll->head = new_item;
    pthread_mutex_unlock(&ll->lock);
}


static inline int
ll_length(struct linked_list *ll) {
    struct list_item *cur;
    int n;

    printf("ll_length\n");

    pthread_mutex_lock(&ll->lock);
    cur = ll->head;
    n = 0;

    while (cur != NULL) {
        cur = cur->next;
        n++;
    }
    pthread_mutex_unlock(&ll->lock);

    return n;
}


static inline bool
ll_remove_first(struct linked_list *ll) {
    bool removed;

    printf("ll_remove_first\n");

    pthread_mutex_lock(&ll->lock);
    if (ll->head == NULL) {
        removed = false;
    }
    else {
        ll->head = ll->head->next;
        removed = true;
    }
    pthread_mutex_unlock(&ll->lock);

	return removed;
}


static inline int
ll_contains(struct linked_list *ll, int value) {
    struct list_item *cur;
    int i;

    printf("ll_contains\n");

    i = 1;
    pthread_mutex_lock(&ll->lock);
    cur = ll->head;

    while (cur != NULL) {
        if (cur->value == value) {
            break;
        }
        cur = cur->next;
        i++;
    }
    // return 0 if node not found
    if (cur == NULL) {
        i = 0;
    }
    pthread_mutex_unlock(&ll->lock);

	return i;
}


static inline void
ll_print(struct linked_list *ll) {
    struct list_item *cur;
    int length;

    printf("ll_print\n");

    // Outside critical section, since ll_length acquires the lock
    length = ll_length(ll);

    pthread_mutex_lock(&ll->lock);
    cur = ll->head;
    printf("HEAD(size %d) [", length);
    while (cur != NULL) {
        if (cur->next == NULL) {// not the last element, add comma
            printf("%d", cur->value);
        } else {
            printf("%d, ", cur->value);
        }

        cur = cur->next;
    }
    printf("] TAIL\n");
    pthread_mutex_unlock(&ll->lock);
}


#endif

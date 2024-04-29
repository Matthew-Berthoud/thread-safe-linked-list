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


struct linked_list * ll_create(void) {
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
int ll_destroy(struct linked_list *ll) {
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


void ll_add(struct linked_list *ll, int value) {
    struct list_item *new_item, *next;

    printf("ll_add\n");

    new_item = malloc(sizeof(struct list_item));
    assert(new_item != NULL);
    
    new_item->value = value;
    
    do {
        next = ll->head;
        new_item->next = next;
    } while (!__sync_bool_compare_and_swap(&(ll->head), next, new_item));
}


int ll_length(struct linked_list *ll) {
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


bool ll_remove_first(struct linked_list *ll) {
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


bool ll_remove_idx(struct linked_list *ll, int idx) {
    struct list_item *cur, *prev;
    bool removed;
    int i;

    printf("ll_remove_idx\n");

//    if (ll_length(ll) == 0) {
//        return false;
//    }
    if (idx == 1) {
        return ll_remove_first(ll);
    }
    removed = false;
    i = 2;

    pthread_mutex_lock(&ll->lock);
    prev = ll->head;
    // length can become 0 between the earlier check and now, since it's not locked
    if (prev == NULL) {
        pthread_mutex_unlock(&ll->lock);
        return false;
    }
    cur = prev->next;
    while (cur != NULL) {
        if (idx == i) {
            prev->next = cur->next;
            removed = true;
            break;
        }
        prev = cur;
        cur = cur->next;
        i++;
    }
    pthread_mutex_unlock(&ll->lock);

	return removed;
}


int ll_contains(struct linked_list *ll, int value) {
    struct list_item *cur;
    int i;

    printf("ll_contains\n");

    i = 0;
    pthread_mutex_lock(&ll->lock);
    cur = ll->head;

    while (cur != NULL) {
        i++;
        if (cur->value == value) {
            pthread_mutex_unlock(&ll->lock);
            return i; 
        }
        cur = cur->next;
    }
    pthread_mutex_unlock(&ll->lock);
    return 0; // Return 0 if element not found
}


void ll_print(struct linked_list *ll) {
    struct list_item *cur;

    printf("ll_print\n");

    // Using printf inside mutex, which isn't great
    // but this function is mainly for testing anyway
    // so performance isn't a huge concern.
    pthread_mutex_lock(&ll->lock);
    cur = ll->head;
    printf("HEAD [");
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

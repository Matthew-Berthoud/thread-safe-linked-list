#ifndef LIST_H
#define LIST_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define cas __sync_bool_compare_and_swap


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
    } while (!cas(&(ll->head), next, new_item));
}


int ll_length(struct linked_list *ll) {
    struct list_item *cur;
    int n;

    printf("ll_length\n");

    cur = ll->head;
    n = 0;

    while (cur != NULL) {
        cur = cur->next;
        n++;
    }

    return n;
}


bool ll_remove_first(struct linked_list *ll) {
    struct list_item *new_head, *cur_head;

    printf("ll_remove_first\n");

    do {
        cur_head = ll->head;
        if (cur_head == NULL) {
            return false;
        }
        new_head = cur_head->next;
    } while (!cas(&(ll->head), cur_head, new_head));

    return true;
}


bool ll_remove_idx(struct linked_list *ll, int idx) {
    struct list_item *cur, *to_delete;
    int i;

    printf("ll_remove_idx\n");

    if (idx == 1) {
        return ll_remove_first(ll);
    } else if (idx < 1) {
        return false;
    }
    // idx >= 2
rem_idx_loop:
    i = 2;
    cur = ll->head;
    while (cur != NULL) {
        if (i != idx - 1) {
            i++;
            if (!cas(&cur, cur, cur->next)) {
                goto rem_idx_loop;
            }
            continue;
        }
        to_delete = cur->next;
        if (cur->next == NULL) {
            return false;
        } 
        if (to_delete->next == NULL || cas(&(cur->next), to_delete, to_delete->next)) {
            free(to_delete);
            to_delete = NULL;
            return true;
        }
        goto rem_idx_loop;
    }

	return false;
}


int ll_contains(struct linked_list *ll, int value) {
    struct list_item *cur;
    int i;

    printf("ll_contains\n");

contains_loop:
    i = 0;
    cur = ll->head;
    while (cur != NULL) {
        if (cur->value != value) {
            i++;
            if (!cas(&cur, cur, cur->next)) {
                goto contains_loop;
            }
            continue;
        }
        return i; 
    }
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

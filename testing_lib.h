#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define MAX_THREADS 100
#define MIN_THREADS 2
// elements added to list will be multiples of 5
#define MULT 5


void test_add(struct linked_list *ll, int num_elements) {
    int i;

    for (i = 0; i < num_elements; i++) {
        ll_add(ll, i * MULT);
        ll_print(ll);
    }
}


void test_remove_first(struct linked_list *ll, int num_elements) {
    int i;

    for (i = 0; i < num_elements; i++) {
        if (!ll_remove_first(ll)) {
            printf("remove_first failed (size 0)\n");
        }
        ll_print(ll);
    }
}


void test_remove_idx(struct linked_list *ll, int num_elements) {
    int i;

    for (i = 0; i < num_elements; i = i + MULT) {
        // 1, 6, 11, 16, etc... because why not
        if (!ll_remove_idx(ll, i + 1)) {
            printf("remove_idx failed (idx not included)\n");
        }
        ll_print(ll);
    }

}


void test_contains(struct linked_list *ll, int num_elements) {
    int i, idx;

    for (i = 0; i < num_elements; i++) {
        idx = ll_contains(ll, i * MULT);
        if (idx) {
            printf("contains %d at index %d\n", i, idx);
        } else {
            printf("doesn't contain %d\n", i);
        }
        ll_print(ll);
    }
}


void *thread(void *arg) {
    struct linked_list *ll = arg;
    int i;

    // These can be reordered or reused however for testing
    for (i = 0; i < 10; i++) {
        test_add(ll, 5);
        test_contains(ll, 5);
        test_remove_first(ll, 5);
        test_contains(ll, 5);

        test_add(ll, 5);
        test_contains(ll, 5);
        test_remove_idx(ll, 20);
        test_remove_first(ll, 5);
        test_remove_idx(ll, 20);
        test_contains(ll, 5);
    }
    
    printf("exiting thread\n");
    pthread_exit("");
}


void run_threads(struct linked_list *ll, int n) {
    pthread_t threads[n];
    int i, rc;

    printf("beginning thread creation\n");

    for (i = 0; i < n; i++) {
        rc = pthread_create(&threads[i], NULL, thread, (void*)ll);
        assert(rc == 0);
        printf("thread %d created\n", i);
    }

    for (i = 0; i < n; i++) {
        rc = pthread_join(threads[i], NULL);
        assert(rc == 0);
        printf("thread %d joined\n", i);
    }

    printf("all threads finished!\n\n");
    ll_print(ll);
}


void thread_test_list(int n) {
    struct linked_list *ll;
    int rc;

    // make the list
    ll = ll_create(); 
    printf("list created\n");
    ll_print(ll);

    run_threads(ll, n);

    // empty the list (if needed)
    while (ll->head != NULL) {
        ll_remove_first(ll);
    }

    // destroy the list
    rc = ll_destroy(ll);
    if (!rc)
        printf("failed to destroy list!\n");
    else {
        printf("successfully destroyed list!\n");
    }
}

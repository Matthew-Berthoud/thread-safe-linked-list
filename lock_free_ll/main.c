#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "../testing_lib.h"


int main(int argc, char *argv[]) {
    int n_threads;

    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    n_threads = atoi(argv[1]);
    if (n_threads < MIN_THREADS || n_threads > MAX_THREADS) {
        printf("Use 5-200 threads\n");
        return 1;
    }

    thread_test_list(n_threads);
    return 0;
}

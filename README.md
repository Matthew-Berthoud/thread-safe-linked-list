# thread-safe-linked-list
*Thread Safe Linked List in C (CSCI 544 hw 5)*

## Part 1: thread-safe linked list with mutexes
```
cd mutex_ll
make
./test <num_threads>
```

## Part 2: thread-safe and LOCK/WAIT-FREE linked list
```
cd lock_free_ll
make
./test <num_threads>
```

## Testing both more thouroughly

* You can use the apis in `testing_lib.h` in a different way by changing the `*thread` function however you want.
    * You can put the functions in different orders, and make them get called a different number of times.
    * if you do this, run `make clean` in each directory before running `make` again.

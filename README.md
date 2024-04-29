# thread-safe-linked-list
*Thread Safe Linked List in C (CSCI 544 hw 5)*

## Part 1: thread-safe linked list with mutexes
```
cd mutex_ll
make
./test <num_threads>
```
* Also, you can use the apis in testing_lib.h in a different way by changing the `*thread` function.
* I tried to make a random assortment of behavior possible with the way I ordered things.

## Part 2: thread-safe and LOCK/WAIT-FREE linked list
```
cd lock_free_ll
make
./test <num_threads>
```

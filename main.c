#include <stdio.h>
#include <pthread.h>

#include <list.h>


void
run_tests(struct linked_list *ll)
{
    // add some elements
    ll_add(ll, 34);
    ll_print(ll);

    int elements[] = {4, 76, -23, 7, 0, 23, -37};
    for (int i = 0; i < 7; i++) {
        ll_add(ll, elements[i]);
    }
    ll_print(ll);

    // delete some elements
    bool result;
    for (int i = 0; i < 3; i++) {
        result = ll_remove_first(ll);
        if (!result)
            printf("remove returned false\n");
    }
    ll_print(ll);
        
    // try to destroy
    int ret = ll_destroy(ll);
    if (!ret)
        printf("cannot destroy a full list\n");
    else
        printf("destroyed a full list somehow... yikes!\n");
    ll_print(ll);

    // check contains
    for (int i = 3; i < 9; i++) {
        int idx = ll_contains(ll, i);
        if (idx)
            printf("list contains %d at index %d\n", i, idx);
        else
            printf("list doesn't contain %d\n", i);
    }
    ll_print(ll);

    // remove elements to 0
    for (int i = 0; i < 7; i++) { // two bad removals at the end
        result = ll_remove_first(ll);
        if (!result)
            printf("remove returned false\n"); // should print twice at end
    }
    ll_print(ll);
    
}

int
main(void)
{
    struct linked_list *ll = ll_create();
    ll_print(ll);

    run_tests(ll);

    // destroy
    int ret = ll_destroy(ll);
    if (!ret)
        printf("list doesn't register as empty... yikes!\n");
    else {
        printf("successfully destroyed list!\n");
    }

	return 0;
}



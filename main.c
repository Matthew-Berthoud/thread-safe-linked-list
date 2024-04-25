#include <stdio.h>

#include <list.h>


void
print_list(struct linked_list *ll)
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


int
main(void)
{
    struct linked_list *ll = ll_create();
    print_list(ll);

    // add some elements
    ll_add(ll, 34);
    print_list(ll);

    int elements[] = {4, 76, -23, 7, 0, 23, -37};
    for (int i = 0; i < 7; i++) {
        ll_add(ll, elements[i]);
    }
    print_list(ll);

    // delete some elements
    bool result;
    for (int i = 0; i < 3; i++) {
        result = ll_remove_first(ll);
        if (!result)
            printf("remove returned false\n");
    }
    print_list(ll);
        
    // try to destroy
    int ret = ll_destroy(ll);
    if (!ret)
        printf("cannot destroy a full list\n");
    else
        printf("destroyed a full list somehow... yikes!\n");
    print_list(ll);

    // check contains
    for (int i = 3; i < 9; i++) {
        int idx = ll_contains(ll, i);
        if (idx)
            printf("list contains %d at index %d\n", i, idx);
        else
            printf("list doesn't contain %d\n", i);
    }
    print_list(ll);

    // remove elements to 0
    for (int i = 0; i < 7; i++) { // two bad removals at the end
        result = ll_remove_first(ll);
        if (!result)
            printf("remove returned false\n"); // should print twice at end
    }
    print_list(ll);
    
    // destroy
    ret = ll_destroy(ll);
    if (!ret)
        printf("list doesn't register as empty... yikes!\n");
    else {
        printf("successfully destroyed list!\n");
    }

	return 0;
}


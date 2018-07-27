#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pointer_test.h"

p_test* create_p_test() {
    p_test * t_elem = malloc(sizeof(p_test));
    return t_elem;
};

d_mem* create_d_mem(unsigned int initial_capacity) {
    d_mem * m = malloc(sizeof(d_mem));
    m->allocated_n = initial_capacity;
    m->mem = malloc(m->allocated_n * sizeof(p_test *));
    m->name = NULL;
    m->n = 0;

    return m;
}

p_test** create_p_test_array(unsigned int size) {
    srand(time(NULL));

    // allocate 100 pointers, effectively an array
    p_test **t_array = malloc(size * sizeof(p_test *));

    // allocate 100 structs and have the array point to them
    p_test* t_elem = NULL;
    int i;
    for (i = 0; i < size; i++) {
        //t_array[i] = malloc(sizeof(struct Test));
        t_elem = create_p_test();
        t_array[i] = t_elem;
    }

    // lets fill each Test.data with a random number!
    for (i = 0; i < size; i++) {
        t_array[i]->data = rand() % 100;
    }

    return t_array;
}

void fill_p_test_memory(d_mem* m) {
    int size = m->allocated_n;
    int i;
    srand(time(NULL));
    // should have space for 100 pointers
    p_test **t_array = m->mem;

    // allocate 100 structs and have the array point to them
    p_test* t_elem = NULL;
    for (i = 0; i < size; i++) {
        //t_array[i] = malloc(sizeof(struct Test));
        t_elem = create_p_test();
        t_array[i] = t_elem;
        m->n++;
    }

    // lets fill each Test.data with a random number!
    for (i = 0; i < size; i++) {
        t_array[i]->data = rand() % 100;
    }
}

void free_p_test_array(p_test** t_array, unsigned int size) {
    printf("<<< free p_test 1 >>>\n");
    unsigned int i;
    for(i = 0; i < size; i++) {
        printf("  free [%d] = %d", i, t_array[i]->data);
        free(t_array[i]);
    }
    printf("\n<<< free p_test 2 >>>\n");
	free(t_array);
	printf("<<< free p_test end >>>");
}

void free_d_mem(d_mem* m) {
    printf("<<< free p_mem 1 >>>\n");
    if(m->name != NULL) free(m->name);
    printf("<<< free p_mem 2 ([3] = %d) >>>\n", m->mem[3]->data);
    free_p_test_array(m->mem, m->n);
    printf("<<< free p_mem 3 >>>\n");
    free(m);
    printf("<<< free p_mem end >>>\n");
}

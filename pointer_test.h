#ifndef POINTER_TEST_H_INCLUDED
#define POINTER_TEST_H_INCLUDED

typedef struct {
    int data;
} p_test;

typedef struct {
    char* name;
    p_test** mem;
    int allocated_n;
    int n;
} d_mem;

p_test* create_p_test();

d_mem* create_d_mem(unsigned int initial_capacity);

p_test** create_p_test_array(unsigned int size);

void fill_p_test_memory(d_mem* m);

void free_p_test_array(p_test** t_array, unsigned int size);

void free_d_mem(d_mem* m);

#endif // POINTER_TEST_H_INCLUDED

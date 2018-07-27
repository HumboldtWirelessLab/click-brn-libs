#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "scan_type_test.h"

//#include "c_source_data_types.h";
#include "c_source_data_functions.h"
#include "c_source_reader.h"
#include "scan_command_line.h"
#include "reflection.h"
#include "utile.h"

void test_pointer(int* i_p, short* s_p, long* l_p, float* f_p, double* d_p, long double* ld_p, char* str) {
    printf("i_p={%d ..}, s_p={%hd ..}, l_p={%ld ..}, f_p={%f ..}, d={%g ..}, ld_p={%Lg ..}, c_p=\"%s\"\n", i_p, s_p, l_p, f_p, d_p, ld_p, str);
}

void test_types(int i, unsigned int u_i, short s, unsigned short u_s, long l, unsigned long u_l, float f, double d, long double ld, char c) {
    printf("i=%d, u_i=%u, s=%hd, u_s=%hu, l=%ld, u_l=%lu, f=%f, d=%g, ld=%Lg, c='%c'\n", i, u_i, s, u_s, l, u_l, f, d, ld, c);
}

void run_test_types() {
    printf("\n********** start scan type test **********\n\n");
    char str_file[] = "scan_type_test.c";
    char str_p[] = "{-32000, 500, 3} {32000, 6} {1000000000, 56, 1} {5.7898, 0.000001} {4.123456789, 34e-12} {4.123456789, 4.45e36, 2.987} \"blaaaaui \"";
    char str_p_empty[] = "{} {} {} {} {} {} \"\"";
    char str_t[] = "-32000 64000 -32000 64000 -1000000000 2000000000 0.000001 4.123456789 4.45e36 z";
    char str_t_wrong[] = "a b -32.000 c -1000000e10 d f g h 67";
    const int i_test_pointer = 0;
    const int i_test_types = 1;
    int i;
    bool debug = false;
    t_value* t_v;

    src* s_c = NULL;
    src_function* s_f = NULL;
    f_header* f_h = NULL;

    printf("[1] reading source file %s :\n", str_file);
    s_c = read_source(str_file, debug);
    if(s_c == NULL) {
        printf("info: unkown file [%s]\n", str_file);
        return;
    }

    printf("[2] extract functions :\n");
    scan_c_source(s_c, 0, (s_c->src)->length, debug);
    for(i=0; i<s_c->n; ++i) {
        print_reduced_src_function(s_c->functions[i], "");
        print_string(create_function_wrapper("fwrapper", s_c->functions[i]), "   ");
    }

    printf("[3] scan_param for function test_point(...) ->\"%s\"\n", str_p);
    s_f = s_c->functions[i_test_pointer];
    f_h = s_f->header;
    t_v = scan_param(str_p, s_f, false);
    if(t_v == NULL) printf("ERROR: could not read param !!!");
    else {
        print_t_value(t_v, f_h->parameter_n, f_h->parameter_types, "   ");
        free_t_value_array(t_v, f_h->parameter_n, f_h->parameter_types);
    }

    printf("\n[4] scan_param for function test_types(...) ->\"%s\"\n", str_t);
    s_f = s_c->functions[i_test_types];
    f_h = s_f->header;
    t_v = scan_param(str_t, s_f, false);
    if(t_v == NULL) printf("ERROR: could not read param !!!");
    else {
        print_t_value(t_v, f_h->parameter_n, f_h->parameter_types, "   ");
        free_t_value_array(t_v, f_h->parameter_n, f_h->parameter_types);
    }

    printf("[5] scan_param for function test_point(...) ->\"%s\"\n", str_p_empty);
    s_f = s_c->functions[i_test_pointer];
    f_h = s_f->header;
    t_v = scan_param(str_p_empty, s_f, false);
    if(t_v == NULL) printf("ERROR: could not read param !!!");
    else {
        print_t_value(t_v, f_h->parameter_n, f_h->parameter_types, "   ");
        free_t_value_array(t_v, f_h->parameter_n, f_h->parameter_types);
    }

    printf("\n[6] scan_param for function test_types(...) ->\"%s\"\n", str_t_wrong);
    s_f = s_c->functions[i_test_types];
    f_h = s_f->header;
    t_v = scan_param(str_t_wrong, s_f, false);
    if(t_v == NULL) printf("ERROR: could not read param !!!");
    else {
        print_t_value(t_v, f_h->parameter_n, f_h->parameter_types, "   ");
        free_t_value_array(t_v, f_h->parameter_n, f_h->parameter_types);
    }

    free_src(s_c, true, debug);

    printf("\n********** finished scan type test **********\n");
}

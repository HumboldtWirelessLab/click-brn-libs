#ifndef REFLECTION_H_INCLUDED
#define REFLECTION_H_INCLUDED

#include <stdio.h>
//#include <libtcc.h>

#include "c_source_data_types.h"

static const char TCC_INCLUDES[] =
    "#include <tcclib.h>\n" /* include the "Simple libc header for TCC" */
    "//include <stdlib.h>\n"
    "#include <string.h>\n"
    "#include \"c_parameter_type.h\"\n"
    "#ifdef _WIN32\n" /* dynamically linked data needs 'dllimport' */
    " __attribute__((dllimport))\n"
    "#endif\n";

typedef struct {
    tcc_state state;
    void* f;
} tcc_function;

tcc_function* load_func(const char* src, char* f_name);

void free_tcc_function(tcc_function* func);

//char* (*load_readLine(void)) (FILE*);
tcc_function* load_readLine(void);

t_value (*load_function(src_function* s_f))(t_value*);

int load_functions(src* c_src, bool debug);

void sprint_t_value_var(e_type* e_t, char* dest, int index, char* var_name, bool is_array);

char* create_function_wrapper(char* wrapper_name, src_function* s_f);


void reflection_test();

#endif // REFLECTION_H_INCLUDED

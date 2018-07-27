#ifndef C_SOURCE_DATA_FUNCTIONS_H_INCLUDED
#define C_SOURCE_DATA_FUNCTIONS_H_INCLUDED

#include <stdbool.h>
#include "c_source_data_types.h"
#include "hashmap.h"

src_code* create_src_code(int initial_src_length);
src_code* create_src_code2(char* src);

src_code_ref* create_src_ref(src_code* s_c, int src_start_pos, int length);

//f_header* create_f_header(int initial_src_length);
//f_header* create_f_header2(char * src);
f_header* create_f_header(src_code_ref* src_r);

tcc_state* create_tcc_state();

//src_function* create_src_function(char* src, f_header* header);
src_function* create_src_function(src_code_ref* src_r, f_header* header);

src* create_src(int initial_src_length, int initial_function_count);

char* get_name(src_function* s_f);
e_type get_return_type(src_function* s_f);
e_type* get_parameter_types(src_function* s_f);
bool has_unkown_types(src_function* s_f);
bool e_type_is_pointer(e_type* e_t);

void free_src_code(src_code* s_c, bool debug);
void free_src_code_debug(src_code* s_c, char* prefix);

void free_header(f_header* header, bool debug);
void free_header_debug(f_header* header, char* prefix);

void free_src_function(src_function* s_f, bool debug);
void free_src_function_debug(src_function* s_f, char* prefix);

void free_src(src* c_src, bool all, bool debug);
void free_src_debug(src* c_src, char* prefix, bool all);

void free_functions_ht(map_t ht, bool debug);
void free_src_ht(map_t ht, bool all, bool debug);

void free_t_value_array(t_value* params, int n, e_type* e_t);
void free_t_value(t_value* param, e_type* e_t, bool free_self);

#endif // C_SOURCE_DATA_FUNCTIONS_H_INCLUDED

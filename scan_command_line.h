#ifndef SCAN_COMMAND_LINE_H_INCLUDED
#define SCAN_COMMAND_LINE_H_INCLUDED

#include "c_source_data_types.h"
#include "utile.h"

t_value* scan_param(char* src, src_function* s_f, bool debug);
void print_scan_param_error(char* src, e_type* e_t, int i);

//open_delim, separator and close_delim are pointer to single char
error_state* scan_array(char** src, e_type* e_t, const char* open_delim, const char* separator, const char* close_delim, bool debug);
void* get_type_array(e_type* e_t, int size);

char* scan_string(char** src);

#endif // SCAN_COMMAND_LINE_H_INCLUDED

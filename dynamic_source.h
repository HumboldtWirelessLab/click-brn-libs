#ifndef DYNAMIC_SOURCE_H_INCLUDED
#define DYNAMIC_SOURCE_H_INCLUDED

#include <stdbool.h>

#include "c_source_data_types.h"
#include "hashmap.h"

static const int NO_ERROR = 0;
static const int ERROR_FILE_NOT_FOUND = -1;
static const int ERROR_NO_FUNCTIONS_TO_ADD = -2;
static const int ERROR_ALREADY_LOADED = -3;

static const int ERROR_FUNCTION_NOT_FOUND = -4;

struct dyn_src {
    map_t ht_src;
    map_t ht_func;
    bool debug;
    bool print_command_line_info;
    int last_error_state;
    char error_msg[300];
};

typedef struct dyn_src dyn_src;

dyn_src* new_dynamic_source(bool print_command_line_info);

void free_dynamic_source(dyn_src* dynSrc);

int dynamic_load_debug(dyn_src* dynSrc, src* c_src, bool is_reload);

int dynamic_load(dyn_src* dynSrc, src* c_src, bool is_reload);

int dynamic_load_from_file_debug(dyn_src* dynSrc, char* file, bool is_reload);

int dynamic_load_from_file(dyn_src* dynSrc, char* file, bool is_reload);

int dynamic_load_from_string(dyn_src* dynSrc, char* source, bool is_reload);

src_function* find_func(dyn_src* dynSrc, char* f_name);

t_value call_func(src_function* func, t_value* param);

#endif // DYNAMIC_SOURCE_H_INCLUDED

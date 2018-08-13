#ifndef UTILE_H_INCLUDED
#define UTILE_H_INCLUDED

#include "hashmap.h"

typedef struct {
    int error;
    void* p_data;
} error_state;

static const char SCAN_TV_INT[] = "%d";
static const char SCAN_TV_U_INT[] = "%u";
static const char SCAN_TV_SHORT[] = "%hd";
static const char SCAN_TV_U_SHORT[] = "%hu";
static const char SCAN_TV_LONG[] = "%ld";
static const char SCAN_TV_U_LONG[] = "%lu";
static const char SCAN_TV_FLOAT[] = "%f";
static const char SCAN_TV_DOUBLE[] = "%lf";
static const char SCAN_TV_LONG_DOUBLE[] = "%Lf";
static const char SCAN_TV_CHAR[] = "%c";
static const char SCAN_TV_STRING[] = "%s";

static const char PRINT_TV_INT[] = "%d%s";
static const char PRINT_TV_U_INT[] = "%u%s";
static const char PRINT_TV_SHORT[] = "%hd%s";
static const char PRINT_TV_U_SHORT[] = "%hu%s";
static const char PRINT_TV_LONG[] = "%ld%s";
static const char PRINT_TV_U_LONG[] = "%lu%s";
static const char PRINT_TV_FLOAT[] = "%f%s";
static const char PRINT_TV_DOUBLE[] = "%.10lg%s";
static const char PRINT_TV_LONG_DOUBLE[] = "%.20Lg%s";//"%.20Lf%s";
static const char PRINT_TV_CHAR[] = "%c%s";
static const char PRINT_TV_STRING[] = "%s%s";
static const char PRINT_TV_INT_INDENTING_NL[] = "%s%d%s%s";
static const char PRINT_TV_U_INT_INDENTING_NL[] = "%s%u%s%s";
static const char PRINT_TV_SHORT_INDENTING_NL[] = "%s%hd%s%s";
static const char PRINT_TV_U_SHORT_INDENTING_NL[] = "%s%hu%s%s";
static const char PRINT_TV_LONG_INDENTING_NL[] = "%s%ld%s%s";
static const char PRINT_TV_U_LONG_INDENTING_NL[] = "%s%lu%s%s";
static const char PRINT_TV_FLOAT_INDENTING_NL[] = "%s%f%s%s";
static const char PRINT_TV_DOUBLE_INDENTING_NL[] = "%s%.10lg%s%s";
static const char PRINT_TV_LONG_DOUBLE_INDENTING_NL[] = "%s%.20Lg%s%s";//"%s%.20Lf%s%s";
static const char PRINT_TV_CHAR_INDENTING_NL[] = "%s%c%s%s";
static const char PRINT_TV_STRING_INDENTING_NL[] = "%s%s%s%s";

const char* scanf_type_string(e_type* e_t);
const char* printf_type_string(e_type* e_t, bool with_indenting_nl);

char * strrstr(const char *string, const char *find);

char* copy_string(const char* string);

char* create_spacer(char* s, char* to_append);

void print_string(char* s, char* indenting);

void print_sub_string(char* s, int start_pos, int length, char* indenting);

void print_e_type(e_type* e_t, bool is_ref_type, char* indenting);

void print_f_header(f_header* f_h, char* indenting, bool nl);

void print_src_code(src_code* s_c, char* prefix, char* indenting);

void print_src_code_ref(src_code_ref* src_r, char* prefix, char* indenting);

void print_src_function(src_function* s_f, char* indenting);

void print_reduced_src_function(src_function* s_f, char* indenting);

void print_functions_ht(map_t ht);

void print_t_value(t_value* t_v, int n, e_type* e_t, char* indenting);

void print_void(void* v, e_type* e_t, char* indenting, bool with_nl);

unsigned int exists_file(char* file);

error_state* new_error_state(int error, void* p_data);

void free_error_state(error_state* e_s);

bool writeToFile(char* f_name, char* content);

void my_clock();

#endif // UTILE_H_INCLUDED

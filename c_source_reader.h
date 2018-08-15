#ifndef C_SOURCE_READER_H_INCLUDED
#define C_SOURCE_READER_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include "c_source_data_types.h"

static const char COMMENT_LINE[] = "//";
static const char COMMENT_START[] = "/*";
static const char COMMENT_END[] = "*/";

static const char KW_UNSIGNED[] = "unsigned";
static const char KW_INT[] = "int";
static const char KW_SHORT[] = "short";
static const char KW_LONG[] = "long";
static const char KW_DOUBLE[] = "double";
static const char KW_CHAR[] = "char";
static const char KW_VOID[] = "";
static const char KW_P[] = "*";

static const int DEFAULT_SRC_FUNCTION_CAPACITY = 5;

/**
 * Einlesen einer Quellcodedatei, öffnet Datei und ruft read_source(...) auf.
 */
src* read_source_from_file(char* file_name, bool debug);
/**
 * Einlesen eines Quellcodestrings, öffnet Dateistream auf den Quellcodestring und ruft read_source(...) auf.
 */
src* read_source_from_string(char* source, char* source_name, bool debug);
/**
 * Einlesen eines Quellcodestrings, Zeile bei Zeile.
 */
src* read_source_from_string_2(char* source, char* source_name, bool debug);
/**
 * Einlesen einer Quellcodedatei, ablegen in einem src-Struct und Pointer auf dieses zurückgeben.
 */
src* read_source(FILE* file, char* file_name, bool debug);

/**
 * Generiert aus einer Funktionsheader-Zeichenkette (s_h) ein f_header-Struct mit Infos zu
 * Return- und Parameter-Typen.
 */
//f_header* create_f_header_from_string(char* s_h, bool debug);
f_header* create_f_header_from_string(char* s_h, src_code_ref* src_r, bool debug);

/**
 * Extrahiert die Funtionen aus dem durch src-Struct s_c gegebenen C-Quellcode,
 * speichert jeweils die Informationen in einem src_function-Struct und fügt diese
 * dem dem src-Struct an.
 */
void scan_c_source(src *s_c, int start_src_pos, int end_src_pos, bool debug);


void trim_string(char *line, char *trimmed_line);

bool convert_ref_type(char* src, bool debug);

bool is_valid_t(char* src, char* t, char c);

#endif // C_SOURCE_READER_H_INCLUDED

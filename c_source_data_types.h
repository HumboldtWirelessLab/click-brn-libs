#ifndef C_SOURCE_DATA_TYPES_H_INCLUDED
#define C_SOURCE_DATA_TYPES_H_INCLUDED

#include <libtcc.h>
#include "c_parameter_type.h"

/**
 * Zum merken der Übergabe-/Rückgabetypen
 */
typedef enum {
    EMPTY,
    VOID, VOID_P,
    INT, INT_P, U_INT, U_INT_P,
    SHORT, SHORT_P, U_SHORT, U_SHORT_P,
    LONG, LONG_P, U_LONG, U_LONG_P,
    FLOAT, FLOAT_P,
    DOUBLE, DOUBLE_P,
    LONG_DOUBLE, LONG_DOUBLE_P,
    STRING, CHAR_P, CHAR,
    UNKNOWN, UNKNOWN_P
} e_type;

static const char TV_INT[] = "i";
static const char TV_INT_P[] = "i_p";
static const char TV_U_INT[] = "u_i";
static const char TV_U_INT_P[] = "u_i_p";
static const char TV_SHORT[] = "s";
static const char TV_SHORT_P[] = "s_p";
static const char TV_U_SHORT[] = "u_s";
static const char TV_U_SHORT_P[] = "u_s_p";
static const char TV_LONG[] = "l";
static const char TV_LONG_P[] = "l_p";
static const char TV_U_LONG[] = "u_l";
static const char TV_U_LONG_P[] = "u_l_p";
static const char TV_FLOAT[] = "f";
static const char TV_FLOAT_P[] = "f_p";
static const char TV_DOUBLE[] = "d";
static const char TV_DOUBLE_P[] = "d_p";
static const char TV_LONG_DOUBLE[] = "ld";
static const char TV_LONG_DOUBLE_P[] = "ld_p";
static const char TV_CHAR[] = "c";
static const char TV_CHAR_P[] = "c_p";
static const char TV_VOID_P[] = "p";

typedef struct {
    char* src;
	int length;
	int lines;
} src_code;

typedef struct {
    src_code* s_c;
    int src_start_pos;
    int length;
    int lines;
} src_code_ref;

/**
 * Zum Speichern der Header-Informationen zu einer Funktion.
 */
typedef struct {
    src_code_ref* src_r; //Quellcode
    char* name;
    unsigned int return_is_ref_type; //Rückgabewert kann auf Wert in Parameter zeigen, nicht automatisch löschen
    e_type return_type; //Rückgabetyp
    e_type* parameter_types; //Übergabeparametertypen
    unsigned int parameter_n; //Anzahl der Übergabeparameter
    short has_unkown_types;
} f_header;

typedef struct {
    TCCState* s;
    int f_count;
} tcc_state;

/**
 * Zum Speichern aller Informationen zu einer Funktion, samt Quellcode und einem Funktions-Pointer,
 * für den Zugriff auf die Funktion.
 */
typedef struct {
    struct src* c_src;
    f_header* header;
	src_code_ref* src_r; //Quellcode
	//void* func;
	tcc_state* state;
	t_value (*func)(t_value*);
} src_function;

/**
 * Zum Speichern der Inforamtionen der Funktionen einer Quellcodedatei.
 */
typedef struct {
    char* name;
    src_code* src; //Quellcode
    int src_start_pos;
    src_function** functions;
    //struct src_function* functions;
    int n;
    int allocated_n;
} src;

#endif // C_SOURCE_DATA_TYPES_H_INCLUDED

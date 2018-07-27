#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "scan_command_line.h"
#include "c_source_data_functions.h"
//#include "utile.h"

/**
 * Versucht aus der gegebenen Zeichenkette src die für die Funktion s_f passenden
 * Parameter zu extrahieren. Bei Erfolg, wird ein Union-Array vom Typ t_value zurückgegben,
 * welches, wenn es nicht mehr gebraucht wird, mit free(..) freigegeben werden.
 * Ansonsten wird NULL zurückgegeben. Dies ist auch der Fall, wenn die Funktion s_f keine
 * Parameter erwartet.
 */
t_value* scan_param(char* src, src_function* s_f, bool debug) {
    static const char OP_DELIM = '{';
    static const char SEP = ',';
    static const char CL_DELIM = '}';

    t_value* params = NULL;
    f_header* f_h = s_f->header;
    error_state* e_s;
    int error_state = 0;
    if(f_h->parameter_n > 0) {
        params = malloc(sizeof(t_value) * f_h->parameter_n);
        e_type e_t;
        //char* end;
        int i, n;
        for(i=0, n=0; i < f_h->parameter_n && *src; ++i) {
            e_t = f_h->parameter_types[i];
            if(debug) {
                printf("[%d] ", i);
                print_e_type(&e_t, false, "");
                printf(" src=%s\n", src);
            }

            //printf("[1]wait for input...\n");
            //getchar();

            switch(e_t) {
                //case EMPTY:
                //case VOID:
                //case VOID_P:
                case INT_P: e_s = scan_array(&src, &e_t, &OP_DELIM, &SEP, &CL_DELIM, debug);
                    if(e_s->error > 0) {++error_state; free_error_state(e_s);}
                    else {params[i].i_p = e_s->p_data; free_error_state(e_s); continue;}
                    break;
                case U_INT_P: e_s = scan_array(&src, &e_t, &OP_DELIM, &SEP, &CL_DELIM, debug);
                    if(e_s->error > 0) {++error_state; free_error_state(e_s);}
                    else {params[i].u_i_p = e_s->p_data; free_error_state(e_s); continue;}
                    break;
                case SHORT_P: e_s = scan_array(&src, &e_t, &OP_DELIM, &SEP, &CL_DELIM, debug);
                    if(e_s->error > 0) {++error_state; free_error_state(e_s);}
                    else {params[i].s_p = e_s->p_data; free_error_state(e_s); continue;}
                    break;
                case U_SHORT_P: e_s = scan_array(&src, &e_t, &OP_DELIM, &SEP, &CL_DELIM, debug);
                    if(e_s->error > 0) {++error_state; free_error_state(e_s);}
                    else {params[i].u_s_p = e_s->p_data; free_error_state(e_s); continue;}
                    break;
                case LONG_P: e_s = scan_array(&src, &e_t, &OP_DELIM, &SEP, &CL_DELIM, debug);
                    if(e_s->error > 0) {++error_state; free_error_state(e_s);}
                    else {params[i].l_p = e_s->p_data; free_error_state(e_s); continue;}
                    break;
                case U_LONG_P: e_s = scan_array(&src, &e_t, &OP_DELIM, &SEP, &CL_DELIM, debug);
                    if(e_s->error > 0) {++error_state; free_error_state(e_s);}
                    else {params[i].u_l_p = e_s->p_data; free_error_state(e_s); continue;}
                    break;
                case FLOAT_P: e_s = scan_array(&src, &e_t, &OP_DELIM, &SEP, &CL_DELIM, debug);
                    if(e_s->error > 0) {++error_state; free_error_state(e_s);}
                    else {params[i].f_p = e_s->p_data; free_error_state(e_s); continue;}
                    break;
                case DOUBLE_P: e_s = scan_array(&src, &e_t, &OP_DELIM, &SEP, &CL_DELIM, debug);
                    if(e_s->error > 0) {++error_state; free_error_state(e_s);}
                    else {params[i].d_p = e_s->p_data; free_error_state(e_s); continue;}
                    break;
                case LONG_DOUBLE_P: e_s = scan_array(&src, &e_t, &OP_DELIM, &SEP, &CL_DELIM, debug);
                    if(e_s->error > 0) {++error_state; free_error_state(e_s);}
                    else {params[i].ld_p = e_s->p_data; free_error_state(e_s); continue;}
                    break;
                case CHAR_P:
                case STRING: params[i].c_p = scan_string(&src);
                    if(params[i].c_p == NULL) {++error_state;}
                    else continue;
                    break;
                case INT:
                    if((n = sscanf(src, scanf_type_string(&e_t), &(params[i].i))) <= 0) {
                        print_scan_param_error(src, &e_t, i);
                        ++error_state;
                    } else if(debug || error_state > 0) printf("[%d] read %d\n", i, params[i].i);
                    break;
                case U_INT:
                    if((n = sscanf(src, scanf_type_string(&e_t), &(params[i].u_i))) <= 0) {
                        print_scan_param_error(src, &e_t, i);
                        ++error_state;
                    } else if(debug) printf("[%d] read %u\n", i, params[i].u_i);
                    break;
                case SHORT:
                    if((n = sscanf(src, scanf_type_string(&e_t), &(params[i].s))) <= 0) {
                        print_scan_param_error(src, &e_t, i);
                        ++error_state;
                    } else if(debug || error_state > 0) printf("[%d] read %hd\n", i, params[i].s);
                    break;
                case U_SHORT:
                    if((n = sscanf(src, scanf_type_string(&e_t), &(params[i].u_s))) <= 0) {
                        print_scan_param_error(src, &e_t, i);
                        ++error_state;
                    } else if(debug || error_state > 0) printf("[%d] read %hu\n", i, params[i].u_s);
                    break;
                case LONG:
                    if((n = sscanf(src, scanf_type_string(&e_t), &(params[i].l))) <= 0) {
                        print_scan_param_error(src, &e_t, i);
                        ++error_state;
                    } else if(debug || error_state > 0) printf("[%d] read %ld\n", i, params[i].l);
                    break;
                case U_LONG:
                    if((n = sscanf(src, scanf_type_string(&e_t), &(params[i].u_l))) <= 0) {
                        print_scan_param_error(src, &e_t, i);
                        ++error_state;
                    } else if(debug || error_state > 0) printf("[%d] read %lu\n", i, params[i].u_l);
                    break;
                case FLOAT:
                    if((n = sscanf(src, scanf_type_string(&e_t), &(params[i].f))) <= 0) {
                        print_scan_param_error(src, &e_t, i);
                        ++error_state;
                    } else if(debug || error_state > 0) printf("[%d] read %f\n", i, params[i].f);
                    break;
                case DOUBLE:
                    if((n = sscanf(src, scanf_type_string(&e_t), &(params[i].d))) <= 0) {
                        print_scan_param_error(src, &e_t, i);
                        ++error_state;
                    } else {
                        //params[i].d = strtod(src, &end);
                        //src = end;
                        if(debug || error_state > 0) {printf("[%d] read %.10lg", i, params[i].d);}//printf(" \"%s\" \n", end);
                        //continue;
                    }
                    break;
                case LONG_DOUBLE:
                    if((n = sscanf(src, scanf_type_string(&e_t), &(params[i].ld))) <= 0) {
                        print_scan_param_error(src, &e_t, i);
                        ++error_state;
                    } else if(debug || error_state > 0) printf("[%d] read %Lg\n", i, params[i].ld);
                    break;
                case CHAR:
                    if((n = sscanf(src, scanf_type_string(&e_t), &(params[i].c))) <= 0) {
                        print_scan_param_error(src, &e_t, i);
                        ++error_state;
                    } else if(debug || error_state > 0) printf("[%d] read %c\n", i, params[i].c);
                    break;
                //case UNKNOWN:
                //case UNKNOWN_P:
                default:
                    params[i].p = NULL;
                    ++error_state;
            };
            while(*src && !isspace(*(++src))); //eingelesenen Wert überspringen
            //printf("while(*src && !isspace(*(++src))) -> src=%s\n", src);
            while(*src && isspace(*(++src))); //zum nächsten Wert vorgehen
            //printf("while(*src && isspace(*(++src))) ->  src=%s\n", src);

            //printf("[2]wait for input...\n");
            //getchar();
        }
        if(error_state > 0) {
            free_t_value_array(params, f_h->parameter_n, f_h->parameter_types);
            return NULL;
        }
    }
    return params;
}

void print_scan_param_error(char* src, e_type* e_t, int i) {
    printf("ERROR: failed reading [%d] (", i);
    print_e_type(e_t, false, "");
    printf(") -> %s\n", src);
}

/**
 * Versucht ein Array von dem zu e_type passenden C-Basistypen aus der Zeichenkette src zu extrahieren und
 * zurückzugeben. Ist dies nicht möglich, wird NULL zurückgegeben.
 * Man gibt mit open_delim und close_delim die Begrenzungen für das Array an (z.B {}) und mit
 * separator das Trennzeichen der einzelnen Array-Elemente.
 * Bsp. für e_type=DOUBLE_P, open_delim='{', close_delim='}', separator=',' :
 *     {1.0, 2, 3.89}
 *     {} //es würde NULL zurückgegeben
 * Der Pointer auf den src zeigt wird hochgesetzt (hinter die eingelesene Zeichenkette).
 * Das zurückgegebene Array sollte, wenn nicht mehr gebraucht, mit free freigegeben werden.
 */
error_state* scan_array(char** src, e_type* e_t, const char* open_delim, const char* separator, const char* close_delim, bool debug) {
    t_value t_v;
    //char buffer[500];
    const char* scan_tv_s;
    int len, n; //, i
    void* values = NULL;
    char* sO = strchr(*src, *open_delim);
    if(debug)printf("sO=%s\n", sO);
    char* sE = NULL;
    int error_state = 0;
    if(sO != NULL && *sO) {
        sE = sO + 1;
        sE = strchr(sE, *close_delim);
        if(debug) printf("sO=%p - sE=%p, %c - %c\n", sO, sE, *sO, *sE);
        if(sE != NULL && *sE) {
            len = sE - sO - 1;
            if(debug) printf("len = %d\n", len);
            if(len > 0) {
                len = 1;
                char* si = sO;
                //Zählen wieviele Elemente
                while(*(++si) && si < sE) if(*si == *separator) ++len;
                if(debug) printf("Anzahl der Elemente: %d\n", len);
                values = get_type_array(e_t, len);
                scan_tv_s = scanf_type_string(e_t);
                if(debug) printf("scan_tv_s = \"%s\", sO = \"%s\", %c, %p - %p\n", scan_tv_s, sO, *(sO+1), sO+1, sE);
                int i = 0;
                char* end;
                bool find_sep = false;
                while(*(++sO) && sO < sE) {
                    if(find_sep && *sO != *separator) continue;
                    else if(*(++sO) && (len == 1 || sO < sE)) {
                        if(!find_sep) {
                            --sO;
                            find_sep = true;
                        }
                        while(isspace(*sO))++sO;
                        if(debug) printf("sscanf(\"%s\", \"%s\", %p)\n", sO, scan_tv_s, values + i);
                        //n = sscanf(sO, scan_tv_s, values + i++);
                        if(debug) printf("read [%d] ", i);
                        switch(*e_t) {
                            case INT_P: t_v.i_p = values;n = sscanf(sO, scan_tv_s, t_v.i_p + i++);
                                if(n > 0 && debug) print_void(t_v.i_p + i - 1, e_t, "", true);
                                break;
                            case U_INT_P: t_v.u_i_p = values;n = sscanf(sO, scan_tv_s, t_v.u_i_p + i++);
                                if(n > 0 && debug) print_void(t_v.u_i_p + i - 1, e_t, "", true);
                                break;
                            case SHORT_P: t_v.s_p = values;n = sscanf(sO, scan_tv_s, t_v.s_p + i++);
                                if(n > 0 && debug) print_void(t_v.s_p + i - 1, e_t, "", true);
                                break;
                            case U_SHORT_P: t_v.u_s_p = values;n = sscanf(sO, scan_tv_s, t_v.u_s_p + i++);
                                if(n > 0 && debug) print_void(t_v.u_s_p + i - 1, e_t, "", true);
                                break;
                            case LONG_P: t_v.l_p = values;n = sscanf(sO, scan_tv_s, t_v.l_p + i++);
                                if(n > 0 && debug) print_void(t_v.l_p + i - 1, e_t, "", true);
                                break;
                            case U_LONG_P: t_v.u_l_p = values;n = sscanf(sO, scan_tv_s, t_v.u_l_p + i++);
                                if(n > 0 && debug) print_void(t_v.u_l_p + i - 1, e_t, "", true);
                                break;
                            case FLOAT_P: t_v.f_p = values;n = sscanf(sO, scan_tv_s, t_v.f_p + i++);
                                if(n > 0 && debug) print_void(t_v.f_p + i - 1, e_t, "", true);
                                break;
                            case DOUBLE_P: t_v.d_p = values;n = sscanf(sO, scan_tv_s, t_v.d_p + i++);
                                if(n > 0) {
                                    t_v.d_p[i-1] = strtod(sO, &end);
                                    if(debug) print_void(t_v.d_p + i - 1, e_t, "", false);
                                    printf(" src=\"%s\"\n", end);
                                    sO = end - 1;
                                }
                                break;
                            case LONG_DOUBLE_P: t_v.ld_p = values;n = sscanf(sO, scan_tv_s, t_v.ld_p + i++);
                                if(n > 0 && debug) print_void(t_v.ld_p + i - 1, e_t, "", true);
                                break;
                            default: n = 0;
                        }
                        //printf("\n");
                        if(n <= 0) {
                            print_scan_param_error(*src, e_t, i-1);
                            ++error_state;
                        }
                    }
                }
            }
        }
    }
    if(sE != NULL && *sE) *src = ++sE;
    //printf("return %p ", values);
    //print_void(values, e_t, ", ");
    //printf("\n");
    if(error_state > 0) {
        free(values);
        values = NULL;
    }
    return new_error_state(error_state, values);
}

/**
 * Gibt für alle e_type Pointer-Typen (außer VOID_P, UNKNOWN_P) ein Array vom passenden
 * C-Basistypen zurück. Ist der e_type kein Pointer-Typ (oder VOID_P, UNKNOWN_P), wird
 * NULL zurückgegeben.
 * Das zurückgegebene Array sollte, wenn nicht mehr gebraucht, mit free freigegeben werden.
 */
void* get_type_array(e_type* e_t, int size) {
    switch(*e_t) {
        case INT_P: return malloc(sizeof(int) * size);
        case U_INT_P: return malloc(sizeof(unsigned int) * size);
        case SHORT_P: return malloc(sizeof(short) * size);
        case U_SHORT_P: return malloc(sizeof(unsigned short) * size);
        case LONG_P: return malloc(sizeof(long) * size);
        case U_LONG_P: return malloc(sizeof(unsigned long) * size);
        case FLOAT_P: return malloc(sizeof(float) * size);
        case DOUBLE_P: return malloc(sizeof(double) * size);
        case LONG_DOUBLE_P: return malloc(sizeof(long double) * size);
        case CHAR_P:
        case STRING: return malloc(sizeof(char) * size);
        default: return NULL;
    }
}

/**
 * Versucht einen String-Literal einzulesen. Es gibt zwei Formen:
 * 1. mit Anfuehrungszeichen: "..." //kann auch Leerzeichen(Whitespaces) und maskierte Zeichen enthalten
 * 2. ohne Anfuehrungszeichen: ab_de... //wird nur bis zum nächsten Whitespace-Zeichen eingelesen, maskierte Zeichen möglich
 * Der Pointer auf den src zeigt wird hochgesetzt (hinter die eingelesene Zeichenkette).
 * Es wird die eingelesene Zeichenkette zurückgegeben (ohne Anführungszeichen). Diese sollte, wenn nicht mehr
 * gebraucht mit  free(...) freigegeben werden.
 */
char* scan_string(char** src) {
    char buffer[500];
    char* sb = *src;
    char* se;
    //char end_c;
    int n = 0;
    while(isspace(*sb)) ++sb;
    if(*sb) {
        if(*sb == '"') {
            se = sb;
            while(*se && *(++se) != '"') {
                buffer[n++] = *se;
                if(*se == '\\') buffer[n++] = *(++se);
            }
        } else {
            se = sb;
            while(*se && !isspace(*se)) {
                buffer[n++] = *se;
                if(*se == '\\') buffer[n++] = *(++se);
                if(*se) ++se;
            }
        }
        buffer[n] = '\0';
    }
    //char* v = (char*)malloc((n == 0 ? 1 : n) * sizeof(char)); // !!! führt beim Freigeben zum Fehler !!!!
    char* v = (char*)malloc(((n == 0 ? 1 : n) + 1) * sizeof(char));
    //printf("[scan_string]: %s\n", strcpy(v, buffer));
    v = strcpy(v, buffer);
    //printf("[scan_string]: v[%p] =?= v2[%p]\n", v, v2);
    *src = (*se ? ++se : se);

    return v;
}

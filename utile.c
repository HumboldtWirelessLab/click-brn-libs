#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "c_source_reader.h"
#include "utile.h"

void print_f_header(f_header* f_h, char* indenting, bool nl) {
    //char rBO = 40;
    //char rBC = 41;
    if(f_h == NULL) {
        printf("%s---%s", indenting, (nl ? "\n" : ""));
        return;
    }
    print_e_type(&(f_h->return_type), f_h->return_is_ref_type, indenting);
    printf(" %s(", f_h->name);
    if(f_h->parameter_n == 0) printf("VOID)%s", (nl ? "\n" : ""));
    else {
        int n;
        for(n=0; n < f_h->parameter_n; ++n) {
            if(n > 0) printf(", ");
            print_e_type(f_h->parameter_types + n, false, "");
        }
        printf(")%s", (nl ? "\n" : ""));
    }
}

char * strrstr(const char *string, const char *find) {
    //I see the find in string when i print it
    //printf("%s", string);
    char *cp;
    int string_len = strlen(string);
    if(string_len == 0) return NULL;
    int find_len = strlen(find);
    if(find_len == 0 || find_len > string_len) return NULL;
    for (cp = (char *)(string + string_len - find_len); cp >= string; cp--) {
        if (strncmp(cp, find, find_len) == 0) return cp;
    }
    return NULL;
}

char* copy_string(const char* string) {
    if(string == NULL) return NULL;
    int len = strlen(string);
    char* copied = (char*) malloc(sizeof(char) * (len + 1));
    strcpy(copied, string);
    copied[len] = '\0';

    return copied;
}

char* create_spacer(char* s, char* to_append) {
    int len1 = strlen(s);
    int len2 = strlen(to_append);
    char* next_s = malloc((len1 + len2 + 1) * sizeof(char));
    strcpy(next_s, s);
    strcpy(next_s + len1, to_append);
    next_s[len1 + len2] = '\0';
    return next_s;
}

void print_string(char* s, char* indenting) {
    int len = strlen(s);
    char* si = s;
    char* si_end = s + len;
    if(len == 0) {
        printf("%s", indenting);
        return;
    }
    char buffer[len];
    do {
        sscanf(si, "%[^\n]", buffer);
        printf("%s%s\n", indenting, buffer);
        si = strchr(si, '\n');
        if(si == NULL) si = si_end;
    } while(si < si_end && *(*si ? ++si : si));
}

void print_sub_string(char* s, int start_pos, int length, char* indenting) {
    //char* to_print = (char*) malloc(sizeof(char) * (length + 1));
    char* si = s + start_pos;
    char* si_end = si + length;
    //strncpy(to_print, s + start_pos, length);
    //to_print[length] = '\0';
    if(length == 0) {
        printf("%s", indenting);
        return;
    }
    char buffer[length];
    do {
        sscanf(si, "%[^\n]", buffer);
        si = strchr(si, '\n');
        if(si > si_end) buffer[strlen(buffer) - (si - si_end)] = '\0';
        printf("%s%s\n", indenting, buffer);
        //if(si == NULL) si = si_end;
    } while(si < si_end && *(*si ? ++si : si));
    //free(to_print);
}

void print_e_type(e_type* e_t, bool is_ref_type, char* indenting) {
    static const char REF_TYPE[] = "[ref type]-";
    switch(*e_t) {
    case EMPTY: printf("%sEMPTY", indenting);return;
    case VOID: printf("%sVOID", indenting);return;
    case VOID_P: printf("%s%sVOID_P", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case INT: printf("%sINT", indenting);return;
    case INT_P: printf("%s%sINT_P", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case U_INT: printf("%sU_INT", indenting);return;
    case U_INT_P: printf("%s%sU_INT_P", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case SHORT: printf("%sSHORT", indenting);return;
    case SHORT_P: printf("%s%sSHORT_P", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case U_SHORT: printf("%sU_SHORT", indenting);return;
    case U_SHORT_P: printf("%s%sU_SHORT_P", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case LONG: printf("%sLONG", indenting);return;
    case LONG_P: printf("%s%sLONG_P", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case U_LONG: printf("%sU_LONG", indenting);return;
    case U_LONG_P: printf("%s%sU_LONG_P", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case FLOAT: printf("%sFLOAT", indenting);return;
    case FLOAT_P: printf("%s%sFLOAT_P", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case DOUBLE: printf("%sDOUBLE", indenting);return;
    case DOUBLE_P: printf("%s%sDOUBLE_P", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case LONG_DOUBLE: printf("%sLONG_DOUBLE", indenting);return;
    case LONG_DOUBLE_P: printf("%s%sLONG_DOUBLE_P", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case CHAR: printf("%sCHAR", indenting);return;
    case CHAR_P:
    case STRING: printf("%s%sSTRING", indenting, (is_ref_type ? REF_TYPE : ""));return;
    case UNKNOWN: printf("%sUNKNOWN", indenting);return;
    case UNKNOWN_P: printf("%sUNKNOWN_P", indenting);return;
    };
}

void print_t_value(t_value* t_v, int n, e_type* e_t, char* indenting) {
    if(t_v == NULL || e_t == NULL) {
        printf("* t_v == NULL !!\n");
        return;
    } else if(n <= 0) {
        printf("n == 0 !!\n");
        return;
    }
    const char* f_s;
    const char nl[] = "\n";
    int i;
    for(i=0; i < n; ++i) {
        f_s = printf_type_string(e_t + i, true);
        //printf("f_s = %s\n", f_s);
        printf("%s[%d] ", indenting, i);
        switch(e_t[i]) {
            //case EMPTY: printf("%sEMPTY", indenting);continue;
            //case VOID: printf("%sVOID", indenting);continue;
            case VOID_P: printf(f_s, "st_v.p = ", t_v[i].p, "", nl);continue;//printf("st_v.p = %p\n", t_v[i].p);continue;
            case INT: printf(f_s, "st_v.i = ", t_v[i].i, "", nl);continue;
                //printf("t_v.i = %d\n", t_v[i].i);continue;
            case INT_P:
                if(t_v[i].i_p == NULL) printf("t_v.i_p = NULL\n");
                else printf(f_s, "st_v.i_p = ", t_v[i].i_p[0], " ..", nl);
                //printf("t_v.i_p = %d...\n", t_v[i].i_p[0]);
                continue;
            case U_INT: printf("t_v.u_i = %u\n", t_v[i].u_i);continue;
            case U_INT_P:
                if(t_v[i].u_i_p == NULL) printf("t_v.u_i_p = NULL\n");
                else printf(f_s, "st_v.u_i_p = ", t_v[i].u_i_p[0], " ..", nl);
                //printf("t_v.u_i_p = %u...\n", t_v[i].u_i_p[0]);
                continue;
            case SHORT: printf(f_s, "st_v.s = ", t_v[i].s, "", nl);continue;
                //printf("t_v.s = %hd\n", t_v[i].s);continue;
            case SHORT_P:
                if(t_v[i].s_p == NULL) printf("t_v.s_p = NULL\n");
                else printf(f_s, "st_v.s_p = ", t_v[i].s_p[0], " ..", nl);
                //printf("t_v.s_p = %hd...\n", t_v[i].s_p[0]);
                continue;
            case U_SHORT: printf(f_s, "st_v.u_s = ", t_v[i].u_s, "", nl);continue;
                //printf("t_v.u_s = %hu\n", t_v[i].u_s);continue;
            case U_SHORT_P:
                if(t_v[i].u_s_p == NULL) printf("t_v.u_s_p = NULL\n");
                else printf(f_s, "st_v.u_s_p = ", t_v[i].u_s_p[0], " ..", nl);
                //printf("t_v.u_s_p = %hu...\n", t_v[i].u_s_p[0]);
                continue;
            case LONG: printf(f_s, "st_v.l = ", t_v[i].l, "", nl);continue;
                //printf("t_v.l = %ld\n", t_v[i].l);continue;
            case LONG_P:
                if(t_v[i].l_p == NULL) printf("t_v.l_p = NULL\n");
                else printf(f_s, "st_v.l_p = ", t_v[i].l_p[0], " ..", nl);
                //printf("t_v.l_p = %ld...\n", t_v[i].l_p[0]);
                continue;
            case U_LONG: printf(f_s, "st_v.u_l = ", t_v[i].u_l, "", nl);continue;
                //printf("t_v.u_l = %lu\n", t_v[i].u_l);continue;
            case U_LONG_P:
                if(t_v[i].u_l_p == NULL) printf("t_v.u_l_p = NULL\n");
                else printf(f_s, "st_v.u_l_p = ", t_v[i].u_l_p[0], " ..", nl);
                //printf("t_v.u_l_p = %lu...\n", t_v[i].u_l_p[0]);
                continue;
            case FLOAT: printf(f_s, "st_v.f = ", t_v[i].f, "", nl);continue;
                //printf("t_v.f = %f\n", t_v[i].f);continue;
            case FLOAT_P:
                if(t_v[i].f_p == NULL) printf("t_v.f_p = NULL\n");
                else printf(f_s, "st_v.f_p = ", t_v[i].f_p[0], " ..", nl);
                //printf("t_v.f_p = %f...\n", t_v[i].f_p[0]);continue;
                continue;
            case DOUBLE: printf(f_s, "st_v.d = ", t_v[i].d, "", nl);continue;
                //printf("t_v.d = %g\n", t_v[i].d);continue;
            case DOUBLE_P:
                if(t_v[i].d_p == NULL) printf("t_v.d_p = NULL\n");
                else printf(f_s, "st_v.d_p = ", t_v[i].d_p[0], " ..", nl);
                //printf("t_v.d_p = %g...\n", t_v[i].d_p[0]);
                continue;
            case LONG_DOUBLE: printf(f_s, "st_v.ld = ", t_v[i].ld, "", nl);continue;
                //printf("t_v.ld = %Lg\n", t_v[i].ld);continue;
            case LONG_DOUBLE_P:
                if(t_v[i].ld_p == NULL) printf("t_v.ld_p = NULL\n");
                else printf(f_s, "st_v.ld_p = ", t_v[i].ld_p[0], " ..", nl);
                //printf("t_v.ld_p = %Lg...\n", t_v[i].ld_p[0]);
                continue;
            case CHAR: printf("t_v.c = %c\n", t_v[i].c);continue;
            case CHAR_P:
            case STRING: printf("t_v.c_p = \"%s\"\n", t_v[i].c_p);continue;
            //case UNKNOWN: printf("%sUNKNOWN", indenting);continue;
            //case UNKNOWN_P: printf("%sUNKNOWN_P", indenting);continue;
            default: printf("???\n");
        }
    }
}


void print_void(void* v, e_type* e_t, char* indenting, bool with_nl) {
    if(v == NULL) {
        printf("* t == NULL !!\n");
        return;
    }
    t_value t_v;
    char nl[2];
    if(with_nl) {nl[0] = '\n'; nl[1] = '\0';}
    else nl[0] = '\0';
    const char* f_s = printf_type_string(e_t, true);
    if(strlen(indenting) > 0) printf("%s", indenting);
    switch(*e_t) {
        //case EMPTY:
        //case VOID:
        case VOID_P: printf(f_s, indenting, t_v.p, nl);break;
        case INT: t_v.i_p = v; printf(f_s, indenting, *(t_v.i_p), "", nl);break;
        case INT_P: t_v.i_p = v; printf(f_s, indenting, t_v.i_p[0], " ..", nl);break;
        case U_INT: t_v.u_i_p = v; printf(f_s, indenting, *(t_v.u_i_p), "", nl);break;
        case U_INT_P: t_v.u_i_p = v; printf(f_s, indenting, t_v.u_i_p[0], " ..", nl);break;
        case SHORT: t_v.s_p = v; printf(f_s, indenting, *(t_v.s_p), "", nl);break;
        case SHORT_P: t_v.s_p = v; printf(f_s, indenting, t_v.s_p[0], " ..", nl);break;
        case U_SHORT: t_v.u_s_p = v; printf(f_s, indenting, *(t_v.u_s_p), "", nl);break;
        case U_SHORT_P: t_v.u_s_p = v; printf(f_s, indenting, t_v.u_s_p[0], " ..", nl);break;
        case LONG: t_v.l_p = v; printf(f_s, indenting, *(t_v.l_p), "", nl);break;
        case LONG_P: t_v.l_p = v; printf(f_s, indenting, t_v.l_p[0], " ..", nl);break;
        case U_LONG: t_v.u_l_p = v; printf(f_s, indenting, *(t_v.u_l_p), "", nl);break;
        case U_LONG_P: t_v.u_l_p = v; printf(f_s, indenting, t_v.u_l_p[0], " ..", nl);break;
        case FLOAT: t_v.f_p = v; printf(f_s, indenting, *(t_v.f_p), "", nl);break;
        case FLOAT_P: t_v.f_p = v; printf(f_s, indenting, t_v.f_p[0], " ..", nl);break;
        case DOUBLE: t_v.d_p = v; printf(f_s, indenting, *(t_v.d_p), "", nl);break;
        case DOUBLE_P: t_v.d_p = v; printf(f_s, indenting, t_v.d_p[0], " ..", nl);break;
        case LONG_DOUBLE: t_v.ld_p = v; printf(f_s, indenting, *(t_v.ld_p), "", nl);break;
        case LONG_DOUBLE_P: t_v.ld_p = v; printf(f_s, indenting, t_v.ld_p[0], " ..", nl);break;
        case CHAR: t_v.c_p = v; printf(f_s, indenting, *(t_v.c_p), "", nl);break;
        case CHAR_P:
        case STRING: t_v.c_p = v; printf(f_s, indenting, t_v.c_p, "", nl);break;
        //case UNKNOWN:
        //case UNKNOWN_P:
        default: printf("???\n");
    }
}

/*const char* printf_type_string(e_type* e_t) {
    static const char per = '%';
    char nl[2];
    if(with_nl) {nl[0] = '\n'; nl[1] = '\0';}
    else nl[0] = '\0';
    switch(*e_t) {
        //case EMPTY: printf("%sEMPTY", indenting);return;
        //case VOID: printf("%sVOID", indenting);return;
        case VOID_P: sprintf(pf_str, "%cp%s", per, nl);return;
        case INT: sprintf(pf_str, "%cd%s", per, nl);return;
        case INT_P: sprintf(pf_str, "cd ..%s", per, nl);return;
        case U_INT: sprintf(pf_str, "%cu%s", per, nl);return;
        case U_INT_P: sprintf("%cu ..%s", &per, nl);return;
        case SHORT: sprintf(pf_str, "%chd%s", per, nl);return;
        case SHORT_P: sprintf(pf_str, "%chd ..%s", per, nl);return;
        case U_SHORT: sprintf(pf_str, "%chu%s", per, nl);return;
        case U_SHORT_P: sprintf(pf_str, "%chu ..%s", per, nl);return;
        case LONG: sprintf(pf_str, "%cld%s", per, nl);return;
        case LONG_P: sprintf(pf_str, "%cld ..%s", per, nl);return;
        case U_LONG: sprintf(pf_str, "%clu%s", per, nl);return;
        case U_LONG_P: sprintf(pf_str, "%clu ..%s", per, nl);return;
        case FLOAT: sprintf(pf_str, "%c.%df%s", per, decimal_place, nl);return;
        case FLOAT_P: sprintf(pf_str, "%c.%df ..%s", per, decimal_place, nl);return;
        case DOUBLE: sprintf(pf_str, "%c.%dlf%s", per, decimal_place, nl);return;
        case DOUBLE_P: sprintf(pf_str, "%c.%dlf ..%s", per, decimal_place, nl);return;
        case LONG_DOUBLE: sprintf(pf_str, "%c.%dLg%s", per, decimal_place, nl);return;
        case LONG_DOUBLE_P: sprintf(pf_str, "%c.%dLg ..%s", per, decimal_place, nl);return;
        case CHAR: sprintf(pf_str, "%cc%s", per, nl);return;
        case CHAR_P:
        case STRING: sprintf(pf_str, "%cs%s", per, nl);return;
        //case UNKNOWN: printf("%sUNKNOWN", indenting);return;
        //case UNKNOWN_P: printf("%sUNKNOWN_P", indenting);return;
        default: sprintf(pf_str, "???%s", nl);
    }
    return pf_str;
}*/

/**
 * Gibt den für e_type passenden Format-String für (s)scanf zurück.
 */
const char* scanf_type_string(e_type* e_t) {
    switch(*e_t) {
        case EMPTY:
        case VOID:
        case VOID_P: return NULL;
        case INT:
        case INT_P: return SCAN_TV_INT;
        case U_INT:
        case U_INT_P: return SCAN_TV_U_INT;
        case SHORT:
        case SHORT_P: return SCAN_TV_SHORT;
        case U_SHORT:
        case U_SHORT_P: return SCAN_TV_U_SHORT;
        case LONG:
        case LONG_P: return SCAN_TV_LONG;
        case U_LONG:
        case U_LONG_P: return SCAN_TV_U_LONG;
        case FLOAT:
        case FLOAT_P: return SCAN_TV_FLOAT;
        case DOUBLE:
        case DOUBLE_P: return SCAN_TV_DOUBLE;
        case LONG_DOUBLE:
        case LONG_DOUBLE_P: return SCAN_TV_LONG_DOUBLE;
        case CHAR: return SCAN_TV_CHAR;
        case CHAR_P:
        case STRING: return SCAN_TV_STRING;
        case UNKNOWN:
        case UNKNOWN_P:
        default: return NULL;
    };
}

/**
 * Gibt den für e_type passenden Format-String für (s)printf zurück.
 * Bsp:
 *    int i = 2;
 *    char indenting[] = "   ";
 *    char extra[] = " some extra stuff";
 *    char nl[] = "\n";
 *    const char* f_s = printf_type_string(&INT, true);
 *    printf(f_s, indenting, i, extra, nl);
 *
 *    f_s = printf_type_string(&INT, false);
 *    printf(f_s, i, extra);
 */
const char* printf_type_string(e_type* e_t, bool with_indenting_nl) {
    switch(*e_t) {
        case EMPTY:
        case VOID:
        case VOID_P: return NULL;
        case INT:
        case INT_P: return (with_indenting_nl ? PRINT_TV_INT_INDENTING_NL : PRINT_TV_INT);
        case U_INT:
        case U_INT_P: return (with_indenting_nl ? PRINT_TV_U_INT_INDENTING_NL : PRINT_TV_U_INT);
        case SHORT:
        case SHORT_P: return (with_indenting_nl ? PRINT_TV_SHORT_INDENTING_NL : PRINT_TV_SHORT);
        case U_SHORT:
        case U_SHORT_P: return (with_indenting_nl ? PRINT_TV_U_SHORT_INDENTING_NL : PRINT_TV_U_SHORT);
        case LONG:
        case LONG_P: return (with_indenting_nl ? PRINT_TV_LONG_INDENTING_NL : PRINT_TV_LONG);
        case U_LONG:
        case U_LONG_P: return (with_indenting_nl ? PRINT_TV_U_LONG_INDENTING_NL : PRINT_TV_U_LONG);
        case FLOAT:
        case FLOAT_P: return (with_indenting_nl ? PRINT_TV_FLOAT_INDENTING_NL : PRINT_TV_FLOAT);
        case DOUBLE:
        case DOUBLE_P: return (with_indenting_nl ? PRINT_TV_DOUBLE_INDENTING_NL : PRINT_TV_DOUBLE);
        case LONG_DOUBLE:
        case LONG_DOUBLE_P: return (with_indenting_nl ? PRINT_TV_LONG_DOUBLE_INDENTING_NL : PRINT_TV_LONG_DOUBLE);
        case CHAR: return (with_indenting_nl ? PRINT_TV_CHAR_INDENTING_NL : PRINT_TV_CHAR);
        case CHAR_P:
        case STRING: return (with_indenting_nl ? PRINT_TV_STRING_INDENTING_NL : PRINT_TV_STRING);
        case UNKNOWN:
        case UNKNOWN_P:
        default: return NULL;
    };
}

void print_src_code(src_code* s_c, char* prefix, char* indenting) {
    //char rBO = 40;
    //char rBC = 41;
    //printf("%s%ssrc_code%clines = %d, length = %d%c[\n", indenting, prefix, rBO, s_c->lines, s_c->length, rBC);
    printf("%s%ssrc_code(clines = %d, length = %d)[\n", indenting, prefix, s_c->lines, s_c->length);
    char* next_indenting = create_spacer(indenting, "   ");
    print_string(s_c->src, next_indenting);
    printf("%s]\n", indenting);
    free(next_indenting);
}

void print_src_code_ref(src_code_ref* src_r, char* prefix, char* indenting) {
    printf("%s%ssrc_code_ref(clines = %d, length = %d)[\n", indenting, prefix, src_r->lines, src_r->length);
    char* next_indenting = create_spacer(indenting, "   ");
    print_sub_string((src_r->s_c)->src, src_r->src_start_pos, src_r->length, next_indenting);
    printf("%s]\n", indenting);
    free(next_indenting);
}

void print_src_function(src_function* s_f, char* indenting) {
    //char cBO = 123;
    //char cBC = 125;
    printf("%ssrc_function [", indenting);
    f_header* f_h = s_f->header;
    print_f_header(f_h, "", false);
    printf("] *** {\n");
    char* next_indenting = create_spacer(indenting, "   ");
    print_src_code_ref(s_f->src_r, "function->", next_indenting);
    print_src_code_ref(f_h->src_r, "header->", next_indenting);
    printf("%s} ***\n", indenting);
    free(next_indenting);
}

void print_reduced_src_function(src_function* s_f, char* indenting) {
    f_header* f_h = s_f->header;
    src_code_ref* src_r = f_h->src_r;
    char* to_print = (char*) malloc(sizeof(char) * (src_r->length + 1));
    strncpy(to_print, (src_r->s_c)->src + src_r->src_start_pos, src_r->length);
    to_print[src_r->length] = '\0';
    printf("%s%s <---> ", indenting, to_print);
    print_f_header(f_h, indenting, true);
    free(to_print);
}

void print_functions_ht(map_t ht) {
    hashmap_map* h_m = (hashmap_map *) ht;
    hashmap_element* h_e;
    src_function* s_f;
    int i, n;
    for(i=0, n=0; i < h_m->table_size; ++i) {
        if(h_m->data[i].in_use) {
            h_e = h_m->data + i;
            s_f = (src_function*)(h_e->data);
            //print_reduced_src_function(s_f, "   ");
            printf("[%d] ", n++);
            print_f_header(s_f->header, "", true);
        }
    }
}

unsigned int exists_file(char* file) {
    FILE* pfilehandle;
    if((pfilehandle = fopen(file, "r")) == NULL)  {
        return false;
    }
    fclose(pfilehandle);
    return true;
}

error_state* new_error_state(int error, void* p_data) {
    error_state* e_s = malloc(sizeof(error_state));
    e_s->error = error;
    e_s->p_data = p_data;
    return e_s;
}

void free_error_state(error_state* e_s) {
    free(e_s);
}

bool writeToFile(char* f_name, char* content) {
    FILE* f = fopen(f_name, "w");
    if(f == NULL) return false;
    fprintf(f, "%s", content);
    fclose(f);
    return true;
}

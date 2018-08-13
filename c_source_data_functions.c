#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "c_source_data_functions.h"
#include "utile.h"

src_code* create_src_code(int initial_src_length) {
    src_code* s_c = (src_code*) malloc(sizeof(src_code));
    s_c->length = 0;
    s_c->lines = 0;
    s_c->src = (char*)malloc(sizeof(char)*(initial_src_length + 1));

    return s_c;
}

src_code* create_src_code2(char* src) {
    src_code* s_c = (src_code*) malloc(sizeof(src_code));
    s_c->src = src;
    s_c->length = strlen(src);
    s_c->lines = s_c->length > 0 ? 1 : 0;
    while((src = strchr(++src, '\n'))) s_c->lines += 1;

    //print_src_code(s_c);

    return s_c;
}

src_code_ref* create_src_ref(src_code* s_c, int src_start_pos, int length) {
    src_code_ref* src_r = (src_code_ref*) malloc(sizeof(src_code_ref));
    src_r->s_c = s_c;
    src_r->src_start_pos = src_start_pos;
    src_r->length = length;
    src_r->lines = src_r->length > 0 ? 1 : 0;
    char* src = (src_r->s_c)->src + src_start_pos;
    char* src_end = src + length;
    //printf("create_src_ref[src=%p, src_start=%p, src_end=%p, src_start_pos=%d, length=%d]\n", (src_r->s_c)->src, src, src_end, src_start_pos, length);
    while(src < src_end && (src = strchr(++src, '\n'))) if(src < src_end) src_r->lines += 1;

    return src_r;
}

/*f_header* create_f_header(int initial_src_length) {
    f_header* f_h = (f_header*) malloc(sizeof(f_header));
    //f_h->src = create_src_code(initial_src_length);
    f_h->name = NULL;
    f_h->return_type = VOID;
    f_h->parameter_types = NULL;
    f_h->parameter_n = 0;
    f_h->has_unkown_types = 0;

    return f_h;
}*/

//f_header* create_f_header2(char * src) {
f_header* create_f_header(src_code_ref* src_r) {
    f_header* f_h = (f_header*) malloc(sizeof(f_header));
    //f_h->src = create_src_code2(src);
    f_h->src_r = src_r;
    f_h->name = NULL;
    f_h->return_type = VOID;
    f_h->parameter_types = NULL;
    f_h->parameter_n = 0;
    f_h->has_unkown_types = 0;

    return f_h;
}

tcc_state* create_tcc_state() {
    tcc_state* state = (tcc_state*) malloc(sizeof(tcc_state));
    state->f_count = 0;
    state->s = NULL;

    return state;
}

//src_function* create_src_function(char* src, f_header* header) {
src_function* create_src_function(src_code_ref* src_r, f_header* header) {
    src_function* s_f = (src_function*) malloc(sizeof(src_function));
    s_f->header = header;
    //s_f->src = create_src_code2(src);
    s_f->src_r = src_r;
    s_f->state = NULL;
    s_f->func = NULL;

    return s_f;
}

src* create_src(int initial_src_length, int initial_function_count) {
    src* c_src = (src*) malloc(sizeof(src));
    c_src->name = NULL;
    c_src->src = create_src_code(initial_src_length);
    c_src->src_start_pos = 0;
    c_src->n = 0;
    c_src->allocated_n = initial_function_count;
    c_src->functions = malloc(initial_function_count * sizeof(src_function*));
    //c_src->functions = (src_function*) malloc(initial_function_count * sizeof(src_function));

    return c_src;
}

char* get_name(src_function* s_f) {
    return (s_f->header)->name;
}

e_type get_return_type(src_function* s_f) {
    return (s_f->header)->return_type;
}

e_type* get_parameter_types(src_function* s_f) {
    return (s_f->header)->parameter_types;
}

bool has_unkown_types(src_function* s_f) {
    return (s_f->header)->has_unkown_types > 0;
}

void free_src_code(src_code* s_c, bool debug) {
    if(s_c == NULL) return;
    if(debug) {
        free_src_code_debug(s_c, "");
    } else {
        free(s_c->src);
        free(s_c);
    }
}

void free_src_code_debug(src_code* s_c, char* prefix) {
    printf("%sfree_src_code [src]\n", prefix);
    free(s_c->src);
    printf("%sfree_src_code [self]\n", prefix);
    free(s_c);
    printf("%sfree_src_code finished\n", prefix);
}

void free_header(f_header* header, bool debug) {
    if(header == NULL) return;
    if(debug) {
        free_header_debug(header, "");
    } else {
        if(header->src_r == NULL) printf("free_header header->src_r == NULL !!!");
        else {
            //src_code*  s_c = header->src;
            //free_src_code(header->src, debug);
            free(header->src_r);
        }
        if(header->name != NULL) free(header->name);
        if(header->parameter_types != NULL) free(header->parameter_types);
        free(header);
    }
}

void free_header_debug(f_header* header , char* prefix) {
    printf("%sfree_header [%s]\n", prefix, (header == NULL ? "--" : header->name));
    if(header == NULL) return;

    char * prefix_next  = create_spacer(prefix, "   ");
    printf("%sfree_header [src_code]\n", prefix);
    //free_src_code_debug(header->src, prefix_next);
    if(header->src_r != NULL) free(header->src_r);
    printf("%sfree_header [name]\n", prefix);
    if(header->name != NULL) free(header->name);
    printf("%sfree_header [parameter_types]\n", prefix);
    if(header->parameter_types != NULL) free(header->parameter_types);
    printf("%sfree_header [self]\n", prefix);
    free(header);
    printf("%sfree_header finished\n", prefix);
    free(prefix_next);
}

void free_src_function(src_function* s_f, bool debug) {
    if(s_f == NULL) return;
    if(debug) {
        free_src_function_debug(s_f, "");
    } else {
        free_header(s_f->header, debug);
        //free_src_code(s_f->src, debug);
        if(s_f->src_r != NULL) free(s_f->src_r);
        if(s_f->func != NULL) {
            if(s_f->state != NULL) {
                tcc_state* state = s_f->state;
                if(state->f_count <= 1) {
                    //printf("free func [free tcc_state]\n");
                    free(state->s);
                    free(state);
                } else
                    state->f_count--;
            }
            //free(s_f->func); //!!! causes error !!!
            s_f->func = NULL;
        }
        free(s_f);
    }
}

void free_src_function_debug(src_function* s_f, char* prefix) {
    printf("%sfree_func [", prefix);
    print_f_header(s_f->header, "", false);
    printf("]\n");
    if(s_f == NULL) return;

    char * prefix_next  = create_spacer(prefix, "   ");
    printf("%sfree_func [f_header]\n", prefix);
    free_header_debug(s_f->header, prefix_next);
    printf("%sfree_func [src_code]\n", prefix);
    //free_src_code_debug(s_f->src, prefix_next);
    if(s_f->src_r != NULL) free(s_f->src_r);
    printf("%sfree_func [function pointer]\n", prefix);
    if(s_f->func != NULL) {
        printf("%sfree_func [proove tcc_state]\n", prefix);
        if(s_f->state != NULL) {
            tcc_state* state = s_f->state;
            printf("%sfree_func [tcc_state not NULL, f_count = %d]\n", prefix, state->f_count);
            if(state->f_count <= 1) {
                printf("%sfree func [free tcc_state]\n", prefix);
                free(state->s);
                free(state);
            } else {
                state->f_count--;
            }
        }
        //printf("%sfree_func [free function pointer]\n", prefix);
        //free(s_f->func); //!!! causes error !!!
        s_f->func = NULL;
    }
    printf("%sfree_func [self]\n", prefix);
    free(s_f);
    printf("%sfree_func finished\n", prefix);
    free(prefix_next);
}

void free_src(src* c_src, bool all, bool debug) {
    if(c_src == NULL) return;
    if(debug) {
        free_src_debug(c_src, "", all);
    } else {
        if(c_src->name != NULL) free(c_src->name);
        free_src_code(c_src->src, debug);
        int i=0;
        //src_function* s_f;
        if(all) {
            for(; i<c_src->n; ++i) free_src_function(c_src->functions[i], debug);
        }
        free(c_src->functions);
        free(c_src);
    }
}

void free_src_debug(src* c_src, char* prefix, bool all) {
    printf("%sfree_src start\n", prefix);
    if(c_src == NULL) return;

    char * prefix_next  = create_spacer(prefix, "   ");
    printf("%sfree_src [name]\n", prefix);
    if(c_src->name != NULL) free(c_src->name);
    free_src_code_debug(c_src->src, prefix_next);
    printf("%sfree_src [all functions]\n", prefix);
    unsigned int i;
    for(i=0; i<c_src->n; ++i) {
        printf("%sfree_src [c_src->functions[%d], n=%d]\n", prefix, i, c_src->n);
        free_src_function_debug(c_src->functions[i], prefix_next);
    }
    printf("%sfree_src [c_src->functions]\n", prefix);
    free(c_src->functions);
    printf("%sfree_src [self]\n", prefix);
    free(c_src);
    printf("%sfree_src finished\n", prefix);
    free(prefix_next);
}

void free_functions_ht(map_t ht, bool debug) {
    hashmap_map* h_m = (hashmap_map *) ht;
    hashmap_element* h_e;
    int i;
    for(i=0; i < h_m->table_size; ++i) {
        if(h_m->data[i].in_use) {
            h_e = h_m->data + i;
            free_src_function((src_function*)(h_e->data), debug);
        }
    }
    hashmap_free(ht);
}

/**
 * all - Parameter für free_src(.., all, ..), ob die src_function Structs auch freigegeben werden sollen.
 */
void free_src_ht(map_t ht, bool all, bool debug) {
    hashmap_map* h_m = (hashmap_map *) ht;
    hashmap_element* h_e;
    int i;
    for(i=0; i < h_m->table_size; ++i) {
        if(h_m->data[i].in_use) {
            h_e = h_m->data + i;
            free_src((src*)(h_e->data), all, debug);
        }
    }
    hashmap_free(ht);
}

void free_t_value_array(t_value* params, int n, e_type* e_t) {
    if(params == NULL || e_t == NULL) return;
    //f_header* f_h = (s_f != NULL ? s_f->header : NULL);
    //int n = (f_h == NULL ? 1 : f_h->parameter_n);
    if(n > 0) {
        void* v;
        int i;
        for(i=0; i < n; ++i) {
            //printf("[%d] wait for input...\n", i);
            //getchar();
            switch(e_t[i]) {
                case VOID_P: v = params[i].p;break;
                case INT_P: v = params[i].i_p;break;
                case U_INT_P: v = params[i].u_i_p;break;
                case SHORT_P: v = params[i].s_p;break;
                case U_SHORT_P: v = params[i].u_s_p;break;
                case LONG_P: v = params[i].l_p;break;
                case U_LONG_P: v = params[i].u_l_p;break;
                case FLOAT_P: v = params[i].f_p;break;
                case DOUBLE_P: v = params[i].d_p;break;
                case LONG_DOUBLE_P: v = params[i].ld_p;break;
                case CHAR_P:
                case STRING:
                    v = params[i].c_p;
                    break;
                case EMPTY: //sollte nicht eintreten !!!
                case INT:
                case U_INT:
                case SHORT:
                case U_SHORT:
                case LONG:
                case U_LONG:
                case FLOAT:
                case DOUBLE:
                case LONG_DOUBLE:
                case UNKNOWN:
                case UNKNOWN_P:
                default: v = NULL;
            };
            //printf("[%d]v == NULL ? %s\n", i, (v == NULL ? "true" : "false"));
            if(v != NULL) free(v);
            //printf("[%d] free(v) done\n", i);
        }
    }
    //printf("[next free(params)] wait for input...\n");
    //getchar();
    free(params);
}

void free_t_value(t_value* param, e_type* e_t, bool free_self) {
    if(param == NULL || e_t == NULL) return;

    switch(*e_t) {
        case VOID_P: free(param->p);break;
        case INT_P: free(param->i_p);break;
        case U_INT_P: free(param->u_i_p);break;
        case SHORT_P: free(param->s_p);break;
        case U_SHORT_P: free(param->u_s_p);break;
        case LONG_P: free(param->l_p);break;
        case U_LONG_P: free(param->u_l_p);break;
        case FLOAT_P: free(param->f_p);break;
        case DOUBLE_P: free(param->d_p);break;
        case LONG_DOUBLE_P: free(param->ld_p);break;
        case CHAR_P:
        case STRING:
            if(param->c_p != NULL) {
                printf("[free_t_value] free string %s\n", param->c_p);
                free(param->c_p);break;
            }
        case EMPTY: //sollte nicht eintreten !!!
        case INT:
        case U_INT:
        case SHORT:
        case U_SHORT:
        case LONG:
        case U_LONG:
        case FLOAT:
        case DOUBLE:
        case LONG_DOUBLE:
        case UNKNOWN:
        case UNKNOWN_P:
        default: break;
    }

    if(free_self) {
        printf("[free_t_value] free itself\n");
        free(param);
    }
}

//void free_wrapper_function_values(t_value* params, int n, e_type* e_t_params, t_value* return_v, e_type* e_t_return) {
//    for(int i=0; i<n; ++i) {
//        if(e_t_params[i] == *e_t_return) {
//
//        }
//    }
//}

bool e_type_is_pointer(e_type* e_t) {
    switch(*e_t) {
        case VOID_P:
        case INT_P:
        case U_INT_P:
        case SHORT_P:
        case U_SHORT_P:
        case LONG_P:
        case U_LONG_P:
        case FLOAT_P:
        case DOUBLE_P:
        case LONG_DOUBLE_P:
        case CHAR_P:
        case STRING: return true;
        //case UNKNOWN_P:
        default: return false;
    };
}

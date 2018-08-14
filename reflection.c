#include <stdlib.h>
//#include <libtcc.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "reflection.h"
#include "c_source_data_functions.h"
#include "utile.h"

static const char sReadLine[] =
"#include <tcclib.h>\n" /* include the "Simple libc header for TCC" */
"//include <stdlib.h>\n"
"#ifdef _WIN32\n" /* dynamically linked data needs 'dllimport' */
" __attribute__((dllimport))\n"
"#endif\n"
"char * readLine(FILE* my_stdin) {\n"
"    char * line = malloc(100), * linep = line;\n"
"    size_t lenmax = 100, len = lenmax;\n"
"    int c;\n"
"\n"
"    if(line == NULL)\n"
"        return NULL;\n"
"\n"
"    for(;;) {\n"
"        c = fgetc(my_stdin);\n"
"        if(c == EOF)\n"
"            break;\n"
"\n"
"        if(--len == 0) {\n"
"            len = lenmax;\n"
"            char * linen = realloc(linep, lenmax *= 2);\n"
"\n"
"            if(linen == NULL) {\n"
"                free(linep);\n"
"                return NULL;\n"
"            }\n"
"            line = linen + (line - linep);\n"
"            linep = linen;\n"
"        }\n"
"        if(c == '\\n') break;\n"
"        *line++ = c;\n"
"    }\n"
"    *line = '\\0';\n"
"    return linep;\n"
"}\n";

tcc_function* load_func(const char* src, char* f_name) {
    tcc_function* func = malloc(sizeof(tcc_function));
    func->state.f_count = 0;
    func->state.s = NULL;
    //TCCState *s = NULL;
    //void* func;

    //s = tcc_new();
    func->state.s = tcc_new();
    if (!func->state.s) {
        fprintf(stderr, "Could not create tcc state\n");
        exit(1);
    }

    /* if tcclib.h and libtcc1.a are not installed, where can we find them */
    /*for (i = 1; i < argc; ++i) {
        char *a = argv[i];
        if (a[0] == '-') {
            if (a[1] == 'B')
                tcc_set_lib_path(s, a+2);
            else if (a[1] == 'I')
                tcc_add_include_path(s, a+2);
            else if (a[1] == 'L')
                tcc_add_library_path(s, a+2);
        }
    }*/

    /* MUST BE CALLED before any compilation */
    tcc_set_output_type(func->state.s, TCC_OUTPUT_MEMORY);

    if (tcc_compile_string(func->state.s, src) == -1) {
        printf("ERROR: tcc_compile_string(s, sReadLine) == -1\n");
        tcc_delete(func->state.s);
        return NULL;
    }

    /* relocate the code */
    if (tcc_relocate(func->state.s, TCC_RELOCATE_AUTO) < 0) {
        printf("ERROR: tcc_relocate(s, TCC_RELOCATE_AUTO) < 0\n");
        tcc_delete(func->state.s);
        return NULL;
    }

    // get entry symbol
    func->f = tcc_get_symbol(func->state.s, f_name);
    if (!func->f) {
        printf("ERROR: tcc_get_symbol(s, \"%s\") failed !!!\n", f_name);
        tcc_delete(func->state.s);
        return NULL;
    }
    func->state.f_count++;
    // delete the state
    //tcc_delete(func->s);

    return func;
}

void free_tcc_function(tcc_function* func) {
    if(func->state.f_count <= 1) tcc_delete(func->state.s);
    else func->state.f_count--;
    free(func);
}

void reflection_test() {
    char s_c[] =
    "#include <tcclib.h>\n" /* include the "Simple libc header for TCC" */
    "//include <stdlib.h>\n"
    "#ifdef _WIN32\n" /* dynamically linked data needs 'dllimport' */
    " __attribute__((dllimport))\n"
    "#endif\n"
    "#define roedel \"roedeln\"\n"
    "void t_echo(char* s) {printf(\"\\n*echo: %s\\n\", s);}\n"
    "void do_something() {\n"
    "   char s[30];\n"
    "   sprintf(s, \"%s .. %s...\", roedel, roedel);\n"
    "   t_echo(s);\n}\n";

    printf("%s\n", s_c);
    tcc_function* func = load_func(s_c, "do_something");
    void (*do_it)(void) = func->f;

    do_it();

    free_tcc_function(func);
}

tcc_function* load_readLine(void) {

    tcc_function* func = load_func(sReadLine, "readLine");
    //char* (*readLine)(FILE*) = load_func(sReadLine, "readLine");

    return func;
}

t_value (*load_function(src_function* s_f))(t_value*) {
    t_value (*func)(t_value*) = NULL;
    return func;
}

int load_functions(src* c_src, bool pToCmd, bool debug) {
    static const char WRAPPER_PREFIX[] = "f_wrapper_";
    static const int W_LEN = 10;
    if(c_src != NULL) {
        if(c_src->n > 0) {
            char** wrapper_names = malloc(sizeof(char*) * c_src->n);
            src_function** functions = malloc(sizeof(src_function*) * c_src->n);
            int added_functions = 0;
            char* f_w;
            int tcc_inc_len = strlen(TCC_INCLUDES);
            int src_max_len = c_src->src->length * 3 + tcc_inc_len;
            if(debug) printf("[a] Allocate src_new with size %d\n", (src_max_len));
            char* src_new = (char*) malloc(sizeof(char) * src_max_len);
            char* si;
            int i, n, len, len_ak = c_src->src->length + tcc_inc_len, fail_count = 0;

            strcpy(src_new, TCC_INCLUDES);
            strcpy(src_new + tcc_inc_len , c_src->src->src);
            if(debug) {
                printf("[b] src_new.length = %d, len_ak = %d\n",(int)strlen(src_new), len_ak);
                print_string(src_new, "[c]   ");
            }
            si = src_new + len_ak;

            //src_code* s_c = c_src->src;
            src_function* s_f;
            f_header* f_h;
            for(i=0, n=0; i<c_src->n; ++i) {
                if(has_unkown_types(c_src->functions[i]) == false) {
                    s_f = c_src->functions[i];
                    functions[added_functions] = s_f;
                    f_h = s_f->header;
                    //hashmap_put(ht, f_h->name, c_src->functions[i]);
                    if(pToCmd) {
                        printf("[0] Added[%d:%d]: %s", i, ++n, (debug ? "\n" : ""));
                        if(debug) print_src_function(c_src->functions[i], "   ");
                        else print_reduced_src_function(c_src->functions[i], "");
                    }
                    len = W_LEN + strlen(f_h->name);
                    wrapper_names[added_functions] = malloc(sizeof(char) * (len + 1));
                    //si = wrapper_names[added_functions];
                    strcpy(wrapper_names[added_functions], WRAPPER_PREFIX);
                    strcpy(wrapper_names[added_functions] + W_LEN, f_h->name);
                    wrapper_names[added_functions][len] = '\0';

                    f_w = create_function_wrapper(wrapper_names[added_functions], s_f);
                    //si = src_new + len_ak;
                    len = strlen(f_w);
                    //printf("[1] %p - %p\n", (si + len), (src_new + src_max_len));
                    if(si + len >= src_new + src_max_len) {
                        if(debug) printf("[1.1] src_new has to be resized [%d -> %d]\n", src_max_len, (src_max_len + src_max_len));
                        si[0] = '\0';
                        src_max_len += src_max_len; //c_src->src->length;
                        src_new = realloc(src_new, sizeof(char) * src_max_len);
                        si = src_new + strlen(src_new);
                    }
                    strcpy(si, f_w);
                    len_ak += len + 1;
                    si += len;
                    *(si++) = '\n';
                    *(si) = '\0';
                    ++added_functions;
                    //printf("[2] len_ak = %d, len = %d\n", len_ak, len);
                    free(f_w);
                } else {
                    //printf("Skipped[%d]: %s", i, (user_debug ? "\n" : ""));
                    //if(user_debug) print_src_function(c_src->functions[i], "   ");
                    //else print_reduced_src_function(c_src->functions[i], "");
                    if(pToCmd) {
                        printf("Skipped[%d]: \n", i);
                        print_src_function(c_src->functions[i], "   ");
                    }
                }
            }
            si[0] = '\0';

            if(debug) {
                print_string(src_new, "   ");
                //if(!writeToFile("obj/generated.c", src_new)) printf("ERROR: Could not write to file obj/generated.c !!!\n");
                printf("\n");
            }

            //if(true) {
            //    free(src_new);
            //    return -1;
            //}

            if(added_functions > 0) {
                TCCState *s = tcc_new();

                if (!s) {
                    fprintf(stderr, "Could not create tcc state\n");
                    return -1;
                }

                tcc_state* state = create_tcc_state();
                state->s = s;

                /* MUST BE CALLED before any compilation */
                tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

                if (tcc_compile_string(s, src_new) == -1) {
                    if(pToCmd) printf("ERROR: tcc_compile_string(s, src) == -1\n");
                    tcc_delete(s);
                    free(state);
                    free(src_new);
                    return -1;
                }

                /* relocate the code */
                if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0) {
                    if(pToCmd) printf("ERROR: tcc_relocate(s, TCC_RELOCATE_AUTO) < 0\n");
                    tcc_delete(s);
                    free(state);
                    free(src_new);
                    return -1;
                }

                for(n = 0; n<added_functions; ++n) {
                    s_f = functions[n];
                    // get entry symbol
                    s_f->func = tcc_get_symbol(s, wrapper_names[n]);
                    if (!s_f->func) {
                        if(pToCmd) printf("ERROR: tcc_get_symbol(s, \"%s\") failed !!!\n", wrapper_names[n]);
                        free(wrapper_names[n]);
                        ++fail_count;
                        continue;
                    }
                    s_f->c_src = (struct src*)c_src;
                    state->f_count++;
                    s_f->state = state;
                    if(pToCmd) printf("[%d] loaded %s (%s) successfully!\n", n, s_f->header->name, wrapper_names[n]);
                    free(wrapper_names[n]);
                }

                free(wrapper_names);
                free(functions);
                if(fail_count == added_functions) {
                    tcc_delete(s);
                    free(state);
                    free(src_new);
                    return -fail_count;
                }
                free(src_new);
                return added_functions;
            }
            free(src_new);
        }
    }
    return 0;
}

char* create_function_wrapper(char* wrapper_name, src_function* s_f) {
    char* wrapper;
    char buffer[1000];
    char part[500];
    char* bi = buffer;
    f_header* f_h = s_f->header;

    /*char wheader[100];
    sprintf(wheader, "t_value %s(t_value* param) {\nt_value result;\n", wrapper_name);
    sprintf(bi, wheader);
    bi += strlen(wheader);*/

    sprintf(bi,"t_value %s(t_value* param) {\nt_value result;\n", wrapper_name);
    bi += strlen(bi);
    //return type void
    if(f_h->return_type == VOID) {
        //printf("return type == VOID\n");
        sprintf(bi, "result.p = NULL;\n%s(", f_h->name);
        bi = buffer + strlen(buffer);
    } else {
        sprint_t_value_var(&(f_h->return_type), part, -1, "result", false);
        sprintf(bi, "%s = %s%s(", part, (f_h->return_type == UNKNOWN ? "&(" : ""), f_h->name);
        bi = buffer + strlen(buffer);
    }
    if(f_h->parameter_n > 0) {
        int n;
        for(n=0; n < f_h->parameter_n; ++n) {
            sprint_t_value_var(f_h->parameter_types + n, part, n, "param", true);
            sprintf(bi, "%s", part);
            bi += strlen(part);
        }
    }
    sprintf(bi, ");\nreturn result;\n}");

    int len = strlen(buffer);
    wrapper = malloc((len + 1) * sizeof(char));
    strcpy(wrapper, buffer);
    wrapper[len] = '\0';

    return wrapper;
}

void sprint_t_value_var(e_type* e_t, char* dest, int index, char* var_name, bool is_array) {
    bool is_value_of_p = false;
    char tv_name[6];
    switch(*e_t) {
        case EMPTY:
        case VOID:
        case VOID_P: sprintf(tv_name, "%s", TV_VOID_P);break;
        case INT: sprintf(tv_name, "%s", TV_INT);break;
        case INT_P: sprintf(tv_name, "%s", TV_INT_P);break;
        case U_INT: sprintf(tv_name, "%s", TV_U_INT);break;
        case U_INT_P: sprintf(tv_name, "%s", TV_U_INT_P);break;
        case SHORT: sprintf(tv_name, "%s", TV_SHORT);break;
        case SHORT_P: sprintf(tv_name, "%s", TV_SHORT_P);break;
        case U_SHORT: sprintf(tv_name, "%s", TV_U_SHORT);break;
        case U_SHORT_P: sprintf(tv_name, "%s", TV_U_SHORT_P);break;
        case LONG: sprintf(tv_name, "%s", TV_LONG);break;
        case LONG_P: sprintf(tv_name, "%s", TV_LONG_P);break;
        case U_LONG: sprintf(tv_name, "%s", TV_U_LONG);break;
        case U_LONG_P: sprintf(tv_name, "%s", TV_U_LONG_P);break;
        case FLOAT: sprintf(tv_name, "%s", TV_FLOAT);break;
        case FLOAT_P: sprintf(tv_name, "%s", TV_FLOAT_P);break;
        case DOUBLE: sprintf(tv_name, "%s", TV_DOUBLE);break;
        case DOUBLE_P: sprintf(tv_name, "%s", TV_DOUBLE_P);break;
        case LONG_DOUBLE: sprintf(tv_name, "%s", TV_LONG_DOUBLE);break;
        case LONG_DOUBLE_P: sprintf(tv_name, "%s", TV_LONG_DOUBLE_P);break;
        case CHAR: sprintf(tv_name, "%s", TV_CHAR);break;
        case CHAR_P:
        case STRING: sprintf(tv_name, "%s", TV_CHAR_P);break;
        case UNKNOWN: is_value_of_p = true;
        case UNKNOWN_P: sprintf(tv_name, "%s", TV_VOID_P);break;
    };
    if(is_array) {
        if(is_value_of_p) sprintf(dest, "%s*(%s[%d].%s)", (index > 0 ? ", " : ""), var_name, index, tv_name);
        else sprintf(dest, "%s%s[%d].%s", (index > 0 ? ", " : ""), var_name, index, tv_name);
    } else {
        sprintf(dest, "%s.%s", var_name, tv_name);
    }
}

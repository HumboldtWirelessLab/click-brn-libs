#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "dynamic_source.h"

#include "reflection.h"
#include "samples.h"
//#include "c_source_reader.h"
#include "c_source_data_functions.h"
////#include "read_from_line.h"
#include "scan_command_line.h"
#include "utile.h"
#include "examples_test.h"
#include "scan_type_test.h"

static const char QUIT[] = "quit";
static char* FILES[] = {"t_func_1.c", "t_func_2.c"};

bool debug = false;
bool user_debug = false;

void print_help() {
    printf("\nKommandos:\n-------------\n");
    printf("help|-h                                  - Ausgabe der Kommandos\n");
    printf("set debug true|false                     - Aktivieren oder Deaktivieren der Debug-Ausgaben\n");
    printf("set console_debug true|false             - Aktivieren oder Deaktivieren der Console-Ausgaben\n");
    printf("set measure clock|time                   - Aktivieren ob bei der Zeitmessung in CPU-Clocks oder Mikrosekunden gemessen wird\n");
    printf("<c-source-file>                          - C-Datei die geladen werden soll, es wird versucht die Funktionen zu extrahieren (z.B. functions.c)\n");
    printf("%s                                - C-Quellcodestring der geladen werden soll, ...\n", C_EXAMPLE_NAME);
    printf("reload <c-source-file>                   - C-Datei die (erneut)geladen werden soll, alte Version wird entfernt\n");
    printf("find <function-name>                     - Pruefen ob die Funktion bekannt ist (geladen wurde)\n");
    printf("call <function-name> [param1 param2 ...] - Aufruf der Funktion mit entsprechenden Parameter, sofern verlangt.\n");
    printf("test_functions|test_f                    - fuehrt ein paar Test-Funktionen aus (aus functions.c und pointer_test.c)\n");
    printf("refection_test|ref_t                     - Es werden per Tiny-C Funktionen geladen und ausgefüht.\n");
    printf("run_test_types|run_tt                    - Es wird die Funtion scan_params(..) getestet.\n");
    printf("run_m <loops>                            - Laedt abwechselnd t_func_1.c und t_func_2.c loops mal und gibt die Zeit aus.\n");
    printf("run_s <loops>                            - Laedt abwechselnd die Quellcodestrings aus samples.h .\n");
    printf("q|%s                                   - beendet das Programm\n", QUIT);
}

int main()
{
    //reflection_test();
    //test_functions();

    int e, l, n;
    char * line;
    src_function* s_f;
    f_header* f_h;
    tcc_function* func = load_readLine();
    char* (*readLine)(FILE*) = func->f;
    void (*measure)(int) = my_clock;
    bool isMeasureCPUClocks = true;
    char f_name[100];
    char space[20];
    char* lp;
    //function and source memory
    dyn_src* dynSrc = new_dynamic_source(true);
    bool is_reload = false;

    bool work = true;
    print_help();
    while(work) {
        //printf("\n[while begin] press any key.... \n");
        //getchar();
        printf("\nWaiting for input:> ");
        line = readLine(stdin);
        //line = readLine();
        if(strlen(line) == 0) continue;
        switch(line[0]) {
            case '-':
                if(strcmp(line, "-h") == 0) {
                    print_help();
                    free(line);
                    continue;
                }
                break;
            case 'c':
                if((e = sscanf(line, "call%[ ]%[^ ]", space, f_name)) > 0) {
                    printf("[%d] call function: %s\n", e, f_name);
                    lp = line + 4 + strlen(space) + strlen(f_name);
                    s_f = find_func(dynSrc, f_name);
                    if(s_f == NULL) printf("%s", dynSrc->error_msg);
                    else {
                        print_reduced_src_function(s_f, "");
                        f_h = s_f->header;
                        t_value* param = scan_param(lp, s_f, user_debug);
                        print_t_value(param, f_h->parameter_n, f_h->parameter_types, "   ");
                        //call function with param
                        t_value result = s_f->func(param);
                        print_t_value(&result, 1, &(f_h->return_type), "result:");
                        //free param/return values
                        printf("[call] free params ...\n");
                        free_t_value_array(param, f_h->parameter_n, f_h->parameter_types);
                        if(f_h->return_is_ref_type == true) {
                            printf("[call] don't free return value, can reference an input parameter ...\n");
                        } else {
                            printf("[call] free return value ...\n");
                            free_t_value(&result, &(f_h->return_type), false);
                        }
                    }
                    free(line);
                    continue;
                }
                break;
            case 'C':
                if(strcmp(line, C_EXAMPLE_NAME) == 0) {
                    //printf("scan c_example: \n%s\n", C_EXAMPLE);
                    if(dynSrc->print_command_line_info || dynSrc->debug) {
                        measure(0);
                        e = dynamic_load_from_string_debug(dynSrc, C_EXAMPLE_1, C_EXAMPLE_NAME_1, true);
                        measure(0);
                    } else {
                        measure(0);
                        e = dynamic_load_from_string(dynSrc, C_EXAMPLE_1, C_EXAMPLE_NAME_1, true);
                        measure(0);
                    }
                    free(line);
                    continue;
                }
                break;
            case 'f':
                if(strcmp(line, "find") == 0) {
                    printf("Info: all loaded functions ...\n");
                    print_functions_ht(dynSrc->ht_func);
                    free(line);
                    continue;
                } else if((e = sscanf(line, "find%[ ]%[^ ]", space, f_name)) > 0) {
                    printf("[%d] find function: %s\n", e, f_name);
                    s_f = find_func(dynSrc, f_name);
                    if(s_f == NULL) printf("%s", dynSrc->error_msg);
                    else {
                        print_src_function(s_f, "");
                        //print_src_code_ref(s_f->src_r, "function->", "   ");
                        //print_src_code_ref(s_f->header->src_r, "header->", "   ");
                    }
                    free(line);
                    continue;
                }
                break;
            case 'h':
                if(strcmp(line, "help") == 0) {
                    print_help();
                    free(line);
                    continue;
                }
                break;
            case 'q':
                if(strlen(line) == 1 || strcmp(line, QUIT) == 0) {
                    work = false;
                    free(line);
                    continue;
                }
                break;
            case 'r':
                if(strcmp(line, "reflection_test") == 0 || strcmp(line, "ref_t") == 0) {
                    reflection_test();
                    free(line);
                    continue;
                } else if(strcmp(line, "run_test_types") == 0 || strcmp(line, "run_tt") == 0) {
                    run_test_types();
                    free(line);
                    continue;
                } else if((e = sscanf(line, "reload%[ ]%[^ ]", space, f_name)) > 0) {
                    printf("[%d] reload src: %s\n", e, f_name);
                    free(line);
                    line = copy_string(f_name);
                    is_reload = true;
                } else if((e = sscanf(line, "run_m %d", &l)) > 0) {
                    if(l < 0) l = -l;
                    printf("Start loop for %d\n", l);
                    measure(l);
                    for(n = 0, e = 0;n < l; ++n, ++e ,e %= 2) {
                        if(dynamic_load_from_file(dynSrc, FILES[e], false) != NO_ERROR) printf("[%d] status=%d %s\n", n, dynSrc->last_error_state, dynSrc->error_msg);
                    }
                    measure(l);
                    free(line);
                    continue;
                } else if((e = sscanf(line, "run_s %d", &l)) > 0) {
                    if(l < 0) l = -l;
                    printf("Start string source code loop for %d\n", l);
                    measure(l);
                    for(n = 0, e = 0;n < l; ++n, ++e ,e %= 2) {
                        if(e == 0) {
                            if(dynamic_load_from_string(dynSrc, C_EXAMPLE_1, C_EXAMPLE_NAME_1, true) != NO_ERROR) printf("[%d] status=%d %s\n", n, dynSrc->last_error_state, dynSrc->error_msg);
                        } else {
                            if(dynamic_load_from_string(dynSrc, C_EXAMPLE_2, C_EXAMPLE_NAME_2, true) != NO_ERROR) printf("[%d] status=%d %s\n", n, dynSrc->last_error_state, dynSrc->error_msg);;
                        }
                    }
                    measure(l);
                    free(line);
                    continue;
                }
                break;
            case 's':
                if(strcmp(line, "set debug true") == 0) {
                    if(!dynSrc->debug) {
                        dynSrc->debug = true;
                        printf("info: debug enabled\n");
                    }
                    free(line);
                    continue;
                } else if(strcmp(line, "set debug false") == 0) {
                    if(dynSrc->debug) {
                        dynSrc->debug = false;
                        printf("info: debug disabled\n");
                    }
                    free(line);
                    continue;
                } else if(strcmp(line, "set console_debug true") == 0) {
                    if(!dynSrc->print_command_line_info) {
                        dynSrc->print_command_line_info = true;
                        printf("info: console info enabled\n");
                    }
                    free(line);
                    continue;
                } else if(strcmp(line, "set console_debug false") == 0) {
                    if(dynSrc->print_command_line_info) {
                        dynSrc->print_command_line_info = false;
                        printf("info: console info disabled\n");
                    }
                    free(line);
                    continue;
                } else if(strcmp(line, "set measure clock") == 0) {
                    if(!isMeasureCPUClocks) {
                        isMeasureCPUClocks = true;
                        measure = my_clock;
                        printf("info: enabled measure in CPU clocks\n");
                    }
                    free(line);
                    continue;
                } else if(strcmp(line, "set measure time") == 0) {
                    if(isMeasureCPUClocks) {
                        isMeasureCPUClocks = false;
                        measure = my_clock2;
                        printf("info: set measure in microsecond\n");
                    }
                    free(line);
                    continue;
                }
                break;
            case 't':
                if(strcmp(line, "test_functions") == 0 || strcmp(line, "test_f") == 0) {
                    test_functions();
                    free(line);
                    continue;
                }
                break;
        }

        printf("echo: %s\n",line);
        if(dynSrc->print_command_line_info || dynSrc->debug) {
            measure(0);
            e = dynamic_load_from_file_debug(dynSrc, line, is_reload);
            measure(0);
        } else {
            measure(0);
            e = dynamic_load_from_file(dynSrc, line, is_reload);
            measure(0);
        }
        //printf("begin:                         %d\n", begin);
        //printf("end:                           %d\n", end);
        if(e == NO_ERROR) printf("..done !\n");
        else printf("%s", dynSrc->error_msg);

        free(line);
        //printf("\n[free line] press any key.... \n");
        //getchar();
    }
    printf("\ngood bye..\n");

    //Funktionsspeicher  und Src-Speicher freigeben
    printf("free dyn_src dynSrc:\n");
    free_dynamic_source(dynSrc);

    //clean TCCState
    free_tcc_function(func);

    return 0;
}



/*
int main()
{
    //reflection_test();
    //test_functions();

    int i, e, n; //size
    //void *mem;
    char * line;
    src* c_src;
    src* find_c_src;
    src_function* s_f;
    f_header* f_h;
    tcc_function* func = load_readLine();
    char* (*readLine)(FILE*) = func->f;
    char f_name[100];
    char space[20];
    //char f_param[500];
    char* lp;
    //memory of source
    map_t ht_src = hashmap_new();
    //memory of functions
    map_t ht_func = hashmap_new();
    bool is_reload = false;

    bool work = true;
    print_help();
    while(work) {
        //printf("\n[while begin] press any key.... \n");
        //getchar();
        printf("\nWaiting for input:> ");
        line = readLine(stdin);
        //line = readLine();
        if(strlen(line) == 0) continue;
        switch(line[0]) {
            case '-':
                if(strcmp(line, "-h") == 0) {
                    print_help();
                    free(line);
                    continue;
                }
                break;
            case 'c':
                if((e = sscanf(line, "call%[ ]%[^ ]", space, f_name)) > 0) {
                    printf("[%d] call function: %s\n", e, f_name);
                    lp = line + 4 + strlen(space) + strlen(f_name);
                    if(hashmap_get(ht_func, f_name, (void**)&s_f) == MAP_MISSING) printf("Info: not found !!\n");
                    else {
                        print_reduced_src_function(s_f, "");
                        f_h = s_f->header;
                        t_value* param = scan_param(lp, s_f, user_debug);
                        print_t_value(param, f_h->parameter_n, f_h->parameter_types, "   ");
                        //call function with param
                        t_value result = s_f->func(param);
                        print_t_value(&result, 1, &(f_h->return_type), "result:");
                        //free param/return values
                        printf("[call] free params ...\n");
                        free_t_value_array(param, f_h->parameter_n, f_h->parameter_types);
                        if(f_h->return_is_ref_type == true) {
                            printf("[call] don't free return value, can reference an input parameter ...\n");
                        } else {
                            printf("[call] free return value ...\n");
                            free_t_value(&result, &(f_h->return_type), false);
                        }
                    }
                    free(line);
                    continue;
                }
                break;
            case 'f':
                if(strcmp(line, "find") == 0) {
                    printf("Info: all loaded functions ...\n");
                    print_functions_ht(ht_func);
                    free(line);
                    continue;
                } else if((e = sscanf(line, "find%[ ]%[^ ]", space, f_name)) > 0) {
                    printf("[%d] find function: %s\n", e, f_name);
                    if(hashmap_get(ht_func, f_name, (void**)&s_f) == MAP_MISSING) printf("Info: not found !!\n");
                    else {
                        print_src_function(s_f, "");
                        //print_src_code_ref(s_f->src_r, "function->", "   ");
                        //print_src_code_ref(s_f->header->src_r, "header->", "   ");
                    }
                    free(line);
                    continue;
                }
                break;
            case 'h':
                if(strcmp(line, "help") == 0) {
                    print_help();
                    free(line);
                    continue;
                }
                break;
            case 'q':
                if(strlen(line) == 1 || strcmp(line, QUIT) == 0) {
                    work = false;
                    free(line);
                    continue;
                }
                break;
            case 'r':
                if(strcmp(line, "reflection_test") == 0 || strcmp(line, "ref_t") == 0) {
                    reflection_test();
                    free(line);
                    continue;
                } else if(strcmp(line, "run_test_types") == 0 || strcmp(line, "run_tt") == 0) {
                    run_test_types();
                    free(line);
                    continue;
                } else if((e = sscanf(line, "reload%[ ]%[^ ]", space, f_name)) > 0) {
                    printf("[%d] reload src: %s\n", e, f_name);
                    free(line);
                    line = copy_string(f_name);
                    is_reload = true;
                }
                break;
            case 's':
                if(strcmp(line, "set debug true") == 0) {
                    if(!user_debug) {
                        user_debug = true;
                        printf("info: debug enabled\n");
                    }
                    free(line);
                    continue;
                } else if(strcmp(line, "set debug false") == 0) {
                    if(user_debug) {
                        user_debug = false;
                        printf("info: debug disabled\n");
                    }
                    free(line);
                    continue;
                }
                break;
            case 't':
                if(strcmp(line, "test_functions") == 0 || strcmp(line, "test_f") == 0) {
                    test_functions();
                    free(line);
                    continue;
                }
                break;
        }

        c_src = read_source(line, user_debug);
        if(c_src == NULL) {
            printf("info: unkown/incomplete command or file [%s]\n", line);
            free(line);
            continue;
        }
        n = hashmap_get(ht_src, c_src->name, (void**)&find_c_src);
        //printf("[MAP_MISSING = %d, MAP_OK = %d, MAP_OMEM = %d] found = %d\n", MAP_MISSING, MAP_OK, MAP_OMEM, n);
        if(n == MAP_OK) {
            if(is_reload) {
                is_reload = false;
                printf("[reload] remove old version!!\n");
                for(i=0; i < find_c_src->n; ++i) {
                    s_f = find_c_src->functions[i];
                    if(s_f->state != NULL) {
                        if(hashmap_remove(ht_func, s_f->header->name) == MAP_OK) printf("[reload][%d] removed %s \n", i, s_f->header->name);
                    }
                }
                if(hashmap_remove(ht_src, find_c_src->name) == MAP_OK) printf("[reload] removed src %s \n", find_c_src->name);
                free_src(find_c_src, true, debug);
                printf("[reload] load new version:\n\n");
            } else {
                printf("Info: %s is already loaded!\n", c_src->name);
                free_src(c_src, false, debug);
                free(line);
                continue;
            }
        }

        printf("echo: %s\n",line);
        //is existing file
        //s_c = c_src->src;
        if(user_debug) {
            printf("\nsrc(n = %d, allocated_n = %d, start_pos = %d)\n[\n", c_src->n, c_src->allocated_n, c_src->src_start_pos);
            print_string((c_src->src)->src, "   ");
            printf("]\n");
        }

        scan_c_source(c_src, c_src->src_start_pos, (c_src->src)->length, false);
        n = 0;
        if(load_functions(c_src, user_debug) > 0) {
            //add loaded functions to hashtable
            printf("\nsrc(n = %d, allocated_n = %d): \n", c_src->n, c_src->allocated_n);
            for(i=0; i<c_src->n; ++i) {
                s_f = c_src->functions[i];
                if(s_f->state != NULL) {
                    hashmap_put(ht_func, get_name(s_f), s_f);
                    printf("Added[%d:%d]: %s\n", i, ++n, get_name(s_f));
                } else {
                    printf("Skipped[%d]: %s\n", i, get_name(s_f));
                }
            }
        } else {
            printf("INFO: load_functions returned with <= 0, therefore there are no functions to add or an problem occured!\n");
        }

        if(n > 0) {
            hashmap_put(ht_src, c_src->name, c_src);
            printf("\nInfo: cached %d functions [src.name = %s]\n", hashmap_length(ht_func), c_src->name);
        } else {
            free_src(c_src, true, debug);
            printf("Info: no suitable function found/added!\n");
        }
        //free_src(c_src, false, false);

        free(line);
        //printf("\n[free line] press any key.... \n");
        //getchar();
    }
    printf("\ngood bye..\n");

    //Funktionsspeicher freigeben
    printf("free src_function ht:\n");
    free_functions_ht(ht_func, debug);
    //Src-Speicher freigeben
    printf("free src ht:\n");
    free_src_ht(ht_src, false, debug);
    //clean TCCState
    free_tcc_function(func);

    return 0;
}
*/


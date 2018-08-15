#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dynamic_source.h"
#include "c_source_reader.h"
#include "reflection.h"
#include "c_source_data_functions.h"
#include "utile.h"

dyn_src* new_dynamic_source(bool print_command_line_info) {
    dyn_src* dynSrc = (dyn_src*) malloc(sizeof(dyn_src));
    dynSrc->ht_func = hashmap_new();
    dynSrc->ht_src = hashmap_new();
    dynSrc->debug = false;
    dynSrc->print_command_line_info = print_command_line_info;
    dynSrc->error_msg[0] = '\0';

    return dynSrc;
}

void free_dynamic_source(dyn_src* dynSrc) {
    free_functions_ht(dynSrc->ht_func, dynSrc->debug);
    free_src_ht(dynSrc->ht_src, false, dynSrc->debug);

    free(dynSrc);
}

int dynamic_load_from_file_debug(dyn_src* dynSrc, char* file, bool is_reload) {
    //try to read file
    src* c_src = read_source_from_file(file, dynSrc->debug);
    if(c_src == NULL) {
        sprintf(dynSrc->error_msg, "info: unkown/incomplete command or file [%s]\n", file);
        //if(dynSrc->print_command_line_info) printf("%s", dynSrc->error_msg);
        dynSrc->last_error_state = ERROR_FILE_NOT_FOUND;

        return dynSrc->last_error_state;
    }
    return dynamic_load_debug(dynSrc, c_src, is_reload);
}

int dynamic_load_from_file(dyn_src* dynSrc, char* file, bool is_reload) {
    //try to read file
    src* c_src = read_source_from_file(file, dynSrc->debug);
    if(c_src == NULL) {
        sprintf(dynSrc->error_msg, "info: unkown/incomplete command or file [%s]\n", file);
        dynSrc->last_error_state = ERROR_FILE_NOT_FOUND;

        return dynSrc->last_error_state;
    }
    return dynamic_load(dynSrc, c_src, is_reload);
}

int dynamic_load_from_string_debug(dyn_src* dynSrc, char* source, char* source_name, bool is_reload) {
    //printf("dynamic_load_from_string(...\n%s\n", source);
    src* c_src = read_source_from_string_2(source, source_name, dynSrc->debug);
    if(c_src == NULL) {
        sprintf(dynSrc->error_msg, "info: unkown/incomplete source [%s]\n", source_name);
        dynSrc->last_error_state = ERROR_FILE_NOT_FOUND;

        return dynSrc->last_error_state;
    }
    return dynamic_load_debug(dynSrc, c_src, is_reload);
}

int dynamic_load_from_string(dyn_src* dynSrc, char* source, char* source_name, bool is_reload) {
    src* c_src = read_source_from_string_2(source, source_name, dynSrc->debug);
    if(c_src == NULL) {
        sprintf(dynSrc->error_msg, "info: unkown/incomplete source [%s]\n", source_name);
        dynSrc->last_error_state = ERROR_FILE_NOT_FOUND;

        return dynSrc->last_error_state;
    }
    return dynamic_load(dynSrc, c_src, is_reload);
}

int dynamic_load_debug(dyn_src* dynSrc, src* c_src, bool is_reload) {
    bool pToCmd = dynSrc->print_command_line_info;
    int n = 0, i;
    src* find_c_src = NULL;
    src_function* s_f = NULL;
    //reset error state
    dynSrc->last_error_state = NO_ERROR;
    dynSrc->error_msg[0] = '\0';


    n = hashmap_get(dynSrc->ht_src, c_src->name, (void**)&find_c_src);

    if(n == MAP_OK) {
        if(is_reload) {
            if(pToCmd) printf("[reload] remove old version!!\n");
            for(i=0; i < find_c_src->n; ++i) {
                s_f = find_c_src->functions[i];
                if(s_f->state != NULL) {
                    if(hashmap_remove(dynSrc->ht_func, s_f->header->name) == MAP_OK) if(pToCmd) printf("[reload][%d] removed %s \n", i, s_f->header->name);
                }
            }
            if(hashmap_remove(dynSrc->ht_src, find_c_src->name) == MAP_OK) if(pToCmd) printf("[reload] removed src %s \n", find_c_src->name);
            free_src(find_c_src, true, dynSrc->debug);
            if(pToCmd) printf("[reload] load new version:\n\n");
        } else {
            sprintf(dynSrc->error_msg, "Info: %s is already loaded!\n", c_src->name);
            dynSrc->last_error_state = ERROR_ALREADY_LOADED;
            //if(pToCmd) printf("%s", dynSrc->error_msg);
            free_src(c_src, false, dynSrc->debug);

            return dynSrc->last_error_state;
        }
    }

    if(pToCmd) {
        //is existing file
        //s_c = c_src->src;
        if(dynSrc->debug) {
            printf("\nsrc(n = %d, allocated_n = %d, start_pos = %d)\n[\n", c_src->n, c_src->allocated_n, c_src->src_start_pos);
            print_string((c_src->src)->src, "   ");
            printf("]\n");
        }
    }

    //scan source code and extract functions
    scan_c_source(c_src, c_src->src_start_pos, (c_src->src)->length, false);
    n = 0;
    if(load_functions(c_src, pToCmd, dynSrc->debug) > 0) {
        //add loaded functions to hashtable
        if(pToCmd) printf("\nsrc(n = %d, allocated_n = %d): \n", c_src->n, c_src->allocated_n);
        src_function* o_s_f;
        src* o_s_c;
        for(i=0; i<c_src->n; ++i) {
            s_f = c_src->functions[i];
            if(s_f->state != NULL) {
                if(hashmap_get(dynSrc->ht_func, get_name(s_f), (void**)&o_s_f) == MAP_OK) {
                    if(pToCmd) printf("function[%s] already exists, has to be deleted first.\n", get_name(s_f));
                    hashmap_remove(dynSrc->ht_func, get_name(o_s_f));
                    o_s_c = (o_s_f->state->f_count == 1 ? (src *)(o_s_f->c_src) : NULL);
                    free_src_function(o_s_f, false);
                    if(o_s_c) {
                        hashmap_remove(dynSrc->ht_src, o_s_c->name);
                        if(pToCmd) printf("delete src[%s].\n", o_s_c->name);
                        free_src(o_s_c, false, false);
                    }
                }
                hashmap_put(dynSrc->ht_func, get_name(s_f), s_f);
                if(pToCmd) printf("Added[%d:%d]: %s\n", i, ++n, get_name(s_f));
            } else {
                if(pToCmd) printf("Skipped[%d]: %s\n", i, get_name(s_f));
            }
        }
    } else {
        sprintf(dynSrc->error_msg, "INFO: load_functions returned with <= 0, therefore there are no functions to add or an problem occured!\n");
        dynSrc->last_error_state = ERROR_NO_FUNCTIONS_TO_ADD;

        return dynSrc->last_error_state;
    }

    if(n > 0) {
        hashmap_put(dynSrc->ht_src, c_src->name, c_src);
        if(pToCmd)printf("\nInfo: cached %d functions [src.name = %s]\n", hashmap_length(dynSrc->ht_func), c_src->name);
    } else {
        free_src(c_src, true, dynSrc->debug);
        if(pToCmd) printf("Info: no suitable function found/added!\n");
    }

    return dynSrc->last_error_state;
}

/**
 * No printf to cmd.
 */
int dynamic_load(dyn_src* dynSrc, src* c_src, bool is_reload) {
    int n = 0, i;
    src* find_c_src = NULL;
    src_function* s_f = NULL;
    //reset error state
    dynSrc->last_error_state = NO_ERROR;
    dynSrc->error_msg[0] = '\0';

    n = hashmap_get(dynSrc->ht_src, c_src->name, (void**)&find_c_src);

    if(n == MAP_OK) {
        if(is_reload) {
            for(i=0; i < find_c_src->n; ++i) {
                s_f = find_c_src->functions[i];
                if(s_f->state != NULL) {
                    hashmap_remove(dynSrc->ht_func, s_f->header->name);
                }
            }
            hashmap_remove(dynSrc->ht_src, find_c_src->name);
            free_src(find_c_src, true, false);
        } else {
            sprintf(dynSrc->error_msg, "Info: %s is already loaded!\n", c_src->name);
            dynSrc->last_error_state = ERROR_ALREADY_LOADED;
            free_src(c_src, false, false);

            return dynSrc->last_error_state;
        }
    }

    //scan source code and extract functions
    scan_c_source(c_src, c_src->src_start_pos, (c_src->src)->length, false);
    n = 0;
    if(load_functions(c_src, false, false) > 0) {
        //add loaded functions to hashtable
        src_function* o_s_f;
        src* o_s_c;
        for(i=0; i<c_src->n; ++i) {
            s_f = c_src->functions[i];
            if(s_f->state != NULL) {
                if(hashmap_get(dynSrc->ht_func, get_name(s_f), (void**)&o_s_f) == MAP_OK) {
                    hashmap_remove(dynSrc->ht_func, get_name(o_s_f));
                    o_s_c = (o_s_f->state->f_count == 1 ? (src *)(o_s_f->c_src) : NULL);
                    free_src_function(o_s_f, false);
                    if(o_s_c) {
                        hashmap_remove(dynSrc->ht_src, o_s_c->name);
                        free_src(o_s_c, false, false);
                    }
                }
                hashmap_put(dynSrc->ht_func, get_name(s_f), s_f);
                ++n;
            }
        }
    } else {
        sprintf(dynSrc->error_msg, "INFO: load_functions returned with <= 0, therefore there are no functions to add or an problem occured!\n");
        dynSrc->last_error_state = ERROR_NO_FUNCTIONS_TO_ADD;

        return dynSrc->last_error_state;
    }

    if(n > 0) hashmap_put(dynSrc->ht_src, c_src->name, c_src);
    else free_src(c_src, true, false);

    return dynSrc->last_error_state;
}

src_function* find_func(dyn_src* dynSrc, char* f_name) {
    src_function* s_f;
    if(hashmap_get(dynSrc->ht_func, f_name, (void**)&s_f) == MAP_MISSING) {
        sprintf(dynSrc->error_msg, "Info: not found !!\n");
        dynSrc->last_error_state = ERROR_FUNCTION_NOT_FOUND;
        return NULL;
    }
    return s_f;
}

t_value call_func(src_function* s_f, t_value* param) {
    t_value res = s_f->func(param);

    return res;
}

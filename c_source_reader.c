#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "c_source_reader.h"
#include "c_source_data_functions.h"
#include "buffered_reader.h"
#include "utile.h"



// trim spaces from left
void trim_string(char *line, char *trimmed_line) {
    int flag = 0;

    // from left
    while(*line) {
        if(!isspace((unsigned char) *line) && flag == 0) {
            *trimmed_line++ = *line;
            flag = 1;
        }
        line++;
        if(flag == 1) {
            *trimmed_line++ = *line;
        }
    }

    // from right
    while(1) {
        trimmed_line--;
        if(!isspace((unsigned char) *trimmed_line) && flag == 0) {
            break;
        }
        flag = 0;
        *trimmed_line = '\0';
    }
}
/**
 * trim spaces/new line and remove comments
 * returns 1 if the complete line should be ignored !!
 */
unsigned int trim_and_remove_comments(char *line, char *ptr_line, unsigned int *has_comment, unsigned int *is_multi_line_comment, bool debug) {
    int len, len2;
    char* search_result;
    char* search_result2;
    *has_comment = 0;

    trim_string(line, ptr_line);

    if(*is_multi_line_comment) {
        search_result = strrstr(ptr_line, COMMENT_END);
        if(search_result != NULL) {
            *is_multi_line_comment = 0;
            if(strlen(search_result) > 0) {
                search_result += 2;
                len = strlen(search_result);
                strcpy(ptr_line, search_result);
                ptr_line[len] = '\0';
                *has_comment = 1;
            } else {
                if(debug) printf("skipped: %s\n", ptr_line);
                return 1;
            }
            //ptr_line = (char *)(search_result + 2);
        } else {
            if(debug) printf("skipped: %s\n", ptr_line);
            return 1;
        }
    }

    //printf("%s\n", ptr_line);
    //Auf Kommentar überprüfen
    search_result = strstr(ptr_line, COMMENT_LINE);
    if(search_result != NULL) {
        *search_result = '\0';
        *has_comment = 1;
    }

    if(strlen(ptr_line) > 1 ) {
        do {
            search_result = strstr(ptr_line, COMMENT_START);
            search_result2 = NULL;
            if(search_result != NULL) {
                search_result2 = strstr(search_result, COMMENT_END);
                if(search_result2 != NULL) {
                    search_result2 += 2;
                    len = strlen(search_result2);
                    if(len > 0) {
                        strcpy(search_result, search_result2);
                        search_result2[len] = '\0';
                    } else {
                        *search_result = '\0';
                        search_result2 = NULL;
                    }
                    *has_comment = 1;
                } else {
                    *search_result = '\0';
                    *is_multi_line_comment = 1;
                }
            }
            //printf("search_result = '%s', search_result2 = '%s', ptr_line = '%s'\n", search_result, search_result2, ptr_line);
        } while(search_result2 != NULL && strlen(ptr_line) > 1);
        len = strlen(ptr_line);
        if(*has_comment && len > 0) {
            char t_ptr_line[500] = "";
            trim_string(ptr_line, t_ptr_line);
            len2 = strlen(t_ptr_line);
            if(len2 == 0) ptr_line[0] = '\0';
            else if(len > len2) {
                if(ptr_line != t_ptr_line) strcpy(ptr_line, t_ptr_line);
                ptr_line[len2] = '\0';
            }
        }
    }
    return 0;
}

src* read_source(char* file_name, bool debug) {
    FILE *file;
    unsigned int has_comment = 0, is_multi_line_comment = 0;
    char line[500];
    int line_length, file_size;
    src_code* s_c;
    //src_function* s_f;
    src* all_f = NULL;
    if(debug) printf("\n");

    file = fopen(file_name, "r");
    if(file != NULL) {
        //initialize string buffer
        file_size = get_file_length(file);
        //s_f = create_src_function(file_size);
        all_f = create_src(file_size, DEFAULT_SRC_FUNCTION_CAPACITY);
        s_c = all_f->src;
        //read file line by line
        while(fgets(line, 500, file) != NULL) {
            char ptr_line[500] = "";
            //trim spaces and remove comments
            if(trim_and_remove_comments(line, ptr_line, &has_comment, &is_multi_line_comment, debug)) continue;

            //ist was übrig von der Zeile
            if(strlen(ptr_line) > 0) {
                if(ptr_line[0] == '#' && !(strstr(ptr_line, "#define") == ptr_line)) {//remove simple macros like define, include ...
                    if(s_c->length > 0) {
                        s_c->src[0] = '\0';
                        s_c->length = 0;
                        s_c->lines = 0;
                    }
                    if(debug) printf("skipped: %s\n", ptr_line);
                } else {
                    ++(s_c->lines);
                    line_length = strlen(ptr_line);
                    memcpy(&(s_c->src[s_c->length]), ptr_line, line_length);
                    //if(s_f->lines > 1) strcat(s_f->s, line);
                    //else strcpy(s_f->s, line);
                    s_c->length += line_length;
                    s_c->src[s_c->length] = '\n';
                    ++(s_c->length);

                    if(ptr_line[0] == '#') {//define
                        all_f->src_start_pos = s_c->length;
                    }

                    if(debug) {
                        if(line_length < 10) {
                            if(s_c->lines < 10) printf("00%i : __%i : %s\n", s_c->lines, line_length,ptr_line);
                            else if(s_c->lines < 100) printf("0%i : __%i : %s\n", s_c->lines, line_length,ptr_line);
                            else printf("%i : __%i : %s\n", s_c->lines, line_length,ptr_line);
                        } else if(line_length < 100) {
                            if(s_c->lines < 10) printf("00%i : _%i : %s\n", s_c->lines, line_length,ptr_line);
                            else if(s_c->lines < 100) printf("0%i : _%i : %s\n", s_c->lines, line_length,ptr_line);
                            else printf("%i : _%i : %s\n", s_c->lines, line_length,ptr_line);
                        } else {
                            if(s_c->lines < 10) printf("00%i : %i : %s\n", s_c->lines, line_length,ptr_line);
                            else if(s_c->lines < 100) printf("0%i : %i : %s\n", s_c->lines, line_length,ptr_line);
                            else printf("%i : %i : %s\n", s_c->lines, line_length,ptr_line);
                        }
                    }
                }
                //printf("%i - %i\n", s_f->length, file_size);
            } else if(debug) {
                if(has_comment || is_multi_line_comment) printf("skipped: %s", line);
                else printf("skipped: \n");
            }
        }
        s_c->src[s_c->length] = '\0';
        if(debug) printf("%i - %i", s_c->length, file_size);

        if(s_c->length > 0) {
            all_f->name = copy_string(file_name);
        }
        //all_f->functions[all_f->n] = s_f;
        //++(all_f->n);
    }
    fclose(file);

    return all_f;
}

bool convert_ref_type(char* src, bool debug) {
    if(debug) printf("[convert ref type] %s\n", src);
    if(strstr(src, "unsigned_") == src) {
        src += 8;
        switch(src[1]) {
            case 'i':
                if(strstr(src + 1, "int_ ") == src+1) {
                    src[0] = ' ';src[4] = '*';return true;
                } else return false;
            case 'l':
                if(strstr(src + 1, "long_ ") == src+1) {
                    src[0] = ' ';src[5] = '*';return true;
                } else return false;
            case 's':
                if(strstr(src + 1, "short_ ") == src+1) {
                    src[0] = ' ';src[6] = '*';return true;
                } else return false;
            default:
                return false;
        }
    }
    switch(src[0]) {
        case 'c':
            if(strstr(src, "char_ ") == src) {
                src[4] = '*';return true;
            } else return false;
        case 'd':
            if(strstr(src, "double_ ") == src) {
                src[6] = '*';return true;
            } else return false;
        case 'f':
            if(strstr(src, "float_ ") == src) {
                src[5] = '*';return true;
            } else return false;
        case 'i':
            if(strstr(src, "int_ ") == src) {
                src[3] = '*';return true;
            } else return false;
        case 'l':
            if(strstr(src, "long_ ") == src) {
                src[5] = '*';return true;
            } else if(strstr(src, "long_double_ ") == src) {
                src[4] = ' ';src[11] = '*';return true;
            } else return false;
        case 's':
            if(strstr(src, "short_ ") == src) {
                src[5] = '*';return true;
            } else return false;
        default:
            return false;
    }
}

bool is_valid_t(char* src, char* t, char c) {
    return (strstr(src, t) == src) && (isspace(c) || c == '*');
}

f_header* create_f_header_from_string(char* s_h, src_code_ref* src_r, bool debug) {
    static const unsigned int MOD_RETURN = 0;
    static const unsigned int MOD_NAME = 1;
    static const unsigned int MOD_PARAM = 2;
    static const unsigned int IS_UNSIGNED = 10;
    //debug = true;

    int n = strlen(s_h);
    char* src = (char*)malloc(sizeof(char)*(n + 1));
    strcpy(src, s_h);
    src[n] = '\0';
    f_header* f_h = create_f_header(src_r);
    //src_code* s_c = f_h->src;

    //Anzahl der Parameter
    char* bi = strchr(s_h, '(');
    char* si = bi;
    if(si[1] != ')') n = 1;
    else n = 0;
    while((si = strchr(++si, ','))) ++n;
    if(n > 0) {
        f_h->parameter_types = (e_type*)malloc(sizeof(e_type)*n);
        f_h->parameter_n = n;
    }
    //printf("Anzahl der Parameter n = %d\n", f_h->parameter_n);
    //Typen bestimmen
    unsigned int idle = MOD_RETURN;
    unsigned int state = 0;
    e_type e_t = EMPTY;
    si = s_h;
    int break_cond = strlen(si) * 2;
    bool not_ignore = true;
    bool is_ref_type = false;
    while(--break_cond >= 0 && *si) {
        //Debug-Ausgabe
        if(debug) {
            if(idle == MOD_RETURN) printf("idle = MOD_RETURN");
            else if(idle == MOD_PARAM) printf("idle = MOD_PARAM");
            if(state == IS_UNSIGNED) printf(", state = IS_UNSIGED, e_t = ");
            else printf(", state = ---, e_t = ");
            print_e_type(&e_t, is_ref_type, "");
            printf("\n");
        }
        //Debug-Ende
        switch(*si) {
        case '_':
            if(not_ignore) {
                if(debug) printf("[header from string] found ref type ?: %s\n", si);
                if(strstr(si, "_ref__") == si) {
                    if(convert_ref_type(si+6, debug)) {
                        si += 6;
                        is_ref_type = true;
                        if(debug) printf("[header from string] converted ref type: %s\n", si);
                        continue;
                    }
                }
                not_ignore = false;
            }
        case 'c':
            if(si[1] == 'o') {
                if(is_valid_t(si, "const", si[5])) {
                    si += 6; //const + Leerzeichen
                    //printf("[c] si = '%s'\n", si);
                    continue;
                } else
                    not_ignore = false;
            } else if(is_valid_t(si, "char", si[4])){
                e_t = CHAR;
                si += 4;
                continue;
            } else
                not_ignore = false;
        case 'd':
            if(not_ignore) {
                if(is_valid_t(si, "double", si[6])) {
                    e_t = DOUBLE;
                    si += 6;
                    continue;
                } else {
                    not_ignore = false;
                }
            }
        case 'f':
            if(not_ignore) {
                if(is_valid_t(si, "float", si[5])) {
                    e_t = FLOAT;
                    si += 5;
                    continue;
                } else {
                    not_ignore = false;
                }
            }
        case 'i':
            if(not_ignore) {
                if(is_valid_t(si, "int", si[3])) {
                    e_t = (state == IS_UNSIGNED ? U_INT : INT);
                    si += 3;
                    continue;
                } else
                    not_ignore = false;
            }
        case 'l':
            if(not_ignore) {
                if(is_valid_t(si, "long", si[4])) {
                    si += 4;
                    if(state == IS_UNSIGNED) {
                        e_t = U_LONG;
                    } else {
                        while(isspace(*si)) ++si;
                        if(is_valid_t(si, "double", si[6])) {
                        //if(*si == 'd' && si[1] == 'o' && si[2] == 'u' && si[3] == 'b' && si[4] == 'l' && si[5] == 'e'
                        //   && (isspace(si[6]) || si[6] == '*'))  {
                            e_t = LONG_DOUBLE;
                            si += 6;
                        } else {
                            e_t = LONG;
                            if(*si != '*') --si;
                        }
                    }
                    continue;
                } else
                    not_ignore = false;
            }
        case 's':
            if(not_ignore) {
                if(is_valid_t(si, "short", si[5])) {
                    e_t = (state == IS_UNSIGNED ? U_SHORT : SHORT);
                    si += 5;
                    continue;
                } else
                    not_ignore = false;
            }
        case 'u':
            if(not_ignore) {
                if(is_valid_t(si, "unsigned", si[8])) {
                    state = IS_UNSIGNED;
                    si += 9; //unsigned + Leerzeichen
                    continue;
                } else
                    not_ignore = false;
            }
        case 'v':
            if(not_ignore) {
                if(is_valid_t(si, "void", si[4])) {
                    e_t = VOID;
                    si += 4;
                    continue;
                } else
                    not_ignore = false;
            }
        //case '*':
        //case '(':
        //case ',':
        //    ++si;
        //    break;
        case ')':
            if(not_ignore) {
                if(debug) printf("[)] change idle = %d", idle);
                ++idle;
                if(debug) printf(" to idle = %d\n", idle);
                break;
            }
        default :
            not_ignore = true;
            if(!isspace(*si) && e_t == EMPTY && *si != '*') {
                //unbekannter Typ
                while(!(isspace(*si) || *si == '*')) ++si;
                e_t = UNKNOWN;
                f_h->has_unkown_types++;
            }
            while(isspace(*si)) ++si;
            if(*si == '*') {
                switch(e_t) {
                    case VOID: e_t = VOID_P;break;
                    case INT: e_t = INT_P;break;
                    case U_INT: e_t = U_INT_P;break;
                    case SHORT: e_t = SHORT_P;break;
                    case U_SHORT: e_t = U_SHORT_P;break;
                    case LONG:
                        e_t = LONG_P;
                        //state = 0;
                        break;
                    case U_LONG: e_t = U_LONG_P;break;
                    case FLOAT: e_t = FLOAT_P;break;
                    case DOUBLE: e_t = DOUBLE_P;break;
                    case LONG_DOUBLE: e_t = LONG_DOUBLE_P;break;
                    case CHAR: e_t = STRING;break;
                    case UNKNOWN: e_t = UNKNOWN_P;break;
                    case EMPTY: //sollte nicht eintreten !!!
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
                    case STRING:
                    case UNKNOWN_P: break;
                };
                while(isspace(*(++si)));
            }

            if(idle < MOD_PARAM) {
                if(debug) printf("[default] change idle = %d", idle);
                ++idle;
                if(debug) printf(" to idle = %d\n", idle);
                break;
            } else if(state == 0 && e_t == EMPTY) {
                continue;
            }
        };
        if(idle == MOD_NAME) {//return-Type speichern und Name extrahieren
            if(debug) printf("idle = MOD_NAME\n");
            f_h->return_type = e_t;
            e_t = EMPTY;
            state = 0;
            f_h->name = (char*)malloc(sizeof(char)*(bi - si + 1));
            n = 0;
            while(*si != '(') {//Name extrahieren
                f_h->name[n++] = *si;
                ++si;
            }
            ++si;
            f_h->name[n] = '\0';
            if(is_ref_type) {
                f_h->return_is_ref_type = true;
                is_ref_type = false;
            }
            if(f_h->parameter_n == 0) {//Es gibt keine Parameter
                if(debug) {
                    print_f_header(f_h, "", true);
                    printf("return 1\n");
                }
                return f_h;
            }
            n = 0;
            if(debug) printf("[MOD_NAME] change idle = %d", idle);
            ++idle;
            if(debug) printf(" to idle = %d\n", idle);
        } else if(idle == MOD_PARAM) {//Parameter speichern
            f_h->parameter_types[n++] = e_t;
            e_t = EMPTY;
            state = 0;
            while(*si) {//Bezeichner der Parameter werden ignoriert
                if(*si == ',') {//nächste Parameter
                    while(isspace(*(++si)));
                    //printf("[param] si = '%s'\n", si);
                    break;
                }
                if(*si == ')') {//am Ende angekommen
                    if(debug) {
                        print_f_header(f_h, "", true);
                        print_src_code_ref(f_h->src_r, "[creating] ", "");
                        printf("return 2\n");
                    }
                    return f_h;
                }
                ++si;
            }
        } else {
            break;
        }
    }
    if(debug) {
        print_f_header(f_h, "", true);
        printf("return 3\n");
    }
    return f_h;
}

void scan_c_source(src *s_c, int start_src_pos, int end_src_pos, bool debug) {
    if(debug) printf("\n*************** BEGIN scan_c_source ***************\n");
    char header[500];
    f_header* f_h;
    src_function* s_f = NULL;
    //src_function* func = NULL;
    src_function** f_enlarge = NULL;
    int cH = 0;
    int c, len;
	int infundef = 0;
	int nb = 0,
		np = 0;
    src_code* src_c = s_c->src;
    char* src = src_c->src;
	char* si = (start_src_pos >= 0 ? &src[start_src_pos] : src);
	char* se = &src[end_src_pos];
	char* sf = si;
	char* last_nb = NULL;
	//char* f_src = NULL;
	src_code_ref* src_r = NULL;
	src_code_ref* h_src_r = NULL;
	//int i = (start_src_pos >= 0 ? start_src_pos : 0);
	bool is_string = false;
	for(;si < se; ++si) {
        c = *si;
        if(c == '\\') {
            ++si;
            continue;
        }
        if(c == '"') {
            if(is_string) is_string = false;
            else is_string = true;
            continue;
        } else if(is_string)
            continue;

		if(c=='{'){
			if((np==0)&&(nb==0)){last_nb = si; infundef=1;}
			nb++;
		}
		if (infundef==0) {
            if(c == '\n') {
                if(np == 0) {
                    header[cH] = '\0';
                    //Header-Informationen extrahieren
                    while(*last_nb != ')' && last_nb > sf) --last_nb;
                    h_src_r = create_src_ref(src_c, sf - src, last_nb - sf + 1);
                    f_h = create_f_header_from_string(header, h_src_r, debug);

                    if(debug) {
                        printf("[%s]\n", header);
                        printf("cH = %d, last_np - header = %ld\n", cH, (long)(last_nb - sf));
                        print_src_code_ref(h_src_r, "header->", "");
                    }

                    //Fkt-Info erstellen
                    //s_f = create_src_function(f_src, f_h);
                    s_f = create_src_function(src_r, f_h);
                    /*printf("++++++++++++++++++++ function +++++++++++++++++++\n");
                    print_src_function(s_f, "");
                    printf("+++++++++++++++++ function end ++++++++++++++++++\n");*/
                    //Überprüfen ob reservierter Platz reicht
                    if(s_c->n == s_c->allocated_n) {
                        if(debug) printf("\nINFO: s_c->functions has to be reallocated (n=%d, alloc_n=%d)\n", s_c->n, s_c->allocated_n);
                        s_c->allocated_n += 4;
                        f_enlarge = realloc(s_c->functions, s_c->allocated_n * sizeof(src_function*));
                        if(f_enlarge == NULL) {//Fehler beim Speicheranfordern - Abbruch
                            printf("ERROR: realloc failed!!!\n\n");
                            return;
                        } else if(f_enlarge == s_c->functions) {
                            if(debug) printf("INFO: Speicherbereich erweitert !!!\n\n");
                        } else {
                            if(debug) printf("INFO: Speicherbereich NEU angelegt !!!\n\n");
                            s_c->functions = f_enlarge;
                        }
                    }
                    //Funktion speichern
                    s_c->functions[s_c->n] = s_f;
                    if(debug) {
                        printf("############## func (s_c->functions[%d]) ##################\n", s_c->n);
                        print_src_function(s_c->functions[s_c->n],"");
                        printf("############## func ##################\n\n");
                    }
                    s_c->n++;
                    //Zurücksetzen für nächste Funktion
                    cH = 0;
                    header[0] = '\0';
                    sf = si + 1;

                    //printf("\n[%d] press any key.... si=%p, se=%p\n", s_c->n, si, se);
                    //getchar();
                }
            } else {
                header[cH++] = c;
            }
		}
		if(c=='}'){
			if((np==0)&&(nb==1)){infundef=0;}
			nb--;
			if(nb == 0) {
                //printf("Starting function source code ....\n");
                len = si - sf + 1;
                src_r = create_src_ref(src_c, sf - src_c->src, len);
                //print_sub_string((src_r->s_c)->src, src_r->src_start_pos, src_r->length, "   ");

                ////f_src = (char*)malloc(sizeof(char)*(len + 1));
                ////strncpy(f_src, sf, len);
                ////strncpy(f_src, src_c->src + src_r->src_start_pos, len);
                ////f_src[len] = '\0';
                ////print_string(f_src, "   ");
                //printf("Finished function source code ....\n");
                ////free(f_src);
			}
		}
		if(c=='('){np++;}
		if(c==')'){np--;}
	}
	if(debug) printf("\n**************** END scan_c_source ****************\n");
}

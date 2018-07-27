#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "buffered_reader.h"

string_buffer* create_empty_string_buffer() {
    string_buffer* s_buffer = (string_buffer*) malloc(sizeof(string_buffer));
    s_buffer->lines = 0;
    s_buffer->length = 0;
    s_buffer->file_size = 0;

    return s_buffer;
}

string_buffer* create_string_buffer(int file_size) {
    string_buffer* s_buffer = (string_buffer*) malloc(sizeof(string_buffer));
    s_buffer->lines = 0;
    s_buffer->length = 0;
    s_buffer->file_size = file_size;
    s_buffer->s = (char*)malloc(sizeof(char)*(file_size + 1));
    s_buffer->s_reduced = (char*)malloc(sizeof(char)* file_size);

    return s_buffer;
}

int get_file_length(FILE *file) {
    int len;
    if (fseek(file, 0L, SEEK_END) != 0) // ans Ende springen
    {
        printf("\nERROR in SEEK_END");
        fclose(file);
        return -1;
    }
    if ((len=ftell(file))  < 0L)
    {
        printf("\nERROR in ftell ");
        fclose(file);
        return -1;
    }
    if (fseek(file, 0L, SEEK_SET) != 0) // an den Anfang zurück
    {
        printf("\nERROR in SEEK_SET");
        fclose(file);
        return -1;
    }
    return len;
}

string_buffer* read_buffer(char* file_name) {
    FILE *file;
    string_buffer* s_buffer;

    file = fopen(file_name, "r");
    if(file != NULL) {
        s_buffer = create_string_buffer(get_file_length(file));
        printf("\nsize = %i", s_buffer->file_size);

        //s_buffer->s = (char*)malloc(sizeof(char)*(s_buffer->length + 1));
        fread(s_buffer->s, s_buffer->file_size, 1, file);

        s_buffer->s[s_buffer->file_size] = '\0';
    }
    fclose(file);

    return s_buffer;
}

string_buffer* read_buffer2(char* file_name) {
    FILE *file;
    char line[500];
    //char* ptr_line = line;
    int line_length;
    string_buffer* s_buffer;
    printf("\n");

    file = fopen(file_name, "r");
    if(file != NULL) {
        //initialize string buffer
        s_buffer = create_string_buffer(get_file_length(file));
        //read file line by line
        while(fgets(line, 500, file) != NULL) {
            ++(s_buffer->lines);
            line_length = strlen(line);
            memcpy(&(s_buffer->s[s_buffer->length]), line, line_length);
            //if(s_buffer->lines > 1) strcat(s_buffer->s, line);
            //else strcpy(s_buffer->s, line);
            s_buffer->length += line_length;

            //printf("\n*****************************\n%s", s_buffer->s);
            if(line_length < 10) {
                if(s_buffer->lines < 10) printf("00%i : __%i : %s", s_buffer->lines, line_length,line);
                else if(s_buffer->lines < 100) printf("0%i : __%i : %s", s_buffer->lines, line_length,line);
                else printf("%i : __%i : %s", s_buffer->lines, line_length,line);
            } else if(line_length < 100) {
                if(s_buffer->lines < 10) printf("00%i : _%i : %s", s_buffer->lines, line_length,line);
                else if(s_buffer->lines < 100) printf("0%i : _%i : %s", s_buffer->lines, line_length,line);
                else printf("%i : _%i : %s", s_buffer->lines, line_length,line);
            } else {
                if(s_buffer->lines < 10) printf("00%i : %i : %s", s_buffer->lines, line_length,line);
                else if(s_buffer->lines < 100) printf("0%i : %i : %s", s_buffer->lines, line_length,line);
                else printf("%i : %i : %s", s_buffer->lines, line_length,line);
            }
            //printf("%i - %i\n", s_buffer->length, s_buffer->file_size);

        }
        s_buffer->s[s_buffer->length] = '\0';
        printf("%i - %i", s_buffer->length, s_buffer->file_size);
    }
    fclose(file);

    return s_buffer;
}

void echo_file(char* file_name) {
    FILE *file;
    char line[500];
    int file_size = 0, line_length;
    int line_count = 0;

    printf("\n");

    file = fopen(file_name, "r");
    if(file != NULL) {
        while(fgets(line, 500, file) != NULL) {
            ++line_count;
            file_size += (line_length = strlen(line));
            if(line_length < 10) {
                if(line_count < 10) printf("00%i : __%i : %s", line_count, line_length,line);
                else if(line_count < 100) printf("0%i : __%i : %s", line_count, line_length,line);
                else printf("%i : __%i : %s", line_count, line_length,line);
            } else if(line_length < 100) {
                if(line_count < 10) printf("00%i : _%i : %s", line_count, line_length,line);
                else if(line_count < 100) printf("0%i : _%i : %s", line_count, line_length,line);
                else printf("%i : _%i : %s", line_count, line_length, line);
            } else {
                if(line_count < 10) printf("00%i : %i : %s", line_count, line_length,line);
                else if(line_count < 100) printf("0%i : %i : %s", line_count, line_length,line);
                else printf("%i : %i : %s", line_count, line_length,line);
            }
        }
        printf("%i - %i", file_size, get_file_length(file));
    }
    fclose(file);
}

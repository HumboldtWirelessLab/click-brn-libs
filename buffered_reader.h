#ifndef BUFFERED_READER_H_INCLUDED
#define BUFFERED_READER_H_INCLUDED

typedef struct {
	char* s;
	char* s_reduced;
	int file_size;
	int length;
	int lines;
} string_buffer;

string_buffer* create_empty_string_buffer();

string_buffer* create_string_buffer(int file_size);

int get_file_length(FILE *file);

string_buffer* read_buffer(char* file_name);

string_buffer* read_buffer2(char* file_name);

void echo_file(char* file_name);

#endif // BUFFERED_READER_H_INCLUDED

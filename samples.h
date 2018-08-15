#ifndef SAMPLES_H_INCLUDED
#define SAMPLES_H_INCLUDED
static char* C_EXAMPLE_NAME = "C_EXAMPLE";
static char* C_EXAMPLE_NAME_1 = "C_EXAMPLE_1";
static char* C_EXAMPLE_1 =
"#include <stdio.h>\n"
"\n"
"//Test für Typ long\n"
"long sum_long(long l1, long l2) {\n"
"    return l1 + l2;\n"
"}\n"
"\n"
"void echo(char* s) {\n"
"    printf(\"Echooooo  %s\n\",s);\n"
"}\n";

static char* C_EXAMPLE_NAME_2 = "C_EXAMPLE_2";
static char* C_EXAMPLE_2 =
"#include <stdio.h>\n"
"\n"
"//Test für Typ long\n"
"long sum_long(long l1, long* l2) {\n"
"    return l1 + *l2;\n"
"}\n"
"\n"
"void echo(char* s) {\n"
"    printf(\"%s\n\",s);\n"
"}\n";

#endif // SAMPLES_H_INCLUDED

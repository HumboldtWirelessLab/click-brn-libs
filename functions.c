#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

#define TEST_INT 1000

double sum(double d1, double d2){
    return d1 + d2;
}

/**
 * Multiplikation
 */
double mul(double fak1, double fak2) {
    return fak1 * fak2;
}

//Test für Typ long double
long double mul_ld(long double fak1, long double* fak2) {
    return fak1 * (*fak2);
}

//Zähler == numerator, Nenner == denominator
double divide(double numerator, double denominator) {
    return numerator / denominator;
}

double mod(int i1, int i2) {
    return i1 % i2;
}

//Test für Typ long
long sum_long(long l1, long* l2) {
    return l1 + *l2;
}

void echo(char* s) {
    printf("%s\n",s);
}

char* repeatString(char* toRepeat,
                   unsigned int repeatAmount) {
    size_t len = 0, rLen = strlen(toRepeat);
    char* result = (char*)malloc(sizeof(char)*(rLen * repeatAmount + 1));
    unsigned int i;
    for(i=0; i<repeatAmount; ++i, len += rLen) {
        memcpy(&(result[len]), toRepeat, rLen);
        //printf("[%d] i=%d, len=%d, rLen=%d : %s\n", repeatAmount, i, len, rLen, result);
    }
    result[len] = '\0';
    return result;
}

/* printf("****"); */ /*  */
_ref__char_ findStrInStr(const char *string, const char *find) {
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

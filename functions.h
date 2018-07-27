#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "c_parameter_type.h"

double sum(double d1, double d2);

double mul(double fak1, double fak2);

long double mul_ld(long double fak1, long double* fak2);

//Zähler == numerator, Nenner == denominator
double divide(double numerator, double denominator);

double mod(int i1, int i2);

long sum_long(long l1, long* l2);

void echo(char* s);

char* repeatString(char* toRepeat, unsigned int repeatAmount);

char * findStrInStr(const char *string, const char *find);

#endif // FUNCTIONS_H_INCLUDED

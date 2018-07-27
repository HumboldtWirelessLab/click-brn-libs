#ifndef C_PARAMETER_TYPE_H_INCLUDED
#define C_PARAMETER_TYPE_H_INCLUDED

typedef int* _ref_int_;
typedef unsigned int* _ref__unsigned_int_;
typedef short* _ref__short_;
typedef unsigned short* ref__unsigned_short_;
typedef long* _ref__long_;
typedef unsigned long* _ref__unsigned_long_;
typedef float* _ref__float_;
typedef double* _ref__double_;
typedef long double* _ref__long_double_;
typedef char* _ref__char_;

/**
 * Erfasst die Übergabe-/Rückgabewerte.
 */
typedef union {
    int i;
    int* i_p;
    unsigned int u_i;
    unsigned int* u_i_p;
    short s;
    short* s_p;
    unsigned short u_s;
    unsigned short* u_s_p;
    long l;
    long* l_p;
    unsigned long u_l;
    unsigned long* u_l_p;
    float f;
    float* f_p;
    double d;
    double* d_p;
    long double ld;
    long double* ld_p;
    char c;
    char* c_p;
    void* p;
} t_value;

#endif // C_PARAMETER_TYPE_H_INCLUDED

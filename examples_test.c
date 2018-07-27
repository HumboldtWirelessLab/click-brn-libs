#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdarg.h>

#include "functions.h"
#include "c_source_reader.h"
#include "pointer_test.h"
#include "examples_test.h"

void memcopy_example(void) {
    const char src[50] = "http://www.tutorialspoint.com";
    char dest[50];
    strcpy(dest,"Heloooo!!");
    printf("\nBefore memcpy dest = %s\n", dest);
    memcpy(dest, src, strlen(src)+1);
    printf("After memcpy dest = %s\n", dest);
}

/**
 *  https://www.luis.uni-hannover.de/fileadmin/buecher/inhalt/C_skript.pdf
 */
void variable_parameter_list(int zaehler, ...)
    /* Step 2: Die Ellipse  */
{   va_list ap;
    /* Step 3: Eine Variable vom Typ va_list */
    int arg ;
    if (zaehler) {
        /* Step 4: va_start: va_list-Zeiger initialisier. */
        va_start(ap, zaehler);
        while(zaehler--) {
            /* Step 5: va_arg-Schleife holt nächstes Argument vom Stack */
            arg = va_arg(ap,int);
            printf("%d ",arg);
        }
        printf("\n");
        /* Step 6: Aufräumungsarbeiten: va_end ausführen  */
        va_end(ap);
    }
}

void test_functions() {
    double d1 = 10.0, d2 = 2.5;
    //int i1 = 9, i2 = 2;
    double erg = sum(d1, d2);
    printf("[sum] %f + %f = %f\n", d1, d2, erg);

    erg = mul(d1, d2);
    printf("[mul] %f * %f = %f\n", d1, d2, erg);

    erg = divide(d1, d2);
    printf("[divide] %f / %f = %f\n", d1, d2, erg);

    int iErg = mod(d1, d2);
    printf("[mod] %g / %g = %d\n", d1, d2, iErg);

    char repeat[14] = "Wiederholung-";
    char* result = repeatString(repeat, 4);

    echo(result);

    char toFind[4] = "hol";
    char* found = findStrInStr(result, toFind);
    printf("last occurrence of '%s' in '%s': %s\n", toFind, result, (found == NULL ? "not found" : found));

    free(result);


    printf("\n~~~~~~~~~~~~~~ pointer test ~~~~~~~~~~~~~~~~\n");
    d_mem* m = create_d_mem(100);
    fill_p_test_memory(m);
    //p_test **t_array = create_p_test_array(100);
    p_test** t_array = m->mem;

    // now define a pointer to the array
    p_test ***p = &t_array;
    printf("p points to an array of pointers.\n"
       "The third element of the array points to a structure,\n"
       "and the data member of that structure is: %d\n", (*p)[2]->data);
    printf("%d\n", t_array[2]->data);

    //free_p_test_array(t_array, 100);
    free_d_mem(m);
    printf("~~~~~~~~~~~~ pointer test end ~~~~~~~~~~~~~~\n");



    printf("\n------------- variable parameter list ------------------\n");
    printf("variable_parameter_list(5, 20, 30, 40, 89, 100);\n\nerg: ");
    variable_parameter_list(5, 20, 30, 40, 89, 100);
    printf("\n----------- variable parameter list end ----------------\n");

    //return 0;
}

/*void scan_c_source(char *src, int start_src_pos, int end_src_pos) {
    int c;
	int infundef = 0;
	int nb = 0,
		np = 0;
	//while((c=getc(stdin))!=EOF){
	int i = (start_src_pos >= 0 ? start_src_pos : 0);
	for(;i < end_src_pos; ++i) {
        c = src[i];
		if(c=='{'){
			if((np==0)&&(nb==0)){infundef=1;}
			nb++;
		}
		if (infundef==0) {putc(c,stdout);}
		if(c=='}'){
			if((np==0)&&(nb==1)){infundef=0;}
			nb--;
		}
		if(c=='('){np++;}
		if(c==')'){np--;}
	}
}*/
/*
void scan_c_source(char *src, int start_src_pos, int end_src_pos) {
    char header[500];
    int cH = 0;
    int c;
	int infundef = 0;
	int nb = 0,
		np = 0;
	//while((c=getc(stdin))!=EOF){
	int i = (start_src_pos >= 0 ? start_src_pos : 0);
	for(;i < end_src_pos; ++i) {
        c = src[i];
		if(c=='{'){
			if((np==0)&&(nb==0)){infundef=1;}
			nb++;
		}
		if (infundef==0) {
            if(c == '\n') {
                if(np == 0) {
                    header[cH] = '\0';
                    printf("%s\n", header);
                    create_f_header_from_string(header, debug);
                    cH = 0;
                    header[0] = '\0';
                }
            } else {
                header[cH++] = c;
            }
            //putc(c,stdout);
		}
		if(c=='}'){
			if((np==0)&&(nb==1)){infundef=0;}
			nb--;
		}
		if(c=='('){np++;}
		if(c==')'){np--;}
	}
}*/

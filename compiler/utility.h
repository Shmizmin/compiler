#ifndef utility_h
#define utility_h

#include <stdlib.h>


typedef unsigned int size;

extern size line;

#ifdef __cplusplus
extern "C"
{
#endif
    
char* capture(char* in, size n, size m);
void create_ast(void);
void generate_ast(void);

#ifdef __cplusplus
}
#endif


#endif /* utility.h */

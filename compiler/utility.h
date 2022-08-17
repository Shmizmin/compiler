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

#ifdef __cplusplus
}
#endif


#endif /* utility.h */

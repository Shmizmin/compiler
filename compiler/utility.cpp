#include "utility.h"

#include <string>
#include <cstring>

char* capture(char* in, size n, size m)
{
    std::string temp = in;
    temp = temp.substr(n, temp.length() - m);

    return strdup(temp.data());
}

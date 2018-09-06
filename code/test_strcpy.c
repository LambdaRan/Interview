

#include <stdio.h>
#include <stdlib.h>

char* Strcpy(char* to, const char* from)
{
    char* save = to;
    for (; (*to = *from) != '\0'; ++from, ++to);
    return (save);
}

int main() 
{
    char src[] = "01234567890123456789";
    char dst[64];

    Strcpy(dst, src);
    printf("%s\n", dst);
    return 0;
}

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void upcase(char *s)
{
    while (*s++)
        *s = toupper(*s);     
    
}

int main(void) {    
    for (int sig = 1; sig < 32; sig++)
    {
        char *str = strdup(sys_siglist[sig]);
        if (!str)
            return -1;

        upcase(str);
        printf("%2d -> SIG%s\n", sig, str);

        free(str);
    }

    return 0;
}
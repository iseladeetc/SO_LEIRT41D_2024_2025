#include "erroraux.h"

#include <stdio.h>
#include <stdlib.h>

void fatalErrorSystem (char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
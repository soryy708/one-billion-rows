#include <stdio.h>
#include <stdlib.h>
#include "c-polyfill.h"
#include "signal-handler.h"
#include "generate.h"

#define GENERATE false

int main()
{
    initSignals();

    if (GENERATE)
        generateInputs();

    // TODO: Run through the inputs

    return 0;
}

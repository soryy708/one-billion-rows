#include <stdio.h>
#include <stdlib.h>
#include "c-polyfill.h"
#include "signal-handler.h"
#include "generate.h"
#include "run.h"

#define GENERATE false

int main()
{
    initSignals();

    if (GENERATE)
        generateInputs();

    struct RunResult result = run();
    printf("{");
    for (unsigned int i = 0; i < result.length; ++i)
    {
        struct RunResultEntry entry = result.entries[i];
        printf("%s=%.1f/%.1f/%.1f", entry.station, entry.minimum, entry.mean, entry.maximum);
        if (i + 1 < result.length)
        {
            printf(", ");
        }
    }
    printf("}");

    return 0;
}

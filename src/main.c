#include <stdio.h>
#include <stdlib.h>
#include "c-polyfill.h"
#include "signal-handler.h"
#include "generate.h"
#include "config.h"
#include "debug-log.h"
#include "run/run.h"

int main()
{
    initSignals();

    if (GENERATE)
    {
        debugPrintf("Generating inputs\n");
        generateInputs();
    }
    else
        debugPrintf("Skipping generating inputs\n");

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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "panic.h"
#include "c-polyfill.h"

PanicObserver *observers = nullptr;
unsigned int observersLength = 0;

void panic(char *reason)
{
    fprintf(stderr, "PANIC: %s. Errno: #%d\n", reason, errno);
    if (observers != nullptr)
    {
        for (unsigned int i = 0; i < observersLength; ++i)
            observers[i]();
        free(observers);
    }
    exit(1);
}

void addPanicObserver(PanicObserver observer)
{
    if (observers == nullptr || observersLength == 0)
    {
        observers = malloc(sizeof(PanicObserver));
        if (observers == nullptr)
            return panic("Out of memory");
        observers[0] = observer;
        observersLength = 1;
        return;
    }
    PanicObserver *resized = realloc(observers, (observersLength + 1) * sizeof(PanicObserver));
    if (resized == nullptr)
        return panic("Out of memory");
    observers = resized;
    observers[observersLength] = observer;
    observersLength++;
}

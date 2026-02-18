#ifndef ENTRY_CONSUMER_H_INCLUDED
#define ENTRY_CONSUMER_H_INCLUDED

#include "run.h"

void consumeEntry(char *station, float measurement);
struct RunResult collectConsumedEntries();

#endif

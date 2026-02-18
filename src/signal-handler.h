#ifndef SIGNAL_HANDLER_H_INCLUDED
#define SIGNAL_HANDLER_H_INCLUDED

#include "c-polyfill.h"

void initSignals(void);
bool signalledToStop(void);

#endif

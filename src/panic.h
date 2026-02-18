#ifndef PANIC_H_INCLUDED
#define PANIC_H_INCLUDED

typedef void (*PanicObserver)(void);

void panic(char *);
void addPanicObserver(PanicObserver);

#endif

#ifndef C_POLYFILL_INCLUDED
#define C_POLYFILL_INCLUDED

#if (__STDC_VERSION__ < 202311L)
#define nullptr ((void *)0)
#include <stdbool.h>
#endif

#endif

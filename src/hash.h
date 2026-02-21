#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

typedef unsigned short numeric_hash;

numeric_hash polynomialRollingHash(char *key, const unsigned int modulus, const unsigned int p);

#endif

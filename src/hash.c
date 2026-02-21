#include "hash.h"

numeric_hash polynomialRollingHash(char *key, const unsigned int modulus, const unsigned int p)
{
    numeric_hash hash = 0;
    int pPow = 1;
    for (unsigned int i = 0; key[i] != '\0'; ++i)
    {
        hash = (hash + (key[i] - 'a' + 1) * pPow) % modulus;
        pPow = (pPow * p) % modulus;
    }
    return hash;
}

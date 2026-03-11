#include <stdlib.h>
#include <string.h>
#include "string-set.h"
#include "../c-polyfill.h"

struct StringSet
{
    struct StringSetOptions options;
    char **strings;
    size_t length;
};

struct StringSet *stringSetConstructor(struct StringSetOptions options)
{
    if (options.arena == nullptr)
        return nullptr;
    struct StringSet *ss = malloc(sizeof(struct StringSet));
    if (ss == nullptr)
        return nullptr;
    ss->options = options;
    ss->strings = nullptr;
    ss->length = 0;
    return ss;
}

void stringSetDeconstructor(struct StringSet *ss)
{
    if (ss == nullptr)
        return;
    if (ss->strings != nullptr)
        free(ss->strings);
    free(ss);
}

char *stringSetAdd(struct StringSet *ss, char *string, size_t length)
{
    if (ss == nullptr)
        return nullptr;
    if (ss->strings == nullptr)
    {
        ss->strings = malloc(sizeof(char *));
        if (ss->strings == nullptr)
            return nullptr;
        ss->strings[0] = arenaPush(ss->options.arena, sizeof(char) * (length + 1));
        if (ss->strings[0] == nullptr)
        {
            free(ss->strings);
            ss->strings = nullptr;
            return nullptr;
        }
        memcpy(ss->strings[0], string, length);
        ss->strings[0][length] = '\0';
        ss->length = 1;
        return ss->strings[0];
    }

    // Binary search
    int low = 0;
    int high = ss->length - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int stringOrder = strcmp(string, ss->strings[mid]);
        if (stringOrder == 0)
            return ss->strings[mid];
        if (stringOrder < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    int splicePosition = low;

    char *arenaStr = arenaPush(ss->options.arena, sizeof(char) * (length + 1));
    if (arenaStr == nullptr)
        return nullptr;
    memcpy(arenaStr, string, length);
    arenaStr[length] = '\0';

    char **temp = realloc(ss->strings, (ss->length + 1) * sizeof(char *));
    if (temp == nullptr)
        return nullptr;
    ss->strings = temp;

    if (splicePosition < ss->length)
    {
        memmove(
            &ss->strings[splicePosition + 1],
            &ss->strings[splicePosition],
            (ss->length - splicePosition) * sizeof(char *));
    }

    ss->strings[splicePosition] = arenaStr;
    ++ss->length;
    return ss->strings[splicePosition];
}

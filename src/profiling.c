#include <stdlib.h>
#include <time.h>
#include "profiling.h"
#include "c-polyfill.h"
#include "panic.h"
#include "datastructures/gc.h"

struct ProfileMeasurement
{
    clock_t start;
    clock_t end;
};

struct ProfileMeasurement *profileMeasurementConstructor(void)
{
    struct ProfileMeasurement *profile = gc_malloc(sizeof(struct ProfileMeasurement));
    if (profile == nullptr)
    {
        panic("OOM");
        return nullptr;
    }
    return profile;
}

void profileMeasurementDestructor(struct ProfileMeasurement *profile)
{
    if (profile != nullptr)
    {
        gc_free(profile);
    }
}

void startProfileMeasurement(struct ProfileMeasurement *profile)
{
    if (profile != nullptr)
    {
        profile->start = clock();
    }
}

void endProfileMeasurement(struct ProfileMeasurement *profile)
{
    if (profile != nullptr)
    {
        profile->end = clock();
    }
}

long int getProfileElapsedTime(struct ProfileMeasurement *profile)
{
    if (profile == nullptr)
    {
        return 0;
    }
    return (profile->end - profile->start) / (CLOCKS_PER_SEC / 1000);
}

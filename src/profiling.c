#include <stdlib.h>
#include <sys/time.h>
#include "profiling.h"
#include "c-polyfill.h"
#include "panic.h"

struct ProfileMeasurement
{
    struct timeval start;
    struct timeval end;
};

struct ProfileMeasurement *profileMeasurementConstructor(void)
{
    struct ProfileMeasurement *profile = malloc(sizeof(struct ProfileMeasurement));
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
        free(profile);
    }
}

void startProfileMeasurement(struct ProfileMeasurement *profile)
{
    if (profile != nullptr)
    {
        gettimeofday(&profile->start, nullptr);
    }
}

void endProfileMeasurement(struct ProfileMeasurement *profile)
{
    if (profile != nullptr)
    {
        gettimeofday(&profile->end, nullptr);
    }
}

long int getProfileElapsedTime(struct ProfileMeasurement *profile)
{
    if (profile == nullptr)
    {
        return 0;
    }
    return (profile->start.tv_usec - profile->end.tv_usec);
}

#ifndef PROFILING_H_INCLUDED
#define PROFILING_H_INCLUDED

struct ProfileMeasurement;

struct ProfileMeasurement *profileMeasurementConstructor(void);
void profileMeasurementDestructor(struct ProfileMeasurement *profile);
void startProfileMeasurement(struct ProfileMeasurement *profile);
void endProfileMeasurement(struct ProfileMeasurement *profile);
long int getProfileElapsedTime(struct ProfileMeasurement *profile);

#endif

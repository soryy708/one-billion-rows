#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "generate.h"
#include "c-polyfill.h"
#include "panic.h"
#include "signal-handler.h"

// https://simplemaps.com/data/world-cities
char **cityNames = nullptr;
unsigned int cityNamesLength = 0;

void freeCityNames()
{
    if (cityNames != nullptr)
    {
        for (unsigned int i = 0; i < cityNamesLength; ++i)
            free(cityNames[i]);
        free(cityNames);
        cityNames = nullptr;
        cityNamesLength = 0;
    }
}

void onPanic()
{
    freeCityNames();
}

void pushCity(char *city)
{
    if (city == nullptr)
        return;
    unsigned int length = strlen(city);
    if (length == 0)
        return;

    if (cityNames == nullptr)
    {
        cityNames = malloc(sizeof(char *));
        if (cityNames == nullptr)
            return panic("OOM");
    }
    else
    {
        char **temp = realloc(cityNames, (cityNamesLength + 1) * sizeof(char *));
        if (temp == nullptr)
            return panic("OOM");
        cityNames = temp;
    }
    cityNames[cityNamesLength] = malloc(sizeof(char) * (length + 1));
    strcpy(cityNames[cityNamesLength], city);
    ++cityNamesLength;
}

void loadCities(void)
{
    addPanicObserver(onPanic);

    FILE *file = fopen("worldcities.csv", "r");
    if (file == nullptr)
        return panic("Failed to open file for reading");
    char line[256];
    for (fgets(line, 256, file); !feof(file) && !signalledToStop(); fgets(line, 256, file))
    {
        line[strcspn(line, "\r\n")] = '\0';
        pushCity(line);
    }
    fclose(file);
}

void generateInputs(void)
{
    loadCities();
    if (signalledToStop())
        return;

    srand(time(nullptr));

    // 1B rows of <string: station name>;<double: measurement>
    for (unsigned int i = 0; i < 1000000000u && !signalledToStop(); ++i)
    {
        char *stationName = cityNames[rand() % cityNamesLength];
        printf("%s;%d.%d\n", stationName, rand() % 100, rand() % 10);
    }

    freeCityNames();
}

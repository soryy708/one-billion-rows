#include "entry-producer.h"
#include "entry-consumer.h"

void produceEntry(char *station, float measurement)
{
    // TODO: In future, we can fan out to threads
    consumeEntry(station, measurement);
}

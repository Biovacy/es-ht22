#ifndef RANDOM_H_
#define RANDOM_H_

#include <esp_random.h>

/* Returns a random number contained between min and max.
 *  min: minimum number of ms
 *  max: maximum number of ms
 */

int getRandommsecs(int min, int max);

#endif // RANDOM_H_
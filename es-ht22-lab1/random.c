#include "random.h"

int getRandommsecs(int min, int max) {
    uint32_t random = esp_random();

    /*
     * Use modulus to get a value between (max - min + 1)
     * and 0, if the min is added to the remained a random
     * number in the range of min to max will be generated. 
     */
    return ( random % (max - min + 1) ) + min;
}
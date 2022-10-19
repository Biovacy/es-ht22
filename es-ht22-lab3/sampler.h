#ifndef SAMPLER_H_
#define SAMPLER_H_

/**
 * Init of sampler
 */
void initSampling();

/**
* Starts sampling on a given pin with a given fequency.
* Parameter freq: the sampling frequency
*/
void startSampling();

/**
* Stops the sampling process.
*/
void stopSampling();

/**
 * Returns the frequency from the latest sample.
 */
float getFrequency();

/**
 * 
 */
void resetSampling();

#endif
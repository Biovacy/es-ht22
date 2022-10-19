#include <esp_task_wdt.h>
#include <driver/adc.h>
#include <esp_task_wdt.h>
#include <soc/adc_channel.h>
#include "soc/timer_periph.h"
#include "esp_intr_alloc.h"
#include "hal/timer_types.h"
#include "driver/timer.h"

#include "sampler.h"

int allSamples  = 0;
int sampleCount = 0;
int oldBinary = 0;
int crossingCount = 0;

/**
 * Reset all values.
 */
void resetSampling() {
    allSamples  = 0;
    sampleCount = 0;
    oldBinary = 0;
    crossingCount = 0;
}

void audioCalculation(int measurement) {

    // Increase the average value
    allSamples += measurement;
    sampleCount++; 

    int currentAverage = allSamples / sampleCount;

    int currentBinary = 0;

    if ( measurement >  currentAverage ) {
        currentBinary = 1;
    }
    
    if ( currentBinary != oldBinary ) {
        crossingCount++;
        oldBinary = currentBinary; 
    }

}

/**
 * The Interrupt service
 */
void timerISR(void *args) {
    uint32_t is_t0 = TIMERG0.int_st_timers.t0;

    if (is_t0)
    {

        TIMERG0.int_clr_timers.t0 = 1;

        audioCalculation(adc1_get_raw(ADC1_GPIO32_CHANNEL));

        TIMERG0.hw_timer[0].config.alarm_en = TIMER_ALARM_EN;
    }
    
}

/**
 * Init of sampler
 */
void initSampling() {

    timer_config_t timerConfig;
    timerConfig.auto_reload = TIMER_AUTORELOAD_EN;
    timerConfig.alarm_en = TIMER_ALARM_EN;
    timerConfig.counter_en = TIMER_PAUSE;
    timerConfig.counter_dir = TIMER_COUNT_UP;
    timerConfig.divider = 400;
    timerConfig.intr_type = TIMER_INTR_LEVEL;

    timer_init(TIMER_GROUP_0, TIMER_0, &timerConfig);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 10);
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timerISR,
    NULL, NULL, NULL);

    printf("Setting up timer complete\n");

    adc_power_acquire();

    // gpio_adc_init(ADC_UNIT_1, ADC1_GPIO32_CHANNEL);

    adc1_config_width(ADC_WIDTH_BIT_12);

    adc1_config_channel_atten(ADC1_GPIO32_CHANNEL, ADC_ATTEN_DB_11);

    gpio_pullup_en(32);

    printf("Setting up ADC complete\n");
}

/**
* Starts sampling on a given pin with a given fequency.
* Parameter freq: the sampling frequency
*/
void startSampling() {
    resetSampling();
    timer_start(TIMER_GROUP_0, TIMER_0);
}

/**
* Stops the sampling process.
*/
void stopSampling() {

    timer_pause(TIMER_GROUP_0, TIMER_0);

}

float getFrequency() {
    
    float frequency = crossingCount / ( 2 * sampleCount * 0.00005);
    /*
    printf("Combined sample:    %i\n", allSamples);
    printf("Sample count:   %i\n", sampleCount);
    printf("Crossing count: %i\n", crossingCount);
    */
    return frequency;
}
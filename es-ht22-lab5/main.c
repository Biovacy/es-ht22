/**
 * Auther:  Hampus Oxenholt & Ingvar Pétursson
 * Date:    {unknown}
 */

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <driver/i2c.h>
#include <esp_pm.h>
#include <stdio.h>
#include <math.h>

#include "circular_buffer.h"
#include "MPU6050.h"

// Analyze task info
#define ANALYZATION_NAME "samplingAnalyzation"
#define ANALYZATION_STACK_SIZE 2048

// Sampling task info
#define SAMPLING_TASK_NAME "samplingTask"
#define SAMPLING_STACK_SIZE 2048

// Led task info
#define LED_TASK_NAME "led task"
#define LED_TASK_STACK_SIZE 2048

// The led pin
#define LED_PIN 16

// The button pin
#define BUTTON_PIN 18
#define PUSH_TIME 250000

// The amount of steps needed to reach the goal
#define STEPS_GOAL 10

// The amount of time in between
// two different analyzes, in seconds
#define ANALYZE_PERIOD 5

/**
 * How many data points should be collected 
 * before analyzing the results.
 * Sample with a frequency of 10Hz
 * which is once every 100ms
*/
#define SAMPLE_SIZE 10
#define BUFFER_MAX_SIZE (SAMPLE_SIZE * ANALYZE_PERIOD) 

// 
int lastPushTime = 0;

// Define the buffer
struct circularBuffer buffer; 

// Define the semaphore
SemaphoreHandle_t flag;

// The step counter
unsigned int steps;

/**
 * This function takes a new sample from the accelerometer
 * and puts it into the buffer.
*/
static void samplingTask(void *arg) {

    TickType_t xLastWakeTime = xTaskGetTickCount();
    unsigned int sample;

    while (1) {

        // Get a new sample
        sample = getAccelerationMagnitude(); 

        // Add the new sample to the buffer
        addElement(&buffer, sample);

        // Delay
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000 / SAMPLE_SIZE));    
    }

}

/**
 * This function analyzes the samples in the buffer.
*/
static void samplingAnalyzation(void *arg)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    double standardDeviation = 0; 
    int mean = 0;
    int i = 0; 
    const float constant = 2;

    while (1) {
    
        int n = buffer.numElements;

        if (n != 0) {

            // A temporary array to contain all samples from the buffer
            // while doing the calculation.
            int values[n];

            // Moves all the elements from the buffer to the array
            for (i = 0; i < n; i++) {
                values[i] = removeHead(&buffer);
            }

            // Calculates mean
            for (i = 0; i < n; i++) {
                mean += values[i];
            }

            mean /= n; 

            // Calculate standard deviation
            for (i = 0; i < n; i++) {
                standardDeviation += pow(values[i] - mean, 2);
            }

            standardDeviation = sqrt( standardDeviation / n ); 

            // Compare value to mean + standard deviation
            for (i = 0; i < n; i++) {

                if (values[i] > mean + standardDeviation * constant) {

                    /**
                     * There is no need for debouncing
                     * because the program have such a 
                     * low period, which is every 100ms. 
                    */

                    steps++; 
                }

            }
        }

        printf("Steps: %d\tSD: %f\tMean: %d\n", steps, standardDeviation, mean);

        // Delay
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000 * ANALYZE_PERIOD));
    }
}

// Flash the led when the semaphore is released. 
static void led_task(void *arg) {

    while (1) {
        // wait for semaphore
        if (xSemaphoreTake(flag, portMAX_DELAY) == pdTRUE && esp_timer_get_time() - lastPushTime > PUSH_TIME)
        {
            // flash LED with sequence depending on if step_count > STEPS_GOAL

            if (steps >= STEPS_GOAL) {
                // Flash multiple times to indicate that the step
                // goal has been met.

                for (int i = 0; i < 5; i++) {
                    gpio_set_level(LED_PIN, 1);
                    vTaskDelay(pdMS_TO_TICKS(100));
                    gpio_set_level(LED_PIN, 0);

                    vTaskDelay(pdMS_TO_TICKS(100));
                }

            } 
            else {
                // Flash once shortly to indicate that the step
                // goal has not been met.

                gpio_set_level(LED_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(500));
                gpio_set_level(LED_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(500));
            }

            lastPushTime = esp_timer_get_time();

        }
    }

}

// Button is pressed  
void button_isr_handler(void *arg) {

    gpio_intr_disable(BUTTON_PIN); 

    // Release the semaphore, observe because it
    // was called from an ISR that not all functions
    // can be called from here, thus GiveFromISR part.
    xSemaphoreGiveFromISR(flag, NULL);

    gpio_intr_enable(BUTTON_PIN);
}

void app_main(){   
    printf("Start\n");

    // Enable lightsleep and max, min freq and error check.
    esp_pm_config_esp32_t lightSleep;
    lightSleep.light_sleep_enable = true;
    lightSleep.max_freq_mhz = 80;
    lightSleep.min_freq_mhz = 20;
    esp_err_t res = esp_pm_configure(&lightSleep);
    // ESP_ERROR_CHECK(res);

    // Init the circular buffer
    int *bufferData = (int*) malloc(BUFFER_MAX_SIZE * sizeof(int));
    initCircularBuffer(&buffer, bufferData, BUFFER_MAX_SIZE);

    // Init step count
    steps = 0; 

    // Init semaphore
    flag = xSemaphoreCreateBinary();

    // Init i2c
    initMPU6050();

    // Initialize the button
    gpio_config_t ledConfig;
    ledConfig.pin_bit_mask  = (u_int64_t)1 << LED_PIN;
    ledConfig.mode          = GPIO_MODE_OUTPUT;
    gpio_config(&ledConfig);

    gpio_set_level(LED_PIN, 0);

    // Initialize the led  
    gpio_config_t buttonConfig;
    buttonConfig.pin_bit_mask   = (u_int64_t)1 << BUTTON_PIN;
    buttonConfig.mode           = GPIO_MODE_INPUT;
    buttonConfig.pull_down_en   = GPIO_PULLDOWN_DISABLE;
    buttonConfig.pull_up_en     = GPIO_PULLUP_ENABLE;
    buttonConfig.intr_type      = GPIO_INTR_NEGEDGE;
    gpio_config(&buttonConfig);
    
    // Create interrupt and assign the button to it
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);


    /**
     * Start both samplingAnalyzation & samplingTask with different
     * priorites to combat race-conditions.
    */
    xTaskCreate(samplingAnalyzation,    ANALYZATION_NAME,   ANALYZATION_STACK_SIZE,    NULL, 10,  NULL);
    xTaskCreate(samplingTask,           SAMPLING_TASK_NAME, SAMPLING_STACK_SIZE,       NULL, 5,   NULL);
    xTaskCreate(led_task,               LED_TASK_NAME,      LED_TASK_STACK_SIZE,       NULL, 3,   NULL); 

    vTaskDelay(pdMS_TO_TICKS(10000));
}

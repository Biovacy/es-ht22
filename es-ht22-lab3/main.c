/**
 * Authers:     Hampus Oxenholt (3lax) & Ingvar Pétursson (machine)
 * Date:        2022-10-28
 */

#include <esp_task_wdt.h>
#include <stdio.h>
#include "driver/gpio.h"

#include "sampler.h"
#include "notes.h" 
#include "pins.h"

#define PUSH_TIME 250000
#define BUTTON_PIN 18

int lastPushTime = 0; 
volatile int count = 0; 

// Button 
static void buttonPress_handler(void *arg) {
    gpio_intr_disable(18); 

    if (esp_timer_get_time() - lastPushTime > PUSH_TIME) {
        count++;

        // Make count only work between the values 0 -> 5 
        if (count == 6) {
            count = 0;
        }

        lastPushTime = esp_timer_get_time();
    }



    gpio_intr_enable(18);
}

void app_main()
{
    // Define the button
    initLEDPin(16);
    initLEDPin(12);
    initButtonPin(18);
    initButtonPin(BUTTON_PIN);
    gpio_isr_handler_add(BUTTON_PIN, buttonPress_handler, NULL);

    printf("--- Start!\n");
    initSampling();
    flashLEDs();
    startSampling();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));

        float fr = getFrequency();
        
        printf("Frequency is %.2f\n", fr);
        
        char note[] = "1234";
        
        freq2note(fr, note);
        
        // printf("Note is %s\n", note);

        printf("%i\n", count);

        switch (count)
        {
        case 0:
            printf("%f\n", fr);
            if(fr < 300){
                flashLED(16);
                printf("low\n");
            }else if(fr > 350){
                flashLED(12);
                printf("high\n");
            }else{
                flashLEDs();
                printf("perrfect\n");
            }

            break;

        case 1:

            if(fr < 220){
                flashLED(16);
            }else if(fr > 270){
                flashLED(12);
            }else{
                flashLEDs();
            }

            break;

        case 2:

            if(fr < 170){
                flashLED(16);
            }else if(fr > 210){
                flashLED(12);
            }else{
                flashLEDs();
            }

            break;
        
        case 3:

            if(fr < 120){
                flashLED(16);
            }else if(fr > 170){
                flashLED(12);
            }else{
                flashLEDs();
            }

            break;

        case 4:

            if(fr < 90){
                flashLED(16);
            }else if(fr > 160){
                flashLED(12);
            }else{
                flashLEDs();
            }

            break;

        case 5:

            if(fr < 60){
                flashLED(16);
            }else if(fr > 110){
                flashLED(12);
            }else{
                flashLEDs();
            }

            break;

        default:
            break;
        }

        resetSampling();
    }
}


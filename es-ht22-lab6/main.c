/**
 * Auther:  Hampus Oxenholt & Ingvar Pétursson
 * Date:    {unknown}
 */

#include <stdio.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>
#include <esp_task_wdt.h>
#include <driver/gpio.h>

#include "single_linked_list.h"

#define LED_PIN_LEVEL_UP     12
#define LED_PIN_LEVEL_MIDDLE 14
#define LED_PIN_LEVEL_DOWN   27
#define BUTTON_PIN 26

#define LEVEL_UP     2
#define LEVEL_MIDDLE 1
#define LEVEL_LOW    0

#define PUSH_TIME_US 250000 // 250 ms

#define TIME_DELAY 5000

// Used to represent a travel need of a passenger.
struct travel_need {
    int     origin;
    int     destination;
    bool    passenger;
};

// Check if the fare is complete
bool completedFare = true;

// Create a list with only the travel needs
struct singleLinkedList list;

// Used to not allow button pushes that are too close to each other in time
static volatile uint64_t lastPush = -PUSH_TIME_US;

//Just a counter keeping track of which travel need is next to process.
static volatile int travel_need_counter = 0;

// This data structure holds information about 
static volatile struct travel_need travel_needs[50];

// Declear functions
void stateLevelUp(struct travel_need *travelRequest);
void stateLevelMiddle(struct travel_need *travelRequest);
void stateLevelDown(struct travel_need *travelRequest);

// Pointer to current state
volatile void (*currentState)(struct travel_need *) = NULL;

void *ptrStateLevelUp     = &stateLevelUp;
void *ptrStateLevelMiddle = &stateLevelMiddle;
void *ptrStateLevelDown   = &stateLevelDown;

void offAllLeds() {
    gpio_set_level(LED_PIN_LEVEL_UP, 0);
    gpio_set_level(LED_PIN_LEVEL_MIDDLE, 0);
    gpio_set_level(LED_PIN_LEVEL_DOWN, 0);   
}

void goToStop(int ledPinSource, int ledPinDestination) {
    gpio_set_level(ledPinSource, 0);
    vTaskDelay( pdMS_TO_TICKS(TIME_DELAY) );
    offAllLeds();
    gpio_set_level(ledPinDestination, 1);
}

void goToIntermediateStop() {
    offAllLeds();
    vTaskDelay( pdMS_TO_TICKS(TIME_DELAY) );
}

// State functions
void stateLevelUp(struct travel_need *travelRequest) {

    printf("Arrived at %d floor\n", 2);
    if (travelRequest->passenger == false) {

        if (travelRequest->origin == LEVEL_UP) {
            
            // Put the passenger on the elevator.
            printf("Picked up passenger at floor %d\n", travelRequest->origin);
            travelRequest->passenger = true; 
            vTaskDelay( pdMS_TO_TICKS(TIME_DELAY) );

            if (travelRequest->destination == LEVEL_MIDDLE) {
                // Moves elevator to middle
                goToStop(LED_PIN_LEVEL_UP, LED_PIN_LEVEL_MIDDLE);
                currentState = ptrStateLevelMiddle;
            }
            else {
                // Moves elevator to middle but as a 
                // intermidiate stop. 

                goToIntermediateStop();
                currentState = ptrStateLevelMiddle;
            }

        }

        else {
            
            if (travelRequest->origin == LEVEL_MIDDLE) {
                // Moves elevator to middle

                goToStop(LED_PIN_LEVEL_UP, LED_PIN_LEVEL_MIDDLE);
                currentState = ptrStateLevelMiddle;
            }
            else {
                // Moves elevator to middle but as a 
                // intermidiate stop. 

                goToIntermediateStop();
                currentState = ptrStateLevelMiddle;
            }
            
        }

    }
    else {

        if (travelRequest->destination == LEVEL_UP) {
            // On correct level
            vTaskDelay( pdMS_TO_TICKS(TIME_DELAY) );
            printf("Put down passenger at floor %d\n", travelRequest->destination);
            completedFare = true;
        }
        else if (travelRequest->destination == LEVEL_MIDDLE) {
            // Moves elevator to middle but as a 
            // intermidiate stop. 
            
            goToStop(LED_PIN_LEVEL_UP, LED_PIN_LEVEL_MIDDLE);
            currentState = ptrStateLevelMiddle;
        }
        else {
            goToIntermediateStop();
            currentState = ptrStateLevelMiddle;
        }

    }

}

void stateLevelMiddle(struct travel_need *travelRequest) {

    printf("Arrived at %d floor\n", 1);
    if (travelRequest->passenger == false) {

        if (travelRequest->origin == LEVEL_MIDDLE) {
            printf("Picked up passenger at floor %d\n", travelRequest->origin);
            travelRequest->passenger = true;
            vTaskDelay( pdMS_TO_TICKS(TIME_DELAY) );

            if (travelRequest->destination == LEVEL_UP) {
                goToStop(LED_PIN_LEVEL_MIDDLE, LED_PIN_LEVEL_UP);
                currentState = ptrStateLevelUp;
            }
            else {
                goToStop(LED_PIN_LEVEL_MIDDLE, LED_PIN_LEVEL_DOWN);
                currentState = ptrStateLevelDown;
            }

        }
        else  if (travelRequest->origin == LEVEL_UP) {
            goToStop(LED_PIN_LEVEL_MIDDLE, LED_PIN_LEVEL_UP);
            currentState = ptrStateLevelUp;
        }
        else { // travelRequest->origin == LEVEL_LOW
            goToStop(LED_PIN_LEVEL_MIDDLE, LED_PIN_LEVEL_DOWN);
            currentState = ptrStateLevelDown;
        }

    }
    else {

        if (travelRequest->destination == LEVEL_MIDDLE) {
            // Put passenger down
            vTaskDelay( pdMS_TO_TICKS(TIME_DELAY) );
            printf("Put down passenger at floor %d\n", travelRequest->destination);
            completedFare = true;
        }
        else if (travelRequest->destination == LEVEL_UP) {
            goToStop(LED_PIN_LEVEL_MIDDLE, LED_PIN_LEVEL_UP);
            currentState = ptrStateLevelUp;
        }
        else {
            goToStop(LED_PIN_LEVEL_MIDDLE, LED_PIN_LEVEL_DOWN);
            currentState = ptrStateLevelDown;
        }

    }

}

void stateLevelDown(struct travel_need *travelRequest) {

    printf("Arrived at %d floor\n", 0);
    if (travelRequest->passenger == false) {

        if (travelRequest->origin == LEVEL_LOW) {
            
            // Put the passenger on the elevator.
            printf("Picked up passenger at floor %d\n", travelRequest->origin);
            travelRequest->passenger = true; 
            vTaskDelay( pdMS_TO_TICKS(TIME_DELAY) );

            if (travelRequest->destination == LEVEL_MIDDLE) {
                // Moves elevator to middle
                goToStop(LED_PIN_LEVEL_DOWN, LED_PIN_LEVEL_MIDDLE);
                currentState = ptrStateLevelMiddle;
            }
            else {
                // Moves elevator to middle but as a 
                // intermidiate stop. 

                goToIntermediateStop();
                currentState = ptrStateLevelMiddle;
            }

        }

        else {
            
            if (travelRequest->origin == LEVEL_MIDDLE) {
                // Moves elevator to middle

                goToStop(LED_PIN_LEVEL_DOWN, LED_PIN_LEVEL_MIDDLE);
                currentState = ptrStateLevelMiddle;
            }
            else {
                // Moves elevator to middle but as a 
                // intermidiate stop. 

                goToIntermediateStop();
                currentState = ptrStateLevelMiddle;
            }
            
        }

    }
    else {

        if (travelRequest->destination == LEVEL_LOW) {
            // On correct level
            vTaskDelay( pdMS_TO_TICKS(TIME_DELAY) );
            printf("Put down passenger at floor %d\n", travelRequest->destination);
            completedFare = true;
        }
        else if (travelRequest->destination == LEVEL_MIDDLE) {
            // Moves elevator to middle but as a 
            // intermidiate stop. 

            goToStop(LED_PIN_LEVEL_DOWN, LED_PIN_LEVEL_MIDDLE);
            currentState = ptrStateLevelMiddle;
        }
        else {
            goToIntermediateStop();
            currentState = ptrStateLevelMiddle;
        }

    }

}

// This function is called when button is pushed
static void handle_push(void *arg) {

    // Disable interrupts
    gpio_intr_disable(BUTTON_PIN);

    // Get the current time 
    uint64_t now = esp_timer_get_time();


    // If enough time passed, we should consider this event as a genuine push
    if ((now - lastPush) > PUSH_TIME_US) {
       
        lastPush = now;

        // Get next travel need from list and do something with it
        // struct travel_need current_travel_need = travel_needs[travel_need_counter];

        addElementSingleLinkedList(&list, &travel_needs[travel_need_counter]);

        ets_printf("Loaded next travel\n");

        //Increase travel need counter
        travel_need_counter++;
        travel_need_counter %= 50; 

    } // else ignore



    // Re-enable interrupts
    gpio_intr_enable(BUTTON_PIN);
}

void app_main() {

    // Init list
    initSingleLinkedList(&list);

    // Set current pointer
    currentState = ptrStateLevelMiddle;

    //Initialize travel needs (50 randomly generated travel needs)
    travel_needs[ 0].origin = 2; travel_needs[ 0].destination = 1; travel_needs[ 0].passenger = false;
    travel_needs[ 1].origin = 1; travel_needs[ 1].destination = 2; travel_needs[ 1].passenger = false;
    travel_needs[ 2].origin = 1; travel_needs[ 2].destination = 2; travel_needs[ 2].passenger = false;
    travel_needs[ 3].origin = 0; travel_needs[ 3].destination = 2; travel_needs[ 3].passenger = false;
    travel_needs[ 4].origin = 2; travel_needs[ 4].destination = 1; travel_needs[ 4].passenger = false;
    travel_needs[ 5].origin = 0; travel_needs[ 5].destination = 2; travel_needs[ 5].passenger = false;
    travel_needs[ 6].origin = 1; travel_needs[ 6].destination = 2; travel_needs[ 6].passenger = false;
    travel_needs[ 7].origin = 1; travel_needs[ 7].destination = 0; travel_needs[ 7].passenger = false;
    travel_needs[ 8].origin = 0; travel_needs[ 8].destination = 1; travel_needs[ 8].passenger = false;
    travel_needs[ 9].origin = 1; travel_needs[ 9].destination = 0; travel_needs[ 9].passenger = false;
    travel_needs[10].origin = 1; travel_needs[10].destination = 2; travel_needs[10].passenger = false;
    travel_needs[11].origin = 0; travel_needs[11].destination = 1; travel_needs[11].passenger = false;
    travel_needs[12].origin = 0; travel_needs[12].destination = 2; travel_needs[12].passenger = false;
    travel_needs[13].origin = 0; travel_needs[13].destination = 1; travel_needs[13].passenger = false;
    travel_needs[14].origin = 0; travel_needs[14].destination = 2; travel_needs[14].passenger = false;
    travel_needs[15].origin = 0; travel_needs[15].destination = 1; travel_needs[15].passenger = false;
    travel_needs[16].origin = 2; travel_needs[16].destination = 1; travel_needs[16].passenger = false;
    travel_needs[17].origin = 2; travel_needs[17].destination = 1; travel_needs[17].passenger = false;
    travel_needs[18].origin = 1; travel_needs[18].destination = 0; travel_needs[18].passenger = false;
    travel_needs[19].origin = 2; travel_needs[19].destination = 1; travel_needs[19].passenger = false;
    travel_needs[20].origin = 1; travel_needs[20].destination = 0; travel_needs[20].passenger = false;
    travel_needs[21].origin = 0; travel_needs[21].destination = 1; travel_needs[21].passenger = false;
    travel_needs[22].origin = 1; travel_needs[22].destination = 2; travel_needs[22].passenger = false;
    travel_needs[23].origin = 0; travel_needs[23].destination = 2; travel_needs[23].passenger = false;
    travel_needs[24].origin = 2; travel_needs[24].destination = 1; travel_needs[24].passenger = false;
    travel_needs[25].origin = 1; travel_needs[25].destination = 0; travel_needs[25].passenger = false;
    travel_needs[26].origin = 1; travel_needs[26].destination = 2; travel_needs[26].passenger = false;
    travel_needs[27].origin = 0; travel_needs[27].destination = 2; travel_needs[27].passenger = false;
    travel_needs[28].origin = 1; travel_needs[28].destination = 0; travel_needs[28].passenger = false;
    travel_needs[29].origin = 1; travel_needs[29].destination = 2; travel_needs[29].passenger = false;
    travel_needs[30].origin = 0; travel_needs[30].destination = 1; travel_needs[30].passenger = false;
    travel_needs[31].origin = 1; travel_needs[31].destination = 2; travel_needs[31].passenger = false;
    travel_needs[32].origin = 0; travel_needs[32].destination = 2; travel_needs[32].passenger = false;
    travel_needs[33].origin = 0; travel_needs[33].destination = 2; travel_needs[33].passenger = false;
    travel_needs[34].origin = 1; travel_needs[34].destination = 2; travel_needs[34].passenger = false;
    travel_needs[35].origin = 2; travel_needs[35].destination = 1; travel_needs[35].passenger = false;
    travel_needs[36].origin = 0; travel_needs[36].destination = 2; travel_needs[36].passenger = false;
    travel_needs[37].origin = 1; travel_needs[37].destination = 0; travel_needs[37].passenger = false;
    travel_needs[38].origin = 0; travel_needs[38].destination = 2; travel_needs[38].passenger = false;
    travel_needs[39].origin = 2; travel_needs[39].destination = 1; travel_needs[39].passenger = false;
    travel_needs[40].origin = 0; travel_needs[40].destination = 1; travel_needs[40].passenger = false;
    travel_needs[41].origin = 0; travel_needs[41].destination = 1; travel_needs[41].passenger = false;
    travel_needs[42].origin = 0; travel_needs[42].destination = 1; travel_needs[42].passenger = false;
    travel_needs[43].origin = 1; travel_needs[43].destination = 0; travel_needs[43].passenger = false;
    travel_needs[44].origin = 0; travel_needs[44].destination = 2; travel_needs[44].passenger = false;
    travel_needs[45].origin = 2; travel_needs[45].destination = 1; travel_needs[45].passenger = false;
    travel_needs[46].origin = 2; travel_needs[46].destination = 1; travel_needs[46].passenger = false;
    travel_needs[47].origin = 2; travel_needs[47].destination = 1; travel_needs[47].passenger = false;
    travel_needs[48].origin = 0; travel_needs[48].destination = 2; travel_needs[48].passenger = false;
    travel_needs[49].origin = 1; travel_needs[49].destination = 0; travel_needs[49].passenger = false;

    /*
    while (1) {

        tmpTravel = removeLastElementSingleLinkedList(&list);

        if (tmpTravel == NULL) {
            break; 
        }

        printf("O: %d\t D: %d\n", tmpTravel->origin, tmpTravel->destination);
    }
    */

    // Configure pin LED_PIN_LEVEL_UP as output 
    gpio_config_t ledLevelUp;
    ledLevelUp.pin_bit_mask  = (u_int64_t)1 << LED_PIN_LEVEL_UP;
    ledLevelUp.mode          = GPIO_MODE_OUTPUT;
    gpio_config(&ledLevelUp);

    // Configure pin LED_PIN_LEVEL_MIDDLE as output 
    gpio_config_t ledLevelMiddle;
    ledLevelMiddle.pin_bit_mask  = (u_int64_t)1 << LED_PIN_LEVEL_MIDDLE;
    ledLevelMiddle.mode          = GPIO_MODE_OUTPUT;
    gpio_config(&ledLevelMiddle);

    // Configure pin LED_PIN_LEVEL_DOWN as output 
    gpio_config_t ledLevelDown;
    ledLevelDown.pin_bit_mask  = (u_int64_t)1 << LED_PIN_LEVEL_DOWN;
    ledLevelDown.mode          = GPIO_MODE_OUTPUT;
    gpio_config(&ledLevelDown);

    // Configure pin BUTTON_PIN as input, pull up and with interrupts on the negative edge
    gpio_config_t buttonConfig;
    buttonConfig.pin_bit_mask   = (u_int64_t)1 << BUTTON_PIN;
    buttonConfig.mode           = GPIO_MODE_INPUT;
    buttonConfig.pull_down_en   = GPIO_PULLDOWN_DISABLE;
    buttonConfig.pull_up_en     = GPIO_PULLUP_ENABLE;
    buttonConfig.intr_type      = GPIO_INTR_NEGEDGE;
    gpio_config(&buttonConfig);

    // Activate the interrupts for the GPIOs
    esp_err_t res;
    res = gpio_install_isr_service(0);
    ESP_ERROR_CHECK(res);

    // Add a handler to the ISR for pin BUTTON_PIN
    res = gpio_isr_handler_add(BUTTON_PIN, handle_push, NULL);
    ESP_ERROR_CHECK(res);

    gpio_set_level(LED_PIN_LEVEL_UP,     GPIO_MODE_INPUT);
    gpio_set_level(LED_PIN_LEVEL_MIDDLE, GPIO_MODE_INPUT);
    gpio_set_level(LED_PIN_LEVEL_DOWN,   GPIO_MODE_INPUT);

    struct travel_need travelRequest;
    struct travel_need *tmpTravelRequest; 
    tmpTravelRequest = NULL;
    
    // void (*ptrStateLevelUp)(struct travel_need *) = &stateLevelUp;
    // (*ptrStateLevelUp)(&tmpTravel);

    // This is where you most likely put your main elevator code. 
    while(1) {

        if (completedFare == true) {
            tmpTravelRequest = NULL; 

            while (tmpTravelRequest == NULL) {
                tmpTravelRequest = removeLastElementSingleLinkedList(&list);
                if (tmpTravelRequest == NULL) {
                    printf("No fare in list\n");
                    vTaskDelay( pdMS_TO_TICKS(5 * 1000) );
                }
            }
            
            travelRequest.destination   = tmpTravelRequest->destination;
            travelRequest.origin        = tmpTravelRequest->origin;
            travelRequest.passenger     = tmpTravelRequest->passenger; 

            printf("Next passenger \n\torigin: %d\n\tdestination: %d\n", travelRequest.origin, travelRequest.destination);

            completedFare = false;
        }

        // Go to state
        (*currentState)(&travelRequest);

    }
       
}

#include "pins.h"
#include "driver/gpio.h"

#define BLUELEDPIN 12
#define YELLOWLEDPIN 14

#define BLUEBUTTON 27
#define YELLOWBUTTON 26

void initPins() {
    gpio_config_t configLedA, configLedB, configButtonA, configButtonB;

    configLedA.pin_bit_mask = (u_int64_t)1 << BLUELEDPIN;
    configLedA.mode = GPIO_MODE_OUTPUT;
    configLedA.pull_down_en = 0;
    configLedA.pull_up_en = 0;

    configLedB.pin_bit_mask = (u_int64_t)1 << YELLOWLEDPIN;
    configLedB.mode = GPIO_MODE_OUTPUT;
    configLedB.pull_down_en = 0;
    configLedB.pull_up_en = 0;

    configButtonA.pin_bit_mask = (u_int64_t)1 << BLUEBUTTON;
    configButtonA.mode = GPIO_MODE_INPUT;
    configButtonA.pull_down_en = 0;
    configButtonA.pull_up_en = 0;

    configButtonB.pin_bit_mask = (u_int64_t)1 << YELLOWBUTTON;
    configButtonB.mode = GPIO_MODE_INPUT;
    configButtonB.pull_down_en = 0;
    configButtonB.pull_up_en = 0;

    esp_err_t resultConfigLedA = gpio_config(&configLedA);
    esp_err_t resultConfigLedB = gpio_config(&configLedB);

    esp_err_t resultConfigButtonA = gpio_config(&configButtonA);
    esp_err_t resultConfigButtonB = gpio_config(&configButtonB);

    // Debug :) 
    /*
    if (resultConfigLedA == ESP_OK) {
        printf("%s\n", "resultConfigLedA");
    }

    if (resultConfigLedB == ESP_OK) {
        printf("%s\n", "resultConfigLedB");
    }

    if (resultConfigButtonA == ESP_OK) {
        printf("%s\n", "resultConfigButtonA");
    }

    if (resultConfigButtonB == ESP_OK) {
        printf("%s\n", "resultConfigButtonB");
    }
    */
}

void setLEDA(uint8_t level) {

    if (level) {
        gpio_set_level(BLUELEDPIN, 1);
    }
    else {
        gpio_set_level(BLUELEDPIN, 0);
    }

}

void setLEDB(uint8_t level) {

    if (level) {
        gpio_set_level(YELLOWLEDPIN, 1);
    }
    else {
        gpio_set_level(YELLOWLEDPIN, 0);
    }

}

uint8_t isButtonAPressed() {
    return gpio_get_level(BLUEBUTTON);
}

uint8_t isButtonBPressed() {
    return gpio_get_level(YELLOWBUTTON);
}
#include <esp_task_wdt.h>
#include "driver/gpio.h"
#include "pins.h"

/* initialises the LED pin */
void initLEDPin(uint8_t pinN) {
    gpio_config_t configLed;

    configLed.pin_bit_mask = (u_int64_t)1 << pinN;
    configLed.mode = GPIO_MODE_OUTPUT;
    configLed.pull_down_en = 1;
    configLed.pull_up_en = 0;

    gpio_config(&configLed);
}

/* initialises the button pin */
void initButtonPin(uint8_t pinN) {
    gpio_config_t configButton;

    configButton.pin_bit_mask = (u_int64_t)1 << pinN;
    configButton.mode = GPIO_MODE_INPUT;
    configButton.pull_down_en = GPIO_PULLUP_DISABLE;
    configButton.pull_up_en = GPIO_PULLUP_ENABLE;
    configButton.intr_type = GPIO_INTR_NEGEDGE;

    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3); 
    gpio_config(&configButton); 
}

/* switches LED on if level!=0 or off if level==0*/
void setLED(uint8_t pinN, uint8_t level) {
    gpio_set_level(pinN, level); 
}

void flashLED(int pin){
    setLED(pin, 1);
    vTaskDelay(250);
    setLED(pin, 0);
    vTaskDelay(250);
    setLED(pin, 1);
    vTaskDelay(250);
    setLED(pin, 0);
    vTaskDelay(250);
}

void flashLEDs(){
    setLED(16, 1);
    setLED(12, 1);
    vTaskDelay(250);
    setLED(16, 0);
    setLED(12, 0);
    vTaskDelay(250);
    setLED(16, 1);
    setLED(12, 1);
    vTaskDelay(250);
    setLED(16, 0);
    setLED(12, 0);
    vTaskDelay(250);
}
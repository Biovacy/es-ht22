/**
 * Auther:  Hampus Oxenholt & Ingvar Pétursson
 * Date:    2022-09-07
 */

#include "wait.h"
#include "pins.h"
#include "random.h"

#include <stdio.h>

void app_main()
{
    initPins();

    while (1) {
        // signal that the game is about to start
        // you can flash LEDs in a certain way for it

        unsigned int short_delay = 100;
        unsigned int long_delay  = 250;

        for (int i = 0; i < 3; i++) {
            setLEDA(1);
            setLEDB(1);
            waitMs(short_delay);

            setLEDA(0);
            setLEDB(0);
            waitMs(short_delay);
        }

        for (int i = 0; i < 3; i++) {
            setLEDA(1);
            setLEDB(1);
            waitMs(long_delay);

            setLEDA(0);
            setLEDB(0);
            waitMs(long_delay);
        }

        for (int i = 0; i < 3; i++) {
            setLEDA(1);
            setLEDB(1);
            waitMs(short_delay);

            setLEDA(0);
            setLEDB(0);
            waitMs(short_delay);
        }
        
        // switch both LEDs off
        
        setLEDA(0);
        setLEDB(0);

        // get a random duration between 3 and 5 seconds
        
        uint8_t random_duration = getRandommsecs(3, 5);

        // wait that random duration// switch both LEDs ON
        
        waitMs(random_duration * 1000);

        // Checks for cheaters
        // The cheating system works by checking the buttons right
        // before the leds are turned on, if the button is pressed
        // that indicates that the player is pressing / holding
        // the button before the leds are turned on.
        uint8_t ACheat = !isButtonAPressed();
        uint8_t BCheat = !isButtonBPressed();

        setLEDA(1);
        setLEDB(1);

        // Check who is the winner
        uint8_t winner = 0;

        while (!winner) {
            // check if either button A or B are pressed
            // if any is pressed, set winner to 1 for A or 2 for B
            
            if (!isButtonAPressed() && !ACheat) {
                winner = 1;
            }
            if (!isButtonBPressed() && !BCheat) {
                winner = 2; 
            }
            if (ACheat && BCheat) {
                // No one is the winner, because both cheated.
                winner = 3; 
                printf("%s\n", "Both cheated");
            }

        }

        // if A wins, flash LED A and switch off B

        if (winner == 1) {

            setLEDB(0);
            for (int i = 0; i < 10; i++) {
                setLEDA(1);
                waitMs(25);
                setLEDA(0);
                waitMs(25);
            }

        }

        // if B wins, flash LED B and switch off A
        
        else if (winner == 2) {

            setLEDA(0);
            for (int i = 0; i < 10; i++) {
                setLEDB(1);
                waitMs(25);
                setLEDB(0);
                waitMs(25);
            }

        }

        // switch off both A and B and wait for some time to restart the game
        setLEDA(0);
        setLEDB(0);
        waitMs(1000 * 5);
    
    }
}

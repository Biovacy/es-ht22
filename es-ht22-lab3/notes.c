#include <stdio.h>
#include <math.h>
#include "notes.h"

void freq2note(float freq, char *stringbuff) {

    int n = 12 * log2f(freq / 440);  
    n = n % 12;

    if (n == 0) {
        stringbuff = "A\0";
    }
    if (n == 1 || n == -11) {
        stringbuff = "A#\0";
    }
    if (n == 2 || n == -10) {
        stringbuff = "B\0";
    }
    if (n == 3 || n == -9) {
        stringbuff = "C\0";
    }
    if (n == 4 || n == -8) {
        stringbuff = "C#\0";
    }
    if (n == 5 || n == -7) {
        stringbuff = "D\0";
    }
    if (n == 6 || n == -6) {
        stringbuff = "D#\0";
    }
    if (n == 7 || n == -5) {
        stringbuff = "E\0";
    }
    if (n == 8 || n == -4) {
        stringbuff = "F\0";
    }
    if (n == 9 || n == -3) {
        stringbuff = "F#\0";
    }
    if (n == 10 || n == -2) {
        stringbuff = "G\0";
    }
    if (n == 11 || n == -1) {
        stringbuff = "G#\0";
    }

    printf("%s\n", stringbuff);

}
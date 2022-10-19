/**
 * Author:  Hampus Oxenholt & Ingvar Pétursson
 * Date:    2022-10-14
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>
#include "circular_buffer.h"

/*
 * Change the value of BUFFER_SIZE if you want to change the size of the buffer. 
 */ 
#define BUFFER_SIZE 4

void app_main() {

    // Some code to help you get started
    struct circularBuffer buffer;
    int *buffer_data = (int*) malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);

    // Black box test
    
    // Test 1.
    printf("%d\n", addElement(&buffer, 10) );
    printf("%d\n", removeHead(&buffer) );

    // Test 2.
    printf("%d\n", addElement(&buffer, 6) );
    printf("%d\n", addElement(&buffer, 9) );
    printf("%d\n", removeHead(&buffer) );
    printf("%d\n", removeHead(&buffer) );
    
    // Test 3.
    printf("Add: %d\n", addElement(&buffer, 4) );
    printf("Add: %d\n", addElement(&buffer, 1) );
    printf("Add: %d\n", addElement(&buffer, 2) );
    printf("Add: %d\n", addElement(&buffer, 3) );
    printf("Remove: %d\n", removeHead(&buffer) );
    printf("Remove: %d\n", removeHead(&buffer) );
    printf("Remove: %d\n", removeHead(&buffer) );
    printf("Remove: %d\n", removeHead(&buffer) );
    
    // Test 4.
    printf("Add: %d\n", addElement(&buffer, 0) );
    printf("Add: %d\n", addElement(&buffer, 1) );
    printf("Add: %d\n", addElement(&buffer, 2) );
    printf("Add: %d\n", addElement(&buffer, 3) );
    printf("Add: %d\n", addElement(&buffer, 4) );
    printBuffer(&buffer);
    
    // Test 5.
    printf("%d\n", addElement(&buffer, 10) );
    printf("%d\n", removeHead(&buffer) );
    printf("%d\n", addElement(&buffer, 20) );
    printf("%d\n", removeHead(&buffer) );
    printf("%d\n", addElement(&buffer, 30) );
    printf("%d\n", removeHead(&buffer) );
    printf("%d\n", addElement(&buffer, 40) );
    printf("%d\n", removeHead(&buffer) );
    printf("%d\n", addElement(&buffer, 50) );
    printf("%d\n", removeHead(&buffer) );
    
    // Test 6.
    printf("%d\n", contains(&buffer, 0));
    
    // Test 7.
    printf("%d\n", addElement(&buffer, 50) );
    printf("%d\n", contains(&buffer, 50));
    
    // Test 8.
    printf("%d\n", addElement(&buffer, 30) );
    printf("%d\n", addElement(&buffer, 60) );
    printf("%d\n", contains(&buffer, 60));
    
    // Test 9.
    printf("Add: %d\n", addElement(&buffer, 0) );
    printf("Add: %d\n", addElement(&buffer, 1) );
    printf("Add: %d\n", addElement(&buffer, 2) );
    printf("Add: %d\n", addElement(&buffer, 3) );
    printf("%d\n", contains(&buffer, 3));


    // White box test :)
    
    // initCircularBuffer
    printf("Max length: %d\n", buffer.maxLength);
    printf("Head: %d\n", buffer.head);
    printf("Tail: %d\n", buffer.tail);
    

    // addElements
    // Test 1.
    printf("%d\n", addElement(&buffer, 3) );
    printBuffer(&buffer);
    
    // Test 2.
    buffer.tail = 1;
    buffer.head = 2;
    printf("%d\n", addElement(&buffer, 3));
    printBuffer(&buffer);
    
    // Test 3.
    buffer.tail = 1;
    buffer.head = 2;
    printf("%d\n", addElement(&buffer, 3));
    printf("%d\n", addElement(&buffer, 5));
    printBuffer(&buffer);
    
    // Test 4.
    // buffer.tail = 2;
    // buffer.head = 3;
    printf("Add: %d\n", addElement(&buffer, 0) );
    printf("Add: %d\n", addElement(&buffer, 1) );
    printf("Add: %d\n", addElement(&buffer, 2) );
    printf("Add: %d\n", addElement(&buffer, 3) );
    printBuffer(&buffer);
    

    // removeValue
    // Test 1.
    printf("Add: %d\n", addElement(&buffer, 0) );
    printf("Number of elements %d\n", buffer.numElements);
    printf("Remove %d\n", removeValue(&buffer, 0));
    printf("Number of elements %d\n", buffer.numElements);
    printBuffer(&buffer);
    
    // Test 2.
    printf("Add: %d\n", addElement(&buffer, 0) );
    printf("Add: %d\n", addElement(&buffer, 1) );
    printf("Add: %d\n", addElement(&buffer, 2) );
    printf("Add: %d\n", addElement(&buffer, 3) );
    printf("Remove %d\n", removeValue(&buffer, 2));
    printBuffer(&buffer);
    
    // Test 3. 
    printf("Remove %d\n", removeValue(&buffer, 0));
    printBuffer(&buffer);

    // removeHead
    // Test 1.
    printf("Add: %d\n", addElement(&buffer, 0) );
    printf("removeHead %d\n", removeHead(&buffer));
    printBuffer(&buffer);
    
    // Test 2.
    printf("Add: %d\n", addElement(&buffer, 0) );
    printf("Add: %d\n", addElement(&buffer, 1) );
    printf("removeHead %d\n", removeHead(&buffer));
    printBuffer(&buffer);
    
    // Test 3.
    printf("Add: %d\n", addElement(&buffer, 0) );
    printf("Add: %d\n", addElement(&buffer, 1) );
    printf("Add: %d\n", addElement(&buffer, 2) );
    printf("removeHead %d\n", removeHead(&buffer));
    printBuffer(&buffer);
    
    // Test 4. 
    printf("removeHead %d\n", removeHead(&buffer));
    printBuffer(&buffer);
    
    // ets_delay_us(2000000ul);
    
    free(buffer_data);
}

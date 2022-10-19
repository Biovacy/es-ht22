#include <stdio.h>
#include <limits.h>
#include "circular_buffer.h"

/*
 * Initialize an empty buffer.
 */
void initCircularBuffer(struct circularBuffer* bufferPtr, int* data, int maxLength) {
  bufferPtr->data = data;
  bufferPtr->head = 0;
  bufferPtr->tail = -1;
  bufferPtr->maxLength = maxLength;
  bufferPtr->numElements = 0;
}

/**
 * This function returns the value stored at the index
 * 
 * This function should return:
 *  - The value at the position if no error was detected.
 *  - INT_MIN if invalid index was given. 
*/
int peek(struct circularBuffer* bufferPtr, int index) {
  if (index >= bufferPtr->maxLength) {
    return INT_MIN;
  } 

  return bufferPtr->data[index];
}

/* 
 * This function should check if the buffer pointed to by bufferPtr
 * contains one or more elements with the value specified by the 
 * 'value' argument.
 *
 * The function should return:
 *  - 'value' if the an element with the argument value was found in the queue.
 *  - INT_MIN (defined in limits.h) if value was not found.
 */ 
int contains(struct circularBuffer* bufferPtr, int value) {
  
  for (int i = bufferPtr->head; i <= bufferPtr->tail; i++) {

    i %= bufferPtr->maxLength; 

    if (bufferPtr->data[i] == value) {
      return value;
      break;
    }

  }

  return INT_MIN;

}

/*
 * This function should add the value specified by the 'value' 
 * argument at the tail of the buffer.
 *
 * The function should return:
 *  - 'value' if the value was successfully added to the queue.
 *  - INT_MIN (defined in limits.h) if the value was not added.
 */
int addElement(struct circularBuffer* bufferPtr, int value) {

  bufferPtr->tail = (bufferPtr->tail + 1) % bufferPtr->maxLength;
  bufferPtr->data[ bufferPtr->tail ] = value;

  // If the function had to overwrite an old value, the header
  // head will be moved to point to the new oldest element.
  if (bufferPtr->numElements == bufferPtr->maxLength) {
    bufferPtr->head = (bufferPtr->head + 1) % bufferPtr->maxLength;
  }
  else {
    bufferPtr->numElements++; 
  }
  
  return value;
}

int removeValue(struct circularBuffer* bufferPtr, int value) {
  
  // Check if there is any elements
  if (bufferPtr->numElements <= 0) {
    return INT_MIN;
  }

  int newArray[bufferPtr->maxLength];

  int counter = 0;
  for (int i = bufferPtr->head; i < bufferPtr->head + bufferPtr->numElements; i++) {
    int index = i % bufferPtr->maxLength;

    if (bufferPtr->data[ index ] != value) {
      newArray[counter] = bufferPtr->data[ index ]; 
      counter++;
    }
  }

  if (bufferPtr->numElements == counter) {
    return INT_MIN;
  }

  bufferPtr->numElements = counter;

  for (int i = 0; i < counter; i++) {
    bufferPtr->data[i] = newArray[i];
  }

  bufferPtr->tail = counter - 1;
  bufferPtr->head = 0;

  return value;
}

/* 
 * Remove the oldest element, which is at the head of the queue. 
 * 
 * The function should return:
 *   - 'value' if the head element was successfully removed
 *   - INT_MIN (defined in limits.h) if no element was removed (i.e., the
 *     queue was empty when the function was called.       
 */
int removeHead(struct circularBuffer* bufferPtr) {

  if (bufferPtr->numElements == 0) {
    return INT_MIN;
  }

  int tmpRemovedValue = bufferPtr->data[ bufferPtr->head ]; 

  bufferPtr->head = (bufferPtr->head + 1) % bufferPtr->maxLength;

  bufferPtr->numElements--;

  return tmpRemovedValue; 
}

/* 
 * Print the elements in the buffer from head to tail. 
 */
void printBuffer(struct circularBuffer* bufferPtr) {

  if (bufferPtr->numElements == 0) {
    printf("%s\n", "Empty :)");
  }

  else{

    for (int i = bufferPtr->head; i < bufferPtr->head + bufferPtr->numElements; i++){
      printf("%d: %d\n", i, bufferPtr->data[i]);
    }

  }

}

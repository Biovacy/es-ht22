#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "single_linked_list.h"

int addElementSingleLinkedList(struct singleLinkedList* list, int value){

  struct singleLinkedListElement *currentPtr = list->first;
  struct singleLinkedListElement *previousPtr = NULL;

  // Special case if there are no values
  if (currentPtr == NULL) {
    struct singleLinkedListElement *newElement;
    newElement = (struct singleLinkedListElement *) malloc(sizeof(struct singleLinkedListElement));

    newElement->data = value;
    newElement->next = currentPtr;

    list->first = newElement; 

    return value;
  }

  while (currentPtr != NULL) {

    if ( currentPtr->data >= value ) {

      struct singleLinkedListElement *newElement;
      newElement = (struct singleLinkedListElement *) malloc(sizeof(struct singleLinkedListElement));

     if (newElement == NULL) {
      return INT_MIN; 
     }

      newElement->data = value;
      newElement->next = currentPtr;

      if(previousPtr != NULL){
        previousPtr->next = newElement;
      }
      else {
        list->first = newElement; 
      }

      return value;
    }

    previousPtr = currentPtr;
    currentPtr = currentPtr->next;
  }

  // If the element is larger than all other elements it will not
  // be added in the while loop, and will instead be added here

  struct singleLinkedListElement *newElement;
  newElement = (struct singleLinkedListElement *) malloc(sizeof(struct singleLinkedListElement));

  if (newElement == NULL) {
    return INT_MIN;
  }

  newElement->data = value;
  newElement->next = NULL;

  previousPtr->next = newElement; 

  return value;
} 

void initSingleLinkedList(struct singleLinkedList* list) {
  list->first = NULL;
}

int removeFirstElementSingleLinkedList(struct singleLinkedList* list){

  struct singleLinkedListElement *firstElement = list->first;

  if (firstElement == NULL) {
    return INT_MIN;
  }

  int value = firstElement->data;

  list->first = firstElement->next;

  free(firstElement);

  return value;
} 

int removeLastElementSingleLinkedList(struct singleLinkedList* list){
  
  struct singleLinkedListElement *currentElement; 
  struct singleLinkedListElement *previousElement;
  currentElement  = list->first;
  previousElement = NULL;

  if (currentElement == NULL) {
    return INT_MIN;
  }

  if (currentElement->next == NULL) {
    int value = currentElement->data;
    free(currentElement);
    list->first = NULL;
    return value;
  }

  while(currentElement->next != NULL) {
    previousElement = currentElement;
    currentElement = currentElement->next; 
  }

  previousElement->next = NULL;
  int value = currentElement->data;
  free(currentElement);

  return value;
} 

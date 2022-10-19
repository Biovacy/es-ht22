#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "single_linked_list.h"

struct travel_need *addElementSingleLinkedList(struct singleLinkedList* list, struct travel_need *travelRequest){

  struct singleLinkedListElement *currentPtr = list->first;
  struct singleLinkedListElement *previousPtr = NULL;

  // Special case if there are no values
  if (currentPtr == NULL) {
    struct singleLinkedListElement *newElement;
    newElement = (struct singleLinkedListElement *) malloc(sizeof(struct singleLinkedListElement));

    newElement->data = travelRequest;
    newElement->next = currentPtr;

    list->first = newElement; 

    return travelRequest;
  }

  while (currentPtr != NULL) {

    struct singleLinkedListElement *newElement;
    newElement = (struct singleLinkedListElement *) malloc(sizeof(struct singleLinkedListElement));

    if (newElement == NULL) {
      return NULL; 
    }

    newElement->data = travelRequest;
    newElement->next = currentPtr;

    if(previousPtr != NULL){
      previousPtr->next = newElement;
    }
    else {
      list->first = newElement; 
    }

    return travelRequest;
    
    previousPtr = currentPtr;
    currentPtr = currentPtr->next;
  }

  // If the element is larger than all other elements it will not
  // be added in the while loop, and will instead be added here

  struct singleLinkedListElement *newElement;
  newElement = (struct singleLinkedListElement *) malloc(sizeof(struct singleLinkedListElement));

  if (newElement == NULL) {
    return NULL;
  }

  newElement->data = travelRequest;
  newElement->next = NULL;

  previousPtr->next = newElement; 

  return travelRequest;
} 

void initSingleLinkedList(struct singleLinkedList* list) {
  list->first = NULL;
}

struct travel_need *removeFirstElementSingleLinkedList(struct singleLinkedList* list){

  struct singleLinkedListElement *firstElement = list->first;

  if (firstElement == NULL) {
    return NULL;
  }

  struct travel_need *value = firstElement->data;

  list->first = firstElement->next;

  free(firstElement);

  return value;
} 

struct travel_need *removeLastElementSingleLinkedList(struct singleLinkedList* list){
  
  struct singleLinkedListElement *currentElement; 
  struct singleLinkedListElement *previousElement;
  currentElement  = list->first;
  previousElement = NULL;

  if (currentElement == NULL) {
    return NULL;
  }

  if (currentElement->next == NULL) {
    struct travel_need *value = currentElement->data;
    free(currentElement);
    list->first = NULL;
    return value;
  }

  while(currentElement->next != NULL) {
    previousElement = currentElement;
    currentElement = currentElement->next; 
  }

  previousElement->next = NULL;
  struct travel_need *value = currentElement->data;
  free(currentElement);

  return value;
} 
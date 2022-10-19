#include<stdlib.h>
#include<limits.h>
#include"double_linked_list.h"

int addElementDoubleLinkedList(struct doubleLinkedList* list, int value){
  
  struct doubleLinkedListElement *currentElement; 
  struct doubleLinkedListElement *previousElement;
  currentElement = list->first;
  previousElement = NULL;

  // If the list is empty
  if (currentElement == NULL) {

    struct doubleLinkedListElement *newElement;
    newElement = (struct doubleLinkedListElement *) malloc(sizeof(struct doubleLinkedListElement));

    newElement->data     = value;
    newElement->next     = NULL;
    newElement->previous = NULL;
    list->first          = newElement;

    return value;
  } 
  
  while (currentElement != NULL) {

    if ( currentElement->data >= value ) {

      struct doubleLinkedListElement *newElement;
      newElement = (struct doubleLinkedListElement *) malloc(sizeof(struct doubleLinkedListElement));

      newElement->data = value;
      newElement->next = currentElement;

      if (previousElement == NULL) {
        list->first = newElement;
        newElement->previous = NULL;
      }
      else {
        newElement->previous = previousElement;
        previousElement->next = newElement;
      }

      return value;
    }

    previousElement = currentElement;
    currentElement  = currentElement->next;

  }

  // If the element is larger than all other elements it will not
  // be added in the while loop, and will instead be added here

  struct doubleLinkedListElement *newElement;
  newElement = (struct doubleLinkedListElement *) malloc(sizeof(struct doubleLinkedListElement));
  
  if (newElement == NULL) {
    return INT_MIN;
  }

  newElement->data = value;
  newElement->next = NULL;
  newElement->previous = previousElement;
  previousElement->next = newElement; 
  list->last = newElement;

  return value;
} 


void initDoubleLinkedList(struct doubleLinkedList* list) {
  list->first = NULL;
  list->last  = NULL;
}


int removeFirstElementDoubleLinkedList(struct doubleLinkedList* list){
  
  struct doubleLinkedListElement *firstElement = list->first;

  if (firstElement == NULL) {
    return INT_MIN;
  }  

  int value = firstElement->data;

  if (firstElement->next == NULL) {
    free(firstElement);
    list->first = NULL;
    list->last = NULL;
    return value;
  }

  list->first = firstElement->next;
  firstElement->next->previous = NULL;
  free(firstElement);

  return value; 
} 


int removeLastElementDoubleLinkedList(struct doubleLinkedList* list) {
  
  struct doubleLinkedListElement *lastElement = list->last;

  if (lastElement == NULL) {
    return INT_MIN;
  }

  int value = lastElement->data;

  if (lastElement->previous == NULL) {
    free(lastElement);
    list->first = NULL;
    list->last  = NULL;
    return value;
  }

  list->last = lastElement->previous;
  lastElement->previous->next = NULL;
  free(lastElement);
  
  return value;
}
/**
 * Auther:  Hampus Oxenholt & Ingvar Pétursson
 * Date:    {unknown}
 */

#include <stdio.h>
#include <stdlib.h>

#include "single_linked_list.h"
#include "double_linked_list.h"

#define OUT_OF_MEMORY_ERROR 0

void printDoubleList(struct doubleLinkedList *list) {

    struct doubleLinkedListElement *currentElement; 
    currentElement = list->first;

    while(currentElement != NULL) {
        printf("%d\n", currentElement->data);
        currentElement = currentElement->next; 
    }

}

void printSingleList(struct singleLinkedList *list) {

    struct singleLinkedListElement *currentElement; 
    currentElement = list->first;

    while(currentElement != NULL) {
        printf("%d\n", currentElement->data);
        currentElement = currentElement->next; 
    }

}

void app_main() {

    printf("Single linked list\n");
    struct singleLinkedList singleList;
    initSingleLinkedList(&singleList);

    printf("ADD:    %d\n", addElementSingleLinkedList(&singleList, 5));
    printf("ADD:    %d\n", addElementSingleLinkedList(&singleList, 10));
    printf("ADD:    %d\n", addElementSingleLinkedList(&singleList, 2));
    printSingleList(&singleList);

    printf("REMOVE: %d\n", removeFirstElementSingleLinkedList(&singleList));
    printf("REMOVE: %d\n", removeFirstElementSingleLinkedList(&singleList));
    printf("REMOVE: %d\n", removeFirstElementSingleLinkedList(&singleList));
    printSingleList(&singleList);

    printf("ADD:    %d\n", addElementSingleLinkedList(&singleList, 1));
    printf("ADD:    %d\n", addElementSingleLinkedList(&singleList, 2));
    printf("ADD:    %d\n", addElementSingleLinkedList(&singleList, 3));
    printSingleList(&singleList);

    printf("REMOVE: %d\n", removeLastElementSingleLinkedList(&singleList));
    printf("REMOVE: %d\n", removeLastElementSingleLinkedList(&singleList));
    printf("REMOVE: %d\n", removeLastElementSingleLinkedList(&singleList));
    printSingleList(&singleList);

    // Test removing from a empty list
    printf("ADD:    %d\n", addElementSingleLinkedList(&singleList, 1));
    printf("ADD:    %d\n", addElementSingleLinkedList(&singleList, 2));
    printf("ADD:    %d\n", addElementSingleLinkedList(&singleList, 3));
    printSingleList(&singleList);

    printf("REMOVE: %d\n", removeLastElementSingleLinkedList(&singleList));
    printf("REMOVE: %d\n", removeLastElementSingleLinkedList(&singleList));
    printf("REMOVE: %d\n", removeLastElementSingleLinkedList(&singleList));
    printf("REMOVE: %d\n", removeLastElementSingleLinkedList(&singleList));
    printSingleList(&singleList);

    printf("Double linked list\n");

    struct doubleLinkedList doubleList;
    initDoubleLinkedList(&doubleList);

    printf("ADD: %d\n", addElementDoubleLinkedList(&doubleList, 5));
    printf("ADD: %d\n", addElementDoubleLinkedList(&doubleList, 10));
    printf("ADD: %d\n", addElementDoubleLinkedList(&doubleList, 2));
    printDoubleList(&doubleList);

    printf("REMOVE: %d\n", removeFirstElementDoubleLinkedList(&doubleList));
    printf("REMOVE: %d\n", removeFirstElementDoubleLinkedList(&doubleList));
    printf("REMOVE: %d\n", removeFirstElementDoubleLinkedList(&doubleList));
    printDoubleList(&doubleList);

    printf("ADD: %d\n", addElementDoubleLinkedList(&doubleList, 1));
    printf("ADD: %d\n", addElementDoubleLinkedList(&doubleList, 2));
    printf("ADD: %d\n", addElementDoubleLinkedList(&doubleList, 3));
    printDoubleList(&doubleList);

    printf("REMOVE: %d\n", removeLastElementDoubleLinkedList(&doubleList));
    printf("REMOVE: %d\n", removeLastElementDoubleLinkedList(&doubleList));
    printf("REMOVE: %d\n", removeLastElementDoubleLinkedList(&doubleList));
    printDoubleList(&doubleList);

    // Test remove from empty list
    printf("ADD: %d\n", addElementDoubleLinkedList(&doubleList, 1));
    printf("ADD: %d\n", addElementDoubleLinkedList(&doubleList, 2));
    printf("ADD: %d\n", addElementDoubleLinkedList(&doubleList, 3));
    printDoubleList(&doubleList);

    printf("REMOVE: %d\n", removeLastElementDoubleLinkedList(&doubleList));
    printf("REMOVE: %d\n", removeLastElementDoubleLinkedList(&doubleList));
    printf("REMOVE: %d\n", removeLastElementDoubleLinkedList(&doubleList));
    printf("REMOVE: %d\n", removeLastElementDoubleLinkedList(&doubleList));
    printDoubleList(&doubleList);

    if (OUT_OF_MEMORY_ERROR) {
        while (1) {
            printf("ADD: %d\n", addElementSingleLinkedList(&singleList, 0));
        }
    }
}

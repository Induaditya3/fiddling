#include "d4linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

//allocate memory for first node which is going to be head of linkedlist and return a linked list
linkedlist_t* CreateLinkedList(){
    linkedlist_t* nlist = (linkedlist_t*) malloc(sizeof(linkedlist_t));
    nlist -> head = NULL;
    return nlist;
}

void PrintedLinkedList(linkedlist_t* list){
    node_t* iter = list-> head;
    while(iter != NULL){
        printf("data = %d\n",iter-> data);
        iter = iter-> next;
    }
}

//Recursive definition of FreeLinkedList from head node

// void FreeLinkedList(node_t* head){
//     if (( head)-> next == NULL){
//         free( head);
//     }
//     else{
//         FreeLinkedList(( head)-> next);
//         free( head);
//     }
// }

//follow every links and delete all nodes
void FreeLinkedList(linkedlist_t* list){
    //current - node we want to delete
    node_t* current = list-> head;
    if(current == NULL){
        free(list);
        return; // no node to free 
    }

    //next - node next to current node
    node_t* next = current-> next;
    while(current != NULL){
        free(current);
        current = next;
        if (current != NULL){
            next = current -> next;
        }
    }
    free(list);

}

//create a new node_t and add it to last node of linkedlist_t
void AppendToLinkedList(linkedlist_t* list, int data){
    //allocate memory for new node
    node_t* new = (node_t*) malloc(sizeof(node_t));
    new -> data = data;
    new -> next = NULL;

    //check if list is empty, i.e., head is null
    if (list->head == NULL){
        //assign it to head
        list-> head = new;
    }
    else{
        //find the last node and assign new node to next field of it
        node_t * iter = list-> head; //temporary iterator node for traversing linkedlist
        while (iter-> next != NULL){
            iter = iter-> next;
        }
        iter-> next = new;
    }

}
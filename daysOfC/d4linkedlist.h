#ifndef LINKEDLIST_H
#define LINKEDLIST_H
//data structure for node which holds information about each link in a chain
typedef struct node{
    int data;
    struct node* next;
}node_t;

//data structure for linked list which holds address of first node.
typedef struct linkedlist{
    node_t* head;
}linkedlist_t;

//allocate memory for first node which is going to be head of linkedlist and return a linked list
linkedlist_t* CreateLinkedList();

void PrintedLinkedList(linkedlist_t* list);

//Recursive definition of FreeLinkedList from head node
//void FreeLinkedList(node_t* head);

//follow every links and delete all nodes
void FreeLinkedList(linkedlist_t* list);

//create a new node_t and add it to last node of linkedlist_t
void AppendToLinkedList(linkedlist_t* list, int data);
#endif 
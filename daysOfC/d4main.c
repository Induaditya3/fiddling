#include <stdio.h>
#include "d4linkedlist.h"

void unitTest1(){
    linkedlist_t* newlist = CreateLinkedList();
    
    AppendToLinkedList(newlist, 9);
    AppendToLinkedList(newlist, 8);
    AppendToLinkedList(newlist, 7);
    AppendToLinkedList(newlist, 6);
    AppendToLinkedList(newlist, 5);
    AppendToLinkedList(newlist, 4);

    PrintedLinkedList(newlist);

    FreeLinkedList(newlist);
}

void unitTest2(){
    linkedlist_t* empty = CreateLinkedList();
    PrintedLinkedList(empty);
    FreeLinkedList(empty);
}

void unitTest3(){
    linkedlist_t* single = CreateLinkedList();
    AppendToLinkedList(single,100);
    FreeLinkedList(single);
}
int main(){
    
    unitTest1();
    unitTest2();
    unitTest3();
    return 0;
}
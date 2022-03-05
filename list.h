#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>

#define LISTSIZE 128
#define POISON -17
#define MAXDOTCMD 64

#define INFORMATION __FILE__, __LINE__, __PRETTY_FUNCTION__
#define printInformation printf("\x1b[32mFilename: %s, line of program: %d, function: %s\x1b[0m", INFORMATION);

FILE* graphfile = fopen("graphfile.txt", "w");

struct Node 
{
    int prev = 0;
    int next = 0;
    int val = 0;
};

struct list
{
    struct Node* node = nullptr;
    int tail;
    int free_pos;
    int head;

    int linearized;
    int size;
};

list* listCstr (int listSize);

//***********************************

void lstInsretHead (list* List, int val);

//***********************************

void lstInsretAfter (list* List, int val, int place);

//***********************************

void lstDelete (list* List, int place);

//***********************************

void lstDump(list* List);

//***********************************

void printList(list* List);

//***********************************

void listDstr (list* List);

//***********************************

int logicalToPhysical (list* List, int logical);

//***********************************

void Linearization (list* List);

//***********************************

list* listCstr (int listSize)
{
    list* List = (list*) calloc (1, sizeof(*List));
    List->node = (Node*) calloc(listSize, sizeof(*List->node));

    List->tail = 0;
    List->free_pos = 1;
    List->head = 0;

    List->linearized = 0;
    List->size = 0;

    for (int i = 0; i < listSize - 1; ++i)
    {
        List->node[List->free_pos + i].next = List->free_pos + i + 1;  
    }

    return List;
}

//***********************************

void listDstr (list* List)
{
    assert (List != nullptr);

    free (List->node);
    List->size = 0;
}

//***********************************

void lstInsertHead (list* List, int val)
{
    //vзапоминаем текущее положение головы
    int prevHead = List->head;

    // устанавливаем голову на свободное место 
    List->head = List->free_pos;

    // менянем свободное место на следующее по списку 
    List->free_pos = List->node[List->free_pos].next;

    // Для старой головы следующим элементом является новая
    List->node[prevHead].next = List->head;

    // т.к. голова меняем next на 0
    List->node[List->head].next = 0;

    // устанавливаем предыдущий элемент 
    List->node[List->head].prev = prevHead;

    List->node[List->head].val = val;

    ++List->size;

    if (List->size == 1)
    {
        List->tail = 1;
    }
}

//***********************************

void lstInsertAfter (list* List, int val, int place)
{
    int nextAfterPlace = List->node[place].next;
    List->node[place].next = List->free_pos;

    List->node[List->free_pos].prev = place;

    int free_pos = List->free_pos;
    List->free_pos = List->node[List->free_pos].next;

    List->node[free_pos].val = val;

    List->node[free_pos].next = nextAfterPlace;
    List->node[nextAfterPlace].prev = free_pos;
    
    ++List->size;
}

//***********************************

void lstDelete (list* List, int place)
{
    assert (List!= nullptr);
    assert (List->size != 0);

    // делаем значени POSION
    List->node[place].val = POISON;

    if (List->size == 1)
    {
        ;
    }
    
    else
    {
        // Делаем next у предыдущего элемента nextом текущего (относительно удаляемого)
        int prevPosition = List->node[place].prev;
        List->node[prevPosition].next = List->node[place].next;

        // Делаем у следующего элемента prev прерыдыдущий (относительно удаляемого)
        int nextPosition = List->node[place].next;
        List->node[nextPosition].prev = List->node[place].prev;
    }

    int free_pos = List->free_pos;
    List->node[place].prev = 0;
    List->node[place].next = free_pos;
    List->free_pos = place;

    --List->size;
}

//***********************************

void lstDump(list* List)
{
    fprintf(graphfile, 
                    "digraph G{\n"
                    "   first [shape=record,label=\"  list tail\\n %2d | list head\\n %2d | list size\\n %2d\" ];\n", 
                    List->tail, List->head, List->size);

    fprintf (graphfile, "   edge[color=white];\n"
                        "   rankdir = LR;\n");

    int i = 0;
    int k = List->tail;

    while (i++ < List->size)
    {
        fprintf(graphfile, "   List%d  [shape = record, label = \"<a%02d> address\\n %2d | value %2d | <f%02d> next | <ff%02d> prev\"];\n", 
                               k, k, k, List->node[k].val, k, k);
        
        k = List->node[k].next;
    }

    fprintf (graphfile, "   edge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12];\n");

    i = 0;
    k = List->tail;

    while (i++ < List->size)
    {
        if (List->node[k].next != 0 && List->node[k].val != POISON)
        {
            fprintf (graphfile, "   List%d:  <f%02d>  -> List%d: <a%02d>;\n", 
                                    k, k, List->node[k].next, List->node[k].next);
        }

        k = List->node[k].next;
    }

    fprintf (graphfile, "   edge[color=\"darkblue\",fontcolor=\"blue\",fontsize=12];\n");

    i = 0;
    k = List->head;

    while (i++ < List->size)
    {
        if (List->node[k].prev != 0 && List->node[k].val != POISON)
        {
            fprintf (graphfile, "   List%d:  <ff%02d> -> List%d: <a%02d>;\n", 
                                   k, k, List->node[k].prev, List->node[k].prev);
        }

        k = List->node[k].prev;
    }

    fprintf(graphfile, "}");

    char makeDotFile[MAXDOTCMD];
    sprintf (makeDotFile, "dot -Tpng graphfile.txt > graphfile.png");
    system (makeDotFile);
}

//***********************************

void printList(list* List)
{
    printf ("Print list begin\n\n");
    printf ("tail: %d, head: %d\n", List->tail, List->head);

    int i = 0;
    int k = 1;

    while (i++ < List->size)
    {
        printf("List[%d]: %d\n", k, List->node[k].val);
        k = List->node[k].next;
    }

    printf("\n\n");

    for (int i = List->tail; i <= List->head + 10; ++i)
    {
        printf("List[%d]: %d\n", i, List->node[i].val);
    }

    printf ("Print list end\n\n");
}

//***********************************

int logicalToPhysical (list* List, int logical)
{
    assert (List != nullptr);

    int currPlace = List->tail;

    for (int i = 0; i < logical; ++i)
    {
        currPlace = List->node[currPlace].next;
    }

    return currPlace;
}

//***********************************

void Linearization (list* List)
{
    assert (List != nullptr);

    list* linearList = listCstr (LISTSIZE);

    int physical = 0;

    for (int i = 0; i < List->size; ++i) // так как инкрементируется до вызова функции начинаем с нуля 
    {
        physical = logicalToPhysical (List, i);
        lstInsertHead (linearList, List->node[physical].val);
    }

    memcpy (List->node, linearList->node, linearList->size * sizeof (*linearList));
    listDstr (linearList);

    List->linearized = 1;
}

#endif 
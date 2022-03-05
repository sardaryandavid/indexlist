#include <stdio.h>

#include "list.h"

int main()
{
    list* List = listCstr (LISTSIZE);
    
    lstInsertHead (List, 10);
    lstInsertHead (List, 20);
    lstInsertHead (List, 30);
    lstInsertHead (List, 40);
    lstInsertAfter (List, 100, 2);
    lstInsertHead (List, 50);
    lstInsertAfter (List, 70, 3);
    lstDelete (List, 2);

    lstDump (List);

    printList (List);
    Linearization (List); 
    printList (List);

    listDstr (List);

    return 0;
}
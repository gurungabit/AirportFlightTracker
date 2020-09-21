// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "cs2123p6.h"
/******************** hash **************************************
  int hash(Graph g, char szAirport[])
Purpose:
    Hashes an airport ID to return a number between 0 and g->iOverflowBegin.
Parameters:
    I   Graph g          Graph represented as a double adjacency list
    I   char szAirport   Airport ID to be hashed
Returns:
    Returns a hash value into the primary area.  This value is between 0
    and g->iOverflowBegin - 1.
Notes:
    - Primary area of the hash table has subscripts from 0 to 
      g->iOverflowBegin -1.
    - Overflow area begins with g->iOverflowBegin.
    - The hash function is average at spreading the values. 
    - The hash simply sums the characters in the airport and gets the
      remainder by dividing by g->iOverFlowBegin
**************************************************************************/
int hash(Graph g, char szAirport[])
{
    int iHash = 0;
    int i;
    int iLen = strlen(szAirport);
    // sum the characters in the airport ID as simple integers
    for (i = 0; i < iLen; i++)
    {
        iHash += szAirport[i] - 'A';
    }

    iHash = iHash % g->iOverflowBegin; // restrict it to the primary area
    return iHash;
}
/******************** printHash **************************************
  void printHash(Graph g)
Purpose:
    Prints the hash table in a format that helps show the primary area 
    and overflow area.  For each entry in the primary area, it prints 
    the entry (if present) and prints corresponding synonyms.  It also
    prints the free list.
Parameters:
    I   Graph g          Graph represented as a double adjacency list
Returns:
    n/a
Notes:
    - Primary area of the hash table has subscripts from 0 to 
      g->iOverflowBegin -1.
    - Overflow area begins with g->iOverflowBegin. 
**************************************************************************/
void printHash(Graph g)
{
    int i;
    
    // Print the heading for both types of output (primary area and collisions
    printf("    %2s %-7s %s\n", "Vx", "Airprt", "Chain");
    printf("\t\t\t%2s %-7s %s\n", "Vx", "Airprt", "Chain");
    
    // Iterate over each entry in the primary area
    for (i = 0; i < g->iOverflowBegin; i++)
    {
        int bPrint = FALSE;
        char szAirport[4] = "-";
        char szCollision[10] = " ";
        // If it contains a vertex, remember it and printing is necessary
        if (g->vertexM[i].bExists)
        {
            strcpy(szAirport, g->vertexM[i].szAirport);
            bPrint = TRUE;
        }

        // If it contains a hash chain,  printing is necessary
        if (g->vertexM[i].iHashChainNext != -1)
        {
            bPrint = TRUE;
            strcpy(szCollision, "Collision");
        }
        // Print this vertex if it had an airport or it has a collision
        if (bPrint)
            printf("    %2d %-7s %3d %s\n", i, szAirport
                , g->vertexM[i].iHashChainNext
                , szCollision);
        // Print the synonym chain
        if (g->vertexM[i].iHashChainNext != -1)
            printChain(g, g->vertexM[i].iHashChainNext);
        
    }
    // Print the overflow free list
    printf("    Overflow free list\n    ");
    for (i = g->iFreeHead; i != -1; i = g->vertexM[i].iHashChainNext)
    {
        printf("%2d ", i);
         
    }
    printf("\n");
}
/******************** printChain **************************************
  void printChain(Graph g, int iVertex)
Purpose:
    Prints vertices on a collision chain.  
Parameters:
    I   Graph g          Graph represented as a double adjacency list
    I   int iVertex      First collision in the overflow area for the 
                         current chain.
Returns:
    n/a
Notes:
    - Primary area of the hash table has subscripts from 0 to 
      g->iOverflowBegin -1.
    - Overflow area begins with g->iOverflowBegin. 
**************************************************************************/
void printChain(Graph g, int iVertex)
{
    int i;
    // Traverse the collision chain
    for (i = iVertex; i != -1; i = g->vertexM[i].iHashChainNext)
    {
        printf("\t\t\t%2d %-7s %2d\n", i, g->vertexM[i].szAirport
            , g->vertexM[i].iHashChainNext);
    }
}
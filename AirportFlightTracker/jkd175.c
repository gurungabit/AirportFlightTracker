/******************************************************************************
cs2123p5Driver.c by Abit Gurung
Purpose: 
Command Parameters: 
Input:
    Reads input from a .txt file 
Results:
Returns:
    returns 0 if everything runs correctly!
Notes: 
*******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p6.h"
FILE *pFileData;

AltPath altPath;

AltPath newAltPath()
{
    AltPath ap = (AltPath)malloc(sizeof(AltPathImp));
    if (ap == NULL)
        errExit("Could not allocate Altpath");

    ap->iAltCnt = 0;
    return ap;
}
//Prototypes
int maxEdge(Graph g, int start, int target, int iPrevArrTm2400);
int maxEdgeInit(Graph g, int start, int target);
EdgeNode *allocEdgeNode();
Vertex *newVertex(char szAirport[]);
Graph newGraph();

int main(int argc, char *argv[])
{
    altPath = newAltPath();
    // creates a new graph
    Graph graph = newGraph();
    // flight structurs to store all input datas
    Flight flight;
    // vertex pointer to store in graph
    Vertex *pVertex;
    //input buffer to store input data
    char szInputBuffer[MAX_LINE_SIZE + 1];
    //String to store input Command
    char szCommandToken[MAX_TOKEN + 1];
    //stores one line of input
    char *pszInput;
    //store TRUE or FALSE for airport search
    int flightOrgin = 0;
    int flightDest = 0;

    //reading input from stdin
    pFileData = stdin;
    //Store data count
    int iScanfCnt;

    //Lopp until EOF or NULL
    while (fgets(szInputBuffer, MAX_LINE_SIZE, pFileData) != NULL)
    {
        //if blank, continue;
        if (szInputBuffer[0] == '\n')
            continue;

        //get line of data and COMMANDTOKEN
        pszInput = getToken(szInputBuffer, szCommandToken, MAX_TOKEN);

        //print line
        printf(">> %s", szInputBuffer);

        //if szCommandToken == FLIGHT, check if airport exist, else insert airport and it's predecessors and successors
        if (strcmp(szCommandToken, "FLIGHT") == 0)
        {
            //get flight data and store count to iScanfCnt
            iScanfCnt = sscanf(pszInput, "%s %s %s %d %d %d", flight.szFlightNr, flight.szOrigin, flight.szDest, &flight.iDepTm2400, &flight.iDurationMins, &flight.iZoneChange);

            //if iScanfCnt < expected input show error
            if (iScanfCnt < 6)
            {
                errExit("Expected 6 values, recieved %d", iScanfCnt);
            }

            //search for airport and store TRUE OR FALSE
            flightOrgin = findAirport(graph, flight.szOrigin);
            flightDest = findAirport(graph, flight.szDest);

            //If airport not FOUND create new vertex and increate number of vertices count
            if (flightOrgin == -1)
            {
                //increate verterics count
                graph->iNumVertices++;
                //create new vertex for new airport
                pVertex = newVertex(flight.szOrigin);
                //store new vertex in to graph vertex array
                graph->vertexM[graph->iNumVertices] = *pVertex;
                //copy airport name
                strcpy(graph->vertexM[graph->iNumVertices].szAirport, flight.szOrigin);
                //free vertex since it's already in the graph
                free(pVertex);
            }

            if (flightDest == -1)
            {
                graph->iNumVertices++;
                pVertex = newVertex(flight.szDest);
                graph->vertexM[graph->iNumVertices] = *pVertex;
                strcpy(graph->vertexM[graph->iNumVertices].szAirport, flight.szDest);
                free(pVertex);
            }

            //insert successors and predecessor to correct vertex
            insertFlight(graph, flight);
        }
        //if szCommand = PRTFLIGHTBYORIGIN, print each vertex Successors
        else if (strcmp(szCommandToken, "PRTFLIGHTBYORIGIN") == 0)
        {
            prtFlightsByOrigin(graph);
        }
        //if szCommand = PRTFLIGHTBYDEST, print each vertex precessors
        else if (strcmp(szCommandToken, "PRTFLIGHTBYDEST") == 0)
        {
            prtFlightsByDest(graph);
        }
        //if szCommand = PRTONE, print the passed in flight all precessors and Successors
        else if (strcmp(szCommandToken, "PRTONE") == 0)
        {
            //find which airport
            iScanfCnt = sscanf(pszInput, "%[A-Z]", flight.szOrigin);

            //if none, print error
            if (iScanfCnt != 1)
            {
                errExit("Error: Recieved %d, expected at-least one", iScanfCnt);
            }

            //find out if airport exits
            flightOrgin = findAirport(graph, flight.szOrigin);

            if (flightOrgin == -1)
            {
                printf("Error: Could not find the Flight!\n");
                continue;
            }

            printf("%8s %2s %-62s %s \n", "Vx", "Apt", "Predecessors", "Successors");

            prtOne(graph, flightOrgin);
        }
        //if szCommand = PRTALL, print all flights all precessors and Successors
        else if (strcmp(szCommandToken, "PRTALL") == 0)
        {
            prtAll(graph);
            printf("\n");
        }
        //if szCommand = PRTSUCC, print the passed in flight all precessors and precessor's precessors!
        else if (strcmp(szCommandToken, "PRTSUCC") == 0)
        {
            iScanfCnt = sscanf(pszInput, "%[A-Z]", flight.szOrigin);

            if (iScanfCnt != 1)
            {
                errExit("Error: Recieved %d, expected at-least one", iScanfCnt);
            }

            //find if airport exits
            flightOrgin = findAirport(graph, flight.szOrigin);
            if (flightOrgin == -1)
            {
                printf("Error: Could not find the Flight!\n");
                continue;
            }
            //set all vertex not VISITED
            setNotVisited(graph);
            prtTraversal(graph, flightOrgin, 0);
        }
        else if (strcmp(szCommandToken, "PRTCHRON") == 0)
        {
            iScanfCnt = sscanf(pszInput, "%[A-Z]", flight.szOrigin);

            if (iScanfCnt != 1)
            {
                errExit("Error: Recieved %d, expected at-least one", iScanfCnt);
            }

            flightOrgin = findAirport(graph, flight.szOrigin);
            if (flightOrgin == -1)
            {
                printf("Error: Could not find the Flight!\n");
                continue;
            }
            setNotVisited(graph);
            prtTraversalChron(graph, flightOrgin, 0, 0);
        }
        else if (strcmp(szCommandToken, "*") == 0)
        {
            continue;
        }
        else if (strcmp(szCommandToken, "PRTALTS") == 0)
        {
            iScanfCnt = sscanf(pszInput, "%s %s", flight.szOrigin, flight.szDest);

            if (iScanfCnt < 2)
                errExit("Error: Recieved %d, expected at-least two", iScanfCnt);

            flightOrgin = findAirport(graph, flight.szOrigin);
            if (flightOrgin == -1)
            {
                printf("Error: Could not find the Flight!\n");
                continue;
            }
            flightDest = findAirport(graph, flight.szDest);
            prtAlts(graph, flightOrgin, flightDest);
        }
        else if (strcmp(szCommandToken, "MAXSTEPS") == 0)
        {
            iScanfCnt = sscanf(pszInput, "%s %s", flight.szOrigin, flight.szDest);

            if (iScanfCnt < 2)
                errExit("Error: Recieved %d, expected at-least two", iScanfCnt);

            flightOrgin = findAirport(graph, flight.szOrigin);
            if (flightOrgin == -1)
            {
                printf("Error: Could not find the Flight!\n");
                continue;
            }
            flightDest = findAirport(graph, flight.szDest);
            int iMax = maxStepsChron(graph, flightOrgin, flightDest, 0);

            printf("    Maximum chain chron for %s to %s contains %d steps \n", flight.szOrigin, flight.szDest, iMax);
        }
        else if (strcmp(szCommandToken, "DELETE") == 0)
        {
            iScanfCnt = sscanf(pszInput, "%s", flight.szOrigin);

            flightOrgin = findAirport(graph, flight.szOrigin);
            if (flightOrgin == -1)
            {
                printf("Error: Could not find the Flight!\n");
                continue;
            }
            if (iScanfCnt < 1)
                errExit("Error: Recieved %d, expected at-least one", iScanfCnt);

            deleteAirport(graph, flight.szOrigin);
        }
    }

    free(altPath);
    //printtest(graph);
    return 0;
}
/******************** getToken **************************************
char * getToken (char *pszInputTxt, char szToken[], int iTokenSize)
Purpose:
    Examines the input text to return the next token.  It also 
    returns the position in the text after that token.
Parameters:
    I   char *pszInputTxt       input buffer to be parsed
    O   char szToken[]          Returned token.
    I   int iTokenSize          The size of the token variable.  This is used
                                to prevent overwriting memory.  The size 
                                should be the memory size minus 1 (for
                                the zero byte).
Returns:
    Functionally: 
         Pointer to the next character following the delimiter after the token.  
         NULL - no token found.
    szToken parm - the returned token.  If not found, it will be an 
         empty string.
Notes:
    - If the token is larger than the szToken parm, we return a truncated value.
    - If a token isn't found, szToken is set to an empty string
    - This function does not skip over white space occurring prior to the token.
    - If the input buffer pointer is NULL, the function terminates with
      an algorithm error.
**************************************************************************/
char *getToken(char *pszInputTxt, char szToken[], int iTokenSize)
{
    int iDelimPos;               // found position of delim
    int iCopy;                   // number of characters to copy
    char szDelims[20] = " \n\r"; // delimiters
    szToken[0] = '\0';

    // check for NULL pointer
    if (pszInputTxt == NULL)
        errExit("getToken passed a NULL pointer");

    // Check for no token if at zero byte
    if (*pszInputTxt == '\0')
        return NULL;

    // get the position of the first delim
    iDelimPos = strcspn(pszInputTxt, szDelims);

    // See if the token has no characters before delim
    if (iDelimPos == 0)
        return NULL;

    // see if we have more characters than target token, if so, trunc
    if (iDelimPos > iTokenSize)
        iCopy = iTokenSize; // truncated size
    else
        iCopy = iDelimPos;

    // copy the token into the target token variable
    memcpy(szToken, pszInputTxt, iCopy);
    szToken[iCopy] = '\0'; // null terminate

    // advance the position
    pszInputTxt += iDelimPos;
    if (*pszInputTxt == '\0')
        return pszInputTxt;
    else
        return pszInputTxt + 1;
}
/******************** newGraph **************************************
Graph newGraph();
Purpose:
        allocates memonry for a new graph
Parameters:
    O  new graph
Returns:
    Functionally: 
         Returns allocated memory for new graph 
**************************************************************************/
Graph newGraph()
{
    //malloc size of graph structure
    Graph g = malloc(sizeof(GraphImp));

    //if NULL, print error, count not allocate
    if (g == NULL)
    {
        errExit("allocation failed!");
    }

    //set iNumVertices 0
    g->iNumVertices = 0;

    return g;
}
/******************** newVertex **************************************
Vertex *newVertex(char szAirport[])
Purpose:
        allocates memonry for a new vertex
Parameters:
    I char szAirport[]
    O newVertex
Returns:
    Functionally: 
         Returns allocated memory for new graph 
**************************************************************************/
Vertex *newVertex(char szAirport[])
{
    //malloc size of vertex structure
    Vertex *v = malloc(sizeof(Vertex));

    //if null, print error, could not allocate
    if (v == NULL)
    {
        errExit("Could not allocat vertex!");
    }

    //copy airport name
    strcpy(v->szAirport, szAirport);

    //Set vertex predecessors and successors to NULL;
    v->predecessorList = NULL;

    v->successorList = NULL;

    v->bExists = TRUE;

    return v;
}
/******************** errExit **************************************
    void errExit(char szFmt[], ... )
Purpose:
    Prints an error message defined by the printf-like szFmt and the
    corresponding arguments to that function.  The number of 
    arguments after szFmt varies dependent on the format codes in
    szFmt.  
    It also exits the program, returning ERR_EXIT.
Parameters:
    I   char szFmt[]            This contains the message to be printed
                                and format codes (just like printf) for 
                                values that we want to print.
    I   ...                     A variable-number of additional arguments
                                which correspond to what is needed
                                by the format codes in szFmt. 
Returns:
    Returns a program exit return code:  the value of ERR_EXIT.
Notes:
    - Prints "ERROR: " followed by the formatted error message specified 
      in szFmt. 
    - Prints the file path and file name of the program having the error.
      This is the file that contains this routine.
    - Requires including <stdarg.h>
**************************************************************************/
void errExit(char szFmt[], ...)
{
    va_list args;          // This is the standard C variable argument list type
    va_start(args, szFmt); // This tells the compiler where the variable arguments
                           // begins.  They begin after szFmt.
    printf("ERROR: ");
    vprintf(szFmt, args);                             // vprintf receives a printf format string and  a
                                                      // va_list argument
    va_end(args);                                     // let the C environment know we are finished with the
                                                      // va_list argument
    printf("\n\tEncountered in file %s\n", __FILE__); // this 2nd arg is filled in by
                                                      // the pre-compiler
    exit(ERR_EXIT);
}

int insertFlight(Graph graph, Flight flight)
{
    //insert all the predecessors.
    //allocate new edge
    EdgeNode *pNew = allocEdgeNode();
    //copy all flight information to new edge
    pNew->iDestVertex = findAirport(graph, flight.szDest);
    pNew->iOriginVertex = findAirport(graph, flight.szOrigin);
    strcpy(pNew->flight.szFlightNr, flight.szFlightNr);
    strcpy(pNew->flight.szOrigin, flight.szOrigin);
    strcpy(pNew->flight.szDest, flight.szDest);
    pNew->flight.iDepTm2400 = flight.iDepTm2400;
    pNew->flight.iDurationMins = flight.iDurationMins;
    pNew->flight.iZoneChange = flight.iZoneChange;

    EdgeNode *pCurr; // iterator

    /* Special case for the head end */ // if list is NULL or current flight number is > new, insert new at the head
    if (graph->vertexM[pNew->iDestVertex].predecessorList == NULL || strcmp(graph->vertexM[pNew->iDestVertex].predecessorList->flight.szFlightNr, pNew->flight.szFlightNr) > 0)
    {
        pNew->pNextEdge = graph->vertexM[pNew->iDestVertex].predecessorList;
        graph->vertexM[pNew->iDestVertex].predecessorList = pNew;
    }
    else
    {
        /* Locate the node before the point of insertion */
        //set pCurr to predecessorList and loop though it
        pCurr = graph->vertexM[pNew->iDestVertex].predecessorList;
        //loop through the list and insert in ascending order from right to left
        while (pCurr->pNextEdge != NULL &&
               strcmp(pCurr->pNextEdge->flight.szFlightNr, pNew->flight.szFlightNr) > 0)
        {
            pCurr = pCurr->pNextEdge;
        }
        pNew->pNextEdge = pCurr->pNextEdge;
        pCurr->pNextEdge = pNew;
    }

    //insert all the successors.
    EdgeNode *pNew2 = allocEdgeNode();
    pNew2->iOriginVertex = findAirport(graph, flight.szOrigin);
    pNew2->iDestVertex = findAirport(graph, flight.szDest);
    strcpy(pNew2->flight.szFlightNr, flight.szFlightNr);
    strcpy(pNew2->flight.szDest, flight.szDest);
    strcpy(pNew2->flight.szOrigin, flight.szOrigin);
    pNew2->flight.iDepTm2400 = flight.iDepTm2400;
    pNew2->flight.iDurationMins = flight.iDurationMins;
    pNew2->flight.iZoneChange = flight.iZoneChange;

    EdgeNode *pCurr2;
    if (graph->vertexM[pNew2->iOriginVertex].successorList == NULL || strcmp(graph->vertexM[pNew2->iOriginVertex].successorList->flight.szFlightNr, pNew2->flight.szFlightNr) > 0)
    {
        pNew2->pNextEdge = graph->vertexM[pNew2->iOriginVertex].successorList;
        graph->vertexM[pNew2->iOriginVertex].successorList = pNew2;
    }
    else
    {
        pCurr2 = graph->vertexM[pNew2->iOriginVertex].successorList;
        //loop through the list and insert in ascending order from left to right
        while (pCurr2->pNextEdge != NULL &&
               strcmp(pCurr2->pNextEdge->flight.szFlightNr, pNew2->flight.szFlightNr) < 0)
        {
            pCurr2 = pCurr2->pNextEdge;
        }
        pNew2->pNextEdge = pCurr2->pNextEdge;
        pCurr2->pNextEdge = pNew2;
    }

    return 0;
}

int findAirport(Graph graph, char szAirport[])
{
    int v; // counter variable to traverse through vertices

    for (v = 1; v <= graph->iNumVertices; v++)
    {
        if (strcmp(szAirport, graph->vertexM[v].szAirport) == 0)
            return v;
    } // end for

    return -1;
} // end findAirport

EdgeNode *allocEdgeNode()
{
    EdgeNode *pNew = malloc(sizeof(EdgeNode));

    if (pNew == NULL)
    {
        errExit("Could not allocate a new edge!");
    }

    pNew->pNextEdge = NULL;

    return pNew;
}
void prtFlightsByOrigin(Graph graph)
{
    //Edgenode to loop through
    EdgeNode *s;
    int i;
    int arrival = 0;
    printf("%8s %3s %s %s %4s %5s \n", "Apt", "F#", "Dest", "Dep", "Arr", "Dur");
    //loop through every vertices
    for (i = 1; i <= graph->iNumVertices; i++)
    {
        int iCount = 0;
        //loop through every vertices successorslist
        for (s = graph->vertexM[i].successorList; s != NULL; s = s->pNextEdge)
        {
            iCount++;

            //calculate each edge arrival times
            arrival = calcArr2400(s->flight.iDepTm2400, s->flight.iDurationMins, s->flight.iZoneChange);

            //print first successor with airport name
            if (iCount <= 1)
            {
                printf("%8s %3s %s %5.4d %.4d %4d \n", graph->vertexM[i].szAirport, s->flight.szFlightNr, s->flight.szDest, s->flight.iDepTm2400, arrival, s->flight.iDurationMins);
            }
            else
            {
                printf("%12s %s %5.4d %.4d %4d \n", s->flight.szFlightNr, s->flight.szDest, s->flight.iDepTm2400, arrival, s->flight.iDurationMins);
            }
        }
    }
}
void prtFlightsByDest(Graph graph)
{
    EdgeNode *p;
    int i;
    int arrival = 0;
    printf("%8s %3s %s %s %4s %5s \n", "Apt", "Orig", "F#", "Dep", "Arr", "Dur");
    //loop through every vertices
    for (i = 1; i <= graph->iNumVertices; i++)
    {
        int iCount = 0;
        //loop through every vertices predecessorList
        for (p = graph->vertexM[i].predecessorList; p != NULL; p = p->pNextEdge)
        {
            iCount++;

            //calculate each edge arrival times
            arrival = calcArr2400(p->flight.iDepTm2400, p->flight.iDurationMins, p->flight.iZoneChange);
            //print first successor with airport name
            if (iCount <= 1)
            {
                printf("%8s %3s %s %5.4d %.4d %4d \n", graph->vertexM[i].szAirport, p->flight.szOrigin, p->flight.szFlightNr, p->flight.iDepTm2400, arrival, p->flight.iDurationMins);
            }
            else
            {
                printf("%12s %s %5.4d %.4d %4d \n", p->flight.szOrigin, p->flight.szFlightNr, p->flight.iDepTm2400, arrival, p->flight.iDurationMins);
            }
        }
    }
}

int calcArr2400(int iDepTime2400, int iDurationMins, int iZoneChange)
{
    int arrival;
    arrival = (((iDepTime2400 / 100) + (iDurationMins / 60) + (double)(iDurationMins % 60) / 100) * 100) + (iZoneChange * 100);

    return arrival;
}

void prtOne(Graph graph, int iVertex)
{
    EdgeNode *p, *s;
    int iCount = 9;
    printf("%8d %s ", iVertex - 1, graph->vertexM[iVertex].szAirport);
    for (p = graph->vertexM[iVertex].predecessorList; p != NULL; p = p->pNextEdge)
    {
        printf("%s/%s ", p->flight.szFlightNr, p->flight.szOrigin);
        iCount--;
    }
    while (iCount >= 1)
    {
        printf("...... ");
        iCount--;
    }

    int iCount2 = 0;
    for (s = graph->vertexM[iVertex].successorList; s != NULL; s = s->pNextEdge)
    {
        printf("%s-%s ", s->flight.szFlightNr, s->flight.szDest);
        iCount2++;
    }
    if (iCount2 < 1)
    {
        printf("...... ");
    }
    printf("\n");
}
void prtAll(Graph graph)
{
    int i;
    printf("%8s %2s %-62s %s \n", "Vx", "Apt", "Predecessors", "Successors");

    for (i = 1; i <= graph->iNumVertices; i++)
    {
        prtOne(graph, i);
    }
}
void prtTraversal(Graph graph, int iAirportVertex, int indent)
{
    int i;
    int arrival;
    EdgeNode *s;

    if (graph->vertexM[iAirportVertex].bVisited)
        return;

    graph->vertexM[iAirportVertex].bVisited = TRUE;

    for (s = graph->vertexM[iAirportVertex].successorList; s != NULL; s = s->pNextEdge)
    {
        arrival = calcArr2400(s->flight.iDepTm2400, s->flight.iDurationMins, s->flight.iZoneChange);
        if (graph->vertexM[s->iDestVertex].bVisited == TRUE)
        {
            continue;
        }
        for (i = 0; i < indent; i++)
        {
            printf("    ");
        }
        printf("%8s-%s %.4d-%.4d \n", s->flight.szFlightNr, s->flight.szDest, s->flight.iDepTm2400, arrival);
        prtTraversal(graph, s->iDestVertex, indent + 1);
    }
    graph->vertexM[iAirportVertex].bVisited = FALSE;
}
void prtTraversalChron(Graph graph, int iVertex, int iIndent, int iPrevArrTm2400)
{
    int i;
    int arrival;
    EdgeNode *s;
    if (graph->vertexM[iVertex].bVisited)
        return;
    graph->vertexM[iVertex].bVisited = TRUE;
    for (s = graph->vertexM[iVertex].successorList; s != NULL; s = s->pNextEdge)
    {
        arrival = calcArr2400(s->flight.iDepTm2400, s->flight.iDurationMins, s->flight.iZoneChange);
        if (graph->vertexM[s->iDestVertex].bVisited == TRUE)
        {
            continue;
        }
        if (s->flight.iDepTm2400 <= (iPrevArrTm2400 + SAFE_DELTA_BETWEEN_FLIGHTS))
        {
            continue;
        }
        for (i = 0; i < iIndent; i++)
        {
            printf("    ");
        }
        printf("%8s-%s %.4d-%.4d \n", s->flight.szFlightNr, s->flight.szDest, s->flight.iDepTm2400, arrival);
        prtTraversalChron(graph, s->iDestVertex, iIndent + 1, arrival);
    }
    graph->vertexM[iVertex].bVisited = FALSE;
}
void setNotVisited(Graph graph)
{
    int i;

    for (i = 1; i <= graph->iNumVertices; i++)
    {
        graph->vertexM[i].bVisited = FALSE;
    }
}

/******************** determinePaths **************************************
void determinePaths(Graph graph, int iVertex, int iDestVertex, int iCurStep, Path path, int iPrevArrTm2400)
Purpose:
    This recursive function determines all valids paths in the graph.
Parameters:
    I   Graph graph             Graph to be fille with flight data
    I   int iVertex             Parameter to pass in the vertex of the graph.
    I   int iDestVertex         Parameter to pass in the vertex of the graph
    I   int iCurStep            Used as a subscript into path
    I   Path path               Previous arrival time of the flight in 2400 format
    I   int iPrevArrTm2400      Previous arrival time of the flight in 2400 format 
Returns:
    N/A since function is void 
Notes:
**************************************************************************/
void determinePaths(Graph graph, int iVertex, int iDestVertex, int iCurStep, Path path, int iPrevArrTm2400)
{
    int arrival; // time of arrival
    EdgeNode *s; // pointer variable of type EdgeNode to traverse loop

    // returns false if the vertex is already visited
    if (graph->vertexM[iVertex].bVisited)
        return;

    // mark the vertex as visited
    graph->vertexM[iVertex].bVisited = TRUE;

    // increment iCurStep
    iCurStep++;

    // traverse the successor list
    for (s = graph->vertexM[iVertex].successorList; s != NULL; s = s->pNextEdge)
    {
        arrival = calcArr2400(s->flight.iDepTm2400, s->flight.iDurationMins, s->flight.iZoneChange);
        if (graph->vertexM[s->iDestVertex].bVisited == TRUE)
        {
            continue;
        }
        if (s->flight.iDepTm2400 <= (iPrevArrTm2400 + SAFE_DELTA_BETWEEN_FLIGHTS))
        {
            continue;
        }
        path.stepM[iCurStep] = s;
        //printf("      Current Count: %d \n", iCurStep);
        if (s->iDestVertex == iDestVertex)
        {
            path.iTotalDuration = arrival;
            // printf("===========================================\n");
            path.iStepCnt = iCurStep;
            //printf("            Path Count: %d \n", path.iStepCnt);
            altPath->altPathM[altPath->iAltCnt] = path;
            //printf("       AltPath Count: %d \n", altPath->altPathM[altPath->iAltCnt].iStepCnt);
            altPath->iAltCnt++;
        }
        determinePaths(graph, s->iDestVertex, iDestVertex, iCurStep, path, arrival);
    }
    graph->vertexM[iVertex].bVisited = FALSE;
}

/******************** Airport **************************************
void prtAlts(Graph graph, int iOriginVertex, int iDestVertex)
Purpose:
This function prints the alternative paths from origin to the destination.
    
Parameters:
    I   Graph graph             Graph which needs to be traversed
   
    I   int iOriginVertex       Stores the vertex of the Origin flight

    I   int iDestVertex         Stores the vertex of the destinationOrigin flight    Returns:
        Functionally: 
         returns nothing - VOID function
    
Notes:

**************************************************************************/

void prtAlts(Graph graph, int iOriginVertex, int iDestVertex)
{
    int arrival;
    altPath->iAltCnt = 0;
    Path path;
    int i, j;
    printf("    Alternatives for flights from %s to %s \n", graph->vertexM[iOriginVertex].szAirport, graph->vertexM[iDestVertex].szAirport);
    printf("    Alt TDur             Org Dst Fl  Dep - Arr Dura\n");

    determinePaths(graph, iOriginVertex, iDestVertex, 0, path, 0);

    for (i = 0; i < altPath->iAltCnt; i++)
    {
        // printf("TESTING: %d\n", altPath->altPathM[i].iTotalDuration);
        int hour = (altPath->altPathM[i].iTotalDuration - altPath->altPathM[i].stepM[1]->flight.iDepTm2400) / 100;
        int min = (altPath->altPathM[i].iTotalDuration - altPath->altPathM[i].stepM[1]->flight.iDepTm2400) % 100;
        printf("      %d ", i + 1);
        printf("%2d Hour %d mins\n", hour, min);
        for (j = 1; j <= altPath->altPathM[i].iStepCnt; j++)
        {
            arrival = calcArr2400(altPath->altPathM[i].stepM[j]->flight.iDepTm2400, altPath->altPathM[i].stepM[j]->flight.iDurationMins, altPath->altPathM[i].stepM[j]->flight.iZoneChange);
            printf("%28s %s %s  %.4d-%.4d %d\n", altPath->altPathM[i].stepM[j]->flight.szOrigin, altPath->altPathM[i].stepM[j]->flight.szDest, altPath->altPathM[i].stepM[j]->flight.szFlightNr, altPath->altPathM[i].stepM[j]->flight.iDepTm2400, arrival, altPath->altPathM[i].stepM[j]->flight.iDurationMins);
        }
    }
}

/******************** maxStepsChron **************************************
int maxStepsChron(Graph graph, int iVertex, int iDestVertex, int iPrevArrTm2400)
Purpose:
    This function determines the maximum number of steps it takes to go from the
    origin to the destination in chronological order.
    
Parameters:
    I   Graph graph             Graph to be fille with flight data
    I   int iVertex             Parameter to pass in the vertex of the graph.
    I   int iDestVertex         Parameter to pass in the vertex of the graph
    I   int iPrevArrTm2400      Previous arrival time of the flight in 2400 format                      
Returns:
    The maximum number of steps it takes to get to destination 
Notes:
**************************************************************************/

int maxStepsChron(Graph graph, int iVertex, int iDestVertex, int iPrevArrTm2400)
{
    int i;
    //iCount for array subscript
    //iMax value
    //temp max chain value
    //CurrMax is current maxSteps until dest vertex
    int iCount = 0, iMax = 0, temp = 0, CurrMax = 0;
    //keep track of each CurrMax
    int steps[MAX_STEPS];
    int arrival;
    EdgeNode *s;
    //printf("TESTING HERE Curr: %d Dest: %d \n", iVertex, iDestVertex);
    if (graph->vertexM[iVertex].bVisited)
        return 0;

    graph->vertexM[iVertex].bVisited = TRUE;
    CurrMax++;
    for (s = graph->vertexM[iVertex].successorList; s != NULL; s = s->pNextEdge)
    {
        arrival = calcArr2400(s->flight.iDepTm2400, s->flight.iDurationMins, s->flight.iZoneChange);
        if (graph->vertexM[s->iDestVertex].bVisited == TRUE)
        {
            continue;
        }
        if (s->flight.iDepTm2400 <= (iPrevArrTm2400 + SAFE_DELTA_BETWEEN_FLIGHTS))
        {
            continue;
        }
        if (s->iOriginVertex == iDestVertex)
        {
            break;
        }
        temp = maxStepsChron(graph, s->iDestVertex, iDestVertex, arrival);
        //printf("Test %d \n", temp);
        CurrMax += temp;
        steps[iCount] = CurrMax;
        CurrMax = 1;
        iCount++;
    }
    //printf("Test %d \n", iCount);
    for (i = 0; i < iCount; i++)
    {
        if (steps[i] > iMax)
            iMax = steps[i];
    }
    graph->vertexM[iVertex].bVisited = FALSE;
    return iMax;
}

/******************** deleteAirport **************************************
void deleteAirport(Graph graph, char szAirport[])
Purpose:
    The purpose of this function is to delete the flight which is passed through
the parameter list once the delteelete command is invoked.This function also deletes
    all the predecessors and successors where this airport exists.
        
Parameters:
    I   Graph graph             Graph which needs to be traversed
   
    I   ehar szAirport          Stores the name of the airport which needs to be 
                            deleted/.
    Returns:
    Functionally: 
         returns nothing - VOID function
    
Notes:

**************************************************************************/

void deleteAirport(Graph graph, char szAirport[])
{
    EdgeNode *p, *s, *pRemove, *pRemove2;
    int i, j;

    int iVertex = findAirport(graph, szAirport);

    for (i = 1; i <= graph->iNumVertices; i++)
    {
        if (graph->vertexM[i].bExists == TRUE)
        {

            EdgeNode *pPrevs = NULL;

            for (s = graph->vertexM[i].successorList; s != NULL; s = s->pNextEdge)
            {
                if (strcmp(s->flight.szDest, szAirport) == 0)
                {
                    pRemove = s;

                    if (pPrevs == NULL)
                    {
                        if (s->pNextEdge == NULL)
                        {
                            graph->vertexM[i].successorList = NULL;
                            break;
                        }
                        graph->vertexM[i].successorList = s->pNextEdge;
                    }
                    else if (s->pNextEdge == NULL)
                    {
                        pPrevs->pNextEdge = NULL;
                    }
                    else
                    {
                        pPrevs->pNextEdge = s->pNextEdge;
                    }
                }

                pPrevs = s;
                //free(pRemove);
            }
        }
    }

    //predecessorList
    for (j = 1; j <= graph->iNumVertices; j++)
    {
        if (graph->vertexM[j].bExists == TRUE)
        {

            EdgeNode *pPrevp = NULL;

            for (p = graph->vertexM[j].predecessorList; p != NULL; p = p->pNextEdge)
            {
                if (strcmp(p->flight.szOrigin, szAirport) == 0)
                {
                    pRemove2 = p;

                    if (pPrevp == NULL)
                    {
                        if (p->pNextEdge == NULL)
                        {
                            graph->vertexM[j].predecessorList = NULL;
                            break;
                        }
                        graph->vertexM[j].predecessorList = p->pNextEdge;
                    }
                    else if (p->pNextEdge == NULL)
                    {
                        pPrevp->pNextEdge = NULL;
                    }
                    else
                    {
                        pPrevp->pNextEdge = p->pNextEdge;
                    }
                }

                pPrevp = p;
                //free(pRemove);
            }
        }
    }
    int r;
    for (r = iVertex; r < graph->iNumVertices; r++)
    {
        graph->vertexM[r] = graph->vertexM[r + 1];
    }
    graph->iNumVertices--;
    graph->vertexM[iVertex].bExists = FALSE;
    free(pRemove2);
    free(pRemove);
}
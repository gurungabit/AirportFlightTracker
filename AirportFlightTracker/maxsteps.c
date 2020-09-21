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
    for (int i = 0; i < iCount; i++)
    {
        if (steps[i] > iMax)
            iMax = steps[i];
    }
    graph->vertexM[iVertex].bVisited = FALSE;
    return iMax;
}
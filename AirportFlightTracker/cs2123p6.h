/**********************************************************************
cs2123p6.h
Purpose:
   Defines constants: 
       max constants 
       error constants
       warning constants
       special constants
       boolean constants
   Defines typedef for 
       EdgeNode  - graph edge containing flight information
       Vertex    - contains airport information (szAirport), existence boolean,
                   successor list first node pointer, predecessor list first node pointer
       GraphImp  - array of vertices and a count of them
       Graph     - pointer to an allocated GraphImp
       Path      - one path from an origin to a final destination with possibly many steps
       AltPathImp- the alternate paths to travel from the origin to the final destination       
   Defines function prototypes for functions used in pgm5 (recursive and non-recursive)
   Defines function prototypes for functions used in pgm6 
Notes:
   
**********************************************************************/
/*** constants ***/
#define MAX_TOKEN 50      // Maximum number of actual characters for a token
#define MAX_LINE_SIZE 100 // Maximum number of character per input line
#define MAX_VERTICES 30   // declared number of vertices in vertexM
#define OVERFLOW_BEGIN 7  // begin of overflow area
#define MAX_ALTERNATES 30
#define MAX_STEPS 10

// Error constants (program exit values)
#define ERR_EXIT 999
#define ERR_COMMAND_LINE 900 // invalid command line argument
#define ERR_ALGORITHM 903    // Unexpected error in algorithm

// Warning Messages
#define WARN_Specified_Airport_Not_Found "Specified airport not found: '%s'\n"
#define WARN_FlightNr_Already_Exists "Specified flight nr '%s' already exists from '%s'\n"

// special constants
#define SAFE_DELTA_BETWEEN_FLIGHTS 20 // the departure time in mins of the next flight              \
                                      // should be >= arrival time (in mins) of the previous flight \
                                      // plus this value
// boolean constants
#define FALSE 0
#define TRUE 1

// Flight is a structure type describing one flight
typedef struct
{
    char szFlightNr[3]; // Flight Nr
    char szOrigin[4];   // Origin airport
    char szDest[4];     // Destination airport
    int iDepTm2400;     // 24 hour time of day (e.g., 7:30pm is 1930)
    int iDurationMins;  // how long is the flight in minutes
    int iZoneChange;    // zone change (e.g., LAX to SAT is 2,
                        // SAT to LAX is -2)
} Flight;

// EdgeNode represents one edge in a graph
// It connects two vertices.
typedef struct EdgeNode
{
    int iOriginVertex; // origin subscript
    int iDestVertex;   // destination subscript
    Flight flight;
    struct EdgeNode *pNextEdge; // points to next  edge
} EdgeNode;

typedef struct Vertex
{
    char szAirport[4];         // Airport identifier
    int bVisited;              // TRUE - visited - FALSE - did not visit
    int bExists;               // pgm6 DELETE command needs this.
                               // TRUE - this vertex exists, FALSE - deleted
    EdgeNode *predecessorList; // List of predecessor edges which have
                               // this vertex as a destination
    EdgeNode *successorList;   // List of successor edges which have this
                               // as an origin
    int iHashChainNext;        // pgm 6 extra credit
} Vertex;

// GraphImp of a double adjacency list graph
typedef struct
{
    int iNumVertices;             // Number of vertices in the vertexM array
    Vertex vertexM[MAX_VERTICES]; // Array of vertices
    int iOverflowBegin;           // The subscript of the first overflow entry
                                  // in the array of vertices.
                                  // Any subscript less than this value is in
                                  // the primary area.
    int iFreeHead;                // Subscript of a free list of entries
                                  // in the overflow portion of the
                                  // graph's vertexM array
} GraphImp;

typedef GraphImp *Graph;

// Path describes one path from an origin to a final destination.  It may take
// many steps.
typedef struct
{
    int iStepCnt;               // the number of steps
    EdgeNode *stepM[MAX_STEPS]; // an array of the edge nodes in the path
    int iTotalDuration;         //
} Path;

// AltPathImp represents the alternate paths to travel from the origin to the
// final destination
typedef struct
{
    int iAltCnt;
    Path altPathM[MAX_ALTERNATES];
} AltPathImp;
typedef AltPathImp *AltPath;

// Prototypes

// Recursive functions for program 5
void prtTraversal(Graph graph, int iAirportVertex, int indent);
void prtTraversalChron(Graph graph, int iVertex, int iIndent, int iPrevArrTm2400);

// Non-recursive for program 5
int calcArr2400(int iDepTime2400, int iDurationMins, int iZoneChange);
int findAirport(Graph graph, char szAirport[]);
int insertFlight(Graph graph, Flight flight);
void prtAll(Graph graph);
void prtOne(Graph graph, int iVertex);
void setNotVisited(Graph graph);
void prtFlightsByOrigin(Graph graph);
void prtFlightsByDest(Graph graph);
Graph newGraph();

// Program 6 recursive
int maxStepsChron(Graph graph, int iVertex, int iDestVertex, int iPrevArrTm2400);
void determinePaths(Graph graph, int iVertex, int iDestVertex, int iCurStep, Path path, int iPrevArrTm2400);

// Program 6 function for delete
void deleteAirport(Graph graph, char szAirport[]);

// Program 6 non-recursive functions for Alt Paths
void prtAlts(Graph graph, int iOriginVertex, int iDestVertex);

// hash functions provided by Larry
int hash(Graph g, char szAirport[]);
void printHash(Graph g);
void printChain(Graph g, int iVertex);

// Utility routines provided by Larry in other programs
void errExit(char szFmt[], ...);
char *getToken(char *pszInputTxt, char szToken[], int iTokenSize);

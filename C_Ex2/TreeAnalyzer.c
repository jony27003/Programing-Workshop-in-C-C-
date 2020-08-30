#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"




//ERROR MACROS:
/** @brief A string to represent an invalid input error */
#define INPUT_ERROR "Invalid input\n"

/** @brief A string to represent Memory allocation error */
#define MEMORY_ERROR "Memory allocation failed\n"

/** @brief A string to represent a non tree graph error */
#define NOT_A_TREE_ERROR "The given graph is not a tree\n"

/** @brief A string to represent wrong number of segments input error */
#define NUMBER_OF_INPUT_SEGMENTS_ERROR "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n"

/** @brief A string to represent the maximum length allowed in an input line */
#define MAX_INPUT_LENGTH 1024




// STRUCTS:
/** @brief A struct to represent a vertex in a tree */
typedef struct Vertex
{
    long key; /**< the key that this vertex holds */
    struct Vertex *next; /**< a pointer to the next vertex in the chain */
} Vertex;


/** @brief A struct to represent a Tree of Vertices */
typedef struct Tree
{
    Vertex** treeStructure; /**< an array of Vertices to describe the trees structure */
    int root; /**< the root of this tree */
    long *leafs; /**< an array to represent the leafs of this tree */
    long totalNumberOfVertices; /**< the total number of vertices in this tree */
} Tree;




// METHODS DECLARATION
/**
 * @brief parses the given input
 * @param toParse The input to parse
 * @return 1 if the given input is valid or 0 otherwise.
 */
int parseInput(char *toParse);


/**
 * @brief parses the given file header
 * @param toParse The file header to parse
 * @return the number of different numbers by base 10 in the files header.
 */
long parseHeader(char *toValidate);


/**
 * @brief parses a vertex to a given trees structure
 * @param vertex the vertex to parse
 * @param key the vertex key
 * @param treeStructure the tree Structure
 * @param vertexIndex the vertex Index
 */
void parseTreeVertex(Vertex* vertex, long key, Vertex **treeStructure, long vertexIndex);


/**
 * @brief parses a vertexes number
 * @param the "number" to parse
 * @return the numbers representation ot -1 otherwise.
 */
long parseVertexNum(char *toParse);


/**
 * @brief creates a new tree from the given input
 * @param inputFilePointer a pointer to the input file
 * @param vertex1 the first input vortex
 * @param vertex2 the second input vortex
 * @return a pointer to the created tree or NULL otherwise
 */
Tree* createATree(FILE *inputFilePointer, long vertex1, long vertex2);


/**
 * @brief extracts the root number of a given tree
 * @param the first input vortex the pointer to the given tree
 * @return the number of this trees root
 */
int extractRoot(Tree *pointerToTree);


/**
 * @brief validate the given Vertices numbers
 * @param pointerToTree a pointer to the tree
 * @param vertex1 the first vertex to validate
 * @param vertex2 the second vertex to validate
 * @return 0 if valid -1 if not.
 */
int checkSizeOfInputVertices(Tree* pointerToTree, int vertex1, int vertex2);


/**
 * @brief transfer this tree from a directional to a non directional one
 * @param pointerToTree a pointer to the tree to transform
 */
void fillToNonDirectedTree(Tree* pointerToTree);


/**
 * @brief extracts the given trees span
 * @param pointerToTree a pointer to the tree from which to extract the span
 * @return the trees span or -1 otherwise.
 */
int extractSpan(Tree *pointerToTree);


/**
 * @brief finds the minimum or maximum path in this tree.
 * @param pointerToTree the pointer on the given tree
 * @param flag 1 -> finds max; -1 -> finds min
 * @return the length of the required path
 */
int boundedPaths(Tree* pointerToTree, int flag);


/**
 * @brief updates the length of the required path if necessary according to given parameters
 * @param flag
 * @param key1
 * @param key2
 * @param lowerBound
 * @param upperBound
 * @param lB
 * @param uB
 */
void boundedPathsHelper(int flag, int key1, int key2 , int lowerBound, int upperBound , int* lB, int* uB);


/**
 * @brief finds the max path length from a given vortex
 * @param pointerToTree a pointer to the tree
 * @param initialVertex the vortex from which the path begins
 * @param maxLength a pointer to the maximum path
 * @return the maximum path length or -1;
 */
int maxPathLengthFromVertex(Tree* pointerToTree, int initialVertex, int* maxLength);


/**
 * @brief finds the min path from a given vortex
 * @param pointerToTree a pointer to the tree
 * @param initialVertex the vortex from which the path begins
 * @param target the target node
 * @return prints the required path and returns 0 if successful or 1 otherwise
 */
int shortestPathToVertex(Tree *pointerToTree, long initialVertex, long target);


/**
 * @brief free vertex list
 * @param listToFree the vertex list to free
 * @param listSize the vertex list length
 */
void freeVertexList(Vertex **listToFree, int listSize);


/**
 * @brief free vertex tree
 * @param pointerToTree pointer to tree
 */
void freeVertexTree(Tree* pointerToTree);


/**
 * @brief initiates an integer array to a given value
 * @param arr the array to initiate
 * @param length the length of the array
 * @param toSet the given value
 */
void initiateIntArray(int* arr, int length, int toSet);


/**
 * @brief initiates array to NULL values
 * @param arr the array to initiate
 * @param length the length of the array
 */
void initiateArrayForNull(Vertex** arr, int length);


/**
 * @brief helper function for shortestPathToVertex
 * @param arrayToPrint the array to print
 * @param the initial index
 */
void printshortestPathToVertex(int *arrayToPrint, int index);




// METHODS IMPLEMENTATION
int main(int const numberOfInputSegments, char *inputSegments[])
{
    if (numberOfInputSegments != 4)
    {
        fprintf(stderr, "%s", NUMBER_OF_INPUT_SEGMENTS_ERROR);
        return 1;
    }
    FILE *inputFilePointer = fopen(inputSegments[1], "r");
    long vertex1 = parseVertexNum(inputSegments[2]);
    long vertex2 = parseVertexNum(inputSegments[3]);
    if (inputFilePointer == NULL || vertex1 < 0 || vertex2 < 0)
    {
        fclose(inputFilePointer);
        fprintf(stderr, "%s", INPUT_ERROR);
        return 1;
    }
    Tree* pointerToTree = createATree(inputFilePointer, vertex1, vertex2);
    fclose(inputFilePointer);
    if (pointerToTree == NULL)
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        return 1;
    }
    if (checkSizeOfInputVertices(pointerToTree, vertex1, vertex2) < 0)
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        fprintf(stderr, "%s", INPUT_ERROR);
        return 1;
    }
    printf("Root Vertex: %d\n", (*pointerToTree).root);
    printf("Vertices Count: %ld\n", (*pointerToTree).totalNumberOfVertices);
    printf("Edges Count: %ld\n", (*pointerToTree).totalNumberOfVertices - 1);
    fillToNonDirectedTree(pointerToTree);
    int upperBound = -1;
    int lowerBound = -1;
    upperBound = boundedPaths(pointerToTree, 1);
    lowerBound = boundedPaths(pointerToTree, -1);
    if(upperBound == -1 || lowerBound == -1)
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        fprintf(stderr, MEMORY_ERROR);
        return 1;
    }
    printf("Length of Minimal Branch: %d\n", lowerBound);
    printf("Length of Maximal Branch: %d\n", upperBound);
    int span = -1;
    span = extractSpan(pointerToTree);
    if (span == -1)
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        fprintf(stderr, MEMORY_ERROR);
        return 1;

    }
    printf("Diameter Length: %d\n", span);
    printf("Shortest Path Between %ld and %ld: ", vertex1, vertex2);
    if (shortestPathToVertex(pointerToTree, vertex1, vertex2) == 0)
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        fprintf(stderr, MEMORY_ERROR);
        return 1;
    }
    freeVertexTree(pointerToTree);
    pointerToTree = NULL;
    return 0;
}


int extractRoot(Tree *pointerToTree)
{
    int *treeEdges = (int*) malloc(sizeof(int) * (*pointerToTree).totalNumberOfVertices);
    initiateIntArray(treeEdges, (*pointerToTree).totalNumberOfVertices, 0);
    int i = 0;
    while(i < (*pointerToTree).totalNumberOfVertices)
    {
        Vertex* curVertex = (*pointerToTree).treeStructure[i];
        while (curVertex != NULL)
        {
            ++treeEdges[(*curVertex).key];
            curVertex = (*curVertex).next;
        }
        ++i;
    }
    int counter = 0;
    long index = -1;
    i = 0;
    while(i < (*pointerToTree).totalNumberOfVertices)
    {
        if (treeEdges[i] == 0)
        {
            if (counter > 0)
            {
                free(treeEdges);
                return -1;
            }
            else
            {
                index = i;
                ++counter;
            }
        }
        ++i;
    }
    free(treeEdges);
    return index;
}


long parseVertexNum(char *toParse)
{
    if (strchr(toParse, '-') != NULL || (parseInput(toParse)-1))
    {
        return -1;
    }
    return strtol(toParse, &toParse, 10);
}


int parseInput(char *toParse)
{
    if ((strlen(toParse) == 1) && *toParse == '-')
    {
        return 1;
    }
    int checker = 1;
    while (*toParse != '\0')
    {
        if ((*toParse >= '0' && *toParse <= '9'))
        {
            checker = 0;
        }
        else if (*toParse != ' ')
        {
            return 0;
        }
        ++toParse;
    }
    return !(checker);
}


int checkSizeOfInputVertices(Tree* pointerToTree, int vertex1, int vertex2)
{
    if (vertex1 >= (*pointerToTree).totalNumberOfVertices || vertex2 >= (*pointerToTree).totalNumberOfVertices)
    {
        return -1;
    }
    return 0;
}


void initiateIntArray(int* arr, int length, int toSet)
{
    for(int i = 0; i < length; ++i)
    {
        *(arr + i) = toSet;
    }
    return;
}


void initiateArrayForNull(Vertex** arr, int length)
{
    for(int i = 0; i < length; ++i)
    {
        *(arr + i) = NULL;
    }
    return;
}


Tree* createATree(FILE *inputFilePointer, long vertex1, long vertex2)
{
    Tree *pointerToTree = (Tree*)malloc(sizeof(Tree));
    if (pointerToTree == NULL)
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        fprintf(stderr, MEMORY_ERROR);
    }
    (*pointerToTree).treeStructure = NULL;
    (*pointerToTree).leafs = NULL;
    (*pointerToTree).totalNumberOfVertices = 0;
    (*pointerToTree).root = -1;
    char input[MAX_INPUT_LENGTH];
    if (fgets(input, MAX_INPUT_LENGTH, inputFilePointer) == NULL)
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        fprintf(stderr, INPUT_ERROR);
        return NULL;
    }
    strtok(input, "\n");
    Vertex **treeStructure = NULL;
    char *temp = NULL;
    if (parseInput(input))
    {
        (*pointerToTree).totalNumberOfVertices = strtoul(input, &temp, 10);
        (*pointerToTree).leafs = (long*) malloc(sizeof(long) * strtoul(input, &temp, 10));
        if(checkSizeOfInputVertices(pointerToTree, vertex1, vertex2) || (*pointerToTree).leafs == NULL)
        {
            freeVertexTree(pointerToTree);
            pointerToTree = NULL;
            fprintf(stderr, INPUT_ERROR);
            return NULL;
        }
        initiateIntArray((int*)(*pointerToTree).leafs, (int)strtoul(input, &temp, 10), 0);
        if (parseHeader(input) > 1)
        {
            freeVertexTree(pointerToTree);
            pointerToTree = NULL;
            fprintf(stderr, INPUT_ERROR);
            return NULL;
        }
        treeStructure = (Vertex**)malloc(sizeof(Vertex*) * strtoul(input, &temp, 10));
        if (treeStructure == NULL)
        {
            freeVertexTree(pointerToTree);
            pointerToTree = NULL;
            fprintf(stderr, MEMORY_ERROR);
            return NULL;
        }
        (*pointerToTree).treeStructure = treeStructure;
        initiateArrayForNull(treeStructure, (long)strtoul(input, &temp, 10) );
    }
    else
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        fprintf(stderr, INPUT_ERROR);
        return NULL;
    }
    long edgesCounter = 0;
    long verticesCounter = 0;
    long key = 0;
    long actualNumberOfVertices = strtoul(input, &temp, 10);
    while ((fgets(input, MAX_INPUT_LENGTH, inputFilePointer) != NULL) && (verticesCounter < actualNumberOfVertices))
    {
        strtok(input, "\n");
        if (parseInput(input))
        {
            ++verticesCounter;
            if (*input == '-')
            {
                (*pointerToTree).leafs[key] = 1;
                ++key;
                continue;
            }
            long vertexNum;
            temp = input;
            while (*temp != '\0')
            {
                vertexNum = strtoul(temp, &temp, 10);
                // check if this is this a valid num for a node
                Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));
                if(vertex == NULL)
                {
                    freeVertexTree(pointerToTree);
                    pointerToTree = NULL;
                    treeStructure = NULL;
                    fprintf(stderr, INPUT_ERROR);
                    return NULL;
                }

                if (vertexNum >= actualNumberOfVertices)
                {
                    free((*vertex).next);
                    free(vertex);
                    freeVertexTree(pointerToTree);
                    pointerToTree = NULL;
                    treeStructure = NULL;
                    fprintf(stderr, INPUT_ERROR);
                    return NULL;
                }
                parseTreeVertex(vertex, vertexNum, treeStructure, key);
                ++edgesCounter;
            }
            ++key;
        }
        else
        {
            freeVertexTree(pointerToTree);
            pointerToTree = NULL;
            treeStructure = NULL;
            fprintf(stderr, INPUT_ERROR);
            return NULL;
        }
    }
    if (verticesCounter != actualNumberOfVertices)
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        treeStructure = NULL;
        fprintf(stderr, INPUT_ERROR);
        return NULL;
    }
    if (edgesCounter != (actualNumberOfVertices - 1))
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        treeStructure = NULL;
        fprintf(stderr, NOT_A_TREE_ERROR);
        return NULL;
    }
    (*pointerToTree).root = extractRoot(pointerToTree);
    if ((*pointerToTree).root == -1)
    {
        freeVertexTree(pointerToTree);
        pointerToTree = NULL;
        treeStructure = NULL;
        fprintf(stderr, NOT_A_TREE_ERROR);
        return NULL;
    }
    return pointerToTree;
}


long parseHeader(char *toValidate)
{
    long count = 0;
    while (*toValidate != '\0')
    {
        strtoul(toValidate, &toValidate, 10);
        ++count;
    }
    return count;
}


void parseTreeVertex(Vertex* vertex, long key, Vertex **treeStructure, long vertexIndex)
{
    if(vertex != NULL)
    {
        (*vertex).key = key;
        (*vertex).next = NULL;
    }
    Vertex* current = treeStructure[vertexIndex];
    Vertex* previous = NULL;
    if (current == NULL)
    {
        treeStructure[vertexIndex] = vertex;
    }
    else
    {
        while (current != NULL)
        {
            previous = current;
            current = (*current).next;
        }
        (*previous).next = vertex;
    }
    return;
}


void freeVertexList(Vertex **listToFree, int listSize)
{
    if (listToFree == NULL)
    {
        return;
    }
    int i = 0;
    while(i < listSize)
    {
        if (listToFree[i] != NULL)
        {
            Vertex* vertex = listToFree[i];
            Vertex* temp;
            while (vertex != NULL)
            {
                temp = vertex;
                vertex = (*vertex).next;
                free(temp);
            }
        }
        ++i;
    }
    free(listToFree);
    return;
}


void freeVertexTree(Tree* pointerToTree)
{
    if (pointerToTree != NULL)
    {
        freeVertexList((*pointerToTree).treeStructure, (*pointerToTree).totalNumberOfVertices);
        free((*pointerToTree).leafs);
        free(pointerToTree);
    }
    return;
}


void fillToNonDirectedTree(Tree* pointerToTree)
{
    int i = 0;
    while(i < (*pointerToTree).totalNumberOfVertices)
    {
        Vertex* curVertex = (*pointerToTree).treeStructure[i];
        while (curVertex != NULL)
        {
            Vertex* connectedVertex = (Vertex*) malloc(sizeof(Vertex));
            if (connectedVertex == NULL)
            {
                fprintf(stderr, MEMORY_ERROR);
                return;
            }
            parseTreeVertex(connectedVertex, i, (*pointerToTree).treeStructure, (*curVertex).key);
            curVertex = (*curVertex).next;
        }
        ++i;
    }
    return;
}


int boundedPaths(Tree* pointerToTree, int flag)
{
    int *distanceArray = (int*) malloc(sizeof(int) * (*pointerToTree).totalNumberOfVertices);
    if (distanceArray == NULL)
    {
        fprintf(stderr, MEMORY_ERROR);
        return 0;
    }
    initiateIntArray(distanceArray, (*pointerToTree).totalNumberOfVertices, -1);
    distanceArray[(*pointerToTree).root] = 0;
    Queue* queueForBfs = allocQueue();
    enqueue(queueForBfs, (*pointerToTree).root);
    long currentNumber = 0;
    Vertex* toConnect = NULL;
    int upperBound = 0;
    int lowerBound = (int)pow((*pointerToTree).totalNumberOfVertices, 2) + 1;
    while (!queueIsEmpty(queueForBfs))
    {
        currentNumber = dequeue(queueForBfs);
        toConnect = (*pointerToTree).treeStructure[currentNumber];
        while (toConnect != NULL)
        {
            if (distanceArray[(*toConnect).key] == -1)
            {
                enqueue(queueForBfs, (*toConnect).key);
                distanceArray[(*toConnect).key] = distanceArray[currentNumber] + 1;
                boundedPathsHelper(flag, distanceArray[(*toConnect).key], (*pointerToTree).leafs[(*toConnect).key] ,
                                   lowerBound, upperBound , &lowerBound, &upperBound);
            }
            toConnect = (*toConnect).next;
        }
    }
    free(distanceArray);
    freeQueue(&queueForBfs);
    if(flag < 0)
    {
        return lowerBound;
    }
    return upperBound;
}


void boundedPathsHelper(int flag, int key1, int key2 , int lowerBound, int upperBound , int* lB, int* uB)
{
    if ((flag < 0) && (key1 < lowerBound) && key2 == 1)
    {
        *lB = key1;
    }
    if ((flag > 0) && (key1 > upperBound) && key2 == 1)
    {
        *uB = key1;
    }
    return;
}


int maxPathLengthFromVertex(Tree* pointerToTree, int initialVertex, int* maxLength)
{
    int *distanceArray = (int*) malloc(sizeof(int) * (*pointerToTree).totalNumberOfVertices);
    if (distanceArray == NULL)
    {
        fprintf(stderr, MEMORY_ERROR);
        return -1;
    }
    initiateIntArray(distanceArray, (*pointerToTree).totalNumberOfVertices, -1);
    distanceArray[initialVertex] = 0;
    Queue* queueForBfs = allocQueue();
    enqueue(queueForBfs, initialVertex);
    *maxLength = initialVertex;
    int maxLengthFound = 0;
    while (!queueIsEmpty(queueForBfs))
    {
        long currentNumber = dequeue(queueForBfs);
        Vertex* toConnect = (*pointerToTree).treeStructure[currentNumber];
        while (toConnect != NULL)
        {
            if (distanceArray[(*toConnect).key] == -1)
            {
                enqueue(queueForBfs, (*toConnect).key);
                distanceArray[(*toConnect).key] = distanceArray[currentNumber] + 1;
                if (distanceArray[(*toConnect).key] > maxLengthFound)
                {
                    *maxLength = (*toConnect).key;
                    maxLengthFound = distanceArray[(*toConnect).key];
                }
            }
            toConnect = (*toConnect).next;
        }
    }
    free(distanceArray);
    freeQueue(&queueForBfs);
    return maxLengthFound;
}


int extractSpan(Tree *pointerToTree)
{
    int temp1;
    int temp2;
    int span = -1;
    if (maxPathLengthFromVertex(pointerToTree, (*pointerToTree).root, &temp1))
    {
        span = maxPathLengthFromVertex(pointerToTree, temp1, &temp2);
    }
    return span;
}


int shortestPathToVertex(Tree *pointerToTree, long initialVertex, long target)
{
    int *recallVertex = (int*) malloc(sizeof(int) * (*pointerToTree).totalNumberOfVertices);
    int *distanceArray = (int*) malloc(sizeof(int) * (*pointerToTree).totalNumberOfVertices);
    if (distanceArray == NULL || recallVertex == NULL)
    {
        free(recallVertex);
        free(distanceArray);
        fprintf(stderr, MEMORY_ERROR);
        return 0;
    }
    initiateIntArray(distanceArray, (*pointerToTree).totalNumberOfVertices, -1);
    initiateIntArray(recallVertex, (*pointerToTree).totalNumberOfVertices, -1);
    distanceArray[initialVertex] = 0;
    Queue* queueForBfs = allocQueue();
    enqueue(queueForBfs, initialVertex);
    long currentNumber = 0;
    while (!queueIsEmpty(queueForBfs))
    {
        currentNumber = dequeue(queueForBfs);
        Vertex* toConnect = (*pointerToTree).treeStructure[currentNumber];
        while (toConnect != NULL)
        {
            if (distanceArray[(*toConnect).key] == -1)
            {
                enqueue(queueForBfs, (*toConnect).key);
                distanceArray[(*toConnect).key] = distanceArray[currentNumber] + 1;
                recallVertex[(*toConnect).key] = currentNumber;
            }
            toConnect = (*toConnect).next;
        }
    }
    printshortestPathToVertex(recallVertex, target);
    printf("\n");
    free(recallVertex);
    free(distanceArray);
    freeQueue(&queueForBfs);
    return 1;
}


void printshortestPathToVertex(int *arrayToPrint, int index)
{
    if (arrayToPrint[index] == -1)
    {
        printf("%d ", index);
    }
    else
    {
        printshortestPathToVertex(arrayToPrint, arrayToPrint[index]);
        printf("%d ", index);
    }
    return;
}
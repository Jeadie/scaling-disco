#include <stdlib.h>
#include "graph.h"

 
typedef struct {
  unsigned int* listSizes;  //Length of each list in stack
  unsigned int capacity;    // Number of possible elements in list and number of lists in stack
  unsigned int listPtr;		// number of elements in end list 
  unsigned int stackPtr;	// number of lists in stack
  int** s;					// a stack of lists of nodes
} ListStack; 

typedef struct {
    int* solution_indices; 
	int* solution_order; 
	int count; 
	int capacity; 
} Current_solution; 

int iterative_search(int start, Graph* g, Current_solution* sol, int start_node) ;
Current_solution* create_solution(int capacity); 



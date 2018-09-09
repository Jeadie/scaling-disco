#include "graph.h"
#include <stdlib.h>


typedef struct {
  unsigned int* listSizes;  //Length of each list in stack
  unsigned int capacity;    // Number of possible elements in list and number of lists in stack
  unsigned int listPtr;		// number of elements in end list 
  unsigned int stackPtr;	// number of lists in stack
  int** s;					// a stack of lists of nodes
} ListStack; 



ListStack* create_stack(int capacity){ 
   
	ListStack* stack = (ListStack*) malloc(sizeof(ListStack)); 
 	stack->capacity = capacity; 
	stack->listPtr = 0; 
	stack->stackPtr = 0; 
    stack->listSizes = calloc(sizeof(int), capacity); 
    stack->s = calloc(sizeof(int*), capacity); 
	return stack; 

}

// List Stack functions  
int last_list_is_empty(ListStack* s) {
	return s->listSizes[s->stackPtr -1] == 0; 
} 

void remove_last_list(ListStack* stack) {
	// get list
	int listInd = stack->stackPtr - 1;
	int* list  = stack->s[listInd]; 
	// free list
	free(list); 
	// change stackPtr and zero list length
	stack->listSizes[listInd] = 0; 
    stack->stackPtr--; 
}

int pop_last_node_off_list(ListStack* stack) {
	// get value
    int value = stack->s[stack->stackPtr -1][stack->listSizes[stack->stackPtr -1]-1]; 
	// reduce list size of last list
	stack->listSizes[stack->stackPtr]-- ; 
	return value; 
}  

void new_list(ListStack* stack) {
    // make new list of length capacity
	int* list = calloc(sizeof(int), stack->capacity); 
	// set to last in stack
	stack->s[stack->stackPtr] = list;
	// add to stackptr
	stack->stackPtr++; 
}

void add_to_current_stack(ListStack* stack, int node) {

	// Grab number of elements in last list
	int size = stack->listSizes[stack->stackPtr - 1]; 
// set last element of last list to node
    stack->s[stack->stackPtr-1][size] = node; 
	// increment length of last list
	stack->listSizes[stack->stackPtr -1]++; 
}


#define EMPTY -1; 
typedef struct {
    int* solution_indices; 
	int* solution_order; 
	int count; 
	int capacity; 
} Current_solution; 


Current_solution* create_solution(int capacity) {
    Current_solution* sol = (Current_solution*) calloc(sizeof(Current_solution), 1); 
	sol->count = 0;
	sol->capacity = capacity; 
	sol->solution_order = calloc(sizeof(int), capacity); 
	sol->solution_indices = calloc(sizeof(int), capacity); 
	return sol; 
}

// Solution functions
void remove_last_node(Current_solution* sol) {
	// get node from order    
	int v = sol->solution_order[sol->count -1]; 
	// set index to 0 in lookup table
	sol->solution_indices[v] = 0; 
	// set last element of order to EMPTY
	sol->solution_order[v] = EMPTY; 
}

void add_to_solution(Current_solution* sol, int v) {
    sol->solution_order[sol->count] = v; 
	sol->count++; 
	sol->solution_indices[v] = 1; 
}

int is_complete(Current_solution* sol) {
	return sol->count == sol->capacity; 
}

int* get_solution_order(Current_solution* sol) {
	return sol->solution_order; 
}


int solution_is_not_empty(Current_solution* sol) {
	return sol->count != 0; 
}

int* iterative_search(int start, Graph* g) {
  ListStack* stack = create_stack(g->n); 
  int current_node; 
  Current_solution* sol = create_solution(g->n); 
  
  while (solution_is_not_empty(sol)) {
	// 1. check if last queue is empty. If so backtrack
	if (last_list_is_empty(stack)) {
		remove_last_list(stack); 
		remove_last_node(sol); 
		continue; 
	}
	// 2. grab last item of last list and put as current and to solution
	current_node = pop_last_node_off_list(stack); 
	add_to_solution(sol, current_node); 
	// 3. check solution
	if (is_complete(sol)) {
		return get_solution_order(sol); 
	}
	// 4. add all outgoing nodes from current to stack that aren't already there 
	new_list(stack); 
    for (int j = 0; j < g->n; j++) {
		if((get_graph_value(g, current_node, j) == 1) & (j != current_node) & (sol->solution_indices[j] == 0)) {
			add_to_current_stack(stack, j); 
		}
	}

}
	return NULL; 

}




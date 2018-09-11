#include "graph.h"
#include <stdlib.h>
#include "stack.h"
#define FOUND_PATH 5
#define NO_PATH 6
#include <mpi.h>

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
	// free(list); 
	// change stackPtr and zero list length
	stack->listSizes[listInd] = 0; 
    stack->stackPtr--; 
}

int pop_last_node_off_list(ListStack* stack) {
	// get value
    int value = stack->s[stack->stackPtr -1][stack->listSizes[stack->stackPtr -1]-1]; 
	// reduce list size of last list
	stack->listSizes[stack->stackPtr]--; 
	printf("popped %d off stack.\n", value); 
	return value; 
}  

void new_list(ListStack* stack) {
    // make new list of length capacity
	int* list = calloc(sizeof(int), stack->capacity); 
	// set to last in stack
	stack->s[stack->stackPtr] = list;
	printf("made new list at %d.\n", stack->stackPtr); 
	// add to stackptr
	stack->listSizes[stack->stackPtr] = 0; 
	stack->stackPtr++;

}

void add_to_current_stack(ListStack* stack, int node) {
	printf("adding %d to current list at %d.\n", node, stack->listSizes[stack->stackPtr-1]); 
	// Grab number of elements in last list
	int size = stack->listSizes[stack->stackPtr - 1]; 
// set last element of last list to node
    stack->s[stack->stackPtr-1][size] = node; 
	// increment length of last list
	stack->listSizes[stack->stackPtr -1]++; 
}


#define EMPTY -1; 

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
	sol->count--;
	printf("Removed %d from solution at postiion %d.\n", v, sol->count +1); 
}

void add_to_solution(Current_solution* sol, int v) {
    sol->solution_order[sol->count] = v; 
	sol->count++; 
	sol->solution_indices[v] = 1; 
	printf("Added %d to solution at position %d.\n", v, sol->count -1); 
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

int iterative_search(int start, Graph* g, Current_solution* sol, int node) {
  int rank = -1; 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  ListStack* stack = create_stack(g->n); 
  int current_node; 
  new_list(stack); 
  add_to_current_stack(stack, node); 
  do {
	// 1. check if last queue is empty. If so backtrack
	if (last_list_is_empty(stack)) {
		printf("Backtracking from %d.\n", stack->stackPtr); 
		remove_last_list(stack); 
		remove_last_node(sol); 
		continue; 
	}
	// 2. grab last item of last list and put as current and to solution
	current_node = pop_last_node_off_list(stack); 
	printf("STACK ptr____________%d\n", stack->stackPtr); 
	printf("NODE_________________%d\n", current_node); 
	add_to_solution(sol, current_node); 
	// 3. check solution
	if (is_complete(sol)) {
		printf("%d: Exiting search\n", rank); 
		return FOUND_PATH;  
	}
	// 4. add all outgoing nodes from current to stack that aren't already there 
	new_list(stack); 
    int j; 	
	for (j = 0; j < g->n; j++) {
		if((get_graph_value(g, current_node, j) == 1)& (j != current_node)& (sol->solution_indices[j] == 0))  {
			add_to_current_stack(stack, j); 
		}
	}

}  while (solution_is_not_empty(sol));

	return NO_PATH; 

}




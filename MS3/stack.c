#include "graph.h"
#include <stdlib.h>
#include "stack.h"
#define FOUND_PATH 5
#define NO_PATH 6
#include <mpi.h>
#define EMPTY -1; 


int rank; 
ListStack* create_stack(int capacity){ 
   
	ListStack* stack = (ListStack*) malloc(sizeof(ListStack)); 
 	stack->capacity = capacity; 
	stack->listPtr = 0; 
	stack->stackPtr = 0; 
    stack->listSizes = calloc(sizeof(int), capacity); 
    stack->s = calloc(sizeof(int*), capacity); 
	return stack; 

}
void free_stack(ListStack* stack) {
	free(stack->listSizes); 
	int i; 
	for (i = 0; i < stack->stackPtr; i++) {
        free(stack->s[i]); 
	}
	free(stack->s); 
	free(stack); 
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
    int size = stack->listSizes[stack->stackPtr -1]; 
	int value = stack->s[stack->stackPtr -1][size-1]; 
	stack->s[stack->stackPtr -1][size-1] = EMPTY;  
	
	// reduce list size of last list
	stack->listSizes[stack->stackPtr -1] = size -1;  
	//printf("popped %d off stack.\n", value); 
	//printf("pop end: _______________%d\n",  stack->listSizes[stack->stackPtr -1]); 
	return value; 
}  

void new_list(ListStack* stack) {
    // make new list of length capacity
	int* list = calloc(sizeof(int), stack->capacity); 
	// set to last in stack
	stack->s[stack->stackPtr] = list;
	//printf("made new list at %d.\n", stack->stackPtr); 
	// add to stackptr
	stack->listSizes[stack->stackPtr] = 0; 
	stack->stackPtr++;

}

void add_to_current_stack(ListStack* stack, int node) {
//	printf("adding %d to current list at %d.\n", node, stack->listSizes[stack->stackPtr-1]); 
	
	// Grab number of elements in last list
	int size = stack->listSizes[stack->stackPtr - 1]; 
    
	// set last element of last list to node
    stack->s[stack->stackPtr-1][size] = node; 
	
	// increment length of last list
	stack->listSizes[stack->stackPtr -1]++; 
}



Current_solution* create_solution(int capacity) {
    Current_solution* sol = (Current_solution*) calloc(sizeof(Current_solution), 1); 
	sol->count = 0;
	sol->capacity = capacity; 
	sol->solution_order = calloc(sizeof(int), capacity); 
	sol->solution_indices = calloc(sizeof(int), capacity); 
	return sol; 
}

void alloc_solution(Current_solution* sol) {
    free(sol->solution_order); 
	free(sol->solution_indices); 
	free(sol); 
}

void remove_last_node(Current_solution* sol) {
	// get node from order    
	int v = sol->solution_order[sol->count -1]; 
	// set index to 0 in lookup table
	sol->solution_indices[v] = 0; 
	// set last element of order to EMPTY
	sol->solution_order[sol->count -1] = EMPTY; 
	sol->count--;
//	printf("Removed %d from solution at postiion %d.\n", v, sol->count); 
}

void add_to_solution(Current_solution* sol, int v) {
    sol->solution_order[sol->count] = v; 
	sol->count++; 
	sol->solution_indices[v] = 1; 
//	printf("Added %d to solution at position %d.\n", v, sol->count -1); 
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

int recursive_parallel_dfs(Graph* g, Current_solution* c_sol, int i, int node) {
/*
 * A recursive dfs search using a openMP based thread parallelism for recursive calls. 
 */

// must keep track of be the highest node tried. Will first start at 0, 
    int size = g->n; 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	int j;
    int current_n = c_sol->solution_order[i-1];
	int* out_bound = calloc(sizeof(int), size -i); 
    int out_count = 0; 
	
	#pragma omp parallel private(j, c_sol)
{	
	for (j = 0; j < size; j++) {
		if((get_graph_value(g, current_n, j) == 1) & \
                 (j != current_n) & \
				 (c_sol->solution_indices[j] == 0)) { 
			 //printf("%d at level %d.\n", j, i); 	
             // Set next node that is connected and hasn't been visited
			 add_to_solution(c_sol, j); 
	         // check completeness first. If empty nodes in order, must recurse
		     if(is_complete(c_sol) == 0) {
                 if (recursive_parallel_dfs(g, c_sol, i+1, 0) != 1) {
                      // backtrack and try next node
               //      printf("remove %d at level %d.\n", j,i); 
				     remove_last_node(c_sol); 
                  } else {
                      // backtracking with finished solution. just need to exit.  
                      // TODO free out_bound list
                      // free(out_bound); 
                 //     printf("correct. backtracking\n");
                      return 1; 
                         } 
             } else {
               // printf("Outer correct. backtracking\n");  
				return 1; 
             }   
        }
    }
}
    // TODO: free outbound list
    //free(out_bound); 
	c_sol->solution_indices[current_n] = 0; 
    return 0; 
}
int recursive_dfs(Graph* g, Current_solution* c_sol, int i, int node) {
/*
 * A recursive dfs search with no preprocessing or threading. 
 */ 

// must keep track of be the highest node tried. Will first start at 0, 
    int size = g->n; 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	int j;
    int current_n = c_sol->solution_order[i-1];
	for (j = 0; j < size; j++) {
		if((get_graph_value(g, current_n, j) == 1) & \
                 (j != current_n) & \
				 (c_sol->solution_indices[j] == 0)) { 
		
             // Set next node that is connected and hasn't been visited
			 add_to_solution(c_sol, j); 
	         // check completeness first. If empty nodes in order, must recurse
		     if(is_complete(c_sol) == 0) {
                 if (recursive_dfs(g, c_sol, i+1, 0) != 1) {
                      // backtrack and try next node
				     remove_last_node(c_sol); 
                  } else {
                      return 1; 
                  } 
             } else {
                 return 1; 
             }   
        }
	}
    // TODO: free outbound list
	c_sol->solution_indices[current_n] = 0; 
    return 0; 
}


int recursive_prelist_dfs(Graph* g, Current_solution* c_sol, int i, int node) {
/*
 * A recursive dfs search using a preprocessed list of outgoing nodes. preprocessing is 
 * parallel, recursive calls are not. 
 */ 

// must keep track of be the highest node tried. Will first start at 0, 
    int size = g->n; 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	int j;
    int current_n = c_sol->solution_order[i-1];
	int* out_bound = calloc(sizeof(int), size -i); 
    int out_count = 0; 
	for (j = 0; j < size; j++) {
		if((get_graph_value(g, current_n, j) == 1) & \
                 (j != current_n) & \
				 (c_sol->solution_indices[j] == 0)) { 
		
		out_bound[out_count] = j; 
        out_count++; 
		}
	}

    for (j=0; j < out_count; j++) {
             // Set next node that is connected and hasn't been visited
			 add_to_solution(c_sol, out_bound[j]); 
	         // check completeness first. If empty nodes in order, must recurse
		     if(is_complete(c_sol) == 0) {
                 if (recursive_prelist_dfs(g, c_sol, i+1, 0) != 1) {
                      // backtrack and try next node
				     remove_last_node(c_sol); 
                  } else {
                      // backtracking with finished solution. just need to exit.  
                      // TODO free out_bound list
                      free(out_bound); 
                      return 1; 
                         } 
             } else {
                 return 1; 
             }   
        }
    // TODO: free outbound list
    free(out_bound); 
	c_sol->solution_indices[current_n] = 0; 
    return 0; 
}


int iterative_search(int start, Graph* g, Current_solution* sol, int node) {
  int dfs_mode = 0; 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  int outcome; 
  if (dfs_mode == 2) {
    add_to_solution(sol, node); 
    outcome = recursive_dfs(g, sol,  1, 0) == 1 ? FOUND_PATH : NO_PATH; 

  } else if (dfs_mode == 1) {
	add_to_solution(sol, node); 
    outcome = recursive_prelist_dfs(g, sol,  1, 0) == 1 ? FOUND_PATH : NO_PATH; 
  } else {
   add_to_solution(sol, node); 
   outcome = recursive_parallel_dfs(g, sol,  1, 0) == 1 ? FOUND_PATH : NO_PATH; 
  }
  return outcome; 
  
}
  
int bfs(int start, Graph* g, Current_solution* sol, int node) {
  ListStack* stack = create_stack(g->n); 
  int current_node = -1; 
  new_list(stack); 
  add_to_current_stack(stack, node);
  do {
	
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
		printf("%d: Exiting search\n", rank); 
		free_stack(stack); 
		return FOUND_PATH;  
	}
	// 4. add all outgoing nodes from current to stack that aren't already there 
	new_list(stack); 
    int j; 	
	#pragma omp parallel private(j)
    {
	for (j = 0; j < g->n; j++) {
		if((get_graph_value(g, current_node, j) == 1)& (j != current_node)& (sol->solution_indices[j] == 0))  {
			#pragma omp critical
            {
		    	add_to_current_stack(stack, j); 
            }
		}
	}
    }

}  while (solution_is_not_empty(sol));
	free_stack(stack); 
	return NO_PATH; 
}




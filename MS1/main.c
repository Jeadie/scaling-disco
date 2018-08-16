#include "graph.h"
#include "input_data.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
    int* solution_indices; 
	int* solution_order; 
} Current_solution; 

// Function delcarations
int* find_path(Graph* g); 
int add_index_at_i(Graph* g, Current_solution* c_sol, int i, int node); 
int node_in_solution(int* sol, int n, int len); 

int size; 

int main(int argc, char** argv) {
    clock_t start = clock(); 
	Input* i = parse_input_from_file(argv[1]); 
    Graph* g = create_graph(get_node_count(i), get_edge_count(i), get_edge_source(i), get_edge_dest(i)); 
    if (g == NULL) {
        fprintf(stdout, "pre-process\n");
		exit(EXIT_FAILURE); 
	}
	size = graph_size(g); 
	int* solution = find_path(g); 
	clock_t end = clock(); 
	fprintf(stderr, "%d %f\n", size, ((double)(end-start))/CLOCKS_PER_SEC); 
    
	if(solution != NULL) {
        for(int j = 0; j< size-1; j++) {
            fprintf(stdout, "%s\n", get_node_names(i)[solution[j]]);   
        }
        fprintf(stdout, "%s", get_node_names(i)[solution[size-1]]);   

    } else {
	   fprintf(stdout, "No path \n");
	   exit(EXIT_FAILURE); 
	}
    exit(EXIT_SUCCESS); 
}

int* find_path(Graph* g) {
    int* solution = (int*) calloc(size, sizeof(int)); 
    for (int i =0; i< size; i++) {
        solution[i] = -1; 
	}
	Current_solution* c_sol = (Current_solution*) malloc(sizeof(Current_solution)); 
    c_sol->solution_order = solution; 
	c_sol->solution_indices = (int*) calloc(size, sizeof(int)); 


    //Create an empty path array and add vertex 0 to it. 
    for (int i= 0; i< size; i++) {
		c_sol->solution_order[0] = i;  
		c_sol->solution_indices[i] = 1; 
        if(add_index_at_i(g, c_sol, 1, 0)) {
             return solution; 
        }
    }   
    return NULL; 
}

int add_index_at_i(Graph* g, Current_solution* c_sol, int i, int node) {
// must keep track of be the highest node tried. Will first start at 0, 
    int current_n = c_sol->solution_order[i-1];
	for (int j = 0; j < size; j++) {
		if((get_graph_value(g, current_n, j) == 1) & \
                 (j != current_n) & \
				 (c_sol->solution_indices[j] == 0)) {
             // Set next node that is connected and hasn't been visited
			 c_sol->solution_order[i] = j; 
			 c_sol->solution_indices[j] = 1; 
	         // check completeness first. If empty nodes in order, must recurse
		     if(c_sol->solution_order[size -1] == -1) {
                 if (add_index_at_i(g, c_sol, i+1, 0) != 1) {
                      // backtrack and try next node
		              for(int k = i; k < i+1; k++) {
					      c_sol->solution_indices[c_sol->solution_order[k]] = 0; 
                          c_sol->solution_order[k] = -1; 
                      }
                  } else {
                      // backtracking with finished solution. just need to exit.  
                      return 1; 
                         } 
             } else {
                 return 1; 
             }   
        }

    }
	c_sol->solution_indices[current_n] = 0; 
    return 0; 
}

int node_in_solution(int* sol, int n, int len) {
    for (int i = 0; i< len; i++) {
        if (sol[i] == n) {
            return 1; 
	}
    } 
    return 0; 
}



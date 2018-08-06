#include "graph.h"
#include "input_data.h"
#include <stdlib.h>
#include <stdio.h>

// Function delcarations
int* find_path(Graph* g); 
int add_index_at_i(Graph* g, int* sol, int i, int node); 
int node_in_solution(int* sol, int n, int len); 


int main(int argc, char** argv) {
    Input* i = parse_input_from_file(argv[1]); 
    Graph* g = create_graph(get_node_count(i), get_edge_count(i), get_edge_source(i), get_edge_dest(i)); 
    int* solution = find_path(g); 
    if(solution != NULL) {
        printf("Found a solution: \n"); 
        for(int j = 0; j< graph_size(g); j++) {
            fprintf(stdout, "%s", get_node_names(i)[j]);   
        }
    } else {
        printf("No hamiltonian path in graph");
	}

    return 0; 
}

int* find_path(Graph* g) {
    int* solution = (int*) calloc(graph_size(g), sizeof(int)); 
    for (int i =0; i< graph_size(g); i++) {
        solution[i] = -1; 
	}
    //Create an empty path array and add vertex 0 to it. 
    for (int i= 0; i< graph_size(g); i++) {
        solution[0] = i; 
        if(add_index_at_i(g, solution, 1, 0)) {
             return solution; 
        }
    }   

    return NULL; 
}
    
int add_index_at_i(Graph* g, int* sol, int i, int node) {
// must keep track of be the highest node tried. Will first start at 0, 
    int current_n = sol[i-1]; 
    for (int j = 0; j < graph_size(g); j++) {
     //   printf("%d -> %d, value: %d, solution %d\n", current_n, j, get_graph_value(g, current_n, j), node_in_solution(sol, j, i)); 
		if((get_graph_value(g, current_n, j) == 1) & \
                 (node_in_solution(sol, j, i) == 0) & \
                 (j != current_n)) {
             // Set next node that is connected and hasn't been visited
             sol[i] = j; 
             // check completeness first 
             if(node_in_solution(sol, -1, graph_size(g))) {
                 if (add_index_at_i(g, sol, i+1, 0) != 1) {
                      // backtrack and try next node
		      for(int k = i; k < graph_size(g); k++) {
                          sol[k] = -1; 
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



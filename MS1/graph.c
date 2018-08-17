#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "input_data.h"

int enough_out_edges(int i, Graph* g);
int enough_in_edges(int i, Graph* g);
Graph* pass_sufficient_conditions(Graph* g); 

struct Graph {
  int n; 
  int* matrix;
}; 
 
int graph_size(Graph* g) {
    return g->n; 
}

Graph* create_graph(int n, int e, int* source, int* dest) { 
    Graph* g = (Graph*)  malloc(sizeof(Graph));  
    g->n = n;   
    g->matrix = (int*) calloc(n*n, sizeof(int)); 
    for (int i =0; i<e; i++) {
         set_graph_value(g, source[i], dest[i], 1); 
    }

    return g; //pass_sufficient_conditions(g); 
}

int enough_in_edges(int i, Graph* g) {
   int count = 0; 
   for (int j = 0 ; j < g->n; j++) {
       if (get_graph_value(g, j, i)) {
           count++;
		   if (count > 1) {
               return 2; 
		   }
	   }
   }
   return count; 
} 

int enough_out_edges(int i, Graph* g) {
   int count = 0; 
   for (int j = 0 ; j < g->n; j++) {
       if (get_graph_value(g, i, j)) {
           count++;
		   if (count > 1) {
               return 2; 
		   }
	   }
   }
   return count; 
}

Graph* pass_sufficient_conditions(Graph* g) {
	int only_one_out = 0; 
	int only_one_in = 0; 
	for (int i = 0; i< g->n; i++) {
        int a = enough_out_edges(i, g); 
		int b = enough_in_edges(i, g); 
		if (a + b < 2) {
            if ( a+b ==0 ) {
                return NULL; 
			}
			only_one_out += a;
			only_one_in += b;
			if ((only_one_out > 1) | (only_one_in > 1)) {
                return NULL; 
			}
		}
	}
    return g; 
}


void free_graph(Graph* g) { 
    free(g); 
}

void set_graph_value(Graph* g, int x, int y, int v) {
    g->matrix[(g->n * y) +x] = v; 
}

int get_graph_value(Graph* g, int x, int y) {
    return g->matrix[(g->n * y) +x];
}

int has_edge(Graph* g, int i, int j) {
    return (g->matrix[g->n * i + j]) == 1; 
}

int* out_edges(Graph* g, int n) {    
    int* edges = (int*) malloc(((g->n)+1) * sizeof(int));
    int j = 0; 
    for  (int i =0; i < g->n; i++) {
        if (has_edge(g, n, i)) {
	    edges[j] = i; 
	    j++; 
        }
    }
    edges[j] = -1; 
    return edges; 
}



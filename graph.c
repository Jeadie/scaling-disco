#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "input_data.h"


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
	for (int i=0; i< n; i++) {
        for(int j =0; j< n; j++) {
            printf("%d", get_graph_value(g, j, i)); 
		}
	printf("\n"); 
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



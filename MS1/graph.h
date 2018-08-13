#ifndef graph 
#define graph

#include <stdio.h>
#include <stdlib.h>

typedef struct Graph Graph;

void set_graph_value(Graph* g, int x, int y, int v); 

int get_graph_value(Graph* g, int x, int y);
int graph_size(Graph* g);
Graph* create_graph(int n, int e, int* source, int* dest); 



void free_graph(Graph* g); 

int has_edge(Graph* g, int n, int m); 

int* out_edges(Graph* g, int n);    

#endif


